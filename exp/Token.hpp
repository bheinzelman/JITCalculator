//  Token.h

#pragma once

enum class Token
{
	Add,
	Subtract,
	Multiply,
	Divide,
	LParen,
	RParen,
	Num,
	Error,
	LetKw,
	Id,
	Assign,
    Comma,
	EndOfStream
};

namespace jcToken
{
	int getOperatorPrecedence(Token op);
}
 
