// Parser.cpp

#include "jc.h"

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include "ast.hpp"
#include "jcVariable.hpp"

Parser::Parser(std::shared_ptr<Lexer> lexer)
    : lex(lexer)
{
}

std::vector<std::shared_ptr<Node>> Parser::parse()
{
    std::vector<std::shared_ptr<Node>> output;
    while (auto node = parseLine()) {
        output.push_back(node);
    }
    return output;
}

std::shared_ptr<Node> Parser::parseLine()
{
    if (peekToken() == Token::EndOfStream) {
        return nullptr;
    }

    if (peekToken() == Token::LetKw) {
        return getFunctionDecl();
    } else {
        return getExpression();
    }
}

bool Parser::peekExpression()
{
    if (peekPrefixOp() != Token::Error) {
        return true;
    }
    
    auto token = peekToken();
    return token == Token::Num ||
           token == Token::LParen ||
           token == Token::Id ||
           token == Token::LeftBracket ||
           token == Token::LeftBrace;
}

std::vector<std::shared_ptr<Expression>> Parser::getFunctionCallArgs()
{
    eat(Token::LParen);
    std::vector<std::shared_ptr<Expression>> args;
    while (1) {
        if (peekExpression() == false) {
            break;
        }

        auto argument = getExpression();

        args.push_back(argument);

        if (peekToken() != Token::RParen) {
            eat(Token::Comma);
        }
    }

    eat(Token::RParen);
    return args;
}

std::shared_ptr<Expression> Parser::getPostfixOps(std::shared_ptr<Expression> expIn)
{
    if (peekToken() == Token::LParen) {
        auto functionCallArgs = getFunctionCallArgs();
        return getPostfixOps(std::make_shared<FunctionCallExpression>(expIn, functionCallArgs));
    }
    return expIn;
}

std::shared_ptr<Expression> Parser::getPrefixOps(std::shared_ptr<Expression> expIn, Token prefixOp)
{
    switch (prefixOp) {
        case Token::Subtract:
            return std::make_shared<NegateExpression>(expIn);
        case Token::Bang:
            return std::make_shared<NotExpression>(expIn);
        default:
            JC_THROW("Invalid prefix operator");
    }
}

std::shared_ptr<Expression> Parser::getTerm()
{
    jcMutableVariablePtr value = jcMutableVariable::Create();
    Token tok = nextToken(value);
    if (tok == Token::Num) {
        return std::make_shared<BasicExpression>(BasicExpression(value->asInt()));
    } else if (tok == Token::LParen) {
        auto exp = getExpression();
        eat(Token::RParen);
        return exp;
    } else if (tok == Token::Id) {
        return std::make_shared<VariableExpression>(value->asString());
    } else if (tok == Token::LeftBracket) {
        std::vector<std::shared_ptr<Expression>> elements;
        while (1) {
            if (peekExpression() == false) {
                break;
            }
            elements.push_back(getExpression());
            if (peekToken() != Token::RightBracket) {
                eat(Token::Comma);
            }
        }
        eat(Token::RightBracket);
        return std::make_shared<ListExpression>(elements);
    } else if (tok == Token::LeftBrace) {
        auto closure = getFunctionBody();
        eat(Token::RightBrace);
        return std::make_shared<Closure>(closure);
    }
    return nullptr;
}

Token Parser::peekOperator()
{
    Token tok = peekToken();
    switch (tok) {
    case Token::Add:
    case Token::Subtract:
    case Token::Divide:
    case Token::Multiply:
    case Token::Less_Than:
    case Token::Greater_Than:
    case Token::Less_Than_Equal:
    case Token::Greater_Than_Equal:
    case Token::Equals:
        return tok;
    default:
        break;
    }
    return Token::Error;
}

Token Parser::peekPrefixOp()
{
    Token token = peekToken();
    switch (token) {
        case Token::Subtract:
            return token;
        case Token::Bang:
            return token;
        default:
            return Token::Error;
    }
}

std::vector<std::string> Parser::getFunctionParams()
{
    std::vector<std::string> funcParams;

    if (peekToken() == Token::LParen) {
        eat(Token::LParen);

        while (peekToken() == Token::Id) {
            jcMutableVariablePtr var = jcMutableVariable::Create();
            nextToken(var);

            funcParams.push_back(var->asString());

            if (peekToken() != Token::Comma) {
                break;
            }

            eat(Token::Comma);
        }
        eat(Token::RParen);
    }
    return funcParams;
}

std::vector<std::shared_ptr<Guard>> Parser::getGuards()
{
    std::vector<std::shared_ptr<Guard>> guards;

    while (peekToken() == Token::Pipe) {
        nextToken(nullptr);

        if (peekToken() == Token::ElseKw) {
            nextToken(nullptr);
            break;
        }

        auto guardExpression = getExpression();
        eat(Token::Assign);
        auto bodyExpression = getExpression();

        guards.push_back(std::make_shared<Guard>(guardExpression, bodyExpression));
    }
    return guards;
}

std::shared_ptr<FunctionBody> Parser::getFunctionBody()
{
    std::vector<std::string> funcParams = getFunctionParams();

    std::vector<std::shared_ptr<Guard>> guards = getGuards();

    eat(Token::Assign);
    auto exp = getExpression();
    JC_ASSERT_OR_THROW(exp != nullptr, "Function must have expression");

    return std::make_shared<FunctionBody>(exp, funcParams, guards);
}

std::shared_ptr<FunctionDecl> Parser::getFunctionDecl()
{
    eat(Token::LetKw);

    jcMutableVariablePtr idLex = jcMutableVariable::Create();
    Token tok = nextToken(idLex);

    JC_ASSERT_OR_THROW(tok == Token::Id, "Expected an ID");
    
    auto decl = std::make_shared<FunctionDecl>(idLex->asString(), getFunctionBody());
    return decl;
}

std::shared_ptr<Expression> Parser::getExpression(int prevPrec)
{
    Token prefixOp = peekPrefixOp();
    if (prefixOp != Token::Error) {
        nextToken(nullptr);
    }
    
    std::shared_ptr<Expression> left = getPostfixOps(getTerm());

    if (prefixOp != Token::Error) {
        left = getPrefixOps(left, prefixOp);
    }

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
        } else if (op == Token::EndOfStream) {
            break;
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

Token Parser::nextToken(jcMutableVariablePtr lexeme)
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
