//  Lexer.cpp

#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::string expression) : expression(expression)
{
	this->index = 0;
}

Token Lexer::currentToken() const {
	return _currentToken;
}

bool Lexer::peekToken(Token *token, jcVariablePtr lexeme)
{
	int i = index;
	Token t;
	bool result = getNextToken(&t, lexeme);
	index = i;
	
	if (result) {
		if (token != nullptr) {
			*token = t;
		}
	}
	return result;
}

bool Lexer::getNextToken(Token *token, jcVariablePtr lexeme)
{
	Token nextToken = Token::Error;
	if (hasMoreChars()) {
		char next;
		nextNonWhitespaceChar(&next);
		switch (next) {
			case '(':
				nextToken = Token::LParen;
				break;
			case ')':
				nextToken = Token::RParen;
				break;
			case '+':
				nextToken = Token::Add;
				break;
			case '-':
				nextToken = Token::Subtract;
				break;
			case '/':
				nextToken = Token::Divide;
				break;
			case '*':
				nextToken = Token::Multiply;
				break;
			case '=':
				nextToken = Token::Assign;
				break;
            case ',':
                nextToken = Token::Comma;
                break;
			default:
				break;
		}
		
		if (nextToken != Token::Error) {
			if (token != nullptr) {
				*token = nextToken;
			}
			return true;
		}
		
		// must be a number
		if (isdigit(next)) {
			std::string numString = "";
			numString += next;
			while (isdigit(expression[index])) {
				numString += expression[index++];
				if (!hasMoreChars()) {
					break;
				}
			}
			int value = atoi(numString.c_str());
			if (lexeme != nullptr) {
				lexeme->setInt(value);
			}
			if (token != nullptr) {
				*token = Token::Num;
			}
			return true;
		} else if (isalpha(next)) {
			std::string word(1, next);
			
			while (hasMoreChars() && isalpha(expression[index])) {
				word += std::string(1, expression[index++]);
				if (word == "let") {
					if (token != nullptr) {
						*token = Token::LetKw;
					}
					return true;
				}
			}
			if (token != nullptr) {
				*token = Token::Id;
			}
			if (lexeme != nullptr) {
				lexeme->setString(word);
			}
			return true;
		} else {
			if (token != nullptr) {
				*token = Token::Error;
			}
			return false;
		}		
	} else {
		if (token != nullptr) {
			*token = Token::EndOfStream;
			return true;
		}
	}
	return false;
}


bool Lexer::nextNonWhitespaceChar(char *c)
{
	while (isspace(expression[index]))
	{
		index++;
		if (!hasMoreChars()) {
			break;
		}
	}
	
	if (hasMoreChars()) {
		if (c != nullptr) {
			*c = expression[index];
		}
		index += 1;
		return true;
	}
	return false;
}

bool Lexer::hasMoreChars() const
{
	return index < expression.size();
}

