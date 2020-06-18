#ifndef LOX_VALUE_HPP
#define LOX_VALUE_HPP

#include <iostream>
#include <string>
#include <functional>

#include <fmt/format.h>

#include "lox_exception.hpp"


/* Value interpreted by the interpreter
   TODO(telescreen): Add Type checking
*/
class Value {
public:
    enum class ValueType { NUMBER, BOOL, STRING, NUL } value_type;
    static const char* value_type_string(ValueType vtype) {
        const std::map<ValueType, const char*> value_type_enum {
            { ValueType::NUMBER, "Number" },
            { ValueType::BOOL, "Boolean" },
            { ValueType::STRING, "String" },
            { ValueType::NUL, "Null" },
        };
        auto it  = value_type_enum.find(vtype);
        return it == value_type_enum.end() ? "Unknown type" : it->second;
    };

    Value(): value_type(ValueType::NUL) {}
    Value(double number): number(number), value_type(ValueType::NUMBER) {}
    Value(bool logic_value): logic_value(logic_value), value_type(ValueType::BOOL) {}
    Value(std::string text): text(text), value_type(ValueType::STRING) {}

    // TODO(telescreen): Add Type Checking
    operator double() const { return number; }
    operator bool() const {
        bool eval_bool;
        switch (value_type) {
        case ValueType::NUL:
            eval_bool = false;
            break;
        case ValueType::STRING:
            eval_bool = text.size() > 0;
            break;
        case ValueType::NUMBER:
            eval_bool = number > 0;
            break;
        default:
            eval_bool = logic_value;
            break;
        }
        return eval_bool;
    }
    operator std::string() const { return text; }

    friend Value operator+(const Value& lhs, const Value& rhs) {
        Value val(lhs);
        switch(lhs.value_type) {
        case ValueType::NUMBER:
            val.number += rhs.number;
            break;
        case ValueType::STRING:
            val.text += rhs.text;
            break;
        default:
            throw TypeError(fmt::format("unsupported operand type(s) for +: {} and {}",
                                        Value::value_type_string(lhs.value_type),
                                        Value::value_type_string(rhs.value_type)).c_str());
            break;
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
        case Value::ValueType::NUMBER:
            os << value.number;
            break;
        case Value::ValueType::BOOL:
            os << value.logic_value;
            break;
        case Value::ValueType::STRING:
            os << value.text;
            break;
        default:
            os << "NUL";
            break;
        }
        return os;
    }

    template<typename Comparator>
    friend bool compare(const Value& lhs, const Value& rhs, Comparator comparator) {
        bool ret;
        switch(lhs.value_type) {
        case Value::ValueType::NUMBER:
            ret = comparator(lhs.number, rhs.number);
            break;
        case Value::ValueType::BOOL:
            ret = comparator(lhs.logic_value, rhs.logic_value);
            break;
        case Value::ValueType::STRING:
            ret = comparator(lhs.text, rhs.text);
            break;
        case Value::ValueType::NUL:
            ret = rhs.value_type == Value::ValueType::NUL;
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
