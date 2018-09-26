// Token.cpp

#include "Token.hpp"
namespace jcToken
{
	
int getOperatorPrecedence(Token op)
{
	switch (op) {
		case Token::Add:
		case Token::Subtract:
			return 1;
		case Token::Multiply:
		case Token::Divide:
			return 2;
		default:
			return -1;
	}
}

}
 
