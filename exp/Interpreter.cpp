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

Interpreter::Interpreter()
{
}

void Interpreter::mapLabels(std::vector<bc::Instruction> instructions)
{
    for (int i = 0; i < instructions.size(); i++) {
        bc::Instruction instruction = instructions[i];
        if (instruction.getOp() == bc::Label) {
            mLabelLut[instruction.getOperand(0)->asString()] = i;
        }
    }
}

int Interpreter::interpret(std::vector<bc::Instruction> instructions, int startingPoint)
{
    mapLabels(instructions);

    mVariableLut.push(std::map<std::string, int>());
    mIp = startingPoint;
    bool shouldExit = false;
    while(shouldExit == false) {
        bc::Instruction instruction = instructions[mIp++];

		bc::Op op = instruction.getOp();
		
		switch (op) {
			case bc::Add:
			case bc::Subtract:
			case bc::Multiply:
			case bc::Divide: {
				jcVariablePtr right = popStack();
				jcVariablePtr left = popStack();

                jcVariablePtr result = jcVariable::Create(performArtithmaticOp(op, resolveVariable(right), resolveVariable(left)));
				mStack.push(result);
				break;
			}
            case bc::Push: {
                mStack.push(instruction.getOperand(0));
				break;
            }
            case bc::Pop: {
                jcVariablePtr variableName = instruction.getOperand(0);

                int value;
                if (!resolveRuntimeVariable(variableName->asString(), &value)) {
                    value = resolveVariable(popStack());
                }

                mVariableLut.top()[variableName->asString()] = value;
				break;
            }
            case bc::Set: {
                JC_ASSERT_OR_THROW(instruction.numOperands() == 2, "Invalid number of args for SET");
                // Form SET x, y, x = y

                jcVariablePtr left = instruction.getOperand(0);
                jcVariablePtr right = instruction.getOperand(1);

                setVariable(left->asString(), right);

                break;
            }
            case bc::Call: {
                jcVariablePtr functionName = instruction.getOperand(0);
                JC_ASSERT_OR_THROW(mLabelLut.count(functionName->asString()) > 0, "Function " + functionName->asString() + " does not exist");
                mIp = mLabelLut[functionName->asString()];
                break;
            }
            case bc::Ret: {
                break;
            }
            case bc::Label:
                break;
            case bc::Exit:
                shouldExit = true;
                break;
			default:
				JC_FAIL();
				break;
		}
	}

    mVariableLut.pop();

	return popStack()->asInt();
}

int Interpreter::resolveVariable(jcVariablePtr var)
{
    if (var->getType() == jcVariable::TypeInt) {
        return var->asInt();
    } else {
        int value;
        if (resolveRuntimeVariable(var->asString(), &value)) {
            return value;
        }

        JC_ASSERT_OR_THROW(mVariableLut.top().count(var->asString()) > 0, "undefined variable");
        return mVariableLut.top()[var->asString()];
    }
}

void Interpreter::setVariable(std::string var, jcVariablePtr to)
{
    // Check if runtime var
    if (var == bc::vars::ip) {
        mIp = resolveVariable(to);
        return;
    }
    JC_ASSERT_OR_THROW(mVariableLut.top().count(var) > 0, "Cannot set undefined var");
    mVariableLut.top()[var] = resolveVariable(to);
}

bool Interpreter::resolveRuntimeVariable(std::string var, int *output)
{
    if (var == bc::vars::ip) {
        if (output) {
            *output = mIp;
        }
        return true;
    }
    return false;
}

jcVariablePtr Interpreter::popStack()
{
	JC_ASSERT(mStack.size());
	auto top = mStack.top();
	mStack.pop();
	return top;
}
