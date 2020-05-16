#include "token.h"


Token::Token(TokenType type, std::string lexeme, std::string literal, int line) {
    this->type = type;
    this->literal = literal;
    this->lexeme = lexeme;
    this->line = line;
}


Token::~Token() {
}


TokenType Token::Type() const {
    return type;
}


std::string Token::Literal() const {
    return literal;
}


std::ostream &operator<<(std::ostream &os, const Token &token)
{
    std::cout << "token=" << token.type << " "
              << "lexeme=" << token.lexeme << " "
              << "literal=" << token.literal << " "
              << "at line=" << token.line;
    return os;
}