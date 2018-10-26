//  Token.h

#pragma once

enum class Token {
    Add,
    Subtract,
    Multiply,
    Divide,
    Less_Than,
    Greater_Than,
    Less_Than_Equal,
    Greater_Than_Equal,
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
    EndOfStream
};

namespace jcToken {
int getOperatorPrecedence(Token op);
}
