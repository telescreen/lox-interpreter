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

   program        → declaration* EOF ;
   declaration    → funDecl
                  | varDecl
                  | statement
   funDecl        → "fun" IDENTIFIER "(" parameters? ")" block ;
   varDecl        → "var" IDENTIFIER ("=" expression)? ";"
   statement      → expressionStatement
                  | ifStatement
                  | printStatement
                  | whileStatement
                  | forStatement
                  | block ;
   whileStatement → "while" "(" expression ")" statement;
   forStatement → "for" "(" varDecl | expressionStatement ";"
                            expression? ";" expression? ")" statement;
   ifStatement    → "if" "(" expression ")" statement ("else" statement)? ;
   block          → "{"  declaration* "}" ;
   expressionStatement → expression ;
   expression     → assigment ;
   assignment     → IDENTIFIER "=" assigment
                  | logic_or ;
   logic_or       → logic_and ( "or" logic_and)* ;
   logic_and      → equality ( "and" equality )* ;
   equality       → comparison ( ( "!=" | "==" ) comparison )* ;
   comparison     → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
   addition       → multiplication ( ( "-" | "+" ) multiplication )* ;
   multiplication → unary ( ( "/" | "*" ) unary )* ;
   unary          → ( "!" | "-" | "+") unary | call ;
   call           → primary ( "(" arguments? ")" )* ;
   arguments      → expression ("," expression)*
   primary        → NUMBER | STRING | "false" | "true" | "nil"
                  | "(" expression ")" ;
                  | IDENTIFIER
*/

class Parser {

public:
    Parser(const std::vector<Token> &tokenList);
    virtual ~Parser();

    std::list<std::unique_ptr<Statement>> Parse();

private:
    bool match(TokenType type);

    Token consume(TokenType type, const char* message);

    std::unique_ptr<Statement> statement();

    std::unique_ptr<Statement> declaration();

    std::list<std::unique_ptr<Statement>> block();

    std::unique_ptr<Statement> print_statement();

    std::unique_ptr<Statement> if_statement();

    std::unique_ptr<Statement> while_statement();

    std::unique_ptr<Statement> for_statement();

    std::unique_ptr<Statement> expression_statement();

    std::unique_ptr<Statement> var_declaration();

    std::unique_ptr<Statement> fun_declaration(const char*);

    std::unique_ptr<Expression> expression();

    std::unique_ptr<Expression> assignment();

    std::unique_ptr<Expression> equality();

    std::unique_ptr<Expression> logic_or();

    std::unique_ptr<Expression> logic_and();

    std::unique_ptr<Expression> comparison();

    std::unique_ptr<Expression> addition();

    std::unique_ptr<Expression> multiplication();

    std::unique_ptr<Expression> unary();

    std::unique_ptr<Expression> call();

    std::unique_ptr<Expression> finish_call(std::unique_ptr<Expression>);

    std::unique_ptr<Expression> primary();

    ParserError error(Token token, const char* message);

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
        return peek().type == TokenType::TEOF;
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
