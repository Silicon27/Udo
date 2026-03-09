#include <parser/GrammarStream.hpp>

namespace udo::parse {
    GrammarRule::Outcome GrammarRule::match(const lexer::Token& token) const {
        if (type == lexer::TokenType::IDENTIFIER && token.type == type) {
            if (matched_identifier) {
                *matched_identifier = token.get_lexeme();
                return Outcome::SUCCESS;
            }
            return Outcome::TOKEN_MISMATCH;
        } else if (pos)
        return token.get_type() == type;
    }

    bool Grammar::match(const lexer::Token& token) const {
        if (pos >= rules.size()) {
            return false;
        }
        return rules[pos].match(token);
    }

    Grammar::State Grammar::match_all(const std::span<const lexer::Token> tokens) {
        for (const auto& token : tokens) {
            if (!match(token)) {
                state.pos = pos;
                return state;
            }
            state.pos++;
            pos++;
        }
        return state;
    }
}