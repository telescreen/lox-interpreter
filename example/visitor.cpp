#include <iostream>
#include <string>
#include <memory>

/**
 * Visitor Implementation from CodeProject
 * https://www.codeproject.com/Tips/1018315/Visitor-with-the-Return-Value
 */

class Number;
class Plus;

class INodeVisitor {
public:
    virtual void Visit(Number& n) = 0;
    virtual void Visit(Plus& p) = 0;
};
#define MAKE_VISITABLE virtual void Accept(INodeVisitor& vis) override { vis.Visit(*this); }


class Node {
public:
    virtual void Accept(INodeVisitor& vis) = 0;
};
using NodePtr = std::shared_ptr<Node>;


class Number: public Node {
public:
    int value;
    Number(int value_) : value(value_) {}
    MAKE_VISITABLE
};


class Plus: public Node {
public:
    NodePtr left, right;
    Plus(NodePtr left_, NodePtr right_): left {left_}, right{right_} {}
    MAKE_VISITABLE

};

std::shared_ptr<Plus> plus(NodePtr left, NodePtr right) { return std::make_shared<Plus>(left, right); }
std::shared_ptr<Number> number(int value) { return std::make_shared<Number>(value); }


template<typename VisitorImpl, typename VisitablePtr, typename ResultType>
class ValueGetter {
public:
    static ResultType GetValue(VisitablePtr n) {
        VisitorImpl vis;
        n->Accept(vis);
        return vis.value;
    }

    void Return(ResultType value_) {
        value = value_;
    }

private:
    ResultType value;
};


class Evaluator: public ValueGetter<Evaluator, NodePtr, int>, public INodeVisitor {
public:
    virtual void Visit(Number &n) {
        Return(n.value);
    }

    virtual void Visit(Plus& n) {
        Return(GetValue(n.left) + GetValue(n.right));
    }
};


class Serializer : public ValueGetter<Serializer, NodePtr, std::string>, public INodeVisitor
{
public:
    virtual void Visit(Number& n)
    {
        Return(std::to_string(n.value));
    }

    virtual void Visit(Plus& n)
    {
        Return("(" + GetValue(n.left) + " + " + GetValue(n.right) + ")");
    }
};

int main() {
    auto expr = plus(number(3), plus(number(5), number(7)));
    std::cout << Serializer::GetValue(expr)<< " = " << Evaluator::GetValue(expr) << std::endl;
    return 0;
}
