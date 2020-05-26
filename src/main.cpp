#include <iostream>

#include "lox.hpp"


int main(int argc, char **argv) {
    Lox lox;
    if (argc > 2) {
        std::cout << "Usage: lox [script.l]" << std::endl;
        return 1;
    } else if(argc == 2) {
        //runFile(argv[1]);
    } else {
        lox.Prompt();
    }

    return 0;
}
