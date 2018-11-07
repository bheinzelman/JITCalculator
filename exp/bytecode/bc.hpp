//  bc.hpp

#pragma once

#include "Visitor.h"
#include "ast.hpp"
#include "jcVariable.hpp"

#include <stack>
#include <vector>

// all things bytecode

namespace bc {
enum Op {
    Push,
    Ret,
    Pop,
    Add,
    Subtract,
    Multiply,
    Divide,
    Less_Than,
    Greater_Than,
    Less_Than_Equal,
    Greater_Than_Equal,
    Equals,
    Call,
    Label,
    JmpTrue,
    Jmp,
    Exit,
};

/**
     * runtime variables
     */
namespace vars {
    // instruction pointer
    const std::string ip = "$ip";
}

class Instruction {
public:
    Instruction(bc::Op op, std::vector<jcVariablePtr> operands);
    Instruction(bc::Op op);

    int numOperands() const;

    jcVariablePtr getOperand(int idx) const;

    bc::Op getOp() const;

    std::string toString() const;

private:
    bc::Op mOp;
    std::vector<jcVariablePtr> mOperands;
};

class Generator : Visitor {
public:
    Generator(std::shared_ptr<Node> root);

    std::vector<Instruction> getInstructions();

    void visit(BasicExpression* expression);

    void visit(BinaryExpression* expression);

    void visit(FunctionCallExpression* expression);

    void visit(FunctionDecl* function);

    void visit(VariableExpression* expression);

    void visit(ListExpression* list);

    void visit(Guard* guard);

private:
private:
    std::shared_ptr<Node> mRoot;
    std::vector<Instruction> mOutput;
};

}
