//
// Created by David Yang on 2025-11-29.
//

//
// UDO Test Framework - A modern C++20 testing library
// File: udo_test.hpp
//

#ifndef UDO_TEST_HPP
#define UDO_TEST_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <optional>
#include <any>
#include <map>
#include <algorithm>

namespace udo::test {

// ============================================================================
// ANSI Color Codes
// ============================================================================
namespace color {
    constexpr const char* RESET = "\033[0m";
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* DIM = "\033[2m";
    constexpr const char* RED = "\033[31m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* BLUE = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* WHITE = "\033[37m";
    constexpr const char* BG_RED = "\033[41m";
    constexpr const char* BG_GREEN = "\033[42m";
}

// ============================================================================
// Test Result
// ============================================================================
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
    std::string file;
    int line;
    double duration_ms;
    std::optional<std::string> expected;
    std::optional<std::string> actual;
};

// ============================================================================
// Assertion Exception
// ============================================================================
class AssertionFailure : public std::exception {
public:
    AssertionFailure(std::string msg, std::string file, int line)
        : message(std::move(msg)), filename(std::move(file)), line_number(line) {}

    const char* what() const noexcept override { return message.c_str(); }
    const std::string& file() const { return filename; }
    int line() const { return line_number; }

private:
    std::string message;
    std::string filename;
    int line_number;
};

// ============================================================================
// Test Suite
// ============================================================================
class TestSuite {
public:
    explicit TestSuite(std::string name) : suite_name(std::move(name)) {}

    void add_test(std::string name, std::function<void()> test_fn) {
        tests.push_back({std::move(name), std::move(test_fn)});
    }

    void setup(std::function<void()> fn) { setup_fn = std::move(fn); }
    void teardown(std::function<void()> fn) { teardown_fn = std::move(fn); }
    void before_each(std::function<void()> fn) { before_each_fn = std::move(fn); }
    void after_each(std::function<void()> fn) { after_each_fn = std::move(fn); }

    std::vector<TestResult> run(bool verbose = false) {
        std::vector<TestResult> results;

        if (!verbose) {
            std::cout << color::CYAN << "Running suite: " << color::BOLD
                      << suite_name << color::RESET << "\n";
        }

        if (setup_fn) setup_fn();

        for (size_t i = 0; i < tests.size(); ++i) {
            const auto& [name, test_fn] = tests[i];

            if (before_each_fn) before_each_fn();

            auto start = std::chrono::high_resolution_clock::now();
            TestResult result{name, true, "", "", 0, 0.0};

            try {
                test_fn();
                result.passed = true;
                result.message = "Test passed";
            } catch (const AssertionFailure& e) {
                result.passed = false;
                result.message = e.what();
                result.file = e.file();
                result.line = e.line();
            } catch (const std::exception& e) {
                result.passed = false;
                result.message = std::string("Unexpected exception: ") + e.what();
            } catch (...) {
                result.passed = false;
                result.message = "Unknown exception thrown";
            }

            auto end = std::chrono::high_resolution_clock::now();
            result.duration_ms = std::chrono::duration<double, std::milli>(end - start).count();

            if (after_each_fn) after_each_fn();

            results.push_back(result);

            if (verbose) {
                print_test_result(result, i + 1);
            } else {
                std::cout << (result.passed ? color::GREEN : color::RED)
                          << (result.passed ? "." : "F") << color::RESET << std::flush;
            }
        }

        if (!verbose) std::cout << "\n";
        if (teardown_fn) teardown_fn();

        return results;
    }

private:
    std::string suite_name;
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    std::function<void()> setup_fn;
    std::function<void()> teardown_fn;
    std::function<void()> before_each_fn;
    std::function<void()> after_each_fn;

    void print_test_result(const TestResult& result, int test_num) {
        std::cout << std::left << std::setw(4) << test_num << " ";

        if (result.passed) {
            std::cout << color::GREEN << "✓ PASS" << color::RESET;
        } else {
            std::cout << color::RED << "✗ FAIL" << color::RESET;
        }

        std::cout << " | " << std::setw(50) << result.name
                  << " | " << color::DIM << std::fixed << std::setprecision(2)
                  << result.duration_ms << "ms" << color::RESET << "\n";

        if (!result.passed) {
            std::cout << "     " << color::RED << result.message << color::RESET << "\n";
            if (!result.file.empty()) {
                std::cout << "     " << color::DIM << "at " << result.file
                          << ":" << result.line << color::RESET << "\n";
            }
            if (result.expected.has_value()) {
                std::cout << "     Expected: " << color::GREEN
                          << result.expected.value() << color::RESET << "\n";
            }
            if (result.actual.has_value()) {
                std::cout << "     Actual:   " << color::RED
                          << result.actual.value() << color::RESET << "\n";
            }
        }
    }
};

// ============================================================================
// Test Runner
// ============================================================================
class TestRunner {
public:
    static TestRunner& instance() {
        static TestRunner runner;
        return runner;
    }

