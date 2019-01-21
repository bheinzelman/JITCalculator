//  Interpreter.cpp

#include <iostream>
#include <memory>

#include "Interpreter.hpp"
#include "jc.h"
#include "builtin.hpp"
#include "jcClosure.hpp"
#include "jcArray.hpp"
#include "jcList.hpp"

static inline int performArtithmaticOp(bc::Op op, int right, int left)
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

static inline int performPrefixOp(bc::Op op, int val)
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
            mLabelLut[instruction.getOperand()->asString()] = i;
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

jcVariablePtr Interpreter::interpret(jcVariablePtr callableObject, std::vector<jcVariablePtr> args)
{
    pushState();
    state().callSingleFunction = true;
    state().mIp = -1;
    state().callCount = 1;

    for (auto arg : args) {
        state().mStack.push(arg);
    }

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
    _state& curState = state();
    while (1) {
        bc::Instruction& instruction = mInstructions[curState.mIp++];

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
            curState.mStack.push(result);
            break;
        }
        case bc::Neg:
        case bc::Not: {
            jcVariablePtr top = popStack();
            jcVariablePtr result = jcVariable::Create(performPrefixOp(op, top->asInt()));
            curState.mStack.push(result);
            break;
        }
        case bc::Cons: {
            jcVariablePtr list = popStack();
            jcVariablePtr item = popStack();

            JC_ASSERT_OR_THROW_VM(list->getType() == jcVariable::TypeList, "Must cons to list");

            jcListPtr newList = std::shared_ptr<jcList>(list->asListRaw()->cons(item));
            curState.mStack.push(jcVariable::CreateFromCollection(newList));
            break;
        }
        case bc::Concat: {
            jcVariablePtr var1 = popStack();
            jcVariablePtr var2 = popStack();
            JC_ASSERT_OR_THROW_VM(var1->asCollection() && var2->asCollection(), "Concat params must be collections");

            jcCollection *collection1 = var1->asCollection();
            jcCollection *collection2 = var2->asCollection();

            std::shared_ptr<jcCollection> newCollection = std::shared_ptr<jcCollection>(collection2->concat(*collection1));
            curState.mStack.push(jcVariable::CreateFromCollection(newCollection));
            break;
        }
        case bc::Push: {
            JC_ASSERT(instruction.getOperand());
            jcVariablePtr operand = resolveVariable(instruction.getOperand());
            curState.mStack.push(operand);
            break;
        }
        case bc::PushC: {
            std::string closureName = instruction.getOperand()->asString();
            std::map<std::string, jcVariablePtr> scope = curState.mVariableLut.top();

            jcClosurePtr closure = std::make_shared<jcClosure>(closureName, scope);

            curState.mStack.push(jcVariable::Create(closure));
            break;
        }
        case bc::Pop: {
            jcVariablePtr variableName = instruction.getOperand();

            jcMutableVariablePtr value = jcMutableVariable::Create();
            if (resolveRuntimeVariable(variableName->asString(), value)) {
                curState.mVariableLut.top()[variableName->asString()] = value;
            } else {
                std::string name = variableName->asString();
                curState.mVariableLut.top()[name] = popStack();
            }

            break;
        }
        case bc::Call: {
            curState.callCount += 1;
            callFunction(popStack());
            break;
        }
        case bc::JmpTrue:
        case bc::Jmp: {
            JC_ASSERT_OR_THROW_VM(instruction.numOperands() == 1, "Invalid jmpTrue operands");
            JC_ASSERT_OR_THROW_VM(instruction.getOperand()->getType() == jcVariable::TypeString, "Invalid jmpTrue operands");

            if (op == bc::JmpTrue) {
                bool shouldJump = popStack()->asInt();
                if (shouldJump == false) {
                    break;
                }
            }

            std::string label = instruction.getOperand()->asString();
            JC_ASSERT_OR_THROW_VM(mLabelLut.count(label) > 0, "label " + label + " does not exist");
            curState.mIp = mLabelLut[label] + 1;

            break;
        }
        case bc::Ret: {
            popIp();
            curState.mVariableLut.pop();

            if ((--curState.callCount == 0) && curState.callSingleFunction) {
                goto Interpreter_Exit;
            }
            
            break;
        }
        case bc::Label:
            break;
        case bc::Exit:
            goto Interpreter_Exit;
        default:
            JC_FAIL();
            break;
        }
    }
Interpreter_Exit:
    return resolveVariable(popStack());
}

void Interpreter::callFunction(jcVariablePtr operand)
{
    std::string functionName = "";

    JC_ASSERT_OR_THROW_VM(operand->getType() == jcVariable::TypeString ||
                       operand->getType() == jcVariable::TypeClosure,
                       "Cannot call non-closure or non-id value"
                       );

    auto setupClosure = [&functionName, this](jcClosure* closure) {
        JC_ASSERT(closure);
        functionName = closure->name();

        auto scope = closure->scope();

        for (auto keyVal : scope) {
            state().mStack.push(keyVal.second);
        }
    };

    if (operand->getType() == jcVariable::TypeClosure) {
        jcClosure* closure = operand->asClosureRaw();
        setupClosure(closure);
    } else if (state().mVariableLut.top().count(operand->asString()) > 0) {
        jcVariablePtr functionVar = state().mVariableLut.top()[operand->asString()];
        if (functionVar->getType() == jcVariable::TypeClosure) {
            setupClosure(functionVar->asClosureRaw());
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
        state().callCount -= 1;
        return;
    }

    JC_THROW_VM_EXCEPTION(operand->asString() + " does not exist");
}

jcVariablePtr Interpreter::resolveVariable(const jcVariablePtr &var)
{
    if (var->getType() == jcVariable::TypeInt) {
        return var;
    } else if (var->getType() == jcVariable::TypeArray || var->getType() == jcVariable::TypeList) {
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

        JC_THROW_VM_EXCEPTION("undefined variable: " + var->asString());
    }
}

bool Interpreter::functionExists(const jcVariablePtr &var) const
{
    std::string functionName = "";
    if (var->getType() == jcVariable::TypeClosure) {
        functionName = var->asClosureRaw()->name();
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

void Interpreter::setVariable(std::string var, jcVariablePtr &to)
{
    // Check if runtime var
    if (var == bc::vars::ip) {
        state().mIp = resolveVariable(to)->asInt();
        return;
    }
    JC_ASSERT_OR_THROW_VM(state().mVariableLut.top().count(var) > 0, "Cannot set undefined var");
    state().mVariableLut.top()[var] = resolveVariable(to);
}

bool Interpreter::resolveRuntimeVariable(std::string var, jcMutableVariablePtr &output)
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
    _state& curState = state();
    JC_ASSERT_OR_THROW_VM(curState.mStack.size(), "Popping empty vm-stack!");
    auto top = curState.mStack.top();
    curState.mStack.pop();
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
