#include "environment.h"


Environment::Environment(std::shared_ptr<Environment> environment) {
    enclosing = environment;
}

std::shared_ptr<Environment> Environment::EnclosingScope() const {
    return enclosing;
}

void Environment::Define(const std::string& name, const Value& value) {
    env.insert(std::pair<std::string, Value>(name, value));
}

void Environment::Assign(const Token &name, const Value& value) {
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

Value Environment::Get(Token name) {
    if (env.find(name.lexeme) != env.end()) {
        return env[name.lexeme];
    }

    if (enclosing) return enclosing->Get(name);

    throw RuntimeError(name, "Variable undefined");
}