// Interpreter.hpp

#pragma once

#include <memory>
#include <stack>

#include "Visitor.h"
#include "ast.hpp"

#include "bc.hpp"

class Interpreter
{
public:
	Interpreter(std::vector<bc::Instruction> instructions);
	
	int interpret();
private:
	int popStack();
	
private:
	std::vector<bc::Instruction> mInstructions;
	std::stack<int> mStack;
};
