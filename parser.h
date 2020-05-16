#ifndef LOX_PARSER_H
#define LOX_PARSER_H


#include <vector>
#include <memory>
#include <exception>

#include "ast.h"
#include "token.h"
#include "token_type.h"


/* Recursive Decent Parser
 * Grammar
 * expression     → equality ;
   equality       → comparison ( ( "!=" | "==" ) comparison )* ;
   comparison     → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
   addition       → multiplication ( ( "-" | "+" ) multiplication )* ;
   multiplication → unary ( ( "/" | "*" ) unary )* ;
   unary          → ( "!" | "-" ) unary
                  | primary ;
   primary        → NUMBER | STRING | "false" | "true" | "nil"
                 | "(" expression ")" ;
*/


class ParserError: public std::exception {
    Token token;
    const char *message;
public:
    ParserError(const Token &token, const char *message)
        : token(token), message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }
};

class Parser {

public:
    Parser(const std::vector<Token> &tokenList);
    ~Parser();

    Parser(const Parser&) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(const Parser&) = delete;

    std::unique_ptr<Expression> expression();

    std::unique_ptr<Expression> equality();

    std::unique_ptr<Expression> comparison();

    std::unique_ptr<Expression> addition();

    std::unique_ptr<Expression> multiplication();

    std::unique_ptr<Expression> unary();

    std::unique_ptr<Expression> primary();


private:
    bool match(TokenType type);

    Token consume(TokenType type, const char* message);

    ParserError error(const Token &token, const char* message);

    inline bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().Type() == type;
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
    int current;
    std::vector<Token> tokenList;
};

#endif
