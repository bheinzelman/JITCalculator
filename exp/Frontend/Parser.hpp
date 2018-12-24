//  Parser.hpp

#pragma once

#include "jcVariable.hpp"
#include <memory>

class Lexer;
class Expression;
class Guard;
class Node;
class FunctionDecl;
class FunctionBody;

class Parser {
public:
    Parser(Lexer& lexer);

    std::vector<std::shared_ptr<Node>> parse();
    std::shared_ptr<Node> parseLine();

    // private member variables
private:
    Lexer& lex;
    // private helper functions
private:
    std::shared_ptr<Expression> getTerm();

    std::vector<std::string> getFunctionParams();
    std::vector<std::shared_ptr<Guard>> getGuards();
    std::shared_ptr<FunctionBody> getFunctionBody();

    std::vector<std::shared_ptr<Expression>> getFunctionCallArgs();

    // Will return Error token if the given token is not a prefix operator
    Token peekPrefixOp();

    std::shared_ptr<Expression> getPostfixOps(std::shared_ptr<Expression> expIn);
    std::shared_ptr<Expression> getPrefixOps(std::shared_ptr<Expression> expIn, Token prefixOp);

    // Will return the error token if the next token is not an operator
    Token peekOperator();
    bool peekExpression();

    Token nextToken(jcMutableVariablePtr lex);
    Token peekToken();
    // asserts the next token is of a given type
    void eat(Token token);

    std::shared_ptr<Expression> getExpression(int prevPrec = 1);
    std::shared_ptr<FunctionDecl> getFunctionDecl();
};
