//  Lexer.hpp

#pragma once

#include <string>

#include "Token.hpp"

class Lexer
{
public:
	Lexer(std::string expression);
	
	Token currentToken() const;
	
	bool peekToken(Token *token, int *lexeme=nullptr);
	
	bool getNextToken(Token *token, int *lexeme=nullptr);
	
private:
	std::string expression;
	int index;
	Token _currentToken;
	
private:
	bool nextNonWhitespaceChar(char *c);
	
	bool hasMoreChars() const;
};

