//
// Created by David Yang on 2025-12-06.
//

#ifndef GLOBAL_CONSTANTS_HPP
#define GLOBAL_CONSTANTS_HPP

#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

// -----------------------------------------------
//               OS Specific Constants
// -----------------------------------------------

// get the cache line size for the current platform, used for ASTContext slab allocation
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
    #define CACHE_LINE_SIZE 64
#elif defined(__aarch64__)
    #define CACHE_LINE_SIZE 128
#else
    #define CACHE_LINE_SIZE 64 // default to 64 if unknown platform
#endif

// -----------------------------------------------
//                  Source_Manager
// -----------------------------------------------

#ifndef SOURCE_MANAGER_INVALID_FILE_ID
#define SOURCE_MANAGER_INVALID_FILE_ID -300
#endif

// -----------------------------------------------
//                  Lexer Types
// -----------------------------------------------

namespace udo::lexer {

    enum class TokenType {
        // Keywords
        kw_let,
        kw_as,
        kw_if,
        kw_else,
        kw_functor,
        kw_return,
        kw_i4,
        kw_i8,
        kw_i16,
        kw_i32,
        kw_i64,
        kw_i128,
        kw_f4,
        kw_f8,
        kw_f16,
        kw_f32,
        kw_f64,
        kw_f128,
        kw_char,
        kw_bool,
        kw_import,
        kw_mod,
        kw_export,
        kw_bind,

        // Identifiers and Literals
        IDENTIFIER,
        INT_LITERAL,
        FLOAT_LITERAL,
        NUMBER,

        // Special
        UNKNOWN,
        NEWLINE,
        eof,
        COMMENT,

        // Operators and Punctuation
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

        // misc
        MATCHED_NO_TOKENS,
    };

    // Keyword set for fast lookup
    inline const std::unordered_set<std::string> keywords = {
        "let", "as", "if", "else",
        "functor", "return", "i4",
        "i8", "i16", "i32", "i64", "i128",
        "f4", "f8", "f16", "f32", "f64", "f128",
        "char", "bool", "import", "mod", "export",
        "bind"
    };

    // Check if a string is a keyword
    inline bool is_keyword(const std::string &str) {
        return keywords.contains(str);
    }

    // Get the TokenType for a keyword string
    inline TokenType get_keyword_type(const std::string &str) {
        if (str == "let") return TokenType::kw_let;
        if (str == "as") return TokenType::kw_as;
        if (str == "if") return TokenType::kw_if;
        if (str == "else") return TokenType::kw_else;
        if (str == "functor") return TokenType::kw_functor;
        if (str == "return") return TokenType::kw_return;
        if (str == "i4") return TokenType::kw_i4;
        if (str == "i8") return TokenType::kw_i8;
        if (str == "i16") return TokenType::kw_i16;
        if (str == "i32") return TokenType::kw_i32;
        if (str == "i64") return TokenType::kw_i64;
        if (str == "i128") return TokenType::kw_i128;
        if (str == "f4") return TokenType::kw_f4;
        if (str == "f8") return TokenType::kw_f8;
        if (str == "f16") return TokenType::kw_f16;
        if (str == "f32") return TokenType::kw_f32;
        if (str == "f64") return TokenType::kw_f64;
        if (str == "f128") return TokenType::kw_f128;
        if (str == "char") return TokenType::kw_char;
        if (str == "bool") return TokenType::kw_bool;
        if (str == "import") return TokenType::kw_import;
        if (str == "mod") return TokenType::kw_mod;
        if (str == "export") return TokenType::kw_export;
        if (str == "bind") return TokenType::kw_bind;
        return TokenType::IDENTIFIER; // fallback
    }

    // Get the TokenType for a symbol string
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

    // Lexer symbols list (sorted by length descending for longest-match)
    inline std::vector<std::string> get_symbols() {
        std::vector<std::string> syms = {
            "\\\"", "\\\'", "\\\t", "\\\n", "\\\r", "\\\v", "\\\f", "\\\b", "\\\a",
            "<<@", "...", "==", "!=", "<=", ">=", "=>", "->", "::", "||", "&&",
            "+=", "-=", "<<", ">>", "^+", "^-", "\\\\", "..",
            "=", "+", "-", "*", "/", "(", ")", "{", "}", "[", "]", ";", ",", ":",
            "\"", "\'", "\\", "@", "#", "$", "%", "&", "?", "!", "<", ">", "|", "^", "~", "."
        };
        std::sort(syms.begin(), syms.end(), [](const std::string &a, const std::string &b) {
            return a.size() > b.size();
        });
        return syms;
    }

    // Pre-sorted symbols constant (initialized once)
    inline const std::vector<std::string>& symbols() {
        static const std::vector<std::string> sorted_symbols = get_symbols();
        return sorted_symbols;
    }

} // namespace udo::lexer

#endif //GLOBAL_CONSTANTS_HPP
