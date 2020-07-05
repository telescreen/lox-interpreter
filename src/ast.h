#ifndef LOX_AST_H
#define LOX_AST_H

#include <memory>
#include <list>
#include <vector>

#include "token.h"
#include "value.h"


class PrintStatement;
class ExpressionStatement;
class VarStatement;
class IfStatement;
class FunctionStatement;
class WhileStatement;
class ReturnStatement;
class BlockStatement;
class BinaryExpression;
class UnaryExpression;
class GroupingExpression;
class LiteralExpression;
class LogicalExpression;
class VariableExpression;
class AssignmentExpression;
class CallExpression;


// Abstract Class
class Statement {
public:
    virtual ~Statement() = default;

    class Visitor {
    public:
        virtual void Visit(PrintStatement& stmt) = 0;
        virtual void Visit(ExpressionStatement& stmt) = 0;
        virtual void Visit(VarStatement& stmt) = 0;
        virtual void Visit(IfStatement& stmt) = 0;
        virtual void Visit(ReturnStatement& stmt) = 0;
        virtual void Visit(WhileStatement& stmt) = 0;
        virtual void Visit(BlockStatement& stmt) = 0;
        virtual void Visit(FunctionStatement& stmt) = 0;
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
        virtual void Visit(LogicalExpression& expr) = 0;
        virtual void Visit(CallExpression& expr) = 0;
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
    std::unique_ptr<Expression> expression;

    PrintStatement(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {
    }
    MAKE_STMT_VISITABLE
};


class ExpressionStatement: public Statement {
public:
    std::unique_ptr<Expression> expression;

    ExpressionStatement(std::unique_ptr<Expression> expression):
        expression(std::move(expression)) {
    }
    MAKE_STMT_VISITABLE
};


class IfStatement: public Statement {
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Expression> expression,
                std::unique_ptr<Statement> thenBranch,
                std::unique_ptr<Statement> elseBranch):
        expression(std::move(expression)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {
    }
    MAKE_STMT_VISITABLE
};


class ReturnStatement: public Statement {
public:
    Token keyword;
    std::unique_ptr<Expression> value;
    ReturnStatement(Token keyword, std::unique_ptr<Expression> value):
        keyword(keyword), value(std::move(value)) {}
    MAKE_STMT_VISITABLE
};


class FunctionStatement: public Statement {
public:
    Token name;
    std::vector<Token> params;
    std::list<std::unique_ptr<Statement>> stmts;

    FunctionStatement(Token name,
                      std::vector<Token>& params,
                      std::list<std::unique_ptr<Statement>>& stmts):
        name(name), params(params), stmts(std::move(stmts)) {
    }
    MAKE_STMT_VISITABLE
};


class VarStatement: public Statement {
public:
    Token token;
    std::unique_ptr<Expression> init;

    VarStatement(Token token, std::unique_ptr<Expression> init):
        token(token), init(std::move(init)) {
    }
    MAKE_STMT_VISITABLE
};


class WhileStatement: public Statement {
public:
    std::unique_ptr<Expression> expression;
    std::unique_ptr<Statement> statement;

    WhileStatement(std::unique_ptr<Expression> expression,
                   std::unique_ptr<Statement> statement):
        expression(std::move(expression)),
        statement(std::move(statement)) {
    }
    MAKE_STMT_VISITABLE
};


class BlockStatement: public Statement {
public:
    std::list<std::unique_ptr<Statement>> statements;

    BlockStatement(std::list<std::unique_ptr<Statement>>& stmts)
        : statements(std::move(stmts)) {
    }
    MAKE_STMT_VISITABLE
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


class CallExpression: public Expression {
public:
    std::unique_ptr<Expression> callee;
    Token paren;
    std::list<std::unique_ptr<Expression>> arguments;

    CallExpression(std::unique_ptr<Expression> callee,
                   Token paren,
                   std::list<std::unique_ptr<Expression>>& arguments)
        : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {
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


class LogicalExpression: public Expression {
public:
    Token op;
    std::unique_ptr<Expression> left, right;
    LogicalExpression(std::unique_ptr<Expression> left, Token op, std::unique_ptr<Expression> right)
        : op(op), left(std::move(left)), right(std::move(right)) {
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
