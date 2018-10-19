//  SymbolTable.cpp

#include "SymbolTable.hpp"

std::shared_ptr<SymbolContext> SymbolTable::getContext(const std::string &functionName)
{
    if (mLut.count(functionName)) {
        return mLut[functionName];
    }
    return nullptr;
}

void SymbolTable::setContext(const std::string &functionName, const std::vector<bc::Instruction> &instructions, std::shared_ptr<FunctionDecl> decl)
{
    mLut[functionName] = SymbolContext::Create(instructions, decl);
}

std::vector<bc::Instruction> SymbolTable::asInstructionList() const
{
    std::vector<bc::Instruction> instructions = {};
    for (auto keyVal : mLut) {
        for (auto instruction : keyVal.second->getInstructions()) {
            instructions.push_back(instruction);
        }
    }
    return instructions;
}
