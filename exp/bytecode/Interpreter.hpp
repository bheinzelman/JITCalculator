// Interpreter.hpp

#pragma once

#include <map>
#include <memory>
#include <stack>

#include "SymbolTable.hpp"
#include "Visitor.h"
#include "ast.hpp"

#include "bc.hpp"

class Interpreter {
public:
    Interpreter();

    int interpret(std::vector<bc::Instruction> instructions, int startingPoint);

private:
    jcVariablePtr popStack();

    /**
     Resolves variables to literal values..
     */
    jcVariablePtr resolveVariable(jcVariablePtr var);
    bool resolveRuntimeVariable(std::string var, jcMutableVariablePtr output);
    void setVariable(std::string var, jcVariablePtr to);

    void mapLabels(std::vector<bc::Instruction> instructions);

    // push and pop instruction pointer
    void pushIp();
    void popIp();

    void callFunction(bc::Instruction instruction);

private:
    std::stack<jcVariablePtr> mStack;
    std::stack<int> mIpStack;
    std::stack<std::map<std::string, jcVariablePtr>> mVariableLut;

    // instruction pointer
    int mIp;

    std::map<std::string, int> mLabelLut;
};
