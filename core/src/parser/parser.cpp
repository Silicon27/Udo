//
// Created by David Yang on 2025-10-18.
//

#include <parser/parser.hpp>


namespace udo::parse {
    Token Parser::peek(const int n) const { return tokens[pos + n]; }
    Token Parser::consume(const int n) {Token t = tokens[pos]; pos += n; return t;}
    bool Parser::consume_and_expect(TokenType exp, const Token& curr, diag::DiagID err) {
        if (curr.type == exp) {
            pos++;
            return true; 
        }

        diagnostics_.Report(diag::parse::err_expected_expression)
                << "expected token";
        return false;
    }

    std::string Parser::match(const TokenType exp, const diag::DiagID err) {
        consume_and_expect(exp, peek(), err);
        return previous().lexeme;
    }

    std::string Parser::match(const MatchToken token) {
        consume_and_expect(token.token, peek(), token.diag_id);
        return previous().lexeme;
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
                parse_variable_decl();
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
        constexpr auto initial_let = MatchToken(TokenType::kw_let, diag::common::err_expected_token); // should not error if not matched, if error it could mean memory corruption during program runtime
        constexpr auto variable_identifier = MatchToken(TokenType::IDENTIFIER, diag::common::err_expected_token);
        constexpr auto colon = MatchToken(TokenType::COLON, diag::common::err_expected_token);

        match(initial_let);
        std::string id =
            match(variable_identifier);
        match(colon);

    }

    Parser::Parser(const std::vector<Token> &tokens, Flags flag, ASTContext &context, diag::DiagnosticsEngine& diag)
        : diagnostics_(diag), context_(context), tokens(tokens), flags(std::move(flag)), context(Parser_Context::Top_Level) {
    }


}
