#ifndef LOX_EVALUATOR_HPP
#define LOX_EVALUATOR_HPP

#include <fmt/ostream.h>
#include <fmt/format.h>

#include "ast.h"
#include "lox.hpp"
#include "value.hpp"
#include "lox_exception.hpp"
#include "environment.hpp"


class Interpreter: public Expression::Visitor, public Statement::Visitor {

public:
    Value Evaluate(Expression& p) {
        p.Accept(*this);
        return value;
    }

    void Interpret(const std::vector<std::unique_ptr<Statement>>& statements) {
        for(auto& statement: statements) {
            statement->Accept(*this);
        }
    }

    // Statement::Visitor Interface methods
    void Visit(PrintStatement& stmt) override {
        Value value = Evaluate(*stmt.expression);
        fmt::print(fmt::format("{}\n", value));
    }


    void Visit(ExpressionStatement& stmt) override {
        Evaluate(*stmt.expression);
    }


    void Visit(VarStatement& stmt) override {
        Value val;
        if (stmt.init) {
            val = Evaluate(*stmt.init);
        }
        environment.Define(stmt.token.lexeme, val);
    }


    // Expression::Visitor Interface methods
    void Visit(BinaryExpression& expr) override {
        Value left = Evaluate(*expr.left);
        Value right = Evaluate(*expr.right);

        switch(expr.op.type) {
        case TokenType::PLUS:
            value = left + right;
            break;
        case TokenType::MINUS:
            assertNumber(expr.op, left, right);
            value = left - right;
            break;
        case TokenType::STAR:
            assertNumber(expr.op, left, right);
            value = left * right;
            break;
        case TokenType::SLASH:
            assertNumber(expr.op, left, right);
            value =  left / right;
            break;
        case TokenType::GREATER:
            value = left > right;
            break;
        case TokenType::GREATER_EQUAL:
            value = left >= right;
            break;
        case TokenType::LESS:
            value = left < right;
            break;
        case TokenType::LESS_EQUAL:
            value = left <= right;
            break;
        case TokenType::EQUAL_EQUAL:
            value = left == right;
            break;
        case TokenType::BANG_EQUAL:
            value = left != right;
            break;
        }

    }

    void Visit(UnaryExpression& expr) override {
        Value val = Evaluate(*expr.expression);
        assertNumber(expr.op, val, val);
        switch(expr.op.type) {
        case TokenType::MINUS:
            value = -val;
            break;
        case TokenType::PLUS:
            value = val;
            break;
        }
    }

    void Visit(GroupingExpression& expr) override {
        value = Evaluate(*expr.expression);
    }


    void Visit(LiteralExpression& expr) override {
        value = expr.value;
    }


    void Visit(VariableExpression& expr) override {
        value = environment.Get(expr.token);
    }

    void Visit(AssignmentExpression& expr) override {
        Value value = Evaluate(*expr.value);
        environment.Assign(expr.name, value);
    }

private:
    void assertNumber(const Token &token, const Value& left, const Value& right) {
        if (left.value_type != Value::NUMBER || right.value_type != Value::NUMBER) {
            throw RuntimeError(token, "Operand must be a number");
        }
    }

private:
    Value value;
    Environment environment;
};

#endif
