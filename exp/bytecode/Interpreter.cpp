//  Interpreter.cpp

#include <iostream>
#include <memory>

#include "Interpreter.hpp"
#include "ast.hpp"
#include "jc.h"
#include "builtin.hpp"

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

jcVariablePtr Interpreter::interpret(std::vector<bc::Instruction> instructions, int startingPoint)
{
    mapLabels(instructions);

    mVariableLut.push(std::map<std::string, jcVariablePtr>());
    mIp = startingPoint;
    bool shouldExit = false;
    while (shouldExit == false) {
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

            jcVariablePtr result = jcVariable::Create(performArtithmaticOp(op, resolveVariable(right)->asInt(), resolveVariable(left)->asInt()));
            mStack.push(result);
            break;
        }
        case bc::Push: {
            jcVariablePtr operand = instruction.getOperand(0);
            mStack.push(operand);
            break;
        }
        case bc::Pop: {
            jcVariablePtr variableName = instruction.getOperand(0);

            jcMutableVariablePtr value = jcMutableVariable::Create();
            if (!resolveRuntimeVariable(variableName->asString(), value)) {
                jcVariablePtr var = resolveVariable(popStack());
                if (var != nullptr) {
                    value = jcMutableVariable::Create(*var);
                }
            }

            mVariableLut.top()[variableName->asString()] = value;
            break;
        }
        case bc::Call: {
            callFunction(instruction);
            break;
        }
        case bc::JmpTrue:
        case bc::Jmp: {
            JC_ASSERT_OR_THROW(instruction.numOperands() == 1, "Invalid jmpTrue operands");
            JC_ASSERT_OR_THROW(instruction.getOperand(0)->getType() == jcVariable::TypeString, "Invalid jmpTrue operands");

            if (op == bc::JmpTrue) {
                bool shouldJump = resolveVariable(popStack())->asInt();
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
            if (mStack.top()->getType() != jcVariable::TypeInt) {
                mStack.push(resolveVariable(popStack()));
            }

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

void Interpreter::callFunction(bc::Instruction instruction)
{
    jcVariablePtr functionName = instruction.getOperand(0);
    if (mLabelLut.count(functionName->asString()) > 0) {
        pushIp();
        mVariableLut.push(std::map<std::string, jcVariablePtr>());
        mIp = mLabelLut[functionName->asString()];

        return;
    }

    auto builtinFunctionInfo = lib::builtin::Shared().info(functionName->asString());
    if (builtinFunctionInfo.count(lib::kLibError) == 0)
    {
        jcVariablePtr result = lib::builtin::Shared().execute(functionName->asString(), [this]() {
            return resolveVariable(popStack());
        });
        mStack.push(result);
        return;
    }

    JC_THROW(functionName->asString() + " does not exist");
}

jcVariablePtr Interpreter::resolveVariable(jcVariablePtr var)
{
    if (var->getType() == jcVariable::TypeInt) {
        return var;
    } else if (var->getType() == jcVariable::TypeCollection) {
        jcCollection *collection = var->asCollection();
        JC_ASSERT(collection != nullptr);

        std::vector<jcVariablePtr> resolvedCollection;
        for (int i = 0; i < (int)collection->size(); i++) {
            resolvedCollection.push_back(resolveVariable(collection->at(i)));
        }
        return jcVariable::Create(jcCollection(resolvedCollection));
    } else {
        jcMutableVariablePtr mutablePtr = jcMutableVariable::Create();
        if (resolveRuntimeVariable(var->asString(), mutablePtr)) {
            return mutablePtr;
        }

        JC_ASSERT_OR_THROW(mVariableLut.top().count(var->asString()) > 0, "undefined variable");
        return mVariableLut.top()[var->asString()];
    }
}

void Interpreter::setVariable(std::string var, jcVariablePtr to)
{
    // Check if runtime var
    if (var == bc::vars::ip) {
        mIp = resolveVariable(to)->asInt();
        return;
    }
    JC_ASSERT_OR_THROW(mVariableLut.top().count(var) > 0, "Cannot set undefined var");
    mVariableLut.top()[var] = resolveVariable(to);
}

bool Interpreter::resolveRuntimeVariable(std::string var, jcMutableVariablePtr output)
{
    if (var == bc::vars::ip) {
        if (output) {
            output->setInt(mIp);
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
