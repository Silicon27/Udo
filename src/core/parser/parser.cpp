//
// Created by David Yang on 2025-10-18.
//

#include "parser.hpp"


namespace udo::parse {
    Token Parser::peek(const int n) const { return tokens[pos + n]; }
    Token Parser::consume(const int n) {Token t = tokens[pos]; pos += n; return t;}
    bool Parser::consume_and_expect(TokenType exp, const Token& curr) {
        if (curr.type == exp) {
            pos++;
            return true; 
        }

        diagnostics_.Report(diag::parse::err_expected_expression)
                << "expected token";
        return false;
    }

    bool Parser::is_at_end() const { return pos >= tokens.size() || tokens[pos].type == TokenType::eof; }

    void Parser::parse() {
        for (bool at_eof = parse_first_top_level_decl(); !at_eof; at_eof = is_at_end()) {
            parse_top_level_decl();
        }
    }

    void Parser::parse_top_level_decl() {
        switch (peek().get_type()) {
            case TokenType::kw_let:
                // TODO: parse variable declaration
                break;
            default:
                consume(); // skip unknown token
                break;
        }
    }

    bool Parser::parse_first_top_level_decl() {
        return false;
    }

    void Parser::parse_variable_decl() {

    }

    Parser::Parser(const std::vector<Token> &tokens, Flags flag, std::shared_ptr<ProgramNode> &program, diag::DiagnosticsEngine& diag)
        : diagnostics_(diag), program(program), tokens(tokens), flags(std::move(flag)), context(Parser_Context::Top_Level) {
    }


}
