//  Lexer.hpp

#pragma once

#include <string>

#include "Token.hpp"
#include "jcVariable.hpp"

class Lexer
{
public:
	Lexer(std::string expression);
	
	Token currentToken() const;
	
	bool peekToken(Token *token, jcVariablePtr lexeme=nullptr);
	
	bool getNextToken(Token *token, jcVariablePtr lexeme=nullptr);
	
private:
	std::string expression;
	int index;
	Token _currentToken;
	
private:
	bool nextNonWhitespaceChar(char *c);
	
	bool hasMoreChars() const;
};

