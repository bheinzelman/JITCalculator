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

static int performPrefixOp(bc::Op op, int val)
{
    switch (op) {
        case bc::Neg:
            return -val;
        case bc::Not:
            return !val;
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

void Interpreter::setInstructions(const std::vector<bc::Instruction> &instructions)
{
    mInstructions = instructions;
    mapLabels(mInstructions);
}

jcVariablePtr Interpreter::interpret()
{
    pushState();
    auto value = eval();
    popState();
    return value;
}

jcVariablePtr Interpreter::interpret(jcVariablePtr callableObject)
{
    pushState();
    state().callSingleFunction = true;
    state().mIp = -1;
    state().callCount = 1;

    callFunction(callableObject);
    jcVariablePtr returnValue = nullptr;
    if (state().mIp != -1) {
        returnValue = eval();
    } else {
        returnValue = popStack();
    }
    popState();
    return returnValue;
}

jcVariablePtr Interpreter::eval()
{
    bool shouldExit = false;
    while (shouldExit == false) {
        bc::Instruction instruction = mInstructions[state().mIp++];

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

            jcVariablePtr result = jcVariable::Create(performArtithmaticOp(op, right->asInt(), left->asInt()));
            state().mStack.push(result);
            break;
        }
        case bc::Neg:
        case bc::Not: {
            jcVariablePtr top = popStack();
            jcVariablePtr result = jcVariable::Create(performPrefixOp(op, top->asInt()));
            state().mStack.push(result);
            break;
        }
        case bc::Push: {
            jcVariablePtr operand = resolveVariable(instruction.getOperand(0));
            state().mStack.push(operand);
            break;
        }
        case bc::PushC: {
            std::string closureName = instruction.getOperand(0)->asString();
            std::map<std::string, jcVariablePtr> scope = state().mVariableLut.top();

            jcClosure closure = jcClosure(closureName, scope);

            state().mStack.push(jcVariable::Create(closure));
            break;
        }
        case bc::Pop: {
            jcVariablePtr variableName = instruction.getOperand(0);

            jcMutableVariablePtr value = jcMutableVariable::Create();
            if (resolveRuntimeVariable(variableName->asString(), value)) {
                state().mVariableLut.top()[variableName->asString()] = value;
            } else {
                state().mVariableLut.top()[variableName->asString()] = popStack();
            }

            break;
        }
        case bc::Call: {
            state().callCount += 1;
            callFunction(popStack());
            break;
        }
        case bc::JmpTrue:
        case bc::Jmp: {
            JC_ASSERT_OR_THROW(instruction.numOperands() == 1, "Invalid jmpTrue operands");
            JC_ASSERT_OR_THROW(instruction.getOperand(0)->getType() == jcVariable::TypeString, "Invalid jmpTrue operands");

            if (op == bc::JmpTrue) {
                bool shouldJump = popStack()->asInt();
                if (shouldJump == false) {
                    break;
                }
            }

            std::string label = instruction.getOperand(0)->asString();
            JC_ASSERT_OR_THROW(mLabelLut.count(label) > 0, "label " + label + " does not exist");
            state().mIp = mLabelLut[label];

            break;
        }
        case bc::Ret: {
            popIp();
            state().mVariableLut.pop();

            if ((--state().callCount == 0) && state().callSingleFunction) {
                shouldExit = true;
            }
            
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

void Interpreter::callFunction(jcVariablePtr operand)
{
    std::string functionName = "";

    JC_ASSERT_OR_THROW(operand->getType() == jcVariable::TypeString ||
                       operand->getType() == jcVariable::TypeClosure,
                       "Cannot call non-closure or non-id value"
                       );

    auto setupClosure = [&functionName, this](jcClosure *closure) {
        JC_ASSERT(closure);
        functionName = closure->name();

        auto scope = closure->scope();

        for (auto keyVal : scope) {
            state().mStack.push(keyVal.second);
        }
    };

    if (operand->getType() == jcVariable::TypeClosure) {
        jcClosure *closure = operand->asClosure();
        setupClosure(closure);
    } else if (state().mVariableLut.top().count(operand->asString()) > 0) {
        jcVariablePtr functionVar = state().mVariableLut.top()[operand->asString()];
        if (functionVar->getType() == jcVariable::TypeClosure) {
            setupClosure(functionVar->asClosure());
        } else {
            functionName = functionVar->asString();
        }
    } else {
        functionName = operand->asString();
    }

    if (functionName.size() > 0 && mLabelLut.count(functionName) > 0) {
        pushIp();
        state().mVariableLut.push(std::map<std::string, jcVariablePtr>());
        state().mIp = mLabelLut[functionName];

        return;
    }

    auto builtinFunctionInfo = lib::builtin::Shared().info(functionName);
    if (builtinFunctionInfo.count(lib::kLibError) == 0)
    {
        jcVariablePtr result = lib::builtin::Shared().execute(functionName, *this);
        state().mStack.push(result);
        return;
    }

    JC_THROW(operand->asString() + " does not exist");
}

jcVariablePtr Interpreter::resolveVariable(jcVariablePtr var)
{
    if (var->getType() == jcVariable::TypeInt) {
        return var;
    } else if (var->getType() == jcVariable::TypeCollection) {
        return var;
    } else {
        if (state().mVariableLut.top().count(var->asString()) > 0) {
            return state().mVariableLut.top()[var->asString()];
        }

        // if a function is defined with this value let it through
        if (functionExists(var)) {
            return var;
        }

        jcMutableVariablePtr mutablePtr = jcMutableVariable::Create();
        if (resolveRuntimeVariable(var->asString(), mutablePtr)) {
            return mutablePtr;
        }

        JC_THROW("undefined variable: " + var->asString());
    }
}

bool Interpreter::functionExists(jcVariablePtr var) const
{
    std::string functionName = "";
    if (var->getType() == jcVariable::TypeClosure) {
        functionName = var->asClosure()->name();
    } else {
        functionName = var->asString();
    }

    if (mLabelLut.count(functionName) > 0) {
        return true;
    }

    auto info = lib::builtin::Shared().info(functionName);
    if (info.count(lib::kLibError) == 0) {
        return true;
    }

    return false;
}

void Interpreter::setVariable(std::string var, jcVariablePtr to)
{
    // Check if runtime var
    if (var == bc::vars::ip) {
        state().mIp = resolveVariable(to)->asInt();
        return;
    }
    JC_ASSERT_OR_THROW(state().mVariableLut.top().count(var) > 0, "Cannot set undefined var");
    state().mVariableLut.top()[var] = resolveVariable(to);
}

bool Interpreter::resolveRuntimeVariable(std::string var, jcMutableVariablePtr output)
{
    if (var == bc::vars::ip) {
        if (output) {
            output->setInt(state().mIp);
        }
        return true;
    }
    return false;
}

jcVariablePtr Interpreter::popStack()
{
    JC_ASSERT(state().mStack.size());
    auto top = state().mStack.top();
    state().mStack.pop();
    return top;
}

void Interpreter::pushIp()
{
    state().mIpStack.push(state().mIp);
}

void Interpreter::popIp()
{
    JC_ASSERT(state().mIpStack.size());
    state().mIp = state().mIpStack.top();
    state().mIpStack.pop();
}

void Interpreter::pushState()
{
    Interpreter::_state newState;
    newState.mVariableLut.push(std::map<std::string, jcVariablePtr>());
    mState.push(newState);

}
void Interpreter::popState()
{
    mState.pop();
}
