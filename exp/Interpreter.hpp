// Interpreter.hpp

#pragma once

#include <memory>
#include <stack>
#include <map>

#include "Visitor.h"
#include "SymbolTable.hpp"
#include "ast.hpp"

#include "bc.hpp"

class Interpreter
{
public:
	Interpreter();
	
	int interpret(std::vector<bc::Instruction> instructions, int startingPoint);
private:
	jcVariablePtr popStack();

    int resolveVariable(jcVariablePtr var);
    bool resolveRuntimeVariable(std::string var, int *output);
    void setVariable(std::string var, jcVariablePtr to);

    void mapLabels(std::vector<bc::Instruction> instructions);
	
private:
	std::stack<jcVariablePtr> mStack;
    std::stack<std::map<std::string, int>> mVariableLut;

    // instruction pointer
    int mIp;

    std::map<std::string, int> mLabelLut;
};
