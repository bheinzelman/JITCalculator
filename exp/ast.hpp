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

class FunctionCallExpression : public Expression {
public:
    FunctionCallExpression(const std::string functionId, const std::vector<std::shared_ptr<Expression>> arguments);

    std::string getFunctionId() const;
    std::vector<std::shared_ptr<Expression>> getArguments() const;

    void accept(Visitor* v) override;

private:
    std::string mFunctionId;
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
    Closure(const std::vector<std::string> &scope, std::shared_ptr<FunctionBody> body);

    void accept(Visitor* v) override;
    std::shared_ptr<FunctionBody> getBody() const;
    std::vector<std::string> getScope() const;

private:
    std::vector<std::string> mScope;
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
