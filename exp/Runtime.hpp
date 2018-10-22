//  Runtime.hpp

#pragma once

#include "SymbolTable.hpp"

#include <istream>
#include <string>

class Runtime
{
public:
	Runtime();

    bool evaluate(std::istream &stream, std::vector<int> &outputValues);
	
private:
    SymbolTable mSymbols;
};

