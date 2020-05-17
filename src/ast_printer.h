#ifndef LOX_AST_PRINTER_H
#define LOX_AST_PRINTER_H


#include <string>
#include "ast.h"

class AstPrinter: public IVisitor {
public:

    std::string Print();

    void Visit(const BinaryExpression& expr) const override;

    void Visit(const UnaryExpression& expr) const override;

    void Visit(const GroupingExpression& expr) const override;

    void Visit(const NumberExpression& expr) const override;

    void Visit(const StringExpression& expr) const override;

    void Visit(const BooleanExpression& expr) const override;
};

#endif
