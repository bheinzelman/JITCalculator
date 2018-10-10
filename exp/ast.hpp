//  ast.hpp

#pragma once

#include "jc.h"
#include "Visitor.h"
#include "Token.hpp"

#include <memory>
#include <string>
#include <vector>

static fourcc kExpressionType = 'expr';
static fourcc kFunctionDeclType = 'decl';

class Node
{
public:
	virtual ~Node()
	{
	}
	
	virtual void accept(Visitor *v)=0;
	virtual fourcc type() const = 0;
};

class Expression : public Node
{
public:
	virtual ~Expression()
	{
	}
	
	fourcc type() const override
	{
		return kExpressionType;
	}
};

class BasicExpression : public Expression
{
public:
	BasicExpression(int value);
	
	~BasicExpression();
	
	int getValue() const;
	
	void accept(Visitor *v) override;
private:
	int value;
};

class VariableExpression : public Expression
{
public:
    VariableExpression(std::string variableName);

    std::string getVariableName() const;

    void accept(Visitor *v) override;

private:
    std::string mVariableName;
};

class FunctionCallExpression : public Expression
{
public:
    FunctionCallExpression(const std::string functionId, const std::vector<std::shared_ptr<Expression>> arguments);

    std::string getFunctionId() const;
    std::vector<std::shared_ptr<Expression>> getArguments() const;

    void accept(Visitor *v) override;

private:
    std::string mFunctionId;
    std::vector<std::shared_ptr<Expression>> mArguments;
};

class BinaryExpression : public Expression
{
public:
	BinaryExpression(std::shared_ptr<Expression> left, Token op, std::shared_ptr<Expression> right);
	
	~BinaryExpression();
	
	void accept(Visitor *v) override;
	
	std::shared_ptr<Expression> getLeft();
	
	std::shared_ptr<Expression> getRight();
	
	Token getOperator() const;
	
private:
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;
	Token op;
};

class FunctionDecl : public Node
{
public:
    FunctionDecl(const std::string &id, std::shared_ptr<Expression> exp, std::vector<std::string> params);
	
	void accept(Visitor *v) override;
	
	std::string getId() const;
	std::shared_ptr<Expression> getExpression() const;
    std::vector<std::string> getParameters() const;
	
	fourcc type() const override
	{
		return kFunctionDeclType;
	}
	
private:
	std::string mId;
	std::shared_ptr<Expression> mExpression;
    std::vector<std::string> mParams;
};

