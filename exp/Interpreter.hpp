// Interpreter.hpp

#ifndef Interpreter_hpp
#define Interpreter_hpp

#include <memory>

#include "Visitor.h"
#include "ast.hpp"

class Interpreter : Visitor
{
public:
	Interpreter(std::shared_ptr<Expression> expression);
	
	int interpret();
	
	void visit(BasicExpression *expression);
	
	void visit(BinaryExpression *expression);
	
private:
	int lastValue;
	std::shared_ptr<Expression> expression;
};

#endif
