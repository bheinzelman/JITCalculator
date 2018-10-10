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
	Interpreter(std::vector<bc::Instruction> entryPoint, SymbolTable &symbols);
	
	int interpret();
private:
	jcVariablePtr popStack();

    int resolveVariable(jcVariablePtr var);
	
private:
    std::stack<std::vector<bc::Instruction>> mInstructionStack;
	std::stack<jcVariablePtr> mStack;
    std::stack<std::map<std::string, int>> mVariableLut;
    SymbolTable mSymbols;
};
