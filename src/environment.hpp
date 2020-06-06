#ifndef LOX_ENVIRONMENT_HPP
#define LOX_ENVIRONMENT_HPP

#include "value.hpp"
#include "fmt/format.h"
#include "lox_exception.hpp"

class Environment {
public:
    Environment() {
    }

    Environment(Environment* environment) {
        enclosing = environment;
    }

    void Define(const std::string& name, const Value& value) {
        env.insert(std::pair<std::string, Value>(name, value));
    }

    void Assign(const Token &name, const Value& value) {
        if (env.find(name.lexeme) != env.end()) {
            env[name.lexeme] = value;
            return;
        }

        if (enclosing != nullptr) {
            enclosing->Assign(name, value);
            return;
        }

        throw RuntimeError(name, "Variable undefined");
    }

    Value Get(Token name) {
        if (env.find(name.lexeme) != env.end()) {
            return env[name.lexeme];
        }

        if (enclosing) return enclosing->Get(name);

        throw RuntimeError(name, "Variable undefined");
    }

private:
    std::map<std::string, Value> env;
    Environment* enclosing = nullptr;
};

#endif
