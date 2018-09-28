//  ast.cpp

#include "ast.hpp"
#include "Token.hpp"


BasicExpression::BasicExpression(int value) : value(value)
{
}

BasicExpression::~BasicExpression()
{
}

int BasicExpression::getValue() const
{
	return value;
}
	
void BasicExpression::accept(Visitor *v)
{
	v->visit(this);
}

BinaryExpression::BinaryExpression(std::shared_ptr<Expression> left, Token op, std::shared_ptr<Expression> right)
{
	this->left = left;
	this->op = op;
	this->right = right;
}
	
BinaryExpression::~BinaryExpression()
{
}
	
void BinaryExpression::accept(Visitor *v)
{
	v->visit(this);
}
	
std::shared_ptr<Expression> BinaryExpression::getLeft()
{
	return left;
}
	
std::shared_ptr<Expression> BinaryExpression::getRight()
{
	return right;
}
	
Token BinaryExpression::getOperator() const
{
	return op;
}

FunctionDecl::FunctionDecl(const std::string &id, std::shared_ptr<Expression> exp) : mId(id), mExpression(exp)
{
}

void FunctionDecl::accept(Visitor *v)
{
	v->visit(this);
}

std::string FunctionDecl::getId() const
{
	return mId;
}

std::shared_ptr<Expression> FunctionDecl::getExpression() const
{
	return mExpression;
}
