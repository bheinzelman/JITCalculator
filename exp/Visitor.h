//  Visitor.h

#pragma once

class IntExpression;
class StringExpression;
class VariableExpression;
class BinaryExpression;
class FunctionCallExpression;
class NegateExpression;
class FunctionDecl;
class FunctionBody;
class Guard;
class ListExpression;
class Closure;
class NegateExpression;
class NotExpression;
class TernaryExpresssion;

class Visitor {
public:
    virtual void visit(IntExpression* expression) = 0;
    virtual void visit(StringExpression* expression) = 0;
    virtual void visit(VariableExpression* expression) = 0;
    virtual void visit(FunctionCallExpression* expression) = 0;
    virtual void visit(BinaryExpression* expression) = 0;
    virtual void visit(ListExpression* expression) = 0;
    virtual void visit(NegateExpression* expression) = 0;
    virtual void visit(NotExpression* expression) = 0;
    virtual void visit(TernaryExpresssion* expression) = 0;
    virtual void visit(FunctionDecl* expression) = 0;
    virtual void visit(FunctionBody* expression) = 0;
    virtual void visit(Closure* expression) = 0;
    virtual void visit(Guard* expression) = 0;
};
