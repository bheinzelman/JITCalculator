//  Interpreter.cpp

#include <memory>

#include "Interpreter.hpp"
#include "ast.hpp"

Interpreter::Interpreter(std::shared_ptr<Expression> expression)
{
	this->expression = expression;
}

int Interpreter::interpret()
{
	expression->accept(this);
	return lastValue;
}

void Interpreter::visit(BasicExpression *expression)
{
	lastValue = expression->getValue();
}

void Interpreter::visit(BinaryExpression *expression)
{
	expression->getLeft()->accept(this);
	int left = lastValue;
	
	Token op = expression->getOperator();
	
	expression->getRight()->accept(this);
	int right = lastValue;
	
	switch (op) {
		case Add:
			lastValue = left + right;
			break;
		case Subtract:
			lastValue = left - right;
			break;
		case Multiply:
			lastValue = left * right;
			break;
		case Divide:
			lastValue = left / right;
			break;
		default:
			return;
	}
	
}

