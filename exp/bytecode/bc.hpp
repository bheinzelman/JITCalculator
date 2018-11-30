//  bc.hpp

#pragma once

#include "Visitor.h"
#include "ast.hpp"
#include "jcVariable.hpp"

#include <stack>
#include <vector>
#include <set>
#include <utility>

// all things bytecode

namespace bc {
enum Op {
    /**
     Pushes a value to the argument stack
     */
    Push,

    /**
     Push a closure
     */
    PushC,

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
     Negates the value on the top of the stack -value
     */
    Neg,

    /**
     Inverts the value on the top of the stack !value
     */
    Not,

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
     Note, does not take arguments, will call the function/closure that is at the top
     of the stack
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
 runtime variables
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
    Generator();

    std::vector<Instruction> getInstructions(std::shared_ptr<Node> root);
    std::vector<Instruction> getClosureInstructions();

    void visit(BasicExpression* expression) override;

    void visit(BinaryExpression* expression) override;

    void visit(NegateExpression* expression) override;

    void visit(NotExpression* expression) override;

    void visit(FunctionCallExpression* expression) override;

    void visit(FunctionDecl* function) override;

    void visit(FunctionBody* functionBody) override;

    void visit(Closure* functionBody) override;

    void visit(VariableExpression* expression) override;

    void visit(ListExpression* list) override;

    void visit(Guard* guard) override;

private:
    void generateClosures();
    std::string closureLabel(int idx) const;
private:
    std::vector<Instruction> mOutput;
    // closures to be generated at the end..
    std::vector<std::pair<Closure *, std::set<std::string>>> mClosures;
    std::set<std::string> mScope;
    std::string mCurrentFunctionLabel;

    int mNumClosures=0;
};

}
