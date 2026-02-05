#include "lexer.hpp"

namespace udo::lexer {

inline bool isKeyword(const std::string &str) {
    static const std::unordered_set<std::string> keywords = {
        "declare", "as", "fn", "if", "else",
        "callable", "functor", "return", "i4",
        "i8", "i16", "i32", "i64", "i128",
        "let"
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

        tokens.push_back({TokenType::NEWLINE, "\n", lineNumber, 0});
        unfilteredTokens.push_back({TokenType::NEWLINE, "\n", lineNumber, 0});

        unfilteredLines[lineNumber++] = line;


    }

    tokens.push_back({TokenType::eof, "", lineNumber, 0});
    unfilteredTokens.push_back({TokenType::eof, "", lineNumber, 0});
    return {tokens, unfilteredTokens, unfilteredLines};
}

Token Lexer::tokenizeNumber() {
    int column = static_cast<int>(currentPos) + 1;
    std::string lexeme;
    const std::string &s = currentLine;
    std::size_t start = currentPos;
    bool isFloat = false;
    bool hasDigits = false;
    int base = 10;

    // Detect base prefix
    if (s[currentPos] == '0' && currentPos + 1 < s.size()) {
        char c1 = s[currentPos + 1];
        if (c1 == 'x' || c1 == 'X') {
            base = 16;
            lexeme += s[currentPos++];
            lexeme += s[currentPos++];
        } else if (c1 == 'b' || c1 == 'B') {
            base = 2;
            lexeme += s[currentPos++];
            lexeme += s[currentPos++];
        } else if (c1 == 'o' || c1 == 'O') {
            base = 8;
            lexeme += s[currentPos++];
            lexeme += s[currentPos++];
        } else if (std::isdigit(c1)) {
            // C-style octal: 0755
            base = 8;
            lexeme += s[currentPos++];
        }
    }

    auto validDigit = [base](char c) -> bool {
        if (base == 16) return std::isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        if (base == 10) return std::isdigit(c) != 0;
        if (base == 8) return c >= '0' && c <= '7';
        if (base == 2) return c == '0' || c == '1';
        return false;
    };

    auto isSeparator = [](char c) -> bool {
        return c == '_' || c == '\'';
    };

    // Parse integer part with digit separators
    while (currentPos < s.size()) {
        if (validDigit(s[currentPos])) {
            lexeme += s[currentPos++];
            hasDigits = true;
        } else if (isSeparator(s[currentPos])) {
            // Digit separator: must have digit before and after
            if (hasDigits && currentPos + 1 < s.size() && validDigit(s[currentPos + 1])) {
                lexeme += s[currentPos++];
            } else {
                break;
            }
        } else {
            break;
        }
    }

    if (!hasDigits) {
        return {TokenType::UNKNOWN, s.substr(start, currentPos - start + 1), lineNumber, column};
    }

    // Floating-point handling (only for decimal and hex)
    if (base == 10 || base == 16) {
        if (currentPos < s.size() && s[currentPos] == '.') {
            // Peek ahead to decide if this is a decimal point or range operator
            if (currentPos + 1 < s.size() && validDigit(s[currentPos + 1])) {
                // Definitely a float: digit after dot
                isFloat = true;
                lexeme += s[currentPos++];

                while (currentPos < s.size()) {
                    if (validDigit(s[currentPos]) ||
                        (isSeparator(s[currentPos]) && currentPos + 1 < s.size() && validDigit(s[currentPos + 1]))) {
                        lexeme += s[currentPos++];
                    } else {
                        break;
                    }
                }
            } else if (currentPos + 1 >= s.size() || s[currentPos + 1] != '.') {
                // Trailing dot: 123. is valid, but 123.. is range operator
                isFloat = true;
                lexeme += s[currentPos++];
            }
            // Otherwise: next char is '.', so this is ".." range operator - don't consume
        }

        // Exponent (e/E for decimal, p/P for hex)
        bool needsExponent = (base == 16 && isFloat); // Hex floats MUST have exponent

        if (currentPos < s.size() &&
            (s[currentPos] == 'e' || s[currentPos] == 'E' ||
             (base == 16 && (s[currentPos] == 'p' || s[currentPos] == 'P')))) {

            char expChar = s[currentPos++];
            lexeme += expChar;

            // Optional sign
            if (currentPos < s.size() && (s[currentPos] == '+' || s[currentPos] == '-')) {
                lexeme += s[currentPos++];
            }

            // Exponent digits (always decimal, even for hex floats)
            bool hasExpDigits = false;
            while (currentPos < s.size()) {
                if (std::isdigit(s[currentPos])) {
                    lexeme += s[currentPos++];
                    hasExpDigits = true;
                } else if (isSeparator(s[currentPos]) && hasExpDigits &&
                           currentPos + 1 < s.size() && std::isdigit(s[currentPos + 1])) {
                    lexeme += s[currentPos++];
                } else {
                    break;
                }
            }

            if (!hasExpDigits) {
                return {TokenType::UNKNOWN, s.substr(start, currentPos - start), lineNumber, column};
            }
            isFloat = true;
        } else if (needsExponent) {
            // Hex float without required 'p' exponent
            return {TokenType::UNKNOWN, s.substr(start, currentPos - start), lineNumber, column};
        }
    }

    // Type suffixes
    std::string suffix;
    while (currentPos < s.size() && std::isalpha(s[currentPos])) {
        suffix += s[currentPos++];
    }

    if (!suffix.empty()) {
        std::string lower;
        for (auto c : suffix) {
            lower += static_cast<char>(std::tolower(c));
        }

        // Validate suffix
        if (isFloat) {
            // Float suffixes: f, lf, l (long double)
            if (lower != "f" && lower != "lf" && lower != "l") {
                return {TokenType::UNKNOWN, s.substr(start, currentPos - start), lineNumber, column};
            }
        } else {
            // Integer suffixes: u, l, ul, lu, ll, ull, llu, z, uz, zu
            const std::string validSuffixes[] = {
                "u", "l", "ul", "lu", "ll", "ull", "llu", "z", "uz", "zu"
            };
            bool isValid = false;
            for (const auto& validSuffix : validSuffixes) {
                if (lower == validSuffix) {
                    isValid = true;
                    break;
                }
            }
            if (!isValid) {
                return {TokenType::UNKNOWN, s.substr(start, currentPos - start), lineNumber, column};
            }
        }
        lexeme += suffix;
    }

    TokenType tokType = isFloat ? TokenType::FLOAT_LITERAL : TokenType::INT_LITERAL;
    unfilteredTokens.push_back({tokType, spaces + lexeme, lineNumber, column});
    spaces.clear();
    return {tokType, lexeme, lineNumber, column};
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