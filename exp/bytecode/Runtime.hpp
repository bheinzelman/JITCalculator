//  Runtime.hpp

#pragma once

//#include "SymbolTable.hpp"
#include "bc.hpp"

#include <istream>
#include <string>
#include <functional>
#include <map>

class Runtime {
public:
    Runtime();

    bool evaluateREPL(std::istream& stream, std::vector<jcVariablePtr>& outputValues);

    static void evaluate(std::istream& stream);
private:
    using DefinitionCallback = std::function<void(std::string, std::vector<bc::Instruction>, std::vector<bc::Instruction>)>;
    using ExpressionCallback = std::function<void(std::vector<bc::Instruction>, std::vector<bc::Instruction>)>;
    
    static void traverseStream(std::istream& stream, DefinitionCallback definitionHandler, ExpressionCallback expressionHandler);

    std::vector<bc::Instruction> instructionsFromDefinitions();

    struct FunctionContext {
        std::vector<bc::Instruction> function;
        std::vector<bc::Instruction> closures;
    };

    std::map<std::string, FunctionContext> mReplDefinitions;
};
