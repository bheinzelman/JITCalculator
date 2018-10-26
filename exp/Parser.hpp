//  Parser.hpp

#pragma once

#include "jcVariable.hpp"
#include <memory>

class Lexer;
class Expression;
class Node;
class FunctionDecl;

class Parser {
public:
    Parser(std::shared_ptr<Lexer> lexer);

    std::vector<std::shared_ptr<Node>> parse();
    std::shared_ptr<Node> parseLine();

    // private member variables
private:
    std::shared_ptr<Lexer> lex;
    // private helper functions
private:
    std::shared_ptr<Expression> getTerm();

    // Will return the error token if the next token is not an operator
    Token peekOperator();
    bool peekExpression();

    Token nextToken(jcVariablePtr lex);
    Token peekToken();
    // asserts the next token is of a given type
    void eat(Token token);

    std::shared_ptr<Expression> getExpression(int prevPrec = 1);
    std::shared_ptr<FunctionDecl> getFunctionDecl();
};
