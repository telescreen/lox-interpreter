#include "parser.h"


Parser::Parser(const std::vector<Token> &tokenList):
    tokenList(tokenList),
    current(0) {
}


Parser::~Parser() {
}


bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}


std::unique_ptr<Expression> Parser::expression() {
    return equality();
}


std::unique_ptr<Expression> Parser::equality() {
    std::unique_ptr<Expression> expr = comparison();

    while(match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        std::unique_ptr<Expression> right = comparison();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::comparison() {
    std::unique_ptr<Expression> expr = addition();

    while(match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) ||
          match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
        Token op = previous();
        std::unique_ptr<Expression> right = addition();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::addition() {
    std::unique_ptr<Expression> expr = multiplication();

    while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous();
        std::unique_ptr<Expression> right = multiplication();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::multiplication() {
    std::unique_ptr<Expression> expr = unary();

    while(match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = previous();
        std::unique_ptr<Expression> right = unary();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::unary() {
    if (match(TokenType::BANG) || match(TokenType::MINUS))  {
        Token op = previous();
        std::unique_ptr<Expression> right = unary();
        return std::make_unique<UnaryExpression>(op, std::move(right));
    }
    return primary();
}


std::unique_ptr<Expression> Parser::primary() {
    if (match(TokenType::FALSE)) {
        return std::make_unique<BooleanExpression>(false);
    }

    if (match(TokenType::TRUE)) {
        return std::make_unique<BooleanExpression>(true);
    }

    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberExpression>(previous().Literal());
    }

    if (match(TokenType::STRING)) {
        return std::make_unique<StringExpression>(previous().Literal());
    }

    if (match(TokenType::LEFT_PAREN)) {
        std::unique_ptr<Expression> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' right after expression");
        return std::make_unique<GroupingExpression>(std::move(expr));
    }

    throw error(peek(), "Error parsing primary");
}


Token Parser::consume(TokenType type, const char* message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}


ParserError Parser::error(const Token &token, const char* message) {
    std::cerr << message << std::endl;
    return ParserError(token, message);
}
