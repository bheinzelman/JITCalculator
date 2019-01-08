//  main.cpp

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Runtime.hpp"
#include "jc.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <cstdio>
#include <readline/history.h>
#include <readline/readline.h>

std::string getErrorMessage(const jcException &exception)
{
    switch (exception.getDomain()) {
        case jcException::Domain::Vm:
            return std::string("VM Exception... ") + exception.getMessage();
        case jcException::Domain::Codegen:
            return std::string("Codegen Exception... ") + exception.getMessage();
        case jcException::Domain::Parse:
            return std::string("Parse Exception... ") + exception.getMessage() + "\nLine: " + std::to_string(exception.getLine());
    }
}

void run_shell(std::ostream& stream)
{
    stream << "JITCalculator v" << JC_VERSION_STRING << "\n";

    Runtime runtime;
    while (true) {
        const char* rawIn = readline(">>> ");

        JC_ASSERT(rawIn);
        add_history(rawIn);
        std::string exp = std::string(rawIn);

        if (exp.size() == 0) {
            continue;
        }

        // check for commands
        if (exp.size() >= 2 && exp[0] == '.') {
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
                std::vector<jcVariablePtr> output;

                std::stringstream stream;
                stream << exp;

                runtime.evaluateREPL(stream, output);

                for (auto outputValue : output) {
                    std::cout << outputValue->stringRepresentation() << std::endl;
                }

            } catch (jcException exception) {
                stream << getErrorMessage(exception) << std::endl;
            }
        }
    }
}

void run_file(std::string filename)
{
    std::ifstream inputStream;
    inputStream.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (inputStream.is_open() == false) {
        std::cerr << "Could not open file " << filename;
        return;
    }
    try {
        Runtime::evaluate(inputStream);
    } catch (jcException exception) {
        std::cerr << getErrorMessage(exception) << std::endl;
    }

    inputStream.close();
}

int main(int argc, const char* argv[])
{
    if (argc >= 2) {
        run_file(std::string(argv[1]));
    } else {
        run_shell(std::cout);
    }
}