    void add_suite(std::shared_ptr<TestSuite> suite) {
        suites.push_back(suite);
    }

    int run_all(bool verbose = false, bool stop_on_fail = false) {
        std::cout << "\n" << color::YELLOW << color::BOLD
                  << "╔═══════════════════════════════════════════════════════════════════════════╗\n"
                  << "║                         UDO TEST FRAMEWORK                                ║\n"
                  << "╚═══════════════════════════════════════════════════════════════════════════╝"
                  << color::RESET << "\n\n";

        int total_passed = 0;
        int total_failed = 0;
        double total_time = 0.0;

        for (auto& suite : suites) {
            auto results = suite->run(verbose);

            for (const auto& result : results) {
                if (result.passed) {
                    total_passed++;
                } else {
                    total_failed++;
                    if (stop_on_fail) {
                        print_summary(total_passed, total_failed, total_time);
                        return 1;
                    }
                }
                total_time += result.duration_ms;
            }
        }

        print_summary(total_passed, total_failed, total_time);
        return total_failed > 0 ? 1 : 0;
    }

private:
    std::vector<std::shared_ptr<TestSuite>> suites;

    void print_summary(int passed, int failed, double total_time) {
        std::cout << "\n" << color::BOLD
                  << "╔═══════════════════════════════════════════════════════════════════════════╗\n"
                  << "║ RESULTS: ";

        std::cout << color::GREEN << passed << " PASSED" << color::RESET << color::BOLD << ", ";

        if (failed > 0) {
            std::cout << color::RED << failed << " FAILED" << color::RESET << color::BOLD;
        } else {
            std::cout << color::GREEN << "0 FAILED" << color::RESET << color::BOLD;
        }

        std::cout << " (Total: " << (passed + failed) << ")";

        int spaces = 73 - 19 - std::to_string(passed).length() - std::to_string(failed).length()
                     - std::to_string(passed + failed).length();
        std::cout << std::string(spaces, ' ') << "║\n";

        std::cout << "║ Time: " << std::fixed << std::setprecision(2) << total_time << "ms";
        spaces = 73 - 8 - std::to_string((int)total_time).length();
        std::cout << std::string(spaces, ' ') << "║\n";

        std::cout << "╚═══════════════════════════════════════════════════════════════════════════╝"
                  << color::RESET << "\n\n";
    }
};

// ============================================================================
// Assertion Macros
// ============================================================================
#define UDO_TEST_SUITE(name) \
    auto suite_##name = std::make_shared<udo::test::TestSuite>(#name); \
    struct SuiteRegistrar_##name { \
        SuiteRegistrar_##name() { \
            udo::test::TestRunner::instance().add_suite(suite_##name); \
        } \
    }; \
    static SuiteRegistrar_##name registrar_##name;

#define UDO_TEST(suite_name, test_name) \
    void test_##suite_name##_##test_name(); \
    struct TestRegistrar_##suite_name##_##test_name { \
        TestRegistrar_##suite_name##_##test_name() { \
            suite_##suite_name->add_test(#test_name, test_##suite_name##_##test_name); \
        } \
    }; \
    static TestRegistrar_##suite_name##_##test_name registrar_##suite_name##_##test_name; \
    void test_##suite_name##_##test_name()

#define UDO_SETUP(suite_name) \
    void setup_##suite_name(); \
    struct SetupRegistrar_##suite_name { \
        SetupRegistrar_##suite_name() { \
            suite_##suite_name->setup(setup_##suite_name); \
        } \
    }; \
    static SetupRegistrar_##suite_name setup_registrar_##suite_name; \
    void setup_##suite_name()

#define UDO_TEARDOWN(suite_name) \
    void teardown_##suite_name(); \
    struct TeardownRegistrar_##suite_name { \
        TeardownRegistrar_##suite_name() { \
            suite_##suite_name->teardown(teardown_##suite_name); \
        } \
    }; \
    static TeardownRegistrar_##suite_name teardown_registrar_##suite_name; \
    void teardown_##suite_name()

#define UDO_BEFORE_EACH(suite_name) \
    void before_each_##suite_name(); \
    struct BeforeEachRegistrar_##suite_name { \
        BeforeEachRegistrar_##suite_name() { \
            suite_##suite_name->before_each(before_each_##suite_name); \
        } \
    }; \
    static BeforeEachRegistrar_##suite_name before_each_registrar_##suite_name; \
    void before_each_##suite_name()

#define UDO_AFTER_EACH(suite_name) \
    void after_each_##suite_name(); \
    struct AfterEachRegistrar_##suite_name { \
        AfterEachRegistrar_##suite_name() { \
            suite_##suite_name->after_each(after_each_##suite_name); \
        } \
    }; \
    static AfterEachRegistrar_##suite_name after_each_registrar_##suite_name; \
    void after_each_##suite_name()

