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
    jcVariablePtr interpret(int startingPoint);

    /**
     Calls the function within the callable object
     and returns the top of the stack
     */
    jcVariablePtr interpret(jcVariablePtr callableObject);

    void setInstructions(const std::vector<bc::Instruction> &instructions);

    jcVariablePtr popStack();

private:
    /**
     Resolves variables to literal values..
     */
    jcVariablePtr resolveVariable(jcVariablePtr var);
    jcVariablePtr resolveFunction(jcVariablePtr var);
    bool resolveRuntimeVariable(std::string var, jcMutableVariablePtr output);
    void setVariable(std::string var, jcVariablePtr to);

    void mapLabels(std::vector<bc::Instruction> instructions);

    // push and pop instruction pointer
    void pushIp();
    void popIp();

    void callFunction(jcVariablePtr operand);

    bool functionExists(jcVariablePtr var) const;

private:
    std::stack<jcVariablePtr> mStack;
    std::stack<int> mIpStack;
    std::stack<std::map<std::string, jcVariablePtr>> mVariableLut;

    // instruction pointer
    int mIp;

    std::map<std::string, int> mLabelLut;

    std::vector<bc::Instruction> mInstructions;
    
};
