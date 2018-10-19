//  Calculator.cpp

#include "jc.h"

#include "Runtime.hpp"
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

Runtime::Runtime()
{
}

bool Runtime::evaluate(std::string expression, int *value)
{
	Lexer lex(expression);
	Parser parser(std::make_shared<Lexer>(lex));
	auto ast = parser.parse();
	
	JC_ASSERT(ast);
	
	bc::Generator bcGenerator(ast);
	auto output = bcGenerator.getInstructions();

	if (output.size()) {
        if (ast->type() == kFunctionDeclType) {
            std::shared_ptr<FunctionDecl> functionDecl = std::static_pointer_cast<FunctionDecl>(ast);
            mSymbols.setContext(functionDecl->getId(), output, functionDecl);
            return false;
        }

        output.push_back(bc::Instruction(bc::Exit, {}));

#if 0
        for (auto instruction : output) {
            std::cout << instruction.toString() << std::endl;
        }
#endif
        
		Interpreter interpreter;

        std::vector<bc::Instruction> instructions = mSymbols.asInstructionList();
        output.insert(output.end(), instructions.begin(), instructions.end());

        int myValue = interpreter.interpret(output, 0);
        if (value) {
            *value = myValue;
        }
        return true;
	}
    return false;
	
}
