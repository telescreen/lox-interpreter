#include <fmt/format.h>
#include "lox.hpp"
#include <cstdio>

void runFile(const char* path) {
    FILE *f = fopen(path, "r");
    if (!f) {
        std::perror("Failed to open file");
        return;
    }

    constexpr int BUFSIZE = 8192;
    char buffer[BUFSIZE];
    int result = fread(buffer, 1, BUFSIZE, f);
    if (result == 0) {
        std::perror("Failed to read data from file");
        fclose(f);
        return;
    }

    Lox lox;
    lox.Interpret(buffer);

    fclose(f);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fmt::print("Usage: lox [script.l]\n");
        return EXIT_FAILURE;
    } else if(argc == 2) {
        runFile(argv[1]);
    } else {
        Lox lox;
        lox.Prompt();
    }

    return 0;
}
