#include "parser.h"
#include "lox.hpp"


Parser::Parser(const std::vector<Token> &tokenList): current(0), tokenList(tokenList) {
}


Parser::~Parser() {
}


std::list<std::unique_ptr<Statement>> Parser::Parse() {
    std::list<std::unique_ptr<Statement>> statements;
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
    if (match(TokenType::FUN)) {
        return fun_declaration("function");
    }
    return statement();
}


std::unique_ptr<Statement> Parser::fun_declaration(const char* kind) {
    Token name = consume(TokenType::IDENTIFIER, fmt::format("Expect {} name", kind).c_str());
    consume(TokenType::LEFT_PAREN, "Expect '(' after function declaration");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Cannot have more than 255 parameters");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while(match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after statements.");
    consume(TokenType::LEFT_BRACE, fmt::format("Expect '(' before {} body", kind).c_str());
    auto body = block();
    return std::make_unique<FunctionStatement>(name, parameters, body);
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
    if (match(TokenType::WHILE)) {
        return while_statement();
    }
    if (match(TokenType::FOR)) {
        return for_statement();
    }
    if (match(TokenType::RETURN)) {
        return return_statement();
    }
    if (match(TokenType::LEFT_BRACE)) {
        auto block_statement = block();
        return std::make_unique<BlockStatement>(block_statement);
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
        elseBranch = statement();
    }
    return std::make_unique<IfStatement>(std::move(condition),
        std::move(thenBranch), std::move(elseBranch));
}


std::unique_ptr<Statement> Parser::print_statement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after a statement");
    return std::make_unique<PrintStatement>(std::move(expr));
}

std::unique_ptr<Statement> Parser::return_statement() {
    Token keyword = previous();
    std::unique_ptr<Expression> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value");
    return std::make_unique<ReturnStatement>(keyword, std::move(value));
}


std::unique_ptr<Statement> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after the while keyword");
    auto expr = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after the while expression");
    auto stmt = statement();
    return std::make_unique<WhileStatement>(std::move(expr), std::move(stmt));
}


std::unique_ptr<Statement> Parser::for_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after the for keyword");
    std::unique_ptr<Statement> initializer = nullptr;
    if (match(TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else if (match(TokenType::VAR)) {
        initializer = var_declaration();
    } else {
        initializer = expression_statement();
    }

    std::unique_ptr<Expression> cond = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        cond = expression();
    } else {
        cond = std::make_unique<LiteralExpression>(true);
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition");

    std::unique_ptr<Expression> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after a for clause");

    std::unique_ptr<Statement> body = statement();

    if (increment != nullptr) {
        std::list<std::unique_ptr<Statement>> stmts;
        stmts.push_back(std::move(body));
        stmts.push_back(std::make_unique<ExpressionStatement>(std::move(increment)));
        body = std::make_unique<BlockStatement>(stmts);
    }
    body = std::make_unique<WhileStatement>(std::move(cond), std::move(body));

    if (initializer != nullptr) {
        std::list<std::unique_ptr<Statement>> stmts;
        stmts.push_back(std::move(initializer));
        stmts.push_back(std::move(body));
        body = std::make_unique<BlockStatement>(stmts);
    }

    return body;
}


std::list<std::unique_ptr<Statement>> Parser::block() {
    std::list<std::unique_ptr<Statement>> statements;

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after a block");
    return statements;
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
    auto expr = logic_or();
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


std::unique_ptr<Expression> Parser::logic_or() {
    auto expr = logic_and();
    while (match(TokenType::OR)) {
        Token op = previous();
        auto right = logic_and();
        expr = std::make_unique<LogicalExpression>(std::move(expr), op, std::move(right));
    }
    return expr;
}


std::unique_ptr<Expression> Parser::logic_and() {
    auto expr = equality();
    while (match(TokenType::AND)) {
        Token op = previous();
        auto right = equality();
        expr = std::make_unique<LogicalExpression>(std::move(expr), op, std::move(right));
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
    return call();
}


std::unique_ptr<Expression> Parser::finish_call(std::unique_ptr<Expression> callee) {
    std::list<std::unique_ptr<Expression>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Function cannot have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while(match(TokenType::COMMA));
    }

    Token token =  consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments in func call");
    return std::make_unique<CallExpression>(std::move(callee), token, arguments);
}


std::unique_ptr<Expression> Parser::call() {
    auto expr = primary();

    while(true) {
        if (match(TokenType::LEFT_PAREN)) {
            expr = finish_call(std::move(expr));
        } else break;
    }

    return expr;
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
