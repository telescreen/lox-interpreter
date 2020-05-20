#ifndef LOX_TOKEN_H
#define LOX_TOKEN_H

#include <iostream>
#include "token_type.h"


class Token {
public:
    Token(TokenType type, std::string lexeme, std::string literal, int line);
    ~Token();
    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
};


#endif // LOX_TOKEN_H
