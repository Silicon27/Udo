//
// Lexer Test Suite - Implementation
// Created by David Yang on 2026-02-03.
//

#include "lexer_test.hpp"
#include <lexer/lexer.hpp>
#include <sstream>

namespace udo::test {

using namespace udo::lexer;

// Helper function to tokenize a string
static std::vector<Token> tokenize_string(const std::string& input) {
    std::istringstream stream(input);
    Lexer lexer(stream);
    auto [tokens, unfiltered, lines] = lexer.tokenize();
    return tokens;
}

// Helper to get token without newlines and EOF
static std::vector<Token> get_meaningful_tokens(const std::vector<Token>& tokens) {
    std::vector<Token> result;
    for (const auto& tok : tokens) {
        if (tok.type != TokenType::NEWLINE && tok.type != TokenType::eof) {
            result.push_back(tok);
        }
    }
    return result;
}

void register_lexer_tests(TestRunner& runner) {

    // ========================================================================
    // Basic Token Tests
    // ========================================================================

    auto basic_suite = std::make_unique<TestSuite>("Lexer::BasicTokens");

    basic_suite->add_test("empty_input_produces_eof", []() {
        auto tokens = tokenize_string("");
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::eof));
    });

    basic_suite->add_test("single_newline", []() {
        auto tokens = tokenize_string("\n");
        UDO_ASSERT_GE(tokens.size(), 1u);
        bool has_newline = false;
        for (const auto& t : tokens) {
            if (t.type == TokenType::NEWLINE) has_newline = true;
        }
        UDO_ASSERT_TRUE(has_newline);
    });

    basic_suite->add_test("whitespace_only", []() {
        auto tokens = tokenize_string("   \t  ");
        auto meaningful = get_meaningful_tokens(tokens);
        UDO_ASSERT_EQ(meaningful.size(), 0u);
    });

    runner.add_suite(std::move(basic_suite));

    // ========================================================================
    // Identifier Tests
    // ========================================================================

    auto ident_suite = std::make_unique<TestSuite>("Lexer::Identifiers");

    ident_suite->add_test("simple_identifier", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "foo");
    });

    ident_suite->add_test("identifier_with_underscore", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo_bar"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "foo_bar");
    });

    ident_suite->add_test("identifier_with_numbers", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo123"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "foo123");
    });

    ident_suite->add_test("underscore_prefix", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("_private"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "_private");
    });

    ident_suite->add_test("multiple_identifiers", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo bar baz"));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        UDO_ASSERT_STREQ(tokens[0].lexeme, "foo");
        UDO_ASSERT_STREQ(tokens[1].lexeme, "bar");
        UDO_ASSERT_STREQ(tokens[2].lexeme, "baz");
    });

    runner.add_suite(std::move(ident_suite));

    // ========================================================================
    // Keyword Tests
    // ========================================================================

    auto keyword_suite = std::make_unique<TestSuite>("Lexer::Keywords");

    keyword_suite->add_test("fn_keyword", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("fn"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "fn");
    });

    keyword_suite->add_test("let_keyword", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("let"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD));
    });

    keyword_suite->add_test("if_else_keywords", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("if else"));
        UDO_ASSERT_EQ(tokens.size(), 2u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::KEYWORD));
    });

    keyword_suite->add_test("return_keyword", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("return"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD));
    });

    keyword_suite->add_test("type_keywords", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("i32 i64 i128"));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        for (const auto& t : tokens) {
            UDO_ASSERT_EQ(static_cast<int>(t.type), static_cast<int>(TokenType::KEYWORD));
        }
    });

    keyword_suite->add_test("declare_as_keywords", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("declare as"));
        UDO_ASSERT_EQ(tokens.size(), 2u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::KEYWORD));
    });

    runner.add_suite(std::move(keyword_suite));

    // ========================================================================
    // Integer Literal Tests
    // ========================================================================

    auto int_suite = std::make_unique<TestSuite>("Lexer::IntegerLiterals");

    int_suite->add_test("simple_integer", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("42"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "42");
    });

    int_suite->add_test("zero", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    int_suite->add_test("hex_integer", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0xFF"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "0xFF");
    });

    int_suite->add_test("binary_integer", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0b1010"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "0b1010");
    });

    int_suite->add_test("octal_integer", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0o755"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    int_suite->add_test("integer_with_underscore_separator", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("1_000_000"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    int_suite->add_test("integer_with_suffix_u", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("42u"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    int_suite->add_test("integer_with_suffix_ll", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("42ll"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    runner.add_suite(std::move(int_suite));

    // ========================================================================
    // Float Literal Tests
    // ========================================================================

    auto float_suite = std::make_unique<TestSuite>("Lexer::FloatLiterals");

    float_suite->add_test("simple_float", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("3.14"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
        UDO_ASSERT_STREQ(tokens[0].lexeme, "3.14");
    });

    float_suite->add_test("float_with_exponent", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("1e10"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    float_suite->add_test("float_with_negative_exponent", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("1e-10"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    float_suite->add_test("float_with_dot_and_exponent", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("3.14e2"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    float_suite->add_test("trailing_dot_float", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("123."));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    float_suite->add_test("float_with_f_suffix", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("3.14f"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    runner.add_suite(std::move(float_suite));

    // ========================================================================
    // Symbol/Operator Tests
    // ========================================================================

    auto symbol_suite = std::make_unique<TestSuite>("Lexer::Symbols");

    symbol_suite->add_test("equals_sign", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("="));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::EQUAL));
    });

    symbol_suite->add_test("double_equals", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("=="));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::EQUAL_EQUAL));
    });

    symbol_suite->add_test("not_equals", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("!="));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::BANG_EQUAL));
    });

    symbol_suite->add_test("comparison_operators", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("< <= > >="));
        UDO_ASSERT_EQ(tokens.size(), 4u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::LESS));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::LESS_EQUAL));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::GREATER));
        UDO_ASSERT_EQ(static_cast<int>(tokens[3].type), static_cast<int>(TokenType::GREATER_EQUAL));
    });

    symbol_suite->add_test("arithmetic_operators", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("+ - * /"));
        UDO_ASSERT_EQ(tokens.size(), 4u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::PLUS));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::MINUS));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::STAR));
        UDO_ASSERT_EQ(static_cast<int>(tokens[3].type), static_cast<int>(TokenType::SLASH));
    });

    symbol_suite->add_test("brackets", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("()[]{}"));
        UDO_ASSERT_EQ(tokens.size(), 6u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::LPAREN));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::RPAREN));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::LBRACKET));
        UDO_ASSERT_EQ(static_cast<int>(tokens[3].type), static_cast<int>(TokenType::RBRACKET));
        UDO_ASSERT_EQ(static_cast<int>(tokens[4].type), static_cast<int>(TokenType::LBRACE));
        UDO_ASSERT_EQ(static_cast<int>(tokens[5].type), static_cast<int>(TokenType::RBRACE));
    });

    symbol_suite->add_test("semicolon_and_comma", []() {
        auto tokens = get_meaningful_tokens(tokenize_string(";,"));
        UDO_ASSERT_EQ(tokens.size(), 2u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::SEMICOLON));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::COMMA));
    });

    symbol_suite->add_test("colon_and_double_colon", []() {
        auto tokens = get_meaningful_tokens(tokenize_string(": ::"));
        UDO_ASSERT_EQ(tokens.size(), 2u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::COLON));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::DOUBLE_COLON));
    });

    symbol_suite->add_test("dot_operators", []() {
        auto tokens = get_meaningful_tokens(tokenize_string(". .. ..."));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::DOT));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::DOUBLE_DOT));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::TRIPLE_DOT));
    });

    symbol_suite->add_test("bang_operator", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("!"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::BANG));
    });

    runner.add_suite(std::move(symbol_suite));

    // ========================================================================
    // Complex Expression Tests
    // ========================================================================

    auto expr_suite = std::make_unique<TestSuite>("Lexer::Expressions");

    expr_suite->add_test("variable_declaration", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("let x = 42;"));
        UDO_ASSERT_EQ(tokens.size(), 5u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD)); // let
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::IDENTIFIER)); // x
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::EQUAL)); // =
        UDO_ASSERT_EQ(static_cast<int>(tokens[3].type), static_cast<int>(TokenType::INT_LITERAL)); // 42
        UDO_ASSERT_EQ(static_cast<int>(tokens[4].type), static_cast<int>(TokenType::SEMICOLON)); // ;
    });

    expr_suite->add_test("function_declaration", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("fn add(a: i32, b: i32)"));
        UDO_ASSERT_GE(tokens.size(), 10u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::KEYWORD)); // fn
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::IDENTIFIER)); // add
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::LPAREN)); // (
    });

    expr_suite->add_test("arithmetic_expression", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("a + b * c - d / e"));
        UDO_ASSERT_EQ(tokens.size(), 9u);
    });

    expr_suite->add_test("range_expression", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0..10"));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::DOUBLE_DOT));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    expr_suite->add_test("namespace_access", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("std::vector"));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::DOUBLE_COLON));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::IDENTIFIER));
    });

    runner.add_suite(std::move(expr_suite));

    // ========================================================================
    // Line and Column Tracking Tests
    // ========================================================================

    auto position_suite = std::make_unique<TestSuite>("Lexer::Positions");

    position_suite->add_test("first_token_column", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(tokens[0].column, 1);
    });

    position_suite->add_test("second_token_column", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("foo bar"));
        UDO_ASSERT_EQ(tokens.size(), 2u);
        UDO_ASSERT_EQ(tokens[0].column, 1);
        UDO_ASSERT_EQ(tokens[1].column, 5);
    });

    position_suite->add_test("multiline_line_numbers", []() {
        auto tokens = tokenize_string("foo\nbar\nbaz");
        int foo_line = -1, bar_line = -1, baz_line = -1;
        for (const auto& t : tokens) {
            if (t.lexeme == "foo") foo_line = t.line;
            if (t.lexeme == "bar") bar_line = t.line;
            if (t.lexeme == "baz") baz_line = t.line;
        }
        UDO_ASSERT_EQ(foo_line, 1);
        UDO_ASSERT_EQ(bar_line, 2);
        UDO_ASSERT_EQ(baz_line, 3);
    });

    runner.add_suite(std::move(position_suite));

    // ========================================================================
    // Edge Case Tests
    // ========================================================================

    auto edge_suite = std::make_unique<TestSuite>("Lexer::EdgeCases");

    edge_suite->add_test("consecutive_operators", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("++--"));
        UDO_ASSERT_EQ(tokens.size(), 4u);
    });

    edge_suite->add_test("number_followed_by_dot_dot", []() {
        // 123..456 should be INT_LITERAL followed by DOUBLE_DOT followed by INT_LITERAL
        auto tokens = get_meaningful_tokens(tokenize_string("123..456"));
        UDO_ASSERT_EQ(tokens.size(), 3u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::INT_LITERAL));
        UDO_ASSERT_EQ(static_cast<int>(tokens[1].type), static_cast<int>(TokenType::DOUBLE_DOT));
        UDO_ASSERT_EQ(static_cast<int>(tokens[2].type), static_cast<int>(TokenType::INT_LITERAL));
    });

    edge_suite->add_test("hex_float_with_exponent", []() {
        auto tokens = get_meaningful_tokens(tokenize_string("0x1.5p10"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::FLOAT_LITERAL));
    });

    edge_suite->add_test("long_identifier", []() {
        std::string long_ident = "a_very_long_identifier_name_that_should_still_work_correctly";
        auto tokens = get_meaningful_tokens(tokenize_string(long_ident));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_STREQ(tokens[0].lexeme, long_ident);
    });

    edge_suite->add_test("mixed_case_keywords_are_identifiers", []() {
        // Keywords are case-sensitive, so "Fn" should be identifier
        auto tokens = get_meaningful_tokens(tokenize_string("Fn"));
        UDO_ASSERT_EQ(tokens.size(), 1u);
        UDO_ASSERT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(TokenType::IDENTIFIER));
    });

    runner.add_suite(std::move(edge_suite));
}

} // namespace udo::test

// ============================================================================
// Main function for standalone lexer test executable
// Only compiled when building as standalone (LEXER_TEST_STANDALONE defined)
// ============================================================================
#ifdef LEXER_TEST_STANDALONE
int main(int argc, char* argv[]) {
    using namespace udo::test;

    TestRunner runner;
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        }
    }

    register_lexer_tests(runner);

    return runner.run_all(verbose);
}
#endif
