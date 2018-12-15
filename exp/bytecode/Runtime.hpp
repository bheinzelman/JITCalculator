//  Runtime.hpp

#pragma once

#include "bc.hpp"

#include <istream>
#include <string>
#include <functional>
#include <map>

class Runtime {
public:
    Runtime();

    /**
     This method should be used when evaluating instructions generated from the REPL.
     If a definition is redefined, it will override the previous definition
     */
    bool evaluateREPL(std::istream& stream, std::vector<jcVariablePtr>& outputValues);

    /**
     Use this when evaluating instructions generated from a file
     */
    static void evaluate(std::istream& stream);
private:
    using DefinitionCallback = std::function<void(std::string, std::vector<bc::Instruction>, std::vector<bc::Instruction>)>;
    using ExpressionCallback = std::function<void(std::vector<bc::Instruction>, std::vector<bc::Instruction>)>;

    /**
     Traverses the input stream and generates instructions
     */
    static void traverseStream(std::istream& stream, DefinitionCallback definitionHandler, ExpressionCallback expressionHandler);

    std::vector<bc::Instruction> instructionsFromDefinitions();

    struct FunctionContext {
        std::vector<bc::Instruction> function;
        std::vector<bc::Instruction> closures;
    };

    std::map<std::string, FunctionContext> mReplDefinitions;
};
