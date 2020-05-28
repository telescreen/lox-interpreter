#ifndef LOX_VALUE_HPP
#define LOX_VALUE_HPP

#include <iostream>
#include <string>
#include <functional>


/* Value interpreted by the interpreter
   TODO(telescreen): Add Type checking
*/
class Value {
public:
    enum { NUMBER, BOOL, STRING, NUL } value_type;

    Value(): value_type(NUL) {}
    Value(double number): number(number), value_type(NUMBER) {}
    Value(bool logic_value): logic_value(logic_value), value_type(BOOL) {}
    Value(std::string text): text(text), value_type(STRING) {}

    // TODO(telescreen): Add Type Checking
    operator double() const { return number; }
    operator bool() const { return logic_value; }
    operator std::string() const { return text; }

    friend Value operator+(const Value& lhs, const Value& rhs) {
        Value val(lhs);
        switch(lhs.value_type) {
        case NUMBER:
            val.number += rhs.number;

        case STRING:
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

    friend Value operator>(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::greater<>());
    }

    friend Value operator>=(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::greater_equal<>());
    }

    friend Value operator<(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::less<>());
    }

    friend Value operator<=(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::less_equal<>());
    }

    friend Value operator==(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::equal_to<>());
    }

    friend Value operator!=(const Value& lhs, const Value& rhs) {
        return compare(lhs, rhs, std::not_equal_to<>());
    }

    friend std::ostream& operator<<(std::ostream &os, const Value& value) {
        switch(value.value_type) {
        case Value::NUMBER:
            os << value.number;
            break;
        case Value::BOOL:
            os << value.logic_value;
            break;
        case Value::STRING:
            os << value.text;
            break;
        }
        return os;
    }

    template<typename Comparator>
    friend bool compare(const Value& lhs, const Value& rhs, Comparator comparator) {
        bool ret;
        switch(lhs.value_type) {
        case Value::NUMBER:
            ret = comparator(lhs.number, rhs.number);
            break;
        case Value::BOOL:
            ret = comparator(lhs.logic_value, rhs.logic_value);
            break;
        case Value::STRING:
            ret = comparator(lhs.text, rhs.text);
            break;
        }
        return ret;
    }

private:
    double number;
    bool logic_value;
    std::string text;
};



#endif
