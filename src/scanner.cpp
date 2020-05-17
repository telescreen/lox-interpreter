#include "scanner.h"
#include <cctype>
#include <cstdlib>


const std::map<std::string, TokenType> Scanner::keywords = {
    { "and", TokenType::AND },
    { "or", TokenType::OR },
    { "class", TokenType::CLASS },
    { "else", TokenType::ELSE },
    { "false", TokenType::FALSE },
    { "for", TokenType::FOR },
    { "fun", TokenType::FUN },
    { "if", TokenType::IF },
    { "nil", TokenType::NIL },
    { "print", TokenType::PRINT },
    { "return", TokenType::RETURN },
    { "super", TokenType::SUPER },
    { "this", TokenType::THIS },
    { "true", TokenType::TRUE },
    { "var", TokenType::VAR },
    { "while", TokenType::WHILE },
};


Scanner::Scanner(std::string source) {
    this->source = source;
}


Scanner::~Scanner() {
}


std::vector<Token> Scanner::ScanTokens() {
    start = 0;
    current = 0;
    line = 1;
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break;
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break;
    case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
    case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
    case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
    case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) advance();
        } else {
            addToken(TokenType::SLASH);
        }
        break;
    case '\r':
    case ' ':
    case '\t':
        // Ignore whitespace.
        break;

    case '\n':
        line++;
        break;

    case '"': string(); break;

    default:
        if (isdigit(c)) {
            number();
        } else if (isalpha(c)) {
            identifier();
        }else {
            std::cerr << line << " unexpected character." << std::endl;
        }
        break;
    }
}

void Scanner::identifier() {
    while (isalnum(peek())) advance();
    std::string text = source.substr(start, current-start);
    TokenType type = TokenType::IDENTIFIER;
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        type = it->second;
    }
    addToken(type);
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    /* Do not accept multiple line string */
    if (isAtEnd()) {
        std::cerr << "Unterminated string" << std::endl;
        return;
    }

    advance(); // skip the last '"'
    std::string value = source.substr(start+1, current-start-2);
    addToken(TokenType::STRING, value);
}


void Scanner::number() {
    while(isdigit(peek()))  advance();

    if (peek() == '.' && isdigit(peekNext())) {
        advance();
        while(isdigit(peek())) advance();
    }
    std::string number = source.substr(start, current-start);
    addToken(TokenType::NUMBER, number);
}


bool Scanner::isAtEnd() const {
    return current >= source.length();
}


inline char Scanner::advance() {
    current++;
    return source[current-1];
}


inline bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}


inline char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}


inline char Scanner::peekNext() {
    if (isAtEnd() || current + 1 >= source.length()) return '\0';
    return source[current+1];
}


void Scanner::addToken(TokenType tokenType) {
    std::string text = source.substr(start, current-start);
    tokens.push_back(Token(tokenType, text, "", line));
}

void Scanner::addToken(TokenType tokenType, std::string literal) {
    std::string text = source.substr(start, current-start);
    tokens.push_back(Token(tokenType, text, literal, line));
}
