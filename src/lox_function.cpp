#include "lox_function.h"

#include <time.h>

LoxFunction::LoxFunction(FunctionStatement& declaration):
    declaration(declaration) {
}

Value LoxFunction::Call(Interpreter& interpreter, std::vector<Value>& arguments) {
    auto environment = std::make_shared<Environment>(interpreter.GlobalScope());
    for(int i = 0; i < arguments.size(); ++i) {
        environment->Define(declaration.params[i].lexeme, arguments[i]);
    }
    interpreter.Interpret(declaration.stmts, environment);
    return Value();
}

int LoxFunction::Arity() {
    return declaration.params.size();
}

/* Builtin Functions */

int Clock::Arity() { return 0; }

Value Clock::Call(Interpreter& interpreter, std::vector<Value>& arguments) {
    time_t now = time(NULL);
    return Value(static_cast<double>(now));
}