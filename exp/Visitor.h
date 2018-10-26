//  Visitor.h

#pragma once

class BasicExpression;
class VariableExpression;
class BinaryExpression;
class FunctionCallExpression;
class FunctionDecl;
class Guard;

class Visitor {
public:
    virtual void visit(BasicExpression* expression) = 0;
    virtual void visit(VariableExpression* expression) = 0;
    virtual void visit(FunctionCallExpression* expression) = 0;
    virtual void visit(BinaryExpression* expression) = 0;
    virtual void visit(FunctionDecl* expression) = 0;
    virtual void visit(Guard* expression) = 0;
};
