#ifndef LOX_EXCEPTION_HPP
#define LOX_EXCEPTION_HPP

#include <exception>

class ParserError: public std::exception {
    const Token token;
    const char *message;
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
};


class RuntimeError: public std::exception {
    const Token token;
    const char *message;
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
};

class TypeError: public std::exception {
    const char *message;
public:

    TypeError(const char* message)
        : message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }
};

#endif
