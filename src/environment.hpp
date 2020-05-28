#ifndef LOX_ENVIRONMENT_HPP
#define LOX_ENVIRONMENT_HPP

#include "value.hpp"
#include "fmt/format.h"
#include "lox_exception.hpp"

class Environment {
public:
    void Define(const std::string& name, const Value& value) {
        env.insert(std::pair<std::string, Value>(name, value));
    }

    Value Get(Token name) {
        if (env.find(name.lexeme) != env.end()) {
            return env[name.lexeme];
        }
        throw RuntimeError(name, "Variable undefined");
    }

private:
    std::map<std::string, Value> env;
};

#endif
