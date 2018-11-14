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
    /**
     Pushes a value to the argument stack
     */
    Push,

    /**
     Returns control of the function back to the caller
     */
    Ret,

    /**
     Pop value off argument stack
        - arg - the variable to be placed into
     */
    Pop,

    /**
     Adds the top two values in the argument stack
     */
    Add,

    /**
     Subtracts the top two values in the argument stack
     */
    Subtract,

    /**
     Multiplies the top two values in the argument stack
     */
    Multiply,

    /**
     Divides the top two values in the argument stack
     */
    Divide,

    /**
     Computes less than for top to values in the argument stack
     */
    Less_Than,

    /**
     Computes greater than for top to values in the argument stack
     */
    Greater_Than,

    /**
     Computes less than or equal for top to values in the argument stack
     */
    Less_Than_Equal,

    /**
     Computes greater than or equal for top to values in the argument stack
     */
    Greater_Than_Equal,

    /**
     Computes equality for top to values in the argument stack
     */
    Equals,

    /**
     Calls the function at the given label or line number
     */
    Call,

    /**
     Defines a label
     */
    Label,

    /**
     Conditionally jump if the top of the stack is TRUE
     */
    JmpTrue,

    /**
     Jump to label
     */
    Jmp,

    /**
     Exit the program
     */
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

/**
 Walks through the AST and generates bytecode
 */
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
