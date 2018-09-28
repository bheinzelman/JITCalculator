//  bc.hpp

#pragma once

#include "ast.hpp"
#include "Visitor.h"

#include <vector>
#include <stack>

// all things bytecode

namespace bc
{
	enum Op
	{
		Push,
		Pop,
		Add,
		Subtract,
		Multiply,
		Divide
	};
	
	class Instruction
	{
	public:
		Instruction(bc::Op op, std::vector<int> operands);
		
		int numOperands() const;
		
		int getOperand(int idx) const;
		
		bc::Op getOp() const;
		
		std::string toString() const;
	private:
		bc::Op mOp;
		std::vector<int> mOperands;
	};
	
	class Generator : Visitor
	{
	public:
		Generator(std::shared_ptr<Node> root);
		
		std::vector<Instruction> getInstructions();
		
		void visit(BasicExpression *expression);
		
		void visit(BinaryExpression *expression);
		
		void visit(FunctionDecl *function);
	private:
		
	private:
		std::shared_ptr<Node> mRoot;
		std::vector<Instruction> mOutput;
	};
	
	
}
