//  Parser.hpp

#pragma once

#include <memory>

class Lexer;
class Expression;

class Parser
{
public:
	Parser(std::shared_ptr<Lexer> lexer);
	
	std::shared_ptr<Expression> parse();
	
	// private member variables
private:
	std::shared_ptr<Lexer> lex;
	// private helper functions
private:
	std::shared_ptr<Expression> getTerm();
	
	// Will return the error token if the next token is not an operator
	Token peekOperator();
	
	Token nextToken(int *lex);
	Token peekToken(int *lex);
	// asserts the next token is of a given type
	void eat(Token token);
	
	std::shared_ptr<Expression> getExpression(int prevPrec=1);
};

