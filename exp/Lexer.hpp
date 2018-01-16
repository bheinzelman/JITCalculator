//  Lexer.hpp

#ifndef Lexer_hpp
#define Lexer_hpp

#include <string>

#include "Token.hpp"

class Lexer
{
public:
	Lexer(std::string expression);
	
	Token currentToken() const;
	
	bool peakToken(Token *token, int *lexeme=nullptr);
	
	bool getNextToken(Token *token, int *lexeme=nullptr);
	
private:
	std::string expression;
	int index;
	Token _currentToken;
	
private:
	bool nextNonWhitespaceChar(char *c);
	
	bool hasMoreChars() const;
};

#endif /* Lexer_hpp */
