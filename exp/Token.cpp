// Token.cpp

#include "Token.hpp"

int getOperatorPrecedence(Token op)
{
	switch (op) {
		case Add:
		case Subtract:
			return 1;
		case Multiply:
		case Divide:
			return 2;
		default:
			return -1;

	}
}

