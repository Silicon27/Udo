//
// Created by David Yang on 2026-02-08.
//

#ifndef UDO_AST_CONTEXT_HPP
#define UDO_AST_CONTEXT_HPP

#include <vector>
#include <deque>
#include <memory>
#include <algorithm>
#include <ast/ast.hpp>

namespace udo::ast {

class ASTContext {
    struct Slab {
        char* buffer;
        char* current;
        std::size_t capacity;

        explicit Slab(std::size_t size);
        ~Slab();

        Slab(Slab&& other) noexcept;
        Slab& operator=(Slab&& other) noexcept;

        Slab(const Slab&) = delete;
        Slab& operator=(const Slab&) = delete;

        [[nodiscard]] bool is_full(std::size_t size = 0, std::size_t align = alignof(std::max_align_t)) const;
        [[nodiscard]] std::size_t get_remaining_capacity() const;
        void* allocate(std::size_t size, std::size_t align = alignof(std::max_align_t));
        void reset();
    };

public:
    template <typename VecAlloc = std::allocator<Slab>>
    class BumpPtrAllocator {
        std::deque<Slab, VecAlloc> slabs;
        std::vector<std::size_t> partially_used_slabs;
        std::size_t current_slab_idx{};
        std::size_t slab_size;

    public:
        explicit BumpPtrAllocator(std::size_t initial_slab_size = 1024 * 1024);

        void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t),
                       std::size_t size_of_new_slab = 0, bool reuse_free_slab = true);

        void reset_slab(std::size_t idx);

        [[nodiscard]] int current_slab_index() const { return current_slab_idx; }
        [[nodiscard]] std::size_t num_slabs() const { return slabs.size(); }
        [[nodiscard]] std::size_t num_partially_used_slabs() const { return partially_used_slabs.size(); }

        [[nodiscard]] std::size_t num_allocated_bytes() const;
        [[nodiscard]] std::size_t num_allocated_bytes_used() const;
        [[nodiscard]] std::size_t slab_sizes() const { return slab_size; }
    };

private:
    BumpPtrAllocator<> allocator;
    TranslationUnitDecl* tu_decl;

public:
    explicit ASTContext(std::size_t initial_slab_size = 1024 * 1024);

    [[nodiscard]] TranslationUnitDecl* getTranslationUnitDecl() const { return tu_decl; }

    void* Allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
        return allocator.allocate(size, alignment);
    }

    template <typename T, typename... Args>
    T* Create(Args&&... args) {
        void* storage = Allocate(sizeof(T), alignof(T));
        return new (storage) T(std::forward<Args>(args)...);
    }
};

} // namespace udo::ast

// Template implementation for BumpPtrAllocator
namespace udo::ast {

template <typename VecAlloc>
ASTContext::BumpPtrAllocator<VecAlloc>::BumpPtrAllocator(const std::size_t initial_slab_size)
    : slab_size(initial_slab_size) {
    slabs.emplace_back(slab_size);
}

template <typename VecAlloc>
void* ASTContext::BumpPtrAllocator<VecAlloc>::allocate(const std::size_t size, const std::size_t alignment,
                                                      const std::size_t size_of_new_slab, const bool reuse_free_slab) {
    if (reuse_free_slab && !partially_used_slabs.empty()) {
        for (auto it = partially_used_slabs.begin(); it != partially_used_slabs.end(); ) {
            Slab& slab = slabs[*it];
            if (void* result = slab.allocate(size, alignment)) {
                if (slab.get_remaining_capacity() == 0) {
                    it = partially_used_slabs.erase(it);
                }
                return result;
            }
            ++it;
        }
    }

    if (void* result = slabs[current_slab_idx].allocate(size, alignment)) {
        return result;
    }

    if (slabs[current_slab_idx].get_remaining_capacity() > 0) {
        partially_used_slabs.push_back(current_slab_idx);
    }

    const std::size_t new_slab_size = size_of_new_slab > 0 ? size_of_new_slab : slab_size;
    if (new_slab_size <= size) {
        return nullptr;
    }

    slabs.emplace_back(new_slab_size);
    current_slab_idx = slabs.size() - 1;

    return slabs[current_slab_idx].allocate(size, alignment);
}

template<typename VecAlloc>
void ASTContext::BumpPtrAllocator<VecAlloc>::reset_slab(std::size_t idx) {
    Slab& slab = slabs[idx];
    slab.reset();

    auto it = std::ranges::find(partially_used_slabs, idx);
    if (it != partially_used_slabs.end()) {
        partially_used_slabs.erase(it);
    }
    partially_used_slabs.insert(partially_used_slabs.begin(), idx);
}

template<typename VecAlloc>
std::size_t ASTContext::BumpPtrAllocator<VecAlloc>::num_allocated_bytes() const {
    std::size_t total = 0;
    for (const auto& slab : slabs) {
        total += slab.capacity;
    }
    return total;
}

template<typename VecAlloc>
std::size_t ASTContext::BumpPtrAllocator<VecAlloc>::num_allocated_bytes_used() const {
    std::size_t total = 0;
    for (const auto& slab : slabs) {
        total += slab.current - slab.buffer;
    }
    return total;
}

} // namespace udo::ast

#endif //UDO_AST_CONTEXT_HPP