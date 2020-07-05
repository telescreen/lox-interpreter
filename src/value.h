#ifndef LOX_VALUE_H
#define LOX_VALUE_H

#include <iostream>
#include <string>
#include <memory>
#include <map>

class LoxCallable;

class Value {
public:
    enum class ValueType { FUNCTION, NUMBER, BOOL, STRING, NUL } value_type;
    const char* value_type_string() const {
        const std::map<ValueType, const char*> value_type_enum {
            { ValueType::FUNCTION, "Function" },
            { ValueType::NUMBER, "Number" },
            { ValueType::BOOL, "Boolean" },
            { ValueType::STRING, "String" },
            { ValueType::NUL, "Null" },
        };
        auto it  = value_type_enum.find(value_type);
        return it == value_type_enum.end() ? "Unknown type" : it->second;
    };

    Value();
    Value(double number);
    Value(bool logic_value);
    Value(std::string text);
    Value(std::shared_ptr<LoxCallable> func);

    // TODO(telescreen): Add Type Checking
    operator double() const;
    operator bool() const;
    operator std::string() const;
    operator std::shared_ptr<LoxCallable>() const;

    friend Value operator+(const Value& lhs, const Value& rhs);

    friend Value operator-(const Value& lhs, const Value& rhs);

    friend Value operator-(const Value& rhs);

    friend Value operator*(const Value& lhs, const Value& rhs);

    friend Value operator/(const Value& lhs, const Value& rhs);

    friend Value operator>(const Value& lhs, const Value& rhs);

    friend Value operator>=(const Value& lhs, const Value& rhs);

    friend Value operator<(const Value& lhs, const Value& rhs);

    friend Value operator<=(const Value& lhs, const Value& rhs);

    friend Value operator==(const Value& lhs, const Value& rhs);

    friend Value operator!=(const Value& lhs, const Value& rhs);

    friend std::ostream& operator<<(std::ostream &os, const Value& value);

    template<typename Comparator>
    friend bool compare(const Value& lhs, const Value& rhs, Comparator comparator);

private:
    std::shared_ptr<LoxCallable> func;
    double number;
    bool logic_value;
    std::string text;
};

#endif
