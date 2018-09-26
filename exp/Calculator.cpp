//  Calculator.cpp

#include "jc.h"

#include "Calculator.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "Visitor.h"
#include "ast.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

#include "bc.hpp"

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
	
	JC_ASSERT(ast);
	
	bc::Generator bcGenerator(ast);
	auto output = bcGenerator.getInstructions();
#if 0
	for (auto instruction : output) {
		std::cout << instruction.toString() << std::endl;
	}
#endif
	
	Interpreter interpreter(output);
	return interpreter.interpret();
}
