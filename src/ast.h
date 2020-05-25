#ifndef LOX_AST_H
#define LOX_AST_H

#include <memory>
#include "token.h"


class PrintStatement;
class ExpressionStatement;
class BinaryExpression;
class UnaryExpression;
class GroupingExpression;
class NumberExpression;
class StringExpression;
class BooleanExpression;


class Statement {
public:
    virtual ~Statement() = default;

    class Visitor {
    public:
        virtual void Visit(PrintStatement& stmt) = 0;
        virtual void Visit(ExpressionStatement& stmt) = 0;
    };

    virtual void Accept(Visitor &visitor) = 0;
};

#define MAKE_STMT_VISITABLE virtual void Accept(Statement::Visitor& visitor) override { visitor.Visit(*this); }


class Expression {
public:
    virtual ~Expression() = default;

    class Visitor {
    public:
        virtual void Visit(BinaryExpression& expr) = 0;
        virtual void Visit(UnaryExpression& expr) = 0;
        virtual void Visit(GroupingExpression& expr) = 0;
        virtual void Visit(NumberExpression& expr) = 0;
        virtual void Visit(StringExpression& expr) = 0;
        virtual void Visit(BooleanExpression& expr) = 0;
    };

    virtual void Accept(Visitor& visitor) = 0;

private:
    std::unique_ptr<Expression> expr;
};
using ExpressionUPtr = std::unique_ptr<Expression>;
#define MAKE_EXPR_VISITABLE virtual void Accept(Expression::Visitor& visitor) override { visitor.Visit(*this); }


class PrintStatement: public Statement {
public:
    PrintStatement(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {
    }
    MAKE_STMT_VISITABLE

    std::unique_ptr<Expression> expression;
};


class ExpressionStatement: public Statement {
public:
    ExpressionStatement(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {
    }
    MAKE_STMT_VISITABLE

    std::unique_ptr<Expression> expression;
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
    MAKE_EXPR_VISITABLE
};


class UnaryExpression: public Expression {
public:
    Token op;
    std::unique_ptr<Expression> expression;
    UnaryExpression(Token op, std::unique_ptr<Expression> rhs): op(op), expression(std::move(rhs)) {
    }
    MAKE_EXPR_VISITABLE
};


class GroupingExpression: public Expression {
public:
    std::unique_ptr<Expression> expression;
    GroupingExpression(std::unique_ptr<Expression> rhs): expression(std::move(rhs)) {
    }
    MAKE_EXPR_VISITABLE
};


class NumberExpression: public Expression {
public:
    NumberExpression(const std::string &val) {
        this->value = strtod(val.c_str(), nullptr);
    }
    double GetValue() const { return value; }
    MAKE_EXPR_VISITABLE

private:
    double value;
};


class StringExpression: public Expression {
public:
    StringExpression(std::string value): value(value) {}
    std::string GetValue() const { return value; }
    MAKE_EXPR_VISITABLE


private:
    std::string value;
};


class BooleanExpression: public Expression {
public:
    BooleanExpression(bool value): value(value) {}
    bool GetValue() const { return value; }
    MAKE_EXPR_VISITABLE

private:
    bool value;
};


#endif
