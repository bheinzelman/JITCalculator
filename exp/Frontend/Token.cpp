// Token.cpp

#include "Token.hpp"
namespace jcToken {

int getOperatorPrecedence(Token op)
{
    switch (op) {
    case Token::Less_Than:
    case Token::Greater_Than:
    case Token::Less_Than_Equal:
    case Token::Greater_Than_Equal:
    case Token::Equals:
        return 1;
    case Token::Add:
    case Token::Subtract:
        return 2;
    case Token::Multiply:
    case Token::Divide:
        return 3;
    default:
        return -1;
    }
}

}
