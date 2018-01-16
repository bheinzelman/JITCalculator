// Parser.cpp

#include "Token.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "ast.hpp"

Parser::Parser(std::shared_ptr<Lexer> lexer) : lex(lexer)
{
}
	
std::shared_ptr<Expression> Parser::parse()
{
	return getExpression();
}
	
std::shared_ptr<Expression> Parser::getTerm()
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
	
Token Parser::peekOperator()
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
	
std::shared_ptr<Expression> Parser::getExpression(int prevPrec)
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
