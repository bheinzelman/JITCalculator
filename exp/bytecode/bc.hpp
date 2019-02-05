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
    Push = 0,

    /**
     Push a closure
     */
    PushC = 1,

    /**
     Returns control of the function back to the caller
     */
    Ret = 2,

    /**
     Pop value off argument stack
        - arg - the variable to be placed into
     */
    Pop = 3,

    /**
     Negates the value on the top of the stack -value
     */
    Neg = 4,

    /**
     Inverts the value on the top of the stack !value
     */
    Not = 5,

    /**
     Adds the top two values in the argument stack
     */
    Add = 6,

    /**
     Subtracts the top two values in the argument stack
     */
    Subtract = 7,

    /**
     Multiplies the top two values in the argument stack
     */
    Multiply = 8,

    /**
     Divides the top two values in the argument stack
     */
    Divide = 9,

    /**
     Computes less than for top to values in the argument stack
     */
    Less_Than = 10,

    /**
     Computes greater than for top to values in the argument stack
     */
    Greater_Than = 11,

    /**
     Computes less than or equal for top to values in the argument stack
     */
    Less_Than_Equal = 12,

    /**
     Computes greater than or equal for top to values in the argument stack
     */
    Greater_Than_Equal = 13,

    /**
     Computes equality for top to values in the argument stack
     */
    Equals = 14,

    /**
     Calls the function at the given label or line number
     Note, does not take arguments, will call the function/closure that is at the top
     of the stack
     */
    Call = 15,

    /**
     Defines a label
     */
    Label = 16,

    /**
     Conditionally jump if the top of the stack is TRUE
     */
    JmpTrue = 17,

    /**
     Jump to label
     */
    Jmp = 18,

    /**
     Adds an item to the beginning of a list.
     */
    Cons = 19,

    /**
     Adds two lists together.
     */
    Concat = 20,

    /**
     Exit the program
     */
    Exit = 21,

    /**
     Returns the element at the given index of a collection.
     */
    Index = 22,


    /**
     Returns a slice of the collection
     */
    Slice = 23,
};


class Instruction {
public:
    explicit Instruction(bc::Op op, const jcVariablePtr &operand);
    Instruction(bc::Op op);

    int numOperands() const;

    jcVariablePtr getOperand() const;

    bc::Op getOp() const;

    std::string toString() const;

private:
    bc::Op mOp;
    jcVariablePtr mOperand;
};

/**
 Walks through the AST and generates bytecode
 */
class Generator : public Visitor {
public:
    Generator();

    std::vector<Instruction> getInstructions(std::shared_ptr<Node> root);
    std::vector<Instruction> getClosureInstructions();

    void visit(IntExpression* expression) override;

    void visit(StringExpression* expression) override;

    void visit(BinaryExpression* expression) override;

    void visit(NegateExpression* expression) override;

    void visit(TernaryExpresssion* expression) override;

    void visit(NotExpression* expression) override;

    void visit(FunctionCallExpression* expression) override;

    void visit(FunctionDecl* function) override;

    void visit(FunctionBody* functionBody) override;

    void visit(Closure* functionBody) override;

    void visit(VariableExpression* expression) override;

    void visit(ListExpression* list) override;

    void visit(Guard* guard) override;

    void visit(IndexExpression* expression) override;

    void visit(SliceExpression* expression) override;

private:
    void generateClosures();
    std::string labelMaker();
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
