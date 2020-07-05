#include "lox_function.h"
#include "lox_exception.hpp"

#include <time.h>

LoxFunction::LoxFunction(FunctionStatement& declaration, std::shared_ptr<Environment>& environment):
    declaration(declaration), closure(environment) {
}

Value LoxFunction::Call(Interpreter& interpreter, std::vector<Value>& arguments) {
    /* Each function will be run in its own environment */
    auto previous = interpreter.CurrentScope();
    auto new_scope = std::make_shared<Environment>(closure);

    interpreter.SetScope(new_scope);

    for(int i = 0; i < arguments.size(); ++i) {
        new_scope->Define(declaration.params[i].lexeme, arguments[i]);
    }

    Value return_value;
    try {
        interpreter.Interpret(declaration.stmts);
    } catch(ReturnSignal& returnValue) {
        return_value = returnValue.value;
    }

    interpreter.SetScope(previous);
    return return_value;
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