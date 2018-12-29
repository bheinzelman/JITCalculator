//  Token.h

#pragma once

#include "jc.h"
#include "jcVariable.hpp"

enum class TokenType {
    Add,
    Subtract,
    Multiply,
    Divide,
    Less_Than,
    Greater_Than,
    Less_Than_Equal,
    Greater_Than_Equal,
    Bang,
    Equals,
    LParen,
    RParen,
    Num,
    Error,
    LetKw,
    ElseKw,
    Id,
    Assign,
    Comma,
    Pipe,
    LeftBracket,
    RightBracket,
    LeftBrace,
    RightBrace,
    EndOfStream,
    QuestionMark,
    Colon,
    None
};

namespace jcToken {
    int getOperatorPrecedence(TokenType op);
}

class Token
{
public:
    Token(TokenType type, jcVariablePtr lexeme, int64_t lineNumber)
    : mTokenType(type), mLexeme(lexeme), mLineNumber(lineNumber)
    {
    }

    TokenType getType() const;
    jcVariablePtr getLexeme() const;
    int64_t getLineNumber() const;

private:
    TokenType mTokenType;
    jcVariablePtr mLexeme;
    int64_t mLineNumber;

};