// Basic Assertions
#define UDO_ASSERT(condition) \
    do { \
        if (!(condition)) { \
            throw udo::test::AssertionFailure( \
                "Assertion failed: " #condition, __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_TRUE(condition) UDO_ASSERT(condition)
#define UDO_ASSERT_FALSE(condition) UDO_ASSERT(!(condition))

// For comparing enum types or other non-streamable types
#define UDO_ASSERT_ENUM_EQ(actual, expected) \
    do { \
        auto actual_val = (actual); \
        auto expected_val = (expected); \
        if (actual_val != expected_val) { \
            std::ostringstream oss; \
            oss << "Enum mismatch:\n  Actual:   " << static_cast<int>(actual_val) \
                << "\n  Expected: " << static_cast<int>(expected_val); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_EQ(actual, expected) \
    do { \
        if (!((actual) == (expected))) { \
            std::ostringstream oss; \
            oss << "Expected equality:\n  Actual:   " << (actual) \
                << "\n  Expected: " << (expected); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_NE(actual, expected) \
    do { \
        if ((actual) == (expected)) { \
            std::ostringstream oss; \
            oss << "Expected inequality:\n  Value: " << (actual); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_LT(a, b) \
    do { \
        if (!((a) < (b))) { \
            std::ostringstream oss; \
            oss << "Expected " << (a) << " < " << (b); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_LE(a, b) \
    do { \
        if (!((a) <= (b))) { \
            std::ostringstream oss; \
            oss << "Expected " << (a) << " <= " << (b); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_GT(a, b) \
    do { \
        if (!((a) > (b))) { \
            std::ostringstream oss; \
            oss << "Expected " << (a) << " > " << (b); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_GE(a, b) \
    do { \
        if (!((a) >= (b))) { \
            std::ostringstream oss; \
            oss << "Expected " << (a) << " >= " << (b); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_NEAR(actual, expected, epsilon) \
    do { \
        auto diff = std::abs((actual) - (expected)); \
        if (diff > (epsilon)) { \
            std::ostringstream oss; \
            oss << "Values not near:\n  Actual:   " << (actual) \
                << "\n  Expected: " << (expected) \
                << "\n  Epsilon:  " << (epsilon) \
                << "\n  Diff:     " << diff; \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            throw udo::test::AssertionFailure( \
                "Expected null pointer", __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            throw udo::test::AssertionFailure( \
                "Expected non-null pointer", __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_THROWS(statement, exception_type) \
    do { \
        bool caught = false; \
        try { \
            statement; \
        } catch (const exception_type&) { \
            caught = true; \
        } catch (...) {} \
        if (!caught) { \
            throw udo::test::AssertionFailure( \
                "Expected exception of type " #exception_type " to be thrown", \
                __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_NO_THROW(statement) \
    do { \
        try { \
            statement; \
        } catch (...) { \
            throw udo::test::AssertionFailure( \
                "Expected no exception to be thrown", __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_FAIL(message) \
    throw udo::test::AssertionFailure(message, __FILE__, __LINE__)

// String Assertions
#define UDO_ASSERT_STREQ(actual, expected) \
    do { \
        std::string a_str = (actual); \
        std::string e_str = (expected); \
        if (a_str != e_str) { \
            std::ostringstream oss; \
            oss << "String mismatch:\n  Actual:   \"" << a_str \
                << "\"\n  Expected: \"" << e_str << "\""; \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_STRNE(actual, expected) \
    do { \
        std::string a_str = (actual); \
        std::string e_str = (expected); \
        if (a_str == e_str) { \
            std::ostringstream oss; \
            oss << "Strings should not be equal: \"" << a_str << "\""; \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_CONTAINS(string_val, substring_val) \
    do { \
        std::string UDO_STR_TEMP = (string_val); \
        std::string UDO_SUBSTR_TEMP = (substring_val); \
        if (UDO_STR_TEMP.find(UDO_SUBSTR_TEMP) == std::string::npos) { \
            std::ostringstream UDO_OSS_TEMP; \
            UDO_OSS_TEMP << "String \"" << UDO_STR_TEMP << "\" does not contain \"" << UDO_SUBSTR_TEMP << "\""; \
            std::string UDO_MSG_TEMP = UDO_OSS_TEMP.str(); \
            throw udo::test::AssertionFailure(UDO_MSG_TEMP, __FILE__, __LINE__); \
        } \
    } while(0)

// Container Assertions
#define UDO_ASSERT_EMPTY(container) \
    do { \
        if (!(container).empty()) { \
            std::ostringstream oss; \
            oss << "Container should be empty, size: " << (container).size(); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

#define UDO_ASSERT_SIZE(container, expected_size) \
    do { \
        if ((container).size() != (expected_size)) { \
            std::ostringstream oss; \
            oss << "Container size mismatch:\n  Actual:   " << (container).size() \
                << "\n  Expected: " << (expected_size); \
            throw udo::test::AssertionFailure(oss.str(), __FILE__, __LINE__); \
        } \
    } while(0)

// Run all tests
#define UDO_RUN_ALL_TESTS(verbose) \
    udo::test::TestRunner::instance().run_all(verbose)

} // namespace udo::test

#endif // UDO_TEST_HPP