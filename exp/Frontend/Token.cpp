// Token.cpp

#include "Token.hpp"
namespace jcToken {

int getOperatorPrecedence(TokenType op)
{
    switch (op) {
    case TokenType::QuestionMark:
        return 1;
    case TokenType::Less_Than:
    case TokenType::Greater_Than:
    case TokenType::Less_Than_Equal:
    case TokenType::Greater_Than_Equal:
    case TokenType::Equals:
        return 2;
    case TokenType::Add:
    case TokenType::Subtract:
        return 3;
    case TokenType::Multiply:
    case TokenType::Divide:
        return 4;
    default:
        return -1;
    }
}

}

TokenType Token::getType() const
{
    return mTokenType;
}

jcVariablePtr Token::getLexeme() const
{
    return mLexeme;
}

int64_t Token::getLineNumber() const
{
    return mLineNumber;
}
