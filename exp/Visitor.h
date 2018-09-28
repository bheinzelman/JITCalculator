//  Visitor.h

#pragma once

class BasicExpression;
class BinaryExpression;
class FunctionDecl;

class Visitor
{
public:
	virtual void visit(BasicExpression *expression)=0;
	virtual void visit(BinaryExpression *expression)=0;
	virtual void visit(FunctionDecl *expression)=0;
};

