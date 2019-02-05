//  ast.cpp

#include "ast.hpp"
#include "Token.hpp"

// MARK: - IntExpression

IntExpression::IntExpression(int value)
    : value(value)
{
}

IntExpression::~IntExpression()
{
}

int IntExpression::getValue() const
{
    return value;
}

void IntExpression::accept(Visitor* v)
{
    v->visit(this);
}

// MARK: - StringExpression

StringExpression::StringExpression(const jcStringPtr& value) : mValue(value)
{
}

StringExpression::~StringExpression()
{

}

jcStringPtr StringExpression::getValue() const
{
    return mValue;
}

void StringExpression::accept(Visitor* v)
{
    v->visit(this);
}

// MARK: - BinaryExpression

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

// MARK: - FunctionBody

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

// MARK: - FunctionDecl

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

// MARK: - VariableExpression

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

// MARK: - NegateExpression

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

// MARK: - FunctionCallExpression

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

// MARK: - IndexExpression

IndexExpression::IndexExpression(std::shared_ptr<Expression> callee, std::shared_ptr<Expression> index)
: mCallee(callee), mIndex(index)
{
}

std::shared_ptr<Expression> IndexExpression::getCallee() const
{
    return mCallee;
}

std::shared_ptr<Expression> IndexExpression::getIndex() const
{
    return mIndex;
}

void IndexExpression::accept(Visitor* v)
{
    v->visit(this);
}

// MARK: - SliceExpression

SliceExpression::SliceExpression(std::shared_ptr<Expression> callee, std::shared_ptr<Expression> index1, std::shared_ptr<Expression> index2)
: mCallee(callee), mIndex1(index1), mIndex2(index2)
{
}

std::shared_ptr<Expression> SliceExpression::getCallee() const
{
    return mCallee;
}

std::shared_ptr<Expression> SliceExpression::getIndex1() const
{
    return mIndex1;
}

std::shared_ptr<Expression> SliceExpression::getIndex2() const
{
    return mIndex2;
}

void SliceExpression::accept(Visitor* v)
{
    v->visit(this);
}

// MARK: - Guard

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

// MARK: - ListExpression

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

// MARK: - TernaryExpression

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
