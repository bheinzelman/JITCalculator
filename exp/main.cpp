//  main.cpp

#include "Calculator.hpp"
#include "Codegen.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

#include <iostream>
#include <fstream>

void do_codegen(std::string exp)
{
	Lexer lex(exp);
	Parser parser(std::make_shared<Lexer>(lex));
	auto ast = parser.parse();
	Codegen generator(ast);
	
	std::string code = generator.getCode();
	std::cout << code << std::endl;
	
	// output assembly to file
	std::ofstream outputStream("test/compute.s");
	outputStream << code;
	outputStream.close();
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


