//  Lexer.hpp

#pragma once

#include <istream>
#include <string>

#include "Token.hpp"
#include "jcVariable.hpp"

class Lexer {
public:
    Lexer(std::istream& inputStream);

    Token currentToken() const;

    bool peekToken(Token* token, jcMutableVariablePtr lexeme = nullptr);

    bool getNextToken(Token* token, jcMutableVariablePtr lexeme = nullptr);

private:
    std::istream& mInput;
    Token _currentToken;

private:
    char peek();

    char nextChar();

    int64_t position();
    void seekPosition(int64_t pos);

    bool nextNonWhitespaceChar(char* c);

    bool hasMoreChars();

    Token resolveTwoCharOperator(Token firstOperator);
};
