#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include <vector>

#include "ast.h"
#include "interpreter.h"


class LoxCallable {
public:
    virtual int Arity() = 0;
    virtual Value Call(Interpreter& interpreter, std::vector<Value>& arguments) = 0;
    virtual ~LoxCallable() = default;
};


class LoxFunction: public LoxCallable {
public:
    LoxFunction(FunctionStatement& declaration);
    Value Call(Interpreter& interpreter, std::vector<Value>& arguments) override;
    int Arity() override;

private:
    FunctionStatement& declaration;
};


/* Builtin Functions */

class Clock: public LoxCallable {
public:
    Clock() = default;
    Value Call(Interpreter& interpreter, std::vector<Value>& arguments) override;
    int Arity() override;
};

#endif