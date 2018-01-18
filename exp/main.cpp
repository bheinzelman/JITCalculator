//
//  main.cpp

#include "Calculator.hpp"
#include "Codegen.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include <iostream>

void do_codegen(std::string exp)
{
	Lexer lex(exp);
	Parser parser(std::make_shared<Lexer>(lex));
	auto ast = parser.parse();
	Codegen generator(ast);
	std::cout << generator.getCode() << std::endl;
}

void do_calculate(std::string exp)
{
	Calculator calc(exp);
	std::cout << calc.calculate() << std::endl;
}

int main(int argc, const char * argv[]) {
	std::string exp;
	std::getline(std::cin, exp);
	
	//do_calculate(exp);
	do_codegen(exp);
}


