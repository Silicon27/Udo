//
// Created by David Yang on 2025-2-17.
//

#ifndef GRAMMARSTREAM_HPP
#define GRAMMARSTREAM_HPP
#include <string>

#include <lexer/Lexer.hpp>

namespace udo::parse {
    template <lexer::TokenType T> class GrammarRule;
    class Grammar;
    class GrammarStream;

    template <lexer::TokenType T>
    class GrammarRule {

    };

    /// the underlying object being constructed implicitly via GrammarStream operators
    class Grammar {

    };

    /// GrammarStream is a utility class that provides a stream-like interface for parsing grammar rules. It allows the parser to consume tokens and check for expected tokens conveniently and improves recovery from parsing errors.
    class GrammarStream {
    public:
        GrammarStream(Grammar* grammar) : grammar(grammar) {}
    private:
        Grammar* grammar;
    };
}

#endif