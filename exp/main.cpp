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

void evaluate_stream(std::istream& inputStream, std::ostream& outputStream, Runtime& rt, bool printResults)
{
    std::vector<jcVariablePtr> output;
    try {
        if (!rt.evaluate(inputStream, output)) {
            outputStream << "Error evaluation" << std::endl;
            return;
        }
        if (printResults) {
            for (auto outputValue : output) {
                outputStream << outputValue->stringRepresentation() << std::endl;
            }
        }
    } catch (jcException exception) {
        outputStream << "jcException..." << exception.getMessage() << std::endl;
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
                std::stringstream stream;
                stream << exp;

                evaluate_stream(stream, std::cout, runtime, true);

            } catch (jcException excecption) {
                stream << "jcException... " << excecption.getMessage() << std::endl;
            }
        }
    }
}

void run_file(std::string filename)
{
    std::ifstream inputStream;
    inputStream.open(filename.c_str(), std::ifstream::in | std::ifstream::binary);
    if (inputStream.is_open() == false) {
        std::cout << "Could not open file " << filename;
        return;
    }

    Runtime runtime;
    evaluate_stream(inputStream, std::cout, runtime, false);

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
