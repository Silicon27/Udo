//
// Created by David Yang on 2025-10-18.
//

#ifndef PARSER_HPP
#define PARSER_HPP
#include <string>
#include <ast/ast.hpp>
#include <lexer/lexer.hpp>
#include <error/error.hpp>
#include <cli/compiler_config.hpp>

#include <ast/ASTContext.hpp>

namespace udo::parse {
    using namespace udo::compiler_config;

    class Parse;

    using namespace udo::ast;
    using namespace udo::lexer;

    struct ParserSnapshot {
        int capped_pos;
        const std::vector<Token>& tokens;
    };

    class Parser {
    public:
        enum class Parser_Context {
            Top_Level,
            Namespace,
            Function,
            Statement,
        };


    private:
        diag::DiagnosticsEngine& diagnostics_;
        ASTContext& context_;
        std::vector<Token> tokens;
        Flags flags;
        Parser_Context context;
        int pos = 0;

    public:
        // peek at the current token without consuming it, n=0 means current token, n=1 means next token, etc.
        Token peek(int n = 0) const;
        // previous, peek(-1) alias
        Token previous() const { return peek(-1); }
        /// blind consumation of tokens, no checking, just move the pointer forward and return the token at the original position
        Token consume(int n = 1);
        /// consume the current token and check if it matches the expected type, if it does, return true, otherwise report an error and return false
        bool consume_and_expect(TokenType exp, const Token& curr);

        // EOF/token stream check
        bool is_at_end() const;

        // Entry point is parse()
        void parse();

        // This will be called in a loop from parse() until we reach the end of the token stream.
        void parse_top_level_decl();

        // We also thereby create a specialized first top level declaration
        // parser, which is responsible mainly for parsing module declarations and imports
        // TODO
        bool parse_first_top_level_decl();

        // statement parsers
        void parse_variable_decl();

        // expression parsers


        explicit Parser(const std::vector<Token> &tokens, Flags flag, ASTContext &context, diag::DiagnosticsEngine& diag);
        ~Parser() = default;
    };
}

#endif //PARSER_HPP
