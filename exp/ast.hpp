//  ast.hpp

#ifndef ast_hpp
#define ast_hpp

#include "Visitor.h"
#include "Token.hpp"

#include <memory>

class Expression
{
public:
	virtual ~Expression();
	
	virtual void accept(Visitor *v)=0;
};

class BasicExpression : public Expression
{
public:
	BasicExpression(int value);
	
	~BasicExpression();
	
	int getValue() const;
	
	void accept(Visitor *v);
private:
	int value;
};

class BinaryExpression : public Expression
{
public:
	BinaryExpression(std::shared_ptr<Expression> left, Token op, std::shared_ptr<Expression> right);
	
	~BinaryExpression();
	
	void accept(Visitor *v);
	
	std::shared_ptr<Expression> getLeft();
	
	std::shared_ptr<Expression> getRight();
	
	Token getOperator() const;
	
private:
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;
	Token op;
};


#endif
