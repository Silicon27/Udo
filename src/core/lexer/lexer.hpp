//
// Created by David Yang on 2025-10-18.
//

#ifndef LEXER_HPP
#define LEXER_HPP

#include <istream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <ranges>

namespace udo::lexer {

    enum class TokenType {
        KEYWORD,
        IDENTIFIER,
        INT_LITERAL,
        FLOAT_LITERAL,
        NUMBER,
        UNKNOWN,
        NEWLINE,
        eof,
        COMMENT,
        EQUAL,
        SEMICOLON,
        DOUBLE_COLON,
        COMMA,
        COLON,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        LPAREN,
        RPAREN,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        BANG,
        BANG_EQUAL,
        EQUAL_EQUAL,
        LESS,
        LESS_EQUAL,
        GREATER,
        GREATER_EQUAL,
        DOT,
        DOUBLE_DOT,
        TRIPLE_DOT,
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        int line;
        int column;
    };

    inline bool isKeyword(const std::string &str);

    inline TokenType get_symbol_type(const std::string &str);

    class Lexer {
    public:
        explicit Lexer(std::istream &inputStream);

        std::tuple<std::vector<Token>, std::vector<Token>, std::map<int, std::string>> tokenize();

    private:
        std::istream &input;
        std::string currentLine;
        std::size_t currentPos;
        int lineNumber;
        std::map<int, std::string> unfilteredLines;
        std::vector<std::string> symbols;
        std::string spaces;
        std::vector<Token> unfilteredTokens;

        Token tokenizeNumber();
        Token tokenizeIdentifier();
        Token tokenizeSymbol();
        [[nodiscard]] bool isSymbolStart(char c) const;
    };

} // namespace udo::lexer

#endif // LEXER_HPP