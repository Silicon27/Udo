//
// Created by David Yang on 2025-10-18.
//

#include <optional>

#include <parser/parser.hpp>

#include <support/iris/src/iris.hpp>


namespace udo::parse {
    Token Parser::peek(const int n) const { return tokens[pos + n]; }
    Token Parser::consume(const int n) {Token t = tokens[pos]; pos += n; return t;}
    Token Parser::consume_and_expect(const TokenType exp, const Token& curr, const diag::DiagID err) {
        if (curr.type == exp) {
            pos++;
            return previous();
        }

        diagnostics_.Report(err)
                << "expected token";
        return {TokenType::INVALID_TOKEN, ""};
    }

    Token Parser::match(const TokenType exp, const diag::DiagID err) {
        return consume_and_expect(exp, peek(), err);
    }

    Token Parser::match(MatchToken& token) {
        if (const auto t = consume_and_expect(token.token, peek(), token.diag_id); t.type != TokenType::INVALID_TOKEN) {
            token.is_active = true;
            return t;
        } else {
            return t;
        }
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
        // TODO: to have better recovery, make grammar parsing more as a list, e.g. for variable declaration, we can parse it as a list of tokens in the form of "let <identifier> (':' <type>)? '=' <expression>", and then check if the list matches the expected pattern, if not, report error and try to recover by skipping tokens until we find a token that can start a new declaration or statement.

        auto initial_let = MatchToken(TokenType::kw_let, diag::common::err_expected_token); // should not error if not matched, if error it could mean memory corruption during program runtime
        auto variable_identifier = MatchToken(TokenType::IDENTIFIER, diag::common::err_expected_token);
        auto colon = MatchToken(TokenType::COLON, diag::common::err_expected_token);
        auto equal = MatchToken(TokenType::EQUAL, diag::common::err_expected_token);

        bool deduce_type = false;

        // pre-build the grammar with custom template-based PEG style parser combinators
        // helps recovery later on


        match(initial_let);
        std::string id =
            match(variable_identifier).lexeme;

        // attempt to match `=` or `:`
        attempt(colon);


        }
        
    }

    Parser::Parser(const std::vector<Token> &tokens, Flags flag, ASTContext &context, diag::DiagnosticsEngine& diag)
        : diagnostics_(diag), context_(context), tokens(tokens), flags(std::move(flag)), context(Parser_Context::Top_Level) {
    }


}
