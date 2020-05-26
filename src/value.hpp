#ifndef LOX_VALUE_HPP
#define LOX_VALUE_HPP

#include <string>
#include <iostream>

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



#endif
