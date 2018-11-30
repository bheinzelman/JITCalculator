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
    None
};

namespace jcToken {
int getOperatorPrecedence(Token op);
}
