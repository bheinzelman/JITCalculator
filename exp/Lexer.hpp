//  Lexer.hpp

#pragma once

#include <string>
#include <istream>

#include "Token.hpp"
#include "jcVariable.hpp"

class Lexer
{
public:
    Lexer(std::istream &inputStream);
	
	Token currentToken() const;
	
	bool peekToken(Token *token, jcVariablePtr lexeme=nullptr);
	
	bool getNextToken(Token *token, jcVariablePtr lexeme=nullptr);
	
private:
    std::istream &mInput;
//    int index;
	Token _currentToken;
	
private:
    char peek();

    char nextChar();

    int64_t position();
    void seekPosition(int64_t pos);

	bool nextNonWhitespaceChar(char *c);
	
	bool hasMoreChars();
};

