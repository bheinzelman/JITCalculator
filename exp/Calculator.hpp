//  Calculator.hpp

#ifndef Calculator_hpp
#define Calculator_hpp

#include <string>

class Calculator
{
public:
	Calculator(std::string expression);
	
	int calculate() const;
	
private:
	std::string expression;
};

#endif /* Calculator_hpp */
