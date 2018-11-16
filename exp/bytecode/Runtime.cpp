//  Calculator.cpp

#include "jc.h"

#include "Interpreter.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Runtime.hpp"
#include "Token.hpp"
#include "Visitor.h"
#include "ast.hpp"

#include "bc.hpp"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>

Runtime::Runtime()
{
}

bool Runtime::evaluate(std::istream& stream, std::vector<jcVariablePtr>& outputValues)
{
    Lexer lex(stream);
    Parser parser(std::make_shared<Lexer>(lex));
    std::vector<std::shared_ptr<Node>> nodes = parser.parse();

    if (nodes.size() == 0) {
        return false;
    }

    bc::Generator bcGenerator;

    for (auto ast : nodes) {
        JC_ASSERT(ast);

        auto output = bcGenerator.getInstructions(ast);
        auto closures = bcGenerator.getClosureInstructions();

        if (output.size()) {
            if (ast->type() == kFunctionDeclType) {
                std::shared_ptr<FunctionDecl> functionDecl = std::static_pointer_cast<FunctionDecl>(ast);
                mDefinitions.insert(mDefinitions.end(), output.begin(), output.end());
                mDefinitions.insert(mDefinitions.end(), closures.begin(), closures.end());

                continue;
            }

            output.push_back(bc::Instruction(bc::Exit, {}));

            Interpreter interpreter;

            output.insert(output.end(), mDefinitions.begin(), mDefinitions.end());

            output.insert(output.end(), closures.begin(), closures.end());

#if 0
            for (auto instruction : output) {
                std::cout << instruction.toString() << std::endl;
            }
#endif

            outputValues.push_back(interpreter.interpret(output, 0));
        }
    }
    return true;
}
