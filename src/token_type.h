#ifndef LOX_TOKEN_TYPE_H
#define LOX_TOKEN_TYPE_H

#include <iostream>

enum class TokenType {
    /* Single charater tokens */
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    /* Multiple characters tokens */
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    /* Literals */
    IDENTIFIER, STRING, NUMBER,

    /* Keywords */
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE
};
std::ostream& operator<<(std::ostream &os, const TokenType &tokenType);

#endif // LOX_TOKEN_TYPE_H
