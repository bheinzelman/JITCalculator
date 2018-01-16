//  Token.h

#ifndef Token_h
#define Token_h

enum Token
{
	Add,
	Subtract,
	Multiply,
	Divide,
	LParen,
	RParen,
	Num,
	Error
};

int getOperatorPrecedence(Token op);


#endif 
