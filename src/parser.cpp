#include "parser.h"
#include "lox.hpp"


Parser::Parser(const std::vector<Token> &tokenList):
    tokenList(tokenList),
    current(0) {
}


Parser::~Parser() {
}


std::vector<std::unique_ptr<Statement>> Parser::Parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    while(!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}


bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}


std::unique_ptr<Statement> Parser::declaration() {
    if (match(TokenType::VAR)) {
        return var_declaration();
    }
    return statement();
}


std::unique_ptr<Statement> Parser::var_declaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
    std::unique_ptr<Expression> init = nullptr;
    if (match(TokenType::EQUAL)) {
        init = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    return std::make_unique<VarStatement>(name, std::move(init));
}



std::unique_ptr<Statement> Parser::statement() {
    if (match(TokenType::IF)) {
        return if_statement();
    }
    if (match(TokenType::PRINT)) {
        return print_statement();
    }
    if (match(TokenType::LEFT_BRACE)) {
        return block();
    }
    return expression_statement();
}


std::unique_ptr<Statement> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after if");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition");

    auto thenBranch = statement();
    std::unique_ptr<Statement> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = std::move(statement());
    }
    return std::make_unique<IfStatement>(std::move(condition),
        std::move(thenBranch), std::move(elseBranch));
}


std::unique_ptr<Statement> Parser::print_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after a statement");
    return std::make_unique<PrintStatement>(std::move(expr));
}


std::unique_ptr<Statement> Parser::block() {
    std::vector<std::unique_ptr<Statement>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after a block");

    return std::make_unique<Block>(statements);
}


std::unique_ptr<Statement> Parser::expression_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after a statement");
    return std::make_unique<ExpressionStatement>(std::move(expr));
}


std::unique_ptr<Expression> Parser::expression() {
    return assignment();
}


std::unique_ptr<Expression> Parser::assignment() {
    auto expr = equality();
    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        auto value = assignment();
        if (VariableExpression* v = dynamic_cast<VariableExpression*>(expr.get())) {
            Token name = v->token;
            return std::make_unique<AssignmentExpression>(name, std::move(value));
        }
        throw error(equals, "Invalid assignment targets");
    }
    return expr;
}


std::unique_ptr<Expression> Parser::equality() {
    auto expr = comparison();

    while(match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::comparison() {
    auto expr = addition();

    while(match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) ||
          match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
        Token op = previous();
        auto right = addition();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::addition() {
    auto expr = multiplication();

    while(match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous();
        auto right = multiplication();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::multiplication() {
    auto expr = unary();

    while(match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpression>(op, std::move(expr), std::move(right));
    }

    return expr;
}


std::unique_ptr<Expression> Parser::unary() {
    if (match(TokenType::BANG) || match(TokenType::MINUS) || match(TokenType::PLUS))  {
        Token op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpression>(op, std::move(right));
    }
    return primary();
}


std::unique_ptr<Expression> Parser::primary() {
    if (match(TokenType::NIL)) {
        return std::make_unique<LiteralExpression>(Value());
    }

    if (match(TokenType::FALSE)) {
        return std::make_unique<LiteralExpression>(false);
    }

    if (match(TokenType::TRUE)) {
        return std::make_unique<LiteralExpression>(true);
    }

    if (match(TokenType::NUMBER)) {
        return std::make_unique<LiteralExpression>(std::stod(previous().literal, nullptr));
    }

    if (match(TokenType::STRING)) {
        return std::make_unique<LiteralExpression>(previous().literal);
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<VariableExpression>(previous());
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' right after expression");
        return std::make_unique<GroupingExpression>(std::move(expr));
    }

    throw error(peek(), "Error parsing primary. Primary must be BOOL, NUBMER, or STRING");
}


Token Parser::consume(TokenType type, const char* message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}


ParserError Parser::error(Token token, const char* message) {
    Lox::Error(token, message);
    return ParserError(peek(), message);
}
