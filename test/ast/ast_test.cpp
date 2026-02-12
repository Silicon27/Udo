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

    context_suite->add_test("partially_filled_slab_usage", [] {
        ASTContext::BumpPtrAllocator allocator(64);

        // partially fill a slab, this enables the allocator to push the semi used slab onto the partially filled vector
        void* p1 = allocator.allocate(4, 4);

        UDO_ASSERT_NOT_NULL(p1);
        UDO_ASSERT_EQ(allocator.num_slabs(), 1);

        // now we mandate the allocator create a new slab
        void* p2 = allocator.allocate(60, 8);

        UDO_ASSERT_NOT_NULL(p2);
        UDO_ASSERT_EQ(allocator.num_slabs(), 2);

        // and now we try to allocate some storage that would fit into the partially filled slab
        void* p3 = allocator.allocate(4, 4, 0,true);

        UDO_ASSERT_NOT_NULL(p3);
        UDO_ASSERT_EQ(allocator.num_slabs(), 2);
    });

    context_suite->add_test("reset_slab_reorders_partially_used", [] {
        ASTContext::BumpPtrAllocator allocator(64);

        allocator.allocate(40); // Slab 0, 24 left.
        allocator.allocate(40); // Slab 1 (current), 24 left. Slab 0 is in partially_used.

        UDO_ASSERT_EQ(allocator.num_slabs(), 2);
        UDO_ASSERT_EQ(allocator.num_partially_used_slabs(), 1);

        // Reset Slab 0. It's already there, so it should be moved to front (it's already the only one).
        allocator.reset_slab(0);
        UDO_ASSERT_EQ(allocator.num_partially_used_slabs(), 1);

        // Allocate 40 again. Slab 1 is current, but Slab 0 is in partially_used and it's tried first.
        // Slab 0 now has 64 available.
        void* p = allocator.allocate(40);
        UDO_ASSERT_NOT_NULL(p);
        UDO_ASSERT_EQ(allocator.num_slabs(), 2);

        // To verify it used Slab 0:
        // Slab 0 should now have 40 used.
        // Slab 1 should still have 40 used.
        // Total used should be 80.
        UDO_ASSERT_EQ(allocator.num_allocated_bytes_used(), 80);

        // Now trigger Slab 2
        allocator.allocate(40); // Current is Slab 1, but it only has 24 left.
        // Wait, if it tries partially_used first, it tries Slab 0.
        // Slab 0 has 64 - 40 = 24 left.
        // So it tries Slab 0 (24 left) -> fails.
        // Then it tries Slab 1 (current, 24 left) -> fails.
        // Then it creates Slab 2.
        UDO_ASSERT_EQ(allocator.num_slabs(), 3);
        // Slab 1 should have been added to partially_used.
        // partially_used order: [1, 0]
        UDO_ASSERT_EQ(allocator.num_partially_used_slabs(), 2);

        // Now reset Slab 0. It should be moved to front: [0, 1]
        allocator.reset_slab(0);
        UDO_ASSERT_EQ(allocator.num_partially_used_slabs(), 2);

        // Allocation should now prioritize Slab 0.
        // Slab 0 is now empty (64 available).
        void* p2 = allocator.allocate(60);
        UDO_ASSERT_NOT_NULL(p2);
        UDO_ASSERT_EQ(allocator.num_slabs(), 3); // No new slab needed.
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
