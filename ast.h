#ifndef LOX_AST_H
#define LOX_AST_H

#include <memory>
#include "token.h"


class Expression {
public:
    virtual ~Expression() = default;
};



class BinaryExpression: public Expression {
public:
    Token op;
    std::unique_ptr<Expression> left, right;

    BinaryExpression(Token op,
                     std::unique_ptr<Expression> lhs,
                     std::unique_ptr<Expression> rhs)
        : op(op), left(std::move(lhs)), right(std::move(rhs)) {
    }
};


class UnaryExpression: public Expression {
public:
    Token op;
    std::unique_ptr<Expression> right;

    UnaryExpression(Token op, std::unique_ptr<Expression> rhs)
        : op(op), right(std::move(rhs)) {
    }
};


class GroupingExpression: public Expression {
public:
    std::unique_ptr<Expression> right;

    GroupingExpression(std::unique_ptr<Expression> rhs)
        : right(std::move(rhs)) {
    }
};


class NumberExpression: public Expression {
public:
    NumberExpression(const std::string &val) {
        this->value = strtod(val.c_str(), nullptr);
    }
    double GetValue() const { return value; }

private:
    double value;
};


class StringExpression: public Expression {
public:
    StringExpression(std::string value): value(value) {}
    std::string GetValue() const { return value; }

private:
    std::string value;
};


class BooleanExpression: public Expression {
public:
    BooleanExpression(bool value): value(value) {}
    bool GetValue() const { return value; }

private:
    bool value;
};


#endif
