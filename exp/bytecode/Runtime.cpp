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

std::vector<bc::Instruction> Runtime::instructionsFromDefinitions()
{
    std::vector<bc::Instruction> instructions;
    for (auto pair : mReplDefinitions) {
        auto ctx = pair.second;
        instructions.insert(instructions.end(), ctx.function.begin(), ctx.function.end());
        instructions.insert(instructions.end(), ctx.closures.begin(), ctx.closures.end());
    }

    return instructions;
}

void Runtime::traverseStream(std::istream& stream, DefinitionCallback definitionHandler, ExpressionCallback expressionHandler)
{
    Lexer lex(stream);
    Parser parser(std::make_shared<Lexer>(lex));
    std::vector<std::shared_ptr<Node>> nodes = parser.parse();

    if (nodes.size() == 0) {
        return;
    }

    bc::Generator bcGenerator;

    for (auto ast : nodes) {
        JC_ASSERT(ast);

        auto output = bcGenerator.getInstructions(ast);
        auto closures = bcGenerator.getClosureInstructions();

        if (output.size()) {
            if (ast->type() == kFunctionDeclType) {
                std::shared_ptr<FunctionDecl> functionDecl = std::static_pointer_cast<FunctionDecl>(ast);

                definitionHandler(functionDecl->getId(), output, closures);
            } else {
                output.push_back(bc::Instruction(bc::Exit, {}));
                expressionHandler(output, closures);
            }
        }
    }
}

void Runtime::evaluate(std::istream& stream)
{
    std::vector<bc::Instruction> definitions;
    std::vector<bc::Instruction> expressions;
    traverseStream(stream,
       [&definitions](std::string definitionName, std::vector<bc::Instruction> newDefinitions, std::vector<bc::Instruction> closures) {
           definitions.insert(definitions.end(), newDefinitions.begin(), newDefinitions.end());
           definitions.insert(definitions.end(), closures.begin(), closures.end());
       },
       [&definitions, &expressions](std::vector<bc::Instruction> newExpressions, std::vector<bc::Instruction> closures) {
           expressions.insert(expressions.end(), newExpressions.begin(), newExpressions.end());
           definitions.insert(definitions.end(), closures.begin(), closures.end());
       });

    expressions.insert(expressions.end(), definitions.begin(), definitions.end());

    Interpreter interpreter;
    interpreter.setInstructions(expressions);

    interpreter.interpret();
}


bool Runtime::evaluateREPL(std::istream& stream, std::vector<jcVariablePtr>& outputValues)
{
    traverseStream(stream,
    [this](std::string definitionName, std::vector<bc::Instruction> definitions, std::vector<bc::Instruction> closures) {
        FunctionContext ctx;
        ctx.function = definitions;
        ctx.closures = closures;

        mReplDefinitions[definitionName] = ctx;
    },
    [&outputValues, this](std::vector<bc::Instruction> expressions, std::vector<bc::Instruction> closures) {
        expressions.push_back(bc::Instruction(bc::Exit, {}));

        Interpreter interpreter;

        std::vector<bc::Instruction> definitionInstructions = instructionsFromDefinitions();

        // add definitions
        expressions.insert(expressions.end(), definitionInstructions.begin(), definitionInstructions.end());

        // add closures
        expressions.insert(expressions.end(), closures.begin(), closures.end());


        interpreter.setInstructions(expressions);

        outputValues.push_back(interpreter.interpret());
    });
    return true;
}
