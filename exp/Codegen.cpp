//  Codegen.cpp

#include "Codegen.hpp"
#include <iostream>

static std::string MOV = "movl";
static std::string ADD = "addl";
static std::string SUB = "subl";
static std::string MUL = "imull";
static std::string DIV = "idivl";

Operand Operand::operandWithRegisterName(std::string name)
{
	Operand op;
	op.value = name;
	op.isRegister = true;
	return op;
}

Operand Operand::operandWithNumberValue(std::string value)
{
	Operand op;
	op.value = value;
	op.isRegister = false;
	return op;
}

Codegen::Codegen(std::shared_ptr<Expression> ast) : ast(ast)
{
	code = "";
	Operand ecx = Operand::operandWithRegisterName("%ecx");
	Operand edx = Operand::operandWithRegisterName("%edx");
	
	registerQueue.push(ecx);
	registerQueue.push(edx);
}

std::string Codegen::getCode()
{
	initCodegen();
	
	ast->accept(this);
	
	Operand result = stack.top();
	mov(result.value, "%eax");
	
	finalizeCodegen();
	return this->code;
}

void Codegen::initCodegen()
{
	code += "\t.globl _test\n";
	code += "_test:\n";
	code += "\tpushq %rbp\n";
	code += "\tmovq %rsp, %rbp\n";
}

void Codegen::finalizeCodegen()
{
	code += "\tpopq %rbp\n";
	code += "\tretq\n";
}

void Codegen::visit(BasicExpression *expression)
{
	Operand op;
	op.value = "$" + std::to_string(expression->getValue());
	op.isRegister = false;
	
	stack.push(op);
}

void Codegen::visit(BinaryExpression *expression)
{
	
	expression->getLeft()->accept(this);
	
	Token op = expression->getOperator();
	
	expression->getRight()->accept(this);
	
	Operand source = getOperand();
	Operand dest = getOperand();
	
	if (!dest.isRegister) {
		Operand reg = registerQueue.front();
		registerQueue.pop();
		mov(dest.value, reg.value);
		dest = reg;
	}

	switch (op) {
		case Add:
			add(source.value, dest.value);
			break;
		case Subtract:
			sub(source.value, dest.value);
			break;
		case Multiply:
			mult(source.value, dest.value);
			break;
		case Divide:
			divide(source.value, dest.value);
			break;
		default:
			return;
	}
	
	if (source.isRegister) {
		registerQueue.push(source);
	}
	
	stack.push(dest);
}

Operand Codegen::getOperand() {
	auto top = stack.top();
	stack.pop();
	return top;
}

void Codegen::add(std::string source, std::string dest)
{
	addInstruction(ADD, source, dest);
}

void Codegen::mult(std::string source, std::string dest)
{
	addInstruction(MUL, source, dest);
}

void Codegen::sub(std::string source, std::string dest)
{
	addInstruction(SUB, source, dest);
}

void Codegen::divide(std::string source, std::string dest)
{
	addInstruction(DIV, source, dest);
}

void Codegen::mov(std::string source, std::string dest)
{
	addInstruction(MOV, source, dest);
}

void Codegen::addInstruction(std::string instruction, std::string source, std::string dest)
{
	code += "\t" + instruction + " " + source + ", " + dest + "\n";
}
