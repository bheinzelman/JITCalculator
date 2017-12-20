//
//  main.cpp

#include "Calculator.hpp"
#include <iostream>

int main(int argc, const char * argv[]) {
	std::string exp;
	std::getline(std::cin, exp);
	
	Calculator calc(exp);
	
	std::cout << calc.calculate() << std::endl;
}
