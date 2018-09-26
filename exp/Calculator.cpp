//  Calculator.cpp

#include "Calculator.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Visitor.h"
#include "ast.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

#include <iostream>
#include <memory>
#include <cstring>
#include <cstdlib>

Calculator::Calculator(std::string expression) : expression(expression)
{
}

int Calculator::calculate() const
{
	Lexer lex(this->expression);
	Parser parser(std::make_shared<Lexer>(lex));
	auto ast = parser.parse();
	
	assert(ast);
	
	Interpreter interpreter(ast);
	return interpreter.interpret();
	
}
