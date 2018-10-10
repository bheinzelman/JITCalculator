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

Interpreter::Interpreter(std::vector<bc::Instruction> instructions, SymbolTable &symbols): mSymbols(symbols)
{
    mInstructionStack.push(instructions);
}

int Interpreter::interpret()
{
    mVariableLut.push(std::map<std::string, int>());

	for (auto instruction : mInstructionStack.top()) {
		bc::Op op = instruction.getOp();
		
		switch (op) {
			case bc::Add:
			case bc::Subtract:
			case bc::Multiply:
			case bc::Divide: {
				jcVariablePtr right = popStack();
				jcVariablePtr left = popStack();

                jcVariablePtr result = jcVariable::Create();
                result->setInt(performArtithmaticOp(op, resolveVariable(right), resolveVariable(left)));
				mStack.push(result);
				break;
			}
			case bc::Push:
				mStack.push(instruction.getOperand(0));
				break;
            case bc::Pop: {
                jcVariablePtr variableName = instruction.getOperand(0);
                mVariableLut.top()[variableName->asString()] = resolveVariable(popStack());
				break;
            }
            case bc::Call: {
                jcVariablePtr functionName = instruction.getOperand(0);
                auto instructionsCtx = mSymbols.getContext(functionName->asString());
                mInstructionStack.push(instructionsCtx->getInstructions());

                int returnValue = interpret();
                jcVariablePtr returnValuePtr = jcVariable::Create();
                returnValuePtr->setInt(returnValue);
                mStack.push(returnValuePtr);
                break;
            }
			default:
				JC_FAIL();
				break;
		}
	}
    mInstructionStack.pop();
    mVariableLut.pop();

	return popStack()->asInt();
}

int Interpreter::resolveVariable(jcVariablePtr var)
{
    if (var->getType() == jcVariable::TypeInt) {
        return var->asInt();
    } else {
        JC_ASSERT_OR_THROW(mVariableLut.top().count(var->asString()) > 0, "undefined variable");
        return mVariableLut.top()[var->asString()];
    }
}

jcVariablePtr Interpreter::popStack()
{
	JC_ASSERT(mStack.size());
	auto top = mStack.top();
	mStack.pop();
	return top;
}
