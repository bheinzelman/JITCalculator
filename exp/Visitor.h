//  Visitor.h

#pragma once

class BasicExpression;
class VariableExpression;
class BinaryExpression;
class FunctionCallExpression;
class FunctionDecl;
class FunctionBody;
class Guard;
class ListExpression;
class Closure;

class Visitor {
public:
    virtual void visit(BasicExpression* expression) = 0;
    virtual void visit(VariableExpression* expression) = 0;
    virtual void visit(FunctionCallExpression* expression) = 0;
    virtual void visit(BinaryExpression* expression) = 0;
    virtual void visit(ListExpression* expression) = 0;
    virtual void visit(FunctionDecl* expression) = 0;
    virtual void visit(FunctionBody* expression) = 0;
    virtual void visit(Closure* expression) = 0;
    virtual void visit(Guard* expression) = 0;
};
