// Token.cpp

#include "Token.hpp"
namespace jcToken {

int getOperatorPrecedence(Token op)
{
    switch (op) {
    case Token::QuestionMark:
        return 1;
    case Token::Less_Than:
    case Token::Greater_Than:
    case Token::Less_Than_Equal:
    case Token::Greater_Than_Equal:
    case Token::Equals:
        return 2;
    case Token::Add:
    case Token::Subtract:
        return 3;
    case Token::Multiply:
    case Token::Divide:
        return 4;
    default:
        return -1;
    }
}

}
