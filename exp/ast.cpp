//  ast.cpp

#include "ast.hpp"
#include "Token.hpp"

BasicExpression::BasicExpression(int value)
    : value(value)
{
}

BasicExpression::~BasicExpression()
{
}

int BasicExpression::getValue() const
{
    return value;
}

void BasicExpression::accept(Visitor* v)
{
    v->visit(this);
}

BinaryExpression::BinaryExpression(std::shared_ptr<Expression> left, TokenType op, std::shared_ptr<Expression> right)
{
    this->left = left;
    this->op = op;
    this->right = right;
}

BinaryExpression::~BinaryExpression()
{
}

void BinaryExpression::accept(Visitor* v)
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

TokenType BinaryExpression::getOperator() const
{
    return op;
}

FunctionBody::FunctionBody(std::shared_ptr<Expression> exp,
                           std::vector<std::string> params,
                           const std::vector<std::shared_ptr<Guard>>& guards)
: mExpression(exp), mParams(params), mGuards(guards)
{

}

void FunctionBody::accept(Visitor* v)
{
    v->visit(this);
}

std::shared_ptr<Expression> FunctionBody::getDefaultExpression() const
{
    return mExpression;
}

std::vector<std::string> FunctionBody::getParameters() const
{
    return mParams;
}

Closure::Closure(std::shared_ptr<FunctionBody> body)
: mFunctionBody(body)
{
}

void Closure::accept(Visitor* v)
{
    v->visit(this);
}

std::shared_ptr<FunctionBody> Closure::getBody() const
{
    return mFunctionBody;
}

std::vector<std::shared_ptr<Guard>> FunctionBody::getGuards() const
{
    return mGuards;
}

FunctionDecl::FunctionDecl(const std::string& id, std::shared_ptr<FunctionBody> body)
    : mId(id)
    , mBody(body)
{
}

void FunctionDecl::accept(Visitor* v)
{
    v->visit(this);
}

std::string FunctionDecl::getId() const
{
    return mId;
}

std::shared_ptr<FunctionBody> FunctionDecl::getFunctionBody() const
{
    return mBody;
}

VariableExpression::VariableExpression(std::string variableName)
    : mVariableName(variableName)
{
}

std::string VariableExpression::getVariableName() const
{
    return mVariableName;
}

void VariableExpression::accept(Visitor* v)
{
    v->visit(this);
}

NegateExpression::NegateExpression(std::shared_ptr<Expression> exp) : mExpression(exp)
{
}

std::shared_ptr<Expression> NegateExpression::getExpression() const
{
    return mExpression;
}

void NegateExpression::accept(Visitor* v)
{
    v->visit(this);
}

NotExpression::NotExpression(std::shared_ptr<Expression> exp) : mExpression(exp)
{
}

std::shared_ptr<Expression> NotExpression::getExpression() const
{
    return mExpression;
}

void NotExpression::accept(Visitor* v)
{
    v->visit(this);
}

FunctionCallExpression::FunctionCallExpression(std::shared_ptr<Expression> callee, const std::vector<std::shared_ptr<Expression>> arguments)
    : mCallee(callee)
    , mArguments(arguments)
{
}

std::shared_ptr<Expression> FunctionCallExpression::getCallee() const
{
    return mCallee;
}

std::vector<std::shared_ptr<Expression>> FunctionCallExpression::getArguments() const
{
    return mArguments;
}

void FunctionCallExpression::accept(Visitor* v)
{
    v->visit(this);
}

Guard::Guard(const std::shared_ptr<Expression> guardExpression, const std::shared_ptr<Expression> body)
    : mGuardExpression(guardExpression)
    , mBodyExpression(body)
{
}

void Guard::accept(Visitor* v)
{
}

std::shared_ptr<Expression> Guard::getGuardExpression() const
{
    return mGuardExpression;
}

std::shared_ptr<Expression> Guard::getBodyExpression() const
{
    return mBodyExpression;
}

ListExpression::ListExpression(const std::vector<std::shared_ptr<Expression>> &elements) : mElements(elements)
{
}

void ListExpression::accept(Visitor *v)
{
    v->visit(this);
}

std::vector<std::shared_ptr<Expression>> ListExpression::getElements() const
{
    return mElements;
}

TernaryExpresssion::TernaryExpresssion(const std::shared_ptr<Expression> conditionalExpression,
                   const std::shared_ptr<Expression> trueExpression,
                   const std::shared_ptr<Expression> falseExpression)
: mConditionalExpression(conditionalExpression), mTrueExpression(trueExpression), mFalseExpression(falseExpression)
{
}

std::shared_ptr<Expression> TernaryExpresssion::getConditionalExpression() const
{
    return mConditionalExpression;
}

std::shared_ptr<Expression> TernaryExpresssion::getTrueExpression() const
{
    return mTrueExpression;
}

std::shared_ptr<Expression> TernaryExpresssion::getFalseExpression() const
{
    return mFalseExpression;
}

void TernaryExpresssion::accept(Visitor *v)
{
    v->visit(this);
}
