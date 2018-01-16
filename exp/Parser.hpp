//  Parser.hpp

#ifndef Parser_hpp
#define Parser_hpp

#include <memory>

class Lexer;
class Expression;

class Parser
{
public:
	Parser(std::shared_ptr<Lexer> lexer);
	
	std::shared_ptr<Expression> parse();
	
	// private member variables
private:
	std::shared_ptr<Lexer> lex;
	// private helper functions
private:
	std::shared_ptr<Expression> getTerm();
	
	Token peekOperator();
	
	std::shared_ptr<Expression> getExpression(int prevPrec=1);
};


#endif
