#include <iostream>
#include <vector>
#include <string>

#include <linenoise.h>

#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "interpreter.hpp"


bool hadError = false;

class Lox {
public:
    static void Report(int line, std::string where, std::string message) {
        std::cerr << "[line: " << line << "]"
                  << where << ": " << message << std::endl;
        hadError = true;
    }

    static void Error(int line, std::string message) {
        Report(line, "", message);
    }

    void Prompt() {
        linenoiseSetMultiLine(1);
        linenoiseHistoryLoad("history.txt");

        char *buffer;
        while ((buffer=linenoise(">>> ")) != NULL) {
            linenoiseHistoryAdd(buffer);
            linenoiseHistorySave("history.txt");

            if (buffer[0] != '\0') {

                try {
                    Scanner scanner(buffer);
                    std::vector<Token> tokens = scanner.ScanTokens();
                    Parser parser(tokens);
                    auto expr = parser.Parse();

                    Interpreter itpr;
                    Value value = itpr.Evaluate(*expr);
                    std::cout << value << std::endl;
                } catch(ParserError& e) {
                    std::cerr << e.what() << std::endl;
                }
            }

            linenoiseFree(buffer);
        }
    }
};


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
