#ifndef LOX_EVALUATOR_HPP
#define LOX_EVALUATOR_HPP

#include <variant>

#include "ast.h"
#include "lox_exception.hpp"


/* Value interpreted by the interpreter */
class Value {
public:
    enum { NUMBER, BOOL, STRING, NUL } value_type;

    Value(): value_type(NUL) {}
    Value(double number): number(number), value_type(NUMBER) {}
    Value(bool logic_value): logic_value(logic_value), value_type(BOOL) {}
    Value(std::string text): text(text), value_type(STRING) {}

    friend Value operator+(const Value& lhs, const Value& rhs) {
        Value val(lhs);
        switch(lhs.value_type) {
        case Value::NUMBER:
            val.number += rhs.number;

        case Value::STRING:
            val.text += rhs.text;
        }
        return val;
    }

    friend Value operator-(const Value& lhs, const Value& rhs) {
        return Value(lhs.number - rhs.number);
    }

    friend Value operator-(const Value& rhs) {
        return Value(-rhs.number);
    }

    friend Value operator*(const Value& lhs, const Value& rhs) {
        return Value(lhs.number * rhs.number);
    }

    friend Value operator/(const Value& lhs, const Value& rhs) {
        return Value(lhs.number / rhs.number);
    }

    friend std::ostream& operator<<(std::ostream &os, const Value& value) {
        switch(value.value_type) {
        case Value::NUMBER:
            os << value.number;
            break;
        case Value::BOOL:
            os << value.logic_value;
            break;
        default:
            os << value.text;
        }
        return os;
    }


private:
    double number;
    bool logic_value;
    std::string text;
};



class Interpreter: public Expression::Visitor {
public:

    Value Evaluate(Expression& p) {
        p.Accept(*this);
        return value;
    }

    /* Visitor Interface methods */
    void Visit(BinaryExpression& expr) override {
        Value left = Evaluate(*expr.left);
        Value right = Evaluate(*expr.right);

        switch(expr.op.type) {
        case TokenType::PLUS:
            value = left + right;
            break;
        case TokenType::MINUS:
            value = left - right;
            break;
        case TokenType::STAR:
            value = left * right;
            break;
        case TokenType::SLASH:
            value =  left / right;
            break;
        }
    }

    void Visit(UnaryExpression& expr) override {
        Value val = Evaluate(*expr.expression);
        switch(expr.op.type) {
        case TokenType::MINUS:
            value = -val;
            break;
        case TokenType::PLUS:
            value = val;
            break;
        }
    }

    void Visit(GroupingExpression& expr) override {
        value = Evaluate(*expr.expression);
    }


    void Visit(NumberExpression& expr) override {
        value = Value(expr.GetValue());
    }

    void Visit(StringExpression& expr) override {
        value = Value(expr.GetValue());
    }


    void Visit(BooleanExpression& expr) override {
        value = Value(expr.GetValue());
    }

private:
    Value value;

};

#endif
