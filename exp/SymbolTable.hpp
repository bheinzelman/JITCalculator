//  SymbolTable.hpp

#pragma once

#include "ast.hpp"
#include "bc.hpp"

#include <map>
#include <vector>

class SymbolContext {
public:
    SymbolContext(std::vector<bc::Instruction> instructions, std::shared_ptr<FunctionDecl> decl)
        : mDecl(decl)
        , mInstructions(instructions)
    {
    }

    static std::shared_ptr<SymbolContext> Create(std::vector<bc::Instruction> instructions, std::shared_ptr<FunctionDecl> decl)
    {
        return std::make_shared<SymbolContext>(instructions, decl);
    }

    inline std::shared_ptr<FunctionDecl> getDecl() const
    {
        return mDecl;
    }

    inline std::vector<bc::Instruction> getInstructions() const
    {
        return mInstructions;
    }

private:
    std::shared_ptr<FunctionDecl> mDecl;
    std::vector<bc::Instruction> mInstructions;
};

class SymbolTable {
public:
    std::shared_ptr<SymbolContext> getContext(const std::string& functionName);

    void setContext(const std::string& functionName, const std::vector<bc::Instruction>& instructions, const std::shared_ptr<FunctionDecl> decl);

    std::vector<bc::Instruction> asInstructionList() const;

private:
    std::map<std::string, std::shared_ptr<SymbolContext>> mLut;
};
