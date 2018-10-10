//  main.cpp

#include "Runtime.hpp"
#include "Codegen.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "jc.h"

#include <iostream>
#include <fstream>

#include <cstdio>
#include <readline/readline.h>
#include <readline/history.h>


void run_shell(std::ostream &stream) {
	stream << "JITCalculator v" << JC_VERSION_STRING << "\n";

    Runtime runtime;
	while (true) {
		const char *rawIn = readline(">>> ");
		
		JC_ASSERT(rawIn);
		add_history(rawIn);
		std::string exp = std::string(rawIn);
		
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
			try {
                int output;
                if (runtime.evaluate(exp, &output)) {
                    stream << output << std::endl;
                }
			} catch (jcException excecption) {
				stream << "jcException... " << excecption.getMessage() << std::endl;
			}
		}
	}
}

int main(int argc, const char * argv[]) {
	run_shell(std::cout);
}




