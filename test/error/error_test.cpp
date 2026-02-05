//
// Error Handling Test Suite - Implementation
// Created by David Yang on 2026-02-03.
//

#include "error_test.hpp"
#include <error/error.hpp>

namespace udo::test {

void register_error_tests(TestRunner& runner) {

    // ========================================================================
    // Error Creation Tests
    // ========================================================================

    auto creation_suite = std::make_unique<TestSuite>("Error::Creation");

    creation_suite->add_test("placeholder_test", []() {
        // TODO: Add actual error tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(creation_suite));

    // ========================================================================
    // Error Formatting Tests
    // ========================================================================

    auto format_suite = std::make_unique<TestSuite>("Error::Formatting");

    format_suite->add_test("placeholder_test", []() {
        // TODO: Add actual error formatting tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(format_suite));

    // ========================================================================
    // Diagnostic Tests
    // ========================================================================

    auto diag_suite = std::make_unique<TestSuite>("Error::Diagnostics");

    diag_suite->add_test("placeholder_test", []() {
        // TODO: Add actual diagnostic tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(diag_suite));
}

} // namespace udo::test

// ============================================================================
// Main function for standalone error test executable
// Only compiled when building as standalone (ERROR_TEST_STANDALONE defined)
// ============================================================================
#ifdef ERROR_TEST_STANDALONE
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

    register_error_tests(runner);

    return runner.run_all(verbose);
}
#endif
