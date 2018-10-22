//  bc.hpp

#pragma once

#include "ast.hpp"
#include "Visitor.h"
#include "jcVariable.hpp"

#include <vector>
#include <stack>

// all things bytecode

namespace bc
{
	enum Op
	{
		Push,
        Set,
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
        Exit,
	};

    /**
     * runtime variables
     */
    namespace vars {
        // instruction pointer
        const std::string ip = "$ip";
    }

	class Instruction
	{
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
	
	class Generator : Visitor
	{
	public:
		Generator(std::shared_ptr<Node> root);
		
		std::vector<Instruction> getInstructions();
		
		void visit(BasicExpression *expression);
		
		void visit(BinaryExpression *expression);

        void visit(FunctionCallExpression *expression);
		
		void visit(FunctionDecl *function);

        void visit(VariableExpression *expression);
	private:
		
	private:
		std::shared_ptr<Node> mRoot;
		std::vector<Instruction> mOutput;
	};
	
	
}
