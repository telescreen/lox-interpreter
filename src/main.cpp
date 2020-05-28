#include <fmt/format.h>
#include "lox.hpp"

int main(int argc, char **argv) {
    Lox lox;
    if (argc > 2) {
        fmt::print("Usage: lox [script.l]\n");
        return 1;
    } else if(argc == 2) {
        //runFile(argv[1]);
    } else {
        lox.Prompt();
    }

    return 0;
}
