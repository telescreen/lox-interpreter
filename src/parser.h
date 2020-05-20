#ifndef LOX_PARSER_H
#define LOX_PARSER_H


#include <vector>
#include <memory>

#include "ast.h"
#include "token.h"
#include "token_type.h"
#include "lox_exception.hpp"


/* Recursive Decent Parser
 * Grammar

   program        → statement* EOF ;
   statement      → expressionStatement
                  | printStatement ;
   expressionStatement → expression ;
   expression     → equality ;
   equality       → comparison ( ( "!=" | "==" ) comparison )* ;
   comparison     → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
   addition       → multiplication ( ( "-" | "+" ) multiplication )* ;
   multiplication → unary ( ( "/" | "*" ) unary )* ;
   unary          → ( "!" | "-" ) unary
                  | primary ;
   primary        → NUMBER | STRING | "false" | "true" | "nil"
                 | "(" expression ")" ;
*/

class Parser {

public:
    Parser(const std::vector<Token> &tokenList);
    ~Parser();

    /*std::vector<std::unique_ptr<Statement>> Parse() {
        std::vector<std::unique_ptr<Statement>> statements;
        while(!isAtEnd()) {
            statements.push_back(statement());
        }
        return statements;
        }*/

    std::unique_ptr<Expression> Parse() {
        return expression();
    }


private:
    bool match(TokenType type);

    Token consume(TokenType type, const char* message);

    ParserError error(const Token &token, const char* message);

    std::unique_ptr<Statement> statement();

    std::unique_ptr<Statement> print_statement();

    std::unique_ptr<Statement> expression_statement();

    std::unique_ptr<Expression> expression();

    std::unique_ptr<Expression> equality();

    std::unique_ptr<Expression> comparison();

    std::unique_ptr<Expression> addition();

    std::unique_ptr<Expression> multiplication();

    std::unique_ptr<Expression> unary();

    std::unique_ptr<Expression> primary();

    inline bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    inline Token advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    inline Token previous() const {
        return tokenList[current-1];
    }

    inline bool isAtEnd() const {
        return current == tokenList.size();
    }

    inline Token peek() const {
        return tokenList[current];
    }


private:
    /* current points to the currently in process token */
    int current;
    std::vector<Token> tokenList;
};

#endif
