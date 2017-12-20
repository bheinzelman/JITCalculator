//  Calculator.cpp

#include "Calculator.hpp"

#include <iostream>
#include <memory>
#include <cstring>
#include <cstdlib>

enum Token
{
	Add,
	Subtract,
	Multiply,
	Divide,
	LParen,
	RParen,
	Num,
	Error
};

void fatalError(std::string errMessage) {
	std::cerr << "Fatal Error:" << errMessage << std::endl;
	exit(1);
}

int getOperatorPrecedence(Token op)
{
	switch (op) {
		case Add:
		case Subtract:
			return 1;
		case Multiply:
		case Divide:
			return 2;
		default:
			return -1;
			
	}
}

class Lexer
{
public:
	Lexer(std::string expression) : expression(expression)
	{
		this->index = 0;
	}
	
	Token currentToken() const {
		return _currentToken;
	}
	
	bool peakToken(Token *token, int *lexeme=nullptr)
	{
		int i = index;
		Token t;
		int value;
		bool result = getNextToken(&t, &value);
		index = i;
		
		if (result) {
			if (token != nullptr) {
				*token = t;
			}
			if (lexeme != nullptr) {
				*lexeme = value;
			}
		}
		return result;
	}
	
	bool getNextToken(Token *token, int *lexeme=nullptr)
	{
		Token nextToken = Error;
		if (hasMoreChars()) {
			char next;
			nextNonWhitespaceChar(&next);
			switch (next) {
				case '(':
					nextToken = LParen;
					break;
				case ')':
					nextToken = RParen;
					break;
				case '+':
					nextToken = Add;
					break;
				case '-':
					nextToken = Subtract;
					break;
				case '/':
					nextToken = Divide;
					break;
				case '*':
					nextToken = Multiply;
					break;
				default:
					break;
			}
			
			if (nextToken != Error) {
				if (token != nullptr) {
					*token = nextToken;
				}
				return true;
			}
			
			// must be a number
			if (isdigit(next)) {
				std::string numString = "";
				numString += next;
				while (isdigit(expression[index])) {
					numString += expression[index++];
					if (!hasMoreChars()) {
						break;
					}
				}
				int value = atoi(numString.c_str());
				if (lexeme != nullptr) {
					*lexeme = value;
				}
				if (token != nullptr) {
					*token = Num;
				}
				return true;
			} else {
				if (token != nullptr) {
					*token = Error;
				}
				return false;
			}
			
		}
		return false;
	}
	
private:
	std::string expression;
	int index;
	Token _currentToken;
	
private:
	
	
	bool nextNonWhitespaceChar(char *c)
	{
		while (isspace(expression[index]))
		{
			index++;
			if (!hasMoreChars()) {
				break;
			}
		}
		
		if (hasMoreChars()) {
			if (c != nullptr) {
				*c = expression[index];
			}
			index += 1;
			return true;
		}
		return false;
	}
	
	bool hasMoreChars() const
	{
		return index < expression.size();
	}
};

class BasicExpression;
class BinaryExpression;

class Visitor
{
public:
	virtual void visit(BasicExpression *expression)=0;
	virtual void visit(BinaryExpression *expression)=0;
};

class Expression
{
public:
	virtual ~Expression()
	{
	}
	
	virtual void accept(Visitor *v)=0;
};

class BasicExpression : public Expression
{
public:
	BasicExpression(int value) : value(value) {}
	~BasicExpression() {}
	int getValue() const { return value; }
	
	void accept(Visitor *v)
	{
		v->visit(this);
	}
private:
	int value;
};

class BinaryExpression : public Expression
{
public:
	BinaryExpression(std::shared_ptr<Expression> left, Token op, std::shared_ptr<Expression> right)
	{
		this->left = left;
		this->op = op;
		this->right = right;
	}
	
	~BinaryExpression()
	{
	}
	
	void accept(Visitor *v)
	{
		v->visit(this);
	}
	
	std::shared_ptr<Expression> getLeft()
	{
		return left;
	}
	
	std::shared_ptr<Expression> getRight()
	{
		return right;
	}
	
	Token getOperator() const
	{
		return op;
	}
	
private:
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;
	Token op;
};

class Parser
{
public:
	Parser(std::shared_ptr<Lexer> lexer) : lex(lexer)
	{
	}
	
	std::shared_ptr<Expression> parse()
	{
		return getExpression();
	}
	
	// private member variables
private:
	std::shared_ptr<Lexer> lex;
	// private helper functions
private:
	std::shared_ptr<Expression> getTerm()
	{
		Token tok;
		int value;
		
		bool result = lex->getNextToken(&tok, &value);
		if (result && tok == Num) {
			return std::make_shared<BasicExpression>(BasicExpression(value));
		} else if (result && tok == LParen) {
			auto exp = getExpression();
			if (lex->getNextToken(&tok)) {
				if (tok == RParen) {
					return exp;
				}
			}
		}
		return nullptr;
	}
	
	Token peekOperator()
	{
		Token tok;
		bool result = lex->peakToken(&tok);
		if (result) {
			switch (tok) {
				case Add:
				case Subtract:
				case Divide:
				case Multiply:
					return tok;
				default:
					break;
			}
		}
		return Error;
	}
	
	std::shared_ptr<Expression> getExpression(int prevPrec=1)
	{
		std::shared_ptr<Expression> left = getTerm();
		while (1) {
			Token op = peekOperator();
			if (getOperatorPrecedence(op) < prevPrec) {
				break;
			}
			lex->getNextToken(nullptr);
			int nextPrec = getOperatorPrecedence(op) + 1;
			auto right = getExpression(nextPrec);
			left = std::make_shared<BinaryExpression>(BinaryExpression(left, op, right));
		}
		return left;
	}
	
};

class Interpreter : Visitor
{
public:
	Interpreter(std::shared_ptr<Expression> expression)
	{
		this->expression = expression;
	}
	
	int interpret()
	{
		expression->accept(this);
		return lastValue;
	}
	
	void visit(BasicExpression *expression)
	{
		lastValue = expression->getValue();
	}
	
	void visit(BinaryExpression *expression)
	{
		expression->getLeft()->accept(this);
		int left = lastValue;
		
		Token op = expression->getOperator();
		
		expression->getRight()->accept(this);
		int right = lastValue;
		
		switch (op) {
			case Add:
				lastValue = left + right;
				break;
			case Subtract:
				lastValue = left - right;
				break;
			case Multiply:
				lastValue = left * right;
				break;
			case Divide:
				lastValue = left / right;
				break;
			default:
				return;
		}
		
	}
	
private:
	int lastValue;
	std::shared_ptr<Expression> expression;
};

Calculator::Calculator(std::string expression) : expression(expression)
{
}

int Calculator::calculate() const
{
	Lexer lex(this->expression);
	Parser parser(std::make_shared<Lexer>(lex));
	auto ast = parser.parse();
	
	Interpreter interpreter(ast);
	return interpreter.interpret();
	
}
