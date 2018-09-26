// Parser.cpp

#include "jc.h"

#include "Token.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "ast.hpp"

Parser::Parser(std::shared_ptr<Lexer> lexer) : lex(lexer)
{
}
	
std::shared_ptr<Expression> Parser::parse()
{
	return getExpression();
}
	
std::shared_ptr<Expression> Parser::getTerm()
{
	
	int value;
	Token tok = nextToken(&value);
	if (tok == Token::Num) {
		return std::make_shared<BasicExpression>(BasicExpression(value));
	} else if (tok == Token::LParen) {
		auto exp = getExpression();
		eat(Token::RParen);
		return exp;
	}
	JC_FAIL_AND_RETURN_NULL();
}
	
Token Parser::peekOperator()
{
	Token tok = peekToken(nullptr);
	switch (tok) {
		case Token::Add:
		case Token::Subtract:
		case Token::Divide:
		case Token::Multiply:
			return tok;
		default:
			break;
	}
	return Token::Error;
}
	
std::shared_ptr<Expression> Parser::getExpression(int prevPrec)
{
	std::shared_ptr<Expression> left = getTerm();
	while (1) {
		Token op = peekOperator();
		if (op != Token::Error && jcToken::getOperatorPrecedence(op) < prevPrec) {
			break;
		} else if (op == Token::Error) {
			if (peekToken(nullptr) == Token::Error) {
				JC_FAIL_AND_RETURN_NULL();
			} else {
				break;
			}
		}
		
		// skip the token
		nextToken(nullptr);
		
		int nextPrec = jcToken::getOperatorPrecedence(op) + 1;
		auto right = getExpression(nextPrec);
		left = std::make_shared<BinaryExpression>(BinaryExpression(left, op, right));
	}
	return left;
}

Token Parser::peekToken(int *lexeme)
{
	Token tok = Token::Error;
	JC_ASSERT(lex->peekToken(&tok));
	JC_ASSERT(tok != Token::Error);
	return tok;
}

Token Parser::nextToken(int *lexeme)
{
	int myLexeme = 0;
	Token tok = Token::Error;
	JC_ASSERT(lex->getNextToken(&tok, &myLexeme) == true);
	JC_ASSERT(tok != Token::Error);
	if (lexeme) {
		*lexeme = myLexeme;
	}
	return tok;
}

void Parser::eat(Token token)
{
	Token newTok = nextToken(nullptr);
	JC_ASSERT(token == newTok);
}

