#include "Lexer.hpp"

namespace udo::lexer {

inline bool isKeyword(const std::string &str) {
    static const std::unordered_set<std::string> keywords = {
        "declare", "as", "fn", "if", "else", "callable", "functor", "return"
    };
    return keywords.contains(str);
}

inline TokenType get_symbol_type(const std::string &str) {
    if (str == "=") return TokenType::EQUAL;
    if (str == ";") return TokenType::SEMICOLON;
    if (str == "::") return TokenType::DOUBLE_COLON;
    if (str == ",") return TokenType::COMMA;
    if (str == ":") return TokenType::COLON;
    if (str == "{") return TokenType::LBRACE;
    if (str == "}") return TokenType::RBRACE;
    if (str == "[") return TokenType::LBRACKET;
    if (str == "]") return TokenType::RBRACKET;
    if (str == "(") return TokenType::LPAREN;
    if (str == ")") return TokenType::RPAREN;
    if (str == "+") return TokenType::PLUS;
    if (str == "-") return TokenType::MINUS;
    if (str == "*") return TokenType::STAR;
    if (str == "/") return TokenType::SLASH;
    if (str == "!") return TokenType::BANG;
    if (str == "!=") return TokenType::BANG_EQUAL;
    if (str == "==") return TokenType::EQUAL_EQUAL;
    if (str == "<") return TokenType::LESS;
    if (str == "<=") return TokenType::LESS_EQUAL;
    if (str == ">") return TokenType::GREATER;
    if (str == ">=") return TokenType::GREATER_EQUAL;
    if (str == "\\") return TokenType::COMMENT;
    if (str == ".") return TokenType::DOT;
    if (str == "..") return TokenType::DOUBLE_DOT;
    if (str == "...") return TokenType::TRIPLE_DOT;
    return TokenType::UNKNOWN;
}

Lexer::Lexer(std::istream &inputStream) 
    : input(inputStream), currentPos(0), lineNumber(1) 
{
    symbols = {
        "\\\"", "\\\'", "\\\t", "\\\n", "\\\r", "\\\v", "\\\f", "\\\b", "\\\a",
        "<<@", "...", "==", "!=", "<=", ">=", "=>", "->", "::", "||", "&&",
        "+=", "-=", "<<", ">>", "^+", "^-", "\\\\", "..",
        "=", "+", "-", "*", "/", "(", ")", "{", "}", "[", "]", ";", ",", ":",
        "\"", "\'", "\\", "@", "#", "$", "%", "&", "?", "!", "<", ">", "|", "^", "~", "."
    };

    std::ranges::sort(symbols, [](const std::string &a, const std::string &b) {
        return a.size() > b.size();
    });
}

std::tuple<std::vector<Token>, std::vector<Token>, std::map<int, std::string>> Lexer::tokenize() {
    std::vector<Token> tokens;
    std::string line;

    while (std::getline(input, line)) {
        currentLine = line;
        currentPos = 0;
        spaces.clear();

        while (currentPos < currentLine.size()) {
            char currentChar = currentLine[currentPos];

            if (std::isspace(currentChar)) {
                spaces += currentChar;
                ++currentPos;
                continue;
            }

            if (std::isdigit(currentChar)) {
                tokens.push_back(tokenizeNumber());
                continue;
            }

            if (std::isalpha(currentChar) || currentChar == '_') {
                tokens.push_back(tokenizeIdentifier());
                continue;
            }

            if (isSymbolStart(currentChar)) {
                tokens.push_back(tokenizeSymbol());
                continue;
            }

            tokens.push_back({TokenType::UNKNOWN, std::string(1, currentChar), lineNumber, static_cast<int>(currentPos + 1)});
            unfilteredTokens.push_back({TokenType::UNKNOWN, std::string(1, currentChar) + spaces, lineNumber, static_cast<int>(currentPos + 1)});
            ++currentPos;
        }

        unfilteredLines[lineNumber] = line;
        ++lineNumber;
    }

    tokens.push_back({TokenType::eof, "", lineNumber, 0});
    unfilteredTokens.push_back({TokenType::eof, "", lineNumber, 0});
    return {tokens, unfilteredTokens, unfilteredLines};
}

Token Lexer::tokenizeNumber() {
    int column = static_cast<int>(currentPos) + 1;
    std::string number;

    while (currentPos < currentLine.size() && std::isdigit(currentLine[currentPos])) {
        number += currentLine[currentPos++];
    }

    unfilteredTokens.push_back({TokenType::NUMBER, spaces + number, lineNumber, column});
    spaces.clear();
    return {TokenType::NUMBER, number, lineNumber, column};
}

Token Lexer::tokenizeIdentifier() {
    int column = static_cast<int>(currentPos) + 1;
    std::string ident;

    while (currentPos < currentLine.size() && (std::isalnum(currentLine[currentPos]) || currentLine[currentPos] == '_')) {
        ident += currentLine[currentPos++];
    }

    TokenType type = isKeyword(ident) ? TokenType::KEYWORD : TokenType::IDENTIFIER;
    unfilteredTokens.push_back({type, spaces + ident, lineNumber, column});
    spaces.clear();
    return {type, ident, lineNumber, column};
}

Token Lexer::tokenizeSymbol() {
    int column = static_cast<int>(currentPos) + 1;

    for (const auto &sym : symbols) {
        std::size_t len = sym.size();
        if (currentPos + len <= currentLine.size() &&
            currentLine.substr(currentPos, len) == sym) {
            currentPos += len;
            TokenType type = get_symbol_type(sym);
            unfilteredTokens.push_back({type, spaces + sym, lineNumber, column});
            spaces.clear();
            return {type, sym, lineNumber, column};
        }
    }

    char unknownChar = currentLine[currentPos++];
    unfilteredTokens.push_back({TokenType::UNKNOWN, spaces + std::string(1, unknownChar), lineNumber, column});
    spaces.clear();
    return {TokenType::UNKNOWN, std::string(1, unknownChar), lineNumber, column};
}

bool Lexer::isSymbolStart(char c) const {
    return std::ranges::any_of(symbols, [c](const std::string &sym) {
        return !sym.empty() && sym[0] == c;
    });
}

} // namespace udo::lexer