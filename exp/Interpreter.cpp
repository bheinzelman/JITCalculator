//  Interpreter.cpp

#include <memory>
#include <iostream>

#include "Interpreter.hpp"
#include "ast.hpp"
#include "jc.h"

static int performArtithmaticOp(bc::Op op, int right, int left)
{
	switch (op) {
		case bc::Add:
			return left + right;
		case bc::Subtract:
			return left - right;
		case bc::Multiply:
			return left * right;
		case bc::Divide:
			return left / right;
		default:
			JC_FAIL();
			break;
	}
}

Interpreter::Interpreter(std::vector<bc::Instruction> instructions): mInstructions(instructions)
{
}

int Interpreter::interpret()
{
	for (auto instruction : mInstructions) {
		bc::Op op = instruction.getOp();
		
		switch (op) {
			case bc::Add:
			case bc::Subtract:
			case bc::Multiply:
			case bc::Divide: {
				int right = popStack();
				int left = popStack();
				mStack.push(performArtithmaticOp(op, right, left));
				break;
			}
			case bc::Push:
				mStack.push(instruction.getOperand(0));
				break;
			case bc::Pop:
				// not implemented
				JC_FAIL();
				break;
			default:
				JC_FAIL();
				break;
		}
	}
	JC_ASSERT(mStack.size() == 1);
	return popStack();
}

int Interpreter::popStack()
{
	JC_ASSERT(mStack.size());
	int top = mStack.top();
	mStack.pop();
	return top;
}
