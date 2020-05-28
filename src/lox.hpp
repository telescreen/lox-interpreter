#ifndef LOX_LOX_HPP
#define LOX_LOX_HPP

#include <string>

#include <linenoise.h>
#include <fmt/format.h>

#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "interpreter.hpp"


class Lox {
public:
    static void Report(int line, std::string where, const std::string& message) {
        fmt::print(stderr, "[line: {}] {}: {}\n", line, where, message);
    }


    static void Error(int line, const std::string& message) {
        Report(line, "", message);
    }


    static void Error(Token token, const std::string& message) {
        if (token.type == TokenType::TEOF) {
            Report(token.line, " at end", message);
        } else {
            Report(token.line, " at '" + token.lexeme + "'", message);
        }
    }


    void Prompt() {
        linenoiseSetMultiLine(1);
        linenoiseHistoryLoad("history.txt");

        char *buffer;
        Interpreter interpreter;

        while ((buffer=linenoise(">>> ")) != NULL) {
            linenoiseHistoryAdd(buffer);
            linenoiseHistorySave("history.txt");

            if (buffer[0] != '\0') {

                try {
                    Scanner scanner(buffer);
                    auto tokens = scanner.ScanTokens();
                    Parser parser(tokens);
                    auto stmts = parser.Parse();
                    interpreter.Interpret(stmts);
                } catch(ParserError& e) {
                } catch(RuntimeError& e) {
                    Lox::Error(e.GetToken(), e.what());
                }
            }

            linenoiseFree(buffer);
        }
    }
};


#endif
