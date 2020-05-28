#include "token_type.h"


std::ostream& operator<<(std::ostream &os, const TokenType &tokenType) {
    switch(tokenType) {
    case TokenType::LEFT_PAREN: os << "LEFT_PAREN"; break;
    case TokenType::RIGHT_PAREN: os << "RIGHT_PAREN"; break;
    case TokenType::LEFT_BRACE: os << "LEFT_BRACEN"; break;
    case TokenType::RIGHT_BRACE: os << "RIGHT_BRACe"; break;
    case TokenType::COMMA: os << "COMMA"; break;
    case TokenType::DOT: os << "DOT"; break;
    case TokenType::MINUS: os << "MINUS"; break;
    case TokenType::PLUS: os << "PLUS"; break;
    case TokenType::SEMICOLON: os << "SEMICOLON"; break;
    case TokenType::SLASH: os << "SLASH"; break;
    case TokenType::STAR: os << "STAR"; break;
    case TokenType::BANG: os << "BANG"; break;
    case TokenType::BANG_EQUAL: os << "BANG_EQUAL"; break;
    case TokenType::EQUAL: os << "EQUAL"; break;
    case TokenType::EQUAL_EQUAL: os << "EQUAL_EQUAL"; break;
    case TokenType::GREATER: os << "GREATER"; break;
    case TokenType::LESS: os << "LESS"; break;
    case TokenType::LESS_EQUAL: os << "LESS_EQUAL"; break;
    case TokenType::IDENTIFIER: os << "IDENTIFIER"; break;
    case TokenType::STRING: os << "STRING"; break;
    case TokenType::NUMBER: os << "NUMBER"; break;
    case TokenType::AND: os << "AND"; break;
    case TokenType::CLASS: os << "CLASS"; break;
    case TokenType::ELSE: os << "ELSE"; break;
    case TokenType::FALSE: os << "FALSE"; break;
    case TokenType::FUN: os << "FUN"; break;
    case TokenType::FOR: os << "FOR"; break;
    case TokenType::IF: os << "IF"; break;
    case TokenType::NIL: os << "NIL"; break;
    case TokenType::OR: os << "OR"; break;
    case TokenType::PRINT: os << "PRINT"; break;
    case TokenType::RETURN: os << "RETURN"; break;
    case TokenType::SUPER: os << "SUPER"; break;
    case TokenType::THIS: os << "THIS"; break;
    case TokenType::TRUE: os << "TRUE"; break;
    case TokenType::VAR: os << "VAR"; break;
    case TokenType::WHILE: os << "WHILE"; break;
    default:
        os << "Unknown Token Type";
        break;
    }
    return os;
}
