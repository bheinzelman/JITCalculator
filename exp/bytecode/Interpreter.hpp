// Interpreter.hpp

#pragma once

#include <map>
#include <memory>
#include <stack>
#include <functional>

#include "ast.hpp"

#include "bc.hpp"

class Interpreter {
public:
    Interpreter();

    /**
     Returns value on the top of the stack
     */
    jcVariablePtr interpret();

    /**
     Calls the function within the callable object
     and returns the top of the stack
     */
    jcVariablePtr interpret(jcVariablePtr callableObject, std::vector<jcVariablePtr> args = {});

    void setInstructions(const std::vector<bc::Instruction> &instructions);

    jcVariablePtr popStack();

private:
    /**
     Returns value on the top of the stack
     */
    jcVariablePtr eval();

    /**
     Resolves variables to literal values..
     */
    jcVariablePtr resolveVariable(const jcVariablePtr &var);
    bool resolveRuntimeVariable(std::string var, jcMutableVariablePtr &output);
    void setVariable(std::string var, jcVariablePtr &to);

    void mapLabels(std::vector<bc::Instruction> instructions);

    // push and pop instruction pointer
    void pushIp();
    void popIp();

    /**
     Calls the function with the given operand
     If the operand is a builtin function, it will be called.
     Otherwise the instruction pointer will be set to the function to
     be called.
     */
    void callFunction(jcVariablePtr operand);

    bool functionExists(const jcVariablePtr &var) const;

private:

    struct _state {
        std::stack<jcVariablePtr> mStack;
        std::stack<int> mIpStack;
        std::stack<std::map<std::string, jcVariablePtr>> mVariableLut;

        // instruction pointer
        int mIp=0;

        bool callSingleFunction=false;
        int callCount=0;
    };

    inline _state& state() {
        JC_ASSERT(mState.size());
        return mState.top();
    }
    
    void pushState();
    void popState();

    std::stack<_state> mState;

    std::map<std::string, int> mLabelLut;

    std::vector<bc::Instruction> mInstructions;
    
};
