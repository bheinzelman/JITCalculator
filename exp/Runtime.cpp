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
#if 0
	for (auto instruction : output) {
		std::cout << instruction.toString() << std::endl;
	}
#endif
	if (output.size()) {
        if (ast->type() == kFunctionDeclType) {
            std::shared_ptr<FunctionDecl> functionDecl = std::static_pointer_cast<FunctionDecl>(ast);
            mSymbols.setContext(functionDecl->getId(), output, functionDecl);
            return false;
        }
        
		Interpreter interpreter(output, mSymbols);
        int myValue = interpreter.interpret();
        if (value) {
            *value = myValue;
        }
        return true;
	}
    return false;
	
}
