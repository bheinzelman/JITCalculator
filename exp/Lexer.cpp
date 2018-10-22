//  Lexer.cpp

#include "Lexer.hpp"
#include "Token.hpp"

Lexer::Lexer(std::istream &inputStream) : mInput(inputStream)
{
}

Token Lexer::currentToken() const {
	return _currentToken;
}

bool Lexer::peekToken(Token *token, jcVariablePtr lexeme)
{
    int64_t i = position();

	Token t;
	bool result = getNextToken(&t, lexeme);
    seekPosition(i);
	
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
			while (isdigit(peek())) {
				numString += nextChar();
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
			
			while (hasMoreChars() && isalpha(peek())) {
				word += std::string(1, nextChar());
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

char Lexer::peek()
{
    return mInput.peek();
}

char Lexer::nextChar()
{
    return mInput.get();
}

int64_t Lexer::position()
{
    return mInput.tellg();
}

void Lexer::seekPosition(int64_t pos)
{
    mInput.seekg(pos);
}

bool Lexer::nextNonWhitespaceChar(char *c)
{
	while (hasMoreChars() && isspace(peek()))
	{
        nextChar();
	}
	
	if (hasMoreChars()) {
		if (c != nullptr) {
			*c = peek();
		}
        nextChar();
		return true;
	}
	return false;
}

bool Lexer::hasMoreChars()
{
    return !(mInput.eof() || peek() == EOF);
}

