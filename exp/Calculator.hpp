//  Calculator.hpp

#pragma once

#include <string>

class Calculator
{
public:
	Calculator(std::string expression);
	
	int calculate() const;
	
private:
	std::string expression;
};

