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
#include <support/global_constants.hpp>

namespace udo::lexer {

    struct Token {
        TokenType type;
        std::string lexeme;
        int line;
        int column;

        TokenType get_type() const { return type; }
        std::string get_lexeme() const { return lexeme; }
        int get_line() const { return line; }
        int get_column() const { return column; }
    };


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
        const std::vector<std::string>& symbols;
        std::string spaces;
        std::vector<Token> unfilteredTokens;

        Token tokenizeNumber();
        Token tokenizeIdentifier();
        Token tokenizeSymbol();
        [[nodiscard]] bool isSymbolStart(char c) const;
    };

} // namespace udo::lexer

#endif // LEXER_HPP