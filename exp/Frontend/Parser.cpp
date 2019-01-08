// Parser.cpp

#include "jc.h"

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Token.hpp"
#include "ast.hpp"
#include "jcVariable.hpp"

Parser::Parser(Lexer& lexer)
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
    if (peekToken().getType() == TokenType::EndOfStream) {
        return nullptr;
    }

    if (peekToken().getType() == TokenType::LetKw) {
        return getFunctionDecl();
    } else {
        return getExpression();
    }
}

bool Parser::peekExpression()
{
    if (peekPrefixOp() != TokenType::Error) {
        return true;
    }
    
    auto token = peekToken().getType();
    return token == TokenType::Num ||
           token == TokenType::LParen ||
           token == TokenType::Id ||
           token == TokenType::LeftBracket ||
           token == TokenType::LeftBrace;
}

std::vector<std::shared_ptr<Expression>> Parser::getFunctionCallArgs()
{
    eat(TokenType::LParen);
    std::vector<std::shared_ptr<Expression>> args;
    while (1) {
        if (peekExpression() == false) {
            break;
        }

        auto argument = getExpression();

        args.push_back(argument);

        if (peekToken().getType() != TokenType::RParen) {
            eat(TokenType::Comma);
        }
    }

    eat(TokenType::RParen);
    return args;
}

std::shared_ptr<Expression> Parser::getPostfixOps(std::shared_ptr<Expression> expIn)
{
    if (peekToken().getType() == TokenType::LParen) {
        auto functionCallArgs = getFunctionCallArgs();
        return getPostfixOps(std::make_shared<FunctionCallExpression>(expIn, functionCallArgs));
    }
    return expIn;
}

std::shared_ptr<Expression> Parser::getPrefixOps(std::shared_ptr<Expression> expIn, TokenType prefixOp)
{
    switch (prefixOp) {
        case TokenType::Subtract:
            return std::make_shared<NegateExpression>(expIn);
        case TokenType::Bang:
            return std::make_shared<NotExpression>(expIn);
        default:
            JC_THROW_PARSE_EXCEPTION("Invalid prefix operator", lex.getLineNumber());
    }
}

std::shared_ptr<Expression> Parser::getTerm()
{
    Token tok = nextToken();
    if (tok.getType() == TokenType::Num) {
        return std::make_shared<BasicExpression>(BasicExpression(tok.getLexeme()->asInt()));
    } else if (tok.getType() == TokenType::LParen) {
        auto exp = getExpression();
        eat(TokenType::RParen);
        return exp;
    } else if (tok.getType() == TokenType::Id) {
        return std::make_shared<VariableExpression>(tok.getLexeme()->asString());
    } else if (tok.getType() == TokenType::LeftBracket) {
        std::vector<std::shared_ptr<Expression>> elements;
        while (1) {
            if (peekExpression() == false) {
                break;
            }
            elements.push_back(getExpression());
            if (peekToken().getType() != TokenType::RightBracket) {
                eat(TokenType::Comma);
            }
        }
        eat(TokenType::RightBracket);
        return std::make_shared<ListExpression>(elements);
    } else if (tok.getType() == TokenType::LeftBrace) {
        auto closure = getFunctionBody();
        eat(TokenType::RightBrace);
        return std::make_shared<Closure>(closure);
    }
    return nullptr;
}

TokenType Parser::peekOperator()
{
    TokenType tok = peekToken().getType();
    switch (tok) {
    case TokenType::Add:
    case TokenType::Subtract:
    case TokenType::Divide:
    case TokenType::Multiply:
    case TokenType::Less_Than:
    case TokenType::Greater_Than:
    case TokenType::Less_Than_Equal:
    case TokenType::Greater_Than_Equal:
    case TokenType::Equals:
    case TokenType::QuestionMark:
        return tok;
    default:
        break;
    }
    return TokenType::Error;
}

TokenType Parser::peekPrefixOp()
{
    TokenType token = peekToken().getType();
    switch (token) {
        case TokenType::Subtract:
            return token;
        case TokenType::Bang:
            return token;
        default:
            return TokenType::Error;
    }
}

