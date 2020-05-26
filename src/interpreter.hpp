#ifndef LOX_EVALUATOR_HPP
#define LOX_EVALUATOR_HPP


#include "ast.h"
#include "value.hpp"
#include "lox_exception.hpp"


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
        std::cout << value << std::endl << std::flush;
    }


    void Visit(ExpressionStatement& stmt) override {
        Evaluate(*stmt.expression);
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
            value = left - right;
            break;
        case TokenType::STAR:
            value = left * right;
            break;
        case TokenType::SLASH:
            value =  left / right;
            break;
        }
    }

    void Visit(UnaryExpression& expr) override {
        Value val = Evaluate(*expr.expression);
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
        value = expr.GetValue();
    }

private:
    Value value;

};

#endif
