#ifndef LOX_EVALUATOR_HPP
#define LOX_EVALUATOR_HPP

#include <typeinfo>
#include <list>

#include <fmt/ostream.h>
#include <fmt/format.h>

#include "interpreter.h"
#include "lox_function.h"

Interpreter::Interpreter() {
    auto clock_func = std::make_shared<Clock>(Clock());
    environment->Define("clock", Value(clock_func));
}

void Interpreter::Interpret(const std::list<std::unique_ptr<Statement>>& statements) {
    for(auto& statement: statements) {
        evaluate(*statement);

        // TODO(telescreen): How to get rid of this dynamic type check and cast?
        auto pstmt = statement.get();
        if (typeid(*pstmt) == typeid(ExpressionStatement)) {
            auto expr = dynamic_cast<ExpressionStatement*>(pstmt);
            auto pexpr = expr->expression.get();
            if ((typeid(*pexpr) != typeid(AssignmentExpression)) &&
                (typeid(*pexpr) != typeid(CallExpression)) ) {
                fmt::print(fmt::format("{}\n", value));
            }
        }
    }
}

// Statement::Visitor Interface methods
void Interpreter::Visit(PrintStatement& stmt) {
    Value value = evaluate(*stmt.expression);
    fmt::print(fmt::format("{}\n", value));
}

void Interpreter::Visit(ExpressionStatement& stmt) {
    evaluate(*stmt.expression);
}

void Interpreter::Visit(VarStatement& stmt) {
    Value val;
    if (stmt.init) {
        val = evaluate(*stmt.init);
    }
    environment->Define(stmt.token.lexeme, val);
}

void Interpreter::Visit(IfStatement& stmt) {
    Value cond = evaluate(*stmt.expression);
    // TODO (telescreen): Check type of cond.
    // Only continue if cond can be evaluated to BOOL
    if (cond) {
        evaluate(*stmt.thenBranch);
    } else if (stmt.elseBranch != nullptr) {
        evaluate(*stmt.elseBranch);
    }
}

void Interpreter::Visit(WhileStatement& stmt) {
    while (evaluate(*stmt.expression)) {
        evaluate(*stmt.statement);
    }
}

void Interpreter::Visit(ReturnStatement& stmt) {
    Value val;
    if (stmt.value != nullptr) {
        val = evaluate(*stmt.value);
    }
    throw ReturnSignal(val);
}

void Interpreter::Visit(BlockStatement& stmt) {
    environment = std::make_shared<Environment>(environment);
    Interpret(stmt.statements);
    environment = environment->EnclosingScope();
}

void Interpreter::Visit(FunctionStatement& stmt) {
    auto func = std::make_shared<LoxFunction>(stmt, environment);
    environment->Define(stmt.name.lexeme, Value(func));
}

// Expression::Visitor Interface methods
void Interpreter::Visit(BinaryExpression& expr) {
    Value left = evaluate(*expr.left);
    Value right = evaluate(*expr.right);

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
    default:
        throw RuntimeError(expr.op, "Unknown Binary Operand");
        break;
    }
}

void Interpreter::Visit(UnaryExpression& expr) {
    Value val = evaluate(*expr.expression);
    assertNumber(expr.op, val, val);
    switch(expr.op.type) {
    case TokenType::MINUS:
        value = -val;
        break;
    case TokenType::PLUS:
        value = val;
        break;
    case TokenType::BANG:
        // TODO(telescreen): Implement unary !
    default:
        throw RuntimeError(expr.op, "Unknown Unary Operand");
        break;
    }
}

void Interpreter::Visit(CallExpression& expr) {
    auto callee = evaluate(*expr.callee);

    std::vector<Value> arguments;
    for(auto &arg: expr.arguments) {
        arguments.push_back(evaluate(*arg));
    }

    // TODO (telescreen): Check to see whether callee is callable
    std::shared_ptr<LoxCallable> func = static_cast<std::shared_ptr<LoxCallable>>(callee);
    if (arguments.size() != func->Arity()) {
        throw new RuntimeError(expr.paren,
            fmt::format("Expected %d arguments but got %d",
                        func->Arity(), arguments.size()).c_str());
    }

    value = func->Call(*this, arguments);
}


void Interpreter::Visit(GroupingExpression& expr) {
    value = evaluate(*expr.expression);
}


void Interpreter::Visit(LiteralExpression& expr) {
    value = expr.value;
}

void Interpreter::Visit(LogicalExpression& expr) {
    Value left = evaluate(*expr.left);
    if (expr.op.type == TokenType::OR) {
        if (left) value = left;
        return;
    } else {
        if (!left) value = left;
        return;
    }
    value = evaluate(*expr.right);
}

void Interpreter::Visit(VariableExpression& expr) {
    value = environment->Get(expr.token);
}

void Interpreter::Visit(AssignmentExpression& expr) {
    Value val = evaluate(*expr.value);
    environment->Assign(expr.name, val);
}

std::shared_ptr<Environment> Interpreter::GlobalScope() {
    return globals;
}

std::shared_ptr<Environment> Interpreter::CurrentScope() {
    return environment;
}

void Interpreter::SetScope(std::shared_ptr<Environment>& new_environment) {
    environment = new_environment;
}

#endif
