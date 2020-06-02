#ifndef LOX_AST_H
#define LOX_AST_H

#include <memory>
#include "token.h"
#include "value.hpp"


class PrintStatement;
class ExpressionStatement;
class VarStatement;
class BinaryExpression;
class UnaryExpression;
class GroupingExpression;
class LiteralExpression;
class VariableExpression;
class AssignmentExpression;


// Abstract Class
class Statement {
public:
    virtual ~Statement() = default;

    class Visitor {
    public:
        virtual void Visit(PrintStatement& stmt) = 0;
        virtual void Visit(ExpressionStatement& stmt) = 0;
        virtual void Visit(VarStatement& stmt) = 0;
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
        virtual void Visit(LiteralExpression& expr) = 0;
        virtual void Visit(VariableExpression& expr) = 0;
        virtual void Visit(AssignmentExpression& expr) = 0;
    };

    virtual void Accept(Visitor& visitor) = 0;

private:
    std::unique_ptr<Expression> expr;
};
using ExpressionUPtr = std::unique_ptr<Expression>;
#define MAKE_EXPR_VISITABLE virtual void Accept(Expression::Visitor& visitor) override { visitor.Visit(*this); }


/* Statement Declaration */
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


class VarStatement: public Statement {
public:
    VarStatement(Token token, std::unique_ptr<Expression> init):
        token(token), init(std::move(init)) {
    }
    MAKE_STMT_VISITABLE

    Token token;
    std::unique_ptr<Expression> init;
};


/* Expression Declaration */

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


class LiteralExpression: public Expression {
public:
    Value value;
    LiteralExpression(Value value): value(value) {
    }
    MAKE_EXPR_VISITABLE
};


class VariableExpression: public Expression {
public:
    Token token;
    VariableExpression(Token token): token(token) {
    }
    MAKE_EXPR_VISITABLE
};


class AssignmentExpression: public Expression {
public:
    Token name;
    std::unique_ptr<Expression> value;
    AssignmentExpression(Token name, std::unique_ptr<Expression> value)
        : name(name), value(std::move(value)) {
    }
    MAKE_EXPR_VISITABLE
};

#endif
