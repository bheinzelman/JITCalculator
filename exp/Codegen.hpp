// Codegen.hpp

#pragma once

#include <memory>
#include <string>
#include <stack>
#include <queue>

#include "Visitor.h"
#include "ast.hpp"

struct Operand {
	std::string value;
	bool isRegister;
	static Operand operandWithRegisterName(std::string name);
	static Operand operandWithNumberValue(std::string value);
};

class Codegen : public Visitor
{
public:
	Codegen(std::shared_ptr<Expression> ast);
	
	std::string getCode();
	
	void visit(BasicExpression *expression);
	
	void visit(BinaryExpression *expression);
private:
	std::shared_ptr<Expression> ast;
	std::stack<Operand> stack;
	std::queue<Operand> registerQueue;
	std::string code;
	
private:
	void initCodegen();
	void finalizeCodegen();
	
	Operand getOperand();
	
	void add(std::string source, std::string dest);
	void mult(std::string source, std::string dest);
	void sub(std::string source, std::string dest);
	void divide(std::string source, std::string dest);
	void mov(std::string source, std::string dest);
	
	void addInstruction(std::string instruction, std::string source, std::string dest);
};
