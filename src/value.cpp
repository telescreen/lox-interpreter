#include <fmt/format.h>

#include "value.h"

/* Value interpreted by the interpreter
   TODO(telescreen): Add Type checking
*/
Value::Value(): value_type(ValueType::NUL) {}

Value::Value(double number): number(number), value_type(ValueType::NUMBER) {}

Value::Value(bool logic_value): logic_value(logic_value), value_type(ValueType::BOOL) {}

Value::Value(std::string text): text(text), value_type(ValueType::STRING) {}

Value::Value(std::shared_ptr<LoxCallable> func): func(func), value_type(ValueType::FUNCTION) {}

    // TODO(telescreen): Add Type Checking
Value::operator double() const { return number; }
Value::operator bool() const {
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
    case ValueType::FUNCTION:
        eval_bool = func != nullptr;
    default:
        eval_bool = logic_value;
        break;
    }
    return eval_bool;
}

Value::operator std::string() const { return text; }

Value::operator std::shared_ptr<LoxCallable>() const { return func; }

Value operator+(const Value& lhs, const Value& rhs) {
    Value val(lhs);
    switch(lhs.value_type) {
    case Value::ValueType::NUMBER:
        val.number += rhs.number;
        break;
    case Value::ValueType::STRING:
        val.text += rhs.text;
        break;
    default:
        throw TypeError(fmt::format("unsupported operand type(s) for +: {} and {}",
                                    lhs.value_type_string(), rhs.value_type_string()).c_str());
        break;
    }
    return val;
}

Value operator-(const Value& lhs, const Value& rhs) {
    return Value(lhs.number - rhs.number);
}

Value operator-(const Value& rhs) {
    return Value(-rhs.number);
}

Value operator*(const Value& lhs, const Value& rhs) {
    return Value(lhs.number * rhs.number);
}

Value operator/(const Value& lhs, const Value& rhs) {
    return Value(lhs.number / rhs.number);
}

Value operator>(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::greater<>());
}

Value operator>=(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::greater_equal<>());
}

Value operator<(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::less<>());
}

Value operator<=(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::less_equal<>());
}

Value operator==(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::equal_to<>());
}

Value operator!=(const Value& lhs, const Value& rhs) {
    return compare(lhs, rhs, std::not_equal_to<>());
}

std::ostream& operator<<(std::ostream &os, const Value& value) {
    switch(value.value_type) {
    case Value::ValueType::NUMBER:
        os << value.number;
        break;
    case Value::ValueType::BOOL:
        os << (value.logic_value ? "True" : "False");
        break;
    case Value::ValueType::STRING:
        os << value.text;
        break;
    case Value::ValueType::FUNCTION:
        os << "Function object at" << &value.func;
        break;
    default:
        os << "NUL";
        break;
    }
    return os;
}

template<typename Comparator>
bool compare(const Value& lhs, const Value& rhs, Comparator comparator) {
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
    case Value::ValueType::FUNCTION:
        ret = comparator(&lhs.func, &rhs.func);
        break;
    case Value::ValueType::NUL:
        ret = rhs.value_type == Value::ValueType::NUL;
        break;
    }
    return ret;
}