std::vector<std::string> Parser::getFunctionParams()
{
    std::vector<std::string> funcParams;

    if (peekToken().getType() == TokenType::LParen) {
        eat(TokenType::LParen);

        while (peekToken().getType() == TokenType::Id) {
            jcVariablePtr var = nextToken().getLexeme();

            funcParams.push_back(var->asString());

            if (peekToken().getType() != TokenType::Comma) {
                break;
            }

            eat(TokenType::Comma);
        }
        eat(TokenType::RParen);
    }
    return funcParams;
}

std::vector<std::shared_ptr<Guard>> Parser::getGuards()
{
    std::vector<std::shared_ptr<Guard>> guards;

    while (peekToken().getType() == TokenType::Pipe) {
        skipToken();

        if (peekToken().getType() == TokenType::ElseKw) {
            skipToken();
            break;
        }

        auto guardExpression = getExpression();
        eat(TokenType::Assign);
        auto bodyExpression = getExpression();

        guards.push_back(std::make_shared<Guard>(guardExpression, bodyExpression));
    }
    return guards;
}

std::shared_ptr<FunctionBody> Parser::getFunctionBody()
{
    std::vector<std::string> funcParams = getFunctionParams();

    std::vector<std::shared_ptr<Guard>> guards = getGuards();

    eat(TokenType::Assign);
    auto exp = getExpression();
    JC_ASSERT_OR_THROW_PARSE(exp != nullptr, "Function must have expression", lex.getLineNumber());

    return std::make_shared<FunctionBody>(exp, funcParams, guards);
}

std::shared_ptr<FunctionDecl> Parser::getFunctionDecl()
{
    eat(TokenType::LetKw);

    Token tok = nextToken();

    JC_ASSERT_OR_THROW_PARSE(tok.getType() == TokenType::Id, "Expected an ID", lex.getLineNumber());
    
    auto decl = std::make_shared<FunctionDecl>(tok.getLexeme()->asString(), getFunctionBody());
    return decl;
}

std::shared_ptr<Expression> Parser::getExpression(int prevPrec)
{
    TokenType prefixOp = peekPrefixOp();
    if (prefixOp != TokenType::Error) {
        skipToken();
    }
    
    std::shared_ptr<Expression> left = getPostfixOps(getTerm());

    if (prefixOp != TokenType::Error) {
        left = getPrefixOps(left, prefixOp);
    }

    while (1) {
        TokenType op = peekOperator();
        if (op != TokenType::Error && jcToken::getOperatorPrecedence(op) < prevPrec) {
            break;
        } else if (op == TokenType::Error) {
            if (peekToken().getType() == TokenType::Error) {
                JC_THROW_PARSE_EXCEPTION("Could not parse expression", lex.getLineNumber());
            } else {
                break;
            }
        } else if (op == TokenType::EndOfStream) {
            break;
        }

        // skip the token
        skipToken();

        int nextPrec = jcToken::getOperatorPrecedence(op) + 1;

        // ternary
        if (op == TokenType::QuestionMark) {
            auto trueExpression = getExpression(1);

            eat(TokenType::Colon);

            auto falseExpression = getExpression(1);

            left = std::make_shared<TernaryExpresssion>(left, trueExpression, falseExpression);
        } else {
            auto right = getExpression(nextPrec);
            left = std::make_shared<BinaryExpression>(BinaryExpression(left, op, right));
        }
    }



    return left;
}

Token Parser::peekToken()
{
    Token tokenOut = lex.peekToken();
    JC_ASSERT_OR_THROW_PARSE(tokenOut.getType() != TokenType::Error, "Bad Token", lex.getLineNumber());
    return tokenOut;
}

Token Parser::nextToken()
{
    Token tokenOut = lex.getNextToken();
    JC_ASSERT_OR_THROW_PARSE(tokenOut.getType() != TokenType::Error, "Bad Token", lex.getLineNumber());
    return tokenOut;
}

void Parser::eat(TokenType token)
{
    Token newTok = nextToken();

    if (token != newTok.getType()) {
        std::string errorMessage = "Unexpected token, expected '"
            + jcToken::stringRepresentation(token) + "' got '" + jcToken::stringRepresentation(newTok.getType()) + "'";

        JC_THROW_PARSE_EXCEPTION(errorMessage, lex.getLineNumber());
    }
}

void Parser::skipToken()
{
    lex.skipToken();
}
