//  Lexer.hpp

#pragma once

#include <istream>
#include <string>

#include "Token.hpp"
#include "jcVariable.hpp"

class Lexer {
public:
    Lexer(std::istream& inputStream);

    Token peekToken();

    Token getNextToken();

    void skipToken();

private:
    std::istream& mInput;
    int64_t mLineNumber=0;

private:
    char peek();

    char nextChar();

    int64_t position();
    void seekPosition(int64_t pos);

    bool nextNonWhitespaceChar(char* c);

    bool hasMoreChars();

    TokenType resolveTwoCharOperator(TokenType firstOperator);
};
