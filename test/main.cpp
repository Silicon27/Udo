//
// UDO Test Suite - Main Entry Point
// Created by David Yang on 2026-02-03.
//

#include <string>

#include "suite/udo_test.hpp"
#include "lexer/lexer_test.hpp"
#include "parser/parser_test.hpp"
#include "ast/ast_test.hpp"
#include "error/error_test.hpp"
#include "preprocessor/preprocessor_test.hpp"

int main(int argc, char* argv[]) {
    using namespace udo::test;

    TestRunner runner;

    bool verbose = false;
    bool list_only = false;
    std::string filter;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "-l" || arg == "--list") {
            list_only = true;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "UDO Test Suite\n"
                      << "Usage: " << argv[0] << " [options]\n"
                      << "Options:\n"
                      << "  -v, --verbose     Verbose output\n"
                      << "  -l, --list        List all tests without running\n"
                      << "  -h, --help        Show this help\n";
            return 0;
        }
    }

    // Register all test suites
    register_lexer_tests(runner);
    register_parser_tests(runner);
    register_ast_tests(runner);
    register_error_tests(runner);
    register_preprocessor_tests(runner);

    if (list_only) {
        runner.list_tests();
        return 0;
    }

    return runner.run_all(verbose);
}
