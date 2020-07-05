#ifndef LOX_EVALUATOR_H
#define LOX_EVALUATOR_H

#include <list>

#include "lox_exception.hpp"
#include "ast.h"
#include "value.h"
#include "environment.h"


class Interpreter: public Expression::Visitor, public Statement::Visitor {
public:
    Interpreter();

    void Interpret(const std::list<std::unique_ptr<Statement>>&);

    void Visit(PrintStatement& stmt) override;

    void Visit(ExpressionStatement& stmt) override;

    void Visit(VarStatement& stmt) override;

    void Visit(IfStatement& stmt) override;

    void Visit(ReturnStatement& stmt) override;

    void Visit(WhileStatement& stmt) override;

    void Visit(BlockStatement& stmt) override;

    void Visit(FunctionStatement& stmt) override;

    void Visit(BinaryExpression& expr) override;

    void Visit(UnaryExpression& expr) override;

    void Visit(CallExpression& expr) override;

    void Visit(GroupingExpression& expr) override;

    void Visit(LiteralExpression& expr) override;

    void Visit(LogicalExpression& expr) override;

    void Visit(VariableExpression& expr) override;

    void Visit(AssignmentExpression& expr) override;

    std::shared_ptr<Environment> GlobalScope();

    std::shared_ptr<Environment> CurrentScope();

    void SetScope(std::shared_ptr<Environment>&);

private:
    Value evaluate(Expression& p) {
        p.Accept(*this);
        return value;
    }

    void evaluate(Statement& s) {
        s.Accept(*this);
    }

    void assertNumber(const Token& token, const Value& left, const Value& right) {
        if (left.value_type != Value::ValueType::NUMBER ||
            right.value_type != Value::ValueType::NUMBER) {
            throw RuntimeError(token, "Operand must be a number");
        }
    }

    Value value;   // This is the global variable that store value for all expression evaluation
    std::shared_ptr<Environment> globals = std::make_shared<Environment>();
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(globals);
};

#endif
