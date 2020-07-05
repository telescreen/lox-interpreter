#ifndef LOX_EXCEPTION_HPP
#define LOX_EXCEPTION_HPP

#include <exception>
#include "token.h"
#include "value.h"

class ParserError: public std::exception {
public:
    ParserError(const Token &token, const char* message)
        : token(token), message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }

    const Token& GetToken() const throw() {
        return token;
    }
private:
    const Token token;
    const char *message;
};


class RuntimeError: public std::exception {
public:
    RuntimeError(const Token &token, const char* message)
        : token(token), message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }

    const Token& GetToken() const throw() {
        return token;
    }
private:
    const Token token;
    const char *message;
};

class TypeError: public std::exception {
public:
    TypeError(const char* message)
        : message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }
private:
    const char *message;
};

class ReturnSignal: public std::exception {
public:
    Value value;
    ReturnSignal(Value value): value(value) {}
    virtual const char* what() const throw() {
        return nullptr;
    }
};

#endif
