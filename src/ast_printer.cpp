#include "ast_printer.h"


std::string AstPrinter::Print(const Expression &expression) {
    return expression.accept(*this);
}
