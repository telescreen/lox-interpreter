#ifndef LOX_AST_H
#define LOX_AST_H

#include <memory>
#include "token.h"


class BinaryExpression;
class UnaryExpression;
class GroupingExpression;
class NumberExpression;
class StringExpression;
class BooleanExpression;


class IVisitor {
public:
    virtual ~IVisitor();

    virtual void Visit(const BinaryExpression& expr) const;

    virtual void Visit(const UnaryExpression& expr) const;

    virtual void Visit(const GroupingExpression& expr) const;

    virtual void Visit(const NumberExpression& expr) const;

    virtual void Visit(const StringExpression& expr) const;

    virtual void Visit(const BooleanExpression& expr) const;
};



class Expression {
public:
    virtual ~Expression() = default;

    virtual void Accept(const IVisitor &visitor) const = 0;
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

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }
};


class UnaryExpression: public Expression {
public:
    Token op;
    std::unique_ptr<Expression> right;

    UnaryExpression(Token op, std::unique_ptr<Expression> rhs)
        : op(op), right(std::move(rhs)) {
    }

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }
};


class GroupingExpression: public Expression {
public:
    std::unique_ptr<Expression> right;

    GroupingExpression(std::unique_ptr<Expression> rhs)
        : right(std::move(rhs)) {
    }

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }
};


class NumberExpression: public Expression {
public:
    NumberExpression(const std::string &val) {
        this->value = strtod(val.c_str(), nullptr);
    }
    double GetValue() const { return value; }

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }

private:
    double value;
};


class StringExpression: public Expression {
public:
    StringExpression(std::string value): value(value) {}
    std::string GetValue() const { return value; }

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }

private:
    std::string value;
};


class BooleanExpression: public Expression {
public:
    BooleanExpression(bool value): value(value) {}
    bool GetValue() const { return value; }

    void Accept(const IVisitor &visitor) const override {
        visitor.Visit(*this);
    }

private:
    bool value;
};


#endif
