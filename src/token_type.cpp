#include "token_type.h"

const char* TT2String(TokenType type) {
    const std::map<TokenType,const char*> TokenTypeEnumStrings {
        { TokenType::LEFT_PAREN, "LEFT_PAREN" },
        { TokenType::RIGHT_PAREN, "RIGHT_PAREN" },
        { TokenType::LEFT_BRACE, "LEFT_BRACEN" },
        { TokenType::RIGHT_BRACE, "RIGHT_BRACE" },
        { TokenType::COMMA, "COMMA" },
        { TokenType::DOT, "DOT" },
        { TokenType::MINUS, "MINUS" },
        { TokenType::PLUS, "PLUS" },
        { TokenType::SEMICOLON, "SEMICOLON" },
        { TokenType::SLASH, "SLASH" },
        { TokenType::STAR, "STAR" },
        { TokenType::BANG, "BANG" },
        { TokenType::BANG_EQUAL, "BANG_EQUAL" },
        { TokenType::EQUAL, "EQUAL" },
        { TokenType::EQUAL_EQUAL, "EQUAL_EQUAL" },
        { TokenType::GREATER, "GREATER" },
        { TokenType::LESS, "LESS" },
        { TokenType::LESS_EQUAL, "LESS_EQUAL" },
        { TokenType::IDENTIFIER, "IDENTIFIER" },
        { TokenType::STRING, "STRING" },
        { TokenType::NUMBER, "NUMBER" },
        { TokenType::AND, "AND" },
        { TokenType::CLASS, "CLASS" },
        { TokenType::ELSE, "ELSE" },
        { TokenType::FALSE, "FALSE" },
        { TokenType::FUN, "FUN" },
        { TokenType::FOR, "FOR" },
        { TokenType::IF, "IF" },
        { TokenType::NIL, "NIL" },
        { TokenType::OR, "OR" },
        { TokenType::PRINT, "PRINT" },
        { TokenType::RETURN, "RETURN" },
        { TokenType::SUPER, "SUPER" },
        { TokenType::THIS, "THIS" },
        { TokenType::TRUE, "TRUE" },
        { TokenType::VAR, "VAR" },
        { TokenType::WHILE, "WHILE" }
    };
    auto   it  = TokenTypeEnumStrings.find(type);
    return it == TokenTypeEnumStrings.end() ? "Out of range" : it->second;
}

std::ostream& operator<<(std::ostream &os, const TokenType &tokenType) {
    os << TT2String(tokenType);
    return os;
}