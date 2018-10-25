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
        case bc::Greater_Than:
            return left > right;
        case bc::Less_Than:
            return left < right;
        case bc::Greater_Than_Equal:
            return left >= right;
        case bc::Less_Than_Equal:
            return left <= right;
        case bc::Equals:
            return left == right;
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
			case bc::Divide:
            case bc::Greater_Than:
            case bc::Less_Than:
            case bc::Less_Than_Equal:
            case bc::Greater_Than_Equal:
            case bc::Equals: {
				jcVariablePtr right = popStack();
				jcVariablePtr left = popStack();

                jcVariablePtr result = jcVariable::Create(performArtithmaticOp(op, resolveVariable(right), resolveVariable(left)));
				mStack.push(result);
				break;
			}
            case bc::Push: {
                jcVariablePtr operand = instruction.getOperand(0);
                if (operand->getType() == jcVariable::TypeInt) {
                    mStack.push(operand);
                } else {
                    mStack.push(jcVariable::Create(resolveVariable(operand)));
                }
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
            case bc::Call: {
                jcVariablePtr functionName = instruction.getOperand(0);
                JC_ASSERT_OR_THROW(mLabelLut.count(functionName->asString()) > 0, "Function " + functionName->asString() + " does not exist");
                pushIp();
                mVariableLut.push(std::map<std::string, int>());
                mIp = mLabelLut[functionName->asString()];
                break;
            }
            case bc::JmpTrue:
            case bc::Jmp: {
                JC_ASSERT_OR_THROW(instruction.numOperands() == 1, "Invalid jmpTrue operands");
                JC_ASSERT_OR_THROW(instruction.getOperand(0)->getType() == jcVariable::TypeString, "Invalid jmpTrue operands");

                if (op == bc::JmpTrue) {
                    bool shouldJump = resolveVariable(popStack());
                    if (shouldJump == false) {
                        break;
                    }
                }

                std::string label = instruction.getOperand(0)->asString();
                JC_ASSERT_OR_THROW(mLabelLut.count(label) > 0, "label " + label + " does not exist");
                mIp = mLabelLut[label];

                break;
            }
            case bc::Ret: {
                popIp();
                mVariableLut.pop();
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

	return resolveVariable(popStack());
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

void Interpreter::pushIp()
{
    mIpStack.push(mIp);
}

void Interpreter::popIp()
{
    mIp = mIpStack.top();
    mIpStack.pop();
}

