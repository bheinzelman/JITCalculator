// Token.cpp

#include "Token.hpp"
namespace jcToken {

int getOperatorPrecedence(TokenType op)
{
    switch (op) {
    case TokenType::Concat:
        return 1;
    case TokenType::Cons:
        return 2;
    case TokenType::QuestionMark:
        return 3;
    case TokenType::Less_Than:
    case TokenType::Greater_Than:
    case TokenType::Less_Than_Equal:
    case TokenType::Greater_Than_Equal:
    case TokenType::Equals:
        return 4;
    case TokenType::Add:
    case TokenType::Subtract:
        return 5;
    case TokenType::Multiply:
    case TokenType::Divide:
        return 6;
    default:
        return -1;
    }
}

std::string stringRepresentation(TokenType token)
{
    switch (token) {
        case TokenType::Add:
            return "+";
        case TokenType::Subtract:
            return "-";
        case TokenType::Multiply:
            return "*";
        case TokenType::Divide:
            return "/";
        case TokenType::Less_Than:
            return "<";
        case TokenType::Greater_Than:
            return ">";
        case TokenType::Less_Than_Equal:
            return "<=";
        case TokenType::Greater_Than_Equal:
            return ">=";
        case TokenType::Bang:
            return "!";
        case TokenType::Equals:
            return "==";
        case TokenType::LParen:
            return "(";
        case TokenType::RParen:
            return ")";
        case TokenType::Num:
            return "Num";
        case TokenType::String:
            return "String";
        case TokenType::Error:
            return "Error Token";
        case TokenType::LetKw:
            return "let";
        case TokenType::ElseKw:
            return "else";
        case TokenType::Id:
            return "Id"; 
        case TokenType::Assign:
            return "=";
        case TokenType::Comma:
            return ",";
        case TokenType::Pipe:
            return "|";
        case TokenType::LeftBracket:
            return "[";
        case TokenType::RightBracket:
            return "]";
        case TokenType::LeftBrace:
            return "{";
        case TokenType::RightBrace:
            return "}";
        case TokenType::EndOfStream:
            return "EOS";
        case TokenType::QuestionMark:
            return "?";
        case TokenType::Colon:
            return ":";
        case TokenType::Cons:
            return "::";
        case TokenType::Concat:
            return "++";
        case TokenType::None:
            return "None";
    }
}

}

TokenType Token::getType() const
{
    return mTokenType;
}

jcVariablePtr Token::getLexeme() const
{
    return mLexeme;
}

int64_t Token::getLineNumber() const
{
    return mLineNumber;
}
