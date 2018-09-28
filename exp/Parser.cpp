// Parser.cpp

#include "jc.h"

#include "Token.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "ast.hpp"
#include "jcVariable.hpp"

Parser::Parser(std::shared_ptr<Lexer> lexer) : lex(lexer)
{
}


std::shared_ptr<Node> Parser::parse()
{
	if (peekToken() == Token::LetKw) {
		return getFunctionDecl();
	} else {
		return getExpression();
	}
}

std::shared_ptr<Expression> Parser::getTerm()
{
	jcVariablePtr value = jcVariable::Create();
	Token tok = nextToken(value);
	if (tok == Token::Num) {
		return std::make_shared<BasicExpression>(BasicExpression(value->asInt()));
	} else if (tok == Token::LParen) {
		auto exp = getExpression();
		eat(Token::RParen);
		return exp;
	}
	JC_THROW("Could not parse term");
}
	
Token Parser::peekOperator()
{
	Token tok = peekToken();
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

std::shared_ptr<FunctionDecl> Parser::getFunctionDecl()
{
	eat(Token::LetKw);
	
	jcVariablePtr idLex = jcVariable::Create();
	Token tok = nextToken(idLex);
	
	JC_ASSERT_OR_THROW(tok == Token::Id, "Expected an ID");
	
	eat(Token::LParen);
	/* Params */
	eat(Token::RParen);
	eat(Token::Assign);
	auto exp = getExpression();
	
	auto decl = std::make_shared<FunctionDecl>(idLex->asString(), exp);
	return decl;
}
	
std::shared_ptr<Expression> Parser::getExpression(int prevPrec)
{
	std::shared_ptr<Expression> left = getTerm();
	while (1) {
		Token op = peekOperator();
		if (op != Token::Error && jcToken::getOperatorPrecedence(op) < prevPrec) {
			break;
		} else if (op == Token::Error) {
			if (peekToken() == Token::Error) {
				JC_THROW("Could not parse expression");
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

Token Parser::peekToken()
{
	Token tok = Token::Error;
	JC_ASSERT_OR_THROW(lex->peekToken(&tok), "Bad Token");
	JC_ASSERT_OR_THROW(tok != Token::Error, "Bad Token");
	return tok;
}

Token Parser::nextToken(jcVariablePtr lexeme)
{
	Token tok = Token::Error;
	JC_ASSERT_OR_THROW(lex->getNextToken(&tok, lexeme) == true, "Bad Token");
	JC_ASSERT_OR_THROW(tok != Token::Error, "Bad Token");
	return tok;
}

void Parser::eat(Token token)
{
	Token newTok = nextToken(nullptr);
	JC_ASSERT_OR_THROW(token == newTok, "Unexpected Token");
}

