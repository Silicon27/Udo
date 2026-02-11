//
// AST Test Suite - Implementation
// Created by David Yang on 2026-02-03.
//

#include "ast_test.hpp"
#include <ast/ast.hpp>
#include <ast/ASTContext.hpp>

namespace udo::test {

void register_ast_tests(TestRunner& runner) {

    // ========================================================================
    // AST Node Creation Tests
    // ========================================================================

    auto node_suite = std::make_unique<TestSuite>("AST::NodeCreation");

    node_suite->add_test("placeholder_test", []() {
        // TODO: Add actual AST tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(node_suite));

    // ========================================================================
    // AST Context Tests
    // ========================================================================

    auto context_suite = std::make_unique<TestSuite>("AST::Context");

    context_suite->add_test("alignment_bug_reproduction", []() {
        // We want to verify that is_full(size, alignment) correctly accounts for padding.

        ASTContext::BumpPtrAllocator allocator(64);

        // 1. Fill the slab partially so that the next allocation might require padding.
        // Allocate 4 bytes.
        void* p1 = allocator.allocate(4, 4);
        UDO_ASSERT_NOT_NULL(p1);

        // 2. Request an allocation that exactly fits the remaining capacity IF no padding is needed,
        // but REQUIRES padding.
        // is_full(60, 8) should now return true, causing a new slab to be allocated.
        
        void* p2 = allocator.allocate(60, 8);
        
        // This should NOT be null because it should have been allocated in a new slab.
        UDO_ASSERT_NOT_NULL(p2);

        // ensure a second slab is generated
        UDO_ASSERT_GT(allocator.num_slabs(), 1);
        
        // Check that it's aligned
        UDO_ASSERT_EQ(reinterpret_cast<std::uintptr_t>(p2) % 8, 0);
    });

    runner.add_suite(std::move(context_suite));

    // ========================================================================
    // AST Traversal Tests
    // ========================================================================

    auto traversal_suite = std::make_unique<TestSuite>("AST::Traversal");

    traversal_suite->add_test("placeholder_test", []() {
        // TODO: Add actual AST traversal tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(traversal_suite));

    // ========================================================================
    // AST Visitor Tests
    // ========================================================================

    auto visitor_suite = std::make_unique<TestSuite>("AST::Visitor");

    visitor_suite->add_test("placeholder_test", []() {
        // TODO: Add actual visitor tests
        UDO_ASSERT_TRUE(true);
    });

    runner.add_suite(std::move(visitor_suite));
}

} // namespace udo::test

// ============================================================================
// Main function for standalone AST test executable
// Only compiled when building as standalone (AST_TEST_STANDALONE defined)
// ============================================================================
#ifdef AST_TEST_STANDALONE
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

    register_ast_tests(runner);

    return runner.run_all(verbose);
}
#endif
