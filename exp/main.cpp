//  main.cpp

#include "Calculator.hpp"
#include "Codegen.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

#include <iostream>
#include <fstream>

#define VERSION_STRING "0.0.0"

void run_shell(std::ostream &stream) {
	stream << "JITCalculator v" << VERSION_STRING << "\n";
	while (true) {
		stream << ">>> ";
		std::string exp;
		std::getline(std::cin, exp);
		
		if (exp.size() == 0) {
			continue;
		}
		
		// check for commands
		if (exp.size() == 2 && exp[0] == '.') {
			char cmd = exp[1];
			switch (cmd) {
				case 'q':
					// quit
					return;
				case 'h':
					// help
					continue;
				default:
					stream << "Unknown command\n";
					break;
			}
		
		} else {
			Calculator c(exp);
			stream << c.calculate() << "\n";
		}
	}
}

int main(int argc, const char * argv[]) {
	run_shell(std::cout);
}




