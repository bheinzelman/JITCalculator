//  ast.hpp

#pragma once

#include "Token.hpp"
#include "Visitor.h"
#include "jc.h"

#include <memory>
#include <string>
#include <vector>

static fourcc kExpressionType = 'expr';
static fourcc kFunctionDeclType = 'decl';
static fourcc kFunctionGuardType = 'grd_';
static fourcc kFunctionBody = 'fbdy';

class Node {
public:
    virtual ~Node()
    {
    }

    virtual void accept(Visitor* v) = 0;
    virtual fourcc type() const = 0;
};

class Expression : public Node {
public:
    virtual ~Expression()
    {
    }

    fourcc type() const override
    {
        return kExpressionType;
    }
};

class BasicExpression : public Expression {
public:
    BasicExpression(int value);

    ~BasicExpression();

    int getValue() const;

    void accept(Visitor* v) override;

private:
    int value;
};

class VariableExpression : public Expression {
public:
    VariableExpression(std::string variableName);

    std::string getVariableName() const;

    void accept(Visitor* v) override;

private:
    std::string mVariableName;
};

class NegateExpression : public Expression {
public:
    NegateExpression(std::shared_ptr<Expression> exp);

    std::shared_ptr<Expression> getExpression() const;

    void accept(Visitor* v) override;

private:
    std::shared_ptr<Expression> mExpression;
};

class NotExpression : public Expression {
public:
    NotExpression(std::shared_ptr<Expression> exp);

    std::shared_ptr<Expression> getExpression() const;

    void accept(Visitor* v) override;

private:
    std::shared_ptr<Expression> mExpression;
};

class FunctionCallExpression : public Expression {
public:
    FunctionCallExpression(std::shared_ptr<Expression> callee, const std::vector<std::shared_ptr<Expression>> arguments);

    std::shared_ptr<Expression> getCallee() const;
    std::vector<std::shared_ptr<Expression>> getArguments() const;

    void accept(Visitor* v) override;

private:
    std::shared_ptr<Expression> mCallee;
    std::vector<std::shared_ptr<Expression>> mArguments;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(std::shared_ptr<Expression> left, Token op, std::shared_ptr<Expression> right);

    ~BinaryExpression();

    void accept(Visitor* v) override;

    std::shared_ptr<Expression> getLeft();

    std::shared_ptr<Expression> getRight();

    Token getOperator() const;

private:
    std::shared_ptr<Expression> left;
    std::shared_ptr<Expression> right;
    Token op;
};

class ListExpression : public Expression {
public:
    ListExpression(const std::vector<std::shared_ptr<Expression>> &elements);

    void accept(Visitor *v) override;

    std::vector<std::shared_ptr<Expression>> getElements() const;

private:
    std::vector<std::shared_ptr<Expression>> mElements;
};

class TernaryExpresssion : public Expression {
public:
    TernaryExpresssion(const std::shared_ptr<Expression> conditionalExpression,
                       const std::shared_ptr<Expression> trueExpression,
                       const std::shared_ptr<Expression> falseExpression);

    std::shared_ptr<Expression> getConditionalExpression() const;
    std::shared_ptr<Expression> getTrueExpression() const;
    std::shared_ptr<Expression> getFalseExpression() const;

    void accept(Visitor *v) override;

private:
    std::shared_ptr<Expression> mConditionalExpression;
    std::shared_ptr<Expression> mTrueExpression;
    std::shared_ptr<Expression> mFalseExpression;

private:
};

class Guard : public Node {
public:
    Guard(const std::shared_ptr<Expression> guardExpression,
        const std::shared_ptr<Expression> body);

    void accept(Visitor* v) override;

    std::shared_ptr<Expression> getGuardExpression() const;

    std::shared_ptr<Expression> getBodyExpression() const;

    fourcc type() const override
    {
        return kFunctionGuardType;
    }

private:
    std::shared_ptr<Expression> mGuardExpression;
    std::shared_ptr<Expression> mBodyExpression;
};

class FunctionBody : public Node {
public:
    FunctionBody(std::shared_ptr<Expression> exp,
                 std::vector<std::string> params,
                 const std::vector<std::shared_ptr<Guard>>& guards);

    void accept(Visitor* v) override;

    std::shared_ptr<Expression> getDefaultExpression() const;
    std::vector<std::string> getParameters() const;
    std::vector<std::shared_ptr<Guard>> getGuards() const;

    fourcc type() const override
    {
        return kFunctionBody;
    }

private:
    std::shared_ptr<Expression> mExpression;
    std::vector<std::string> mParams;
    std::vector<std::shared_ptr<Guard>> mGuards;
};

class Closure : public Expression {
public:
    Closure(std::shared_ptr<FunctionBody> body);

    void accept(Visitor* v) override;
    std::shared_ptr<FunctionBody> getBody() const;
    
private:
    std::shared_ptr<FunctionBody> mFunctionBody;
};

class FunctionDecl : public Node {
public:
    FunctionDecl(const std::string& id,
                 std::shared_ptr<FunctionBody> body);

    void accept(Visitor* v) override;

    std::string getId() const;
    std::shared_ptr<FunctionBody> getFunctionBody() const;

    fourcc type() const override
    {
        return kFunctionDeclType;
    }

private:
    std::string mId;
    std::shared_ptr<FunctionBody> mBody;
};
