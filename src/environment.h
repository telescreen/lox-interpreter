#ifndef LOX_ENVIRONMENT_HPP
#define LOX_ENVIRONMENT_HPP

#include "lox_exception.hpp"
#include "value.h"

class Environment {
public:
    Environment() = default;

    Environment(std::shared_ptr<Environment> environment);

    std::shared_ptr<Environment> EnclosingScope() const;

    void Define(const std::string& name, const Value& value);

    void Assign(const Token &name, const Value& value);

    Value Get(Token name);

private:
    std::map<std::string, Value> env;
    std::shared_ptr<Environment> enclosing = nullptr;
};

#endif
