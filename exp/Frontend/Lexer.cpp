//  Lexer.cpp

#include "Lexer.hpp"
#include "Token.hpp"
#include "jc.h"

Lexer::Lexer(std::istream& inputStream)
    : mInput(inputStream)
{
}

Token Lexer::peekToken()
{
    int64_t i = position();
    int64_t tmpLineNum = mLineNumber;
    Token peeked = getNextToken();
    seekPosition(i);
    mLineNumber = tmpLineNum;

    return peeked;
}

void Lexer::skipToken()
{
    getNextToken();
}

int64_t Lexer::getLineNumber() const
{
    return mLineNumber;
}

Token Lexer::getNextToken()
{
    TokenType nextToken = TokenType::Error;
    if (hasMoreChars()) {
        char next;
        if (nextNonWhitespaceChar(&next) == false) {
            return Token(TokenType::EndOfStream, nullptr, mLineNumber);
        }

        switch (next) {
        case '(':
            nextToken = TokenType::LParen;
            break;
        case ')':
            nextToken = TokenType::RParen;
            break;
        case '+':
            nextToken = TokenType::Add;
            break;
        case '!':
            nextToken = TokenType::Bang;
            break;
        case '-':
            nextToken = TokenType::Subtract;
            break;
        case '/':
            nextToken = TokenType::Divide;
            break;
        case '*':
            nextToken = TokenType::Multiply;
            break;
        case '=':
            nextToken = TokenType::Assign;
            break;
        case ',':
            nextToken = TokenType::Comma;
            break;
        case '>':
            nextToken = TokenType::Greater_Than;
            break;
        case '<':
            nextToken = TokenType::Less_Than;
            break;
        case '|':
            nextToken = TokenType::Pipe;
            break;
        case '[':
            nextToken = TokenType::LeftBracket;
            break;
        case ']':
            nextToken = TokenType::RightBracket;
            break;
        case '{':
            nextToken = TokenType::LeftBrace;
            break;
        case '}':
            nextToken = TokenType::RightBrace;
            break;
        case ':':
            nextToken = TokenType::Colon;
            break;
        case '?':
            nextToken = TokenType::QuestionMark;
            break;
        case '#':
            // comment!
            skipTillNewline();
            return getNextToken();

        default:
            break;
        }

        if (nextToken != TokenType::Error) {
            return Token(resolveTwoCharOperator(nextToken), nullptr, mLineNumber);
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

            return Token(TokenType::Num, jcVariable::Create(value), mLineNumber);
        } else if (isalpha(next)) {
            std::string word(1, next);

            while (hasMoreChars() && isalnum(peek())) {
                word += std::string(1, nextChar());

                TokenType keywordToken = TokenType::Error;
                if (word == "let") {
                    keywordToken = TokenType::LetKw;
                } else if (word == "else") {
                    keywordToken = TokenType::ElseKw;
                }

                if (keywordToken != TokenType::Error) {
                    return Token(keywordToken, nullptr, mLineNumber);
                }
            }

            return Token(TokenType::Id, jcVariable::Create(word), mLineNumber);
        } else {
            return Token(TokenType::Error, nullptr, mLineNumber);
        }
    } else {
        return Token(TokenType::EndOfStream, nullptr, mLineNumber);
    }
    return Token(TokenType::Error, nullptr, mLineNumber);
}

TokenType Lexer::resolveTwoCharOperator(TokenType firstOperator)
{
    TokenType returnToken = firstOperator;
    Token peeked = peekToken();
    if (peeked.getType() != TokenType::Error) {
        if (firstOperator == TokenType::Greater_Than && peeked.getType() == TokenType::Assign) {
            returnToken = TokenType::Greater_Than_Equal;
        } else if (firstOperator == TokenType::Less_Than && peeked.getType() == TokenType::Assign) {
            returnToken = TokenType::Less_Than_Equal;
        } else if (firstOperator == TokenType::Assign && peeked.getType() == TokenType::Assign) {
            returnToken = TokenType::Equals;
        }
    }
    if (returnToken != firstOperator) {
        skipToken();
    }
    return returnToken;
}

char Lexer::peek()
{
    return mInput.peek();
}

char Lexer::nextChar()
{
    char next = mInput.get();
    if (next == '\n') mLineNumber++;
    return next;
}

int64_t Lexer::position()
{
    return mInput.tellg();
}

void Lexer::seekPosition(int64_t pos)
{
    mInput.seekg(pos);
}

bool Lexer::nextNonWhitespaceChar(char* c)
{
    while (hasMoreChars() && isspace(peek())) {
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
    return mInput.eof() == false && peek() != EOF && peek() != '\0';
}

void Lexer::skipTillNewline()
{
    while (nextChar() != '\n' && hasMoreChars());
}
