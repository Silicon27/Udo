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

namespace udo::parse {
    using namespace udo::compiler_config;

    class Parse;

    using namespace udo::ast;
    using namespace udo::lexer;

    struct ParserSnapshot {
        int capped_pos;
        const std::vector<Token>& tokens;
        const std::shared_ptr<ASTNode>& current_node;
    };

    class Parser {
        diag::DiagnosticsEngine& diagnostics_;
        std::shared_ptr<ProgramNode> program;
        std::vector<Token> tokens;
        Flags flags;
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

        bool is_at_end() const;

        explicit Parser(const std::vector<Token> &tokens, Flags flag, std::shared_ptr<ProgramNode> &program, diag::DiagnosticsEngine& diag);
        ~Parser() = default;

        void parse();

    };
}

#endif //PARSER_HPP
