#include <iostream>
#include <vector>
#include <string>

#include "scanner.h"
#include "token.h"


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
        std::string buffer;
        while (1) {
            std::cout << "> ";
            std::getline(std::cin, buffer);
            if (std::cin.eof()) {
                return;
            }
            run(buffer);
        }
    }

private:
    void run(std::string source) {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.ScanTokens();

        for(auto &token: tokens) {
            std::cout << token << std::endl;
        }
    }
};


int main(int argc, char **argv) {
    Lox lox;
    if (argc > 2) {
        std::cout << "Usage: lox [script.l]" << std::endl;
        return (1);
    } else if(argc == 2) {
        //runFile(argv[1]);
    } else {
        lox.Prompt();
    }

}
