//  Runtime.hpp

#pragma once

#include "SymbolTable.hpp"

#include <string>

class Runtime
{
public:
	Runtime();
	
    bool evaluate(std::string expression, int *value);
	
private:
    SymbolTable mSymbols;
};

