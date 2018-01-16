//  Visitor.h

#ifndef Visitor_h
#define Visitor_h

class BasicExpression;
class BinaryExpression;

class Visitor
{
public:
	virtual void visit(BasicExpression *expression)=0;
	virtual void visit(BinaryExpression *expression)=0;
};

#endif /* Visitor_h */
