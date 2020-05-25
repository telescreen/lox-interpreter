#ifndef LOX_SCANNER_H
#define LOX_SCANNER_H

#include "token.h"

#include <vector>
#include <string>
#include <map>

class Scanner {
public:
    Scanner(const char *source);
    ~Scanner();
    std::vector<Token> ScanTokens();

private:
    inline char advance();
    inline bool match(char expected);
    inline char peek();
    inline char peekNext();

    bool isAtEnd() const;
    void addToken(TokenType tokenType);
    void addToken(TokenType tokenType, std::string literal);
    void scanToken();
    void string();
    void number();
    void identifier();

private:
    int start;
    int current;
    int line;
    std::string source;
    std::vector<Token> tokens;
    static std::map<std::string, TokenType> const keywords;
};

#endif  // LOX_SCANNER_H
