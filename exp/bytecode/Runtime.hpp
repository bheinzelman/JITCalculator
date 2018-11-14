//  Runtime.hpp

#pragma once

#include "SymbolTable.hpp"

#include <istream>
#include <string>

class Runtime {
public:
    Runtime();

    bool evaluate(std::istream& stream, std::vector<jcVariablePtr>& outputValues);

private:
    SymbolTable mSymbols;
};
