//
// Created by David Yang on 2026-02-08.
//

#ifndef UDO_AST_CONTEXT_HPP
#define UDO_AST_CONTEXT_HPP

#include <vector>
#include <memory>

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
        std::vector<Slab, VecAlloc> slabs;
        std::vector<Slab*> partially_used_slabs;
        int current_slab_idx{};
        std::size_t slab_size;

    public:
        explicit BumpPtrAllocator(std::size_t initial_slab_size = 1024 * 1024);

        /// @brief Allocates storage of the given size and returns a pointer to it.
        /// If the current slab does not have enough space, a new slab is allocated.
        ///
        /// @param size the size of the memory chunk being allocated
        /// @param alignment alignment of chunk within the slab
        /// @param size_of_new_slab size of the new slab to be allocated if the current slab is full, a value <0 means to use member slab_size to construct the new slab
        /// @param reuse_free_slab if true, the allocator would try to reuse the slab that was cast aside in favor of a new bigger slab when allocating storage more than the available amount in the current slab.
        ///
        /// @returns pointer to the allocated memory chunk, or nullptr if allocation fails (e.g. if even a new slab cannot accommodate the requested size - fix: increase size_of_new_slab to allocate a new slab of a custom size)
        void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t),
                       int size_of_new_slab = 0, bool reuse_free_slab = false);

        [[nodiscard]] int current_slab_index() const { return current_slab_idx; }
        [[nodiscard]] std::size_t num_slabs() const { return slabs.size(); }
        [[nodiscard]] std::size_t num_allocated_bytes() const { return slabs.back().current - slabs.front().buffer; }
        [[nodiscard]] std::size_t slab_sizes() const { return slab_size; }
    };
};

// Template implementation for BumpPtrAllocator
template <typename VecAlloc>
ASTContext::BumpPtrAllocator<VecAlloc>::BumpPtrAllocator(const std::size_t initial_slab_size)
    : slab_size(initial_slab_size) {
    slabs.emplace_back(slab_size);
}


template <typename VecAlloc>
void* ASTContext::BumpPtrAllocator<VecAlloc>::allocate(const std::size_t size, const std::size_t alignment,
                                                      const int size_of_new_slab, const bool reuse_free_slab) {
    if (reuse_free_slab && partially_used_slabs.size() > 0) {
        for (const auto it : partially_used_slabs) {
            if (!it->is_full(size, alignment)) {
                const auto result = it->allocate(size, alignment);
                if (it->get_remaining_capacity() == 0) {
                    std::erase(partially_used_slabs, it);
                }
                return result;
            }
        }
    }

    if (slabs[current_slab_idx].is_full(size, alignment)) {
        Slab& active_slab = slabs[current_slab_idx];
        std::size_t new_slab_size = size_of_new_slab > 0 ? size_of_new_slab : slab_size;
        if (new_slab_size <= size) {
            return nullptr;
        }

        if (active_slab.get_remaining_capacity() > 0) {
            partially_used_slabs.push_back(&active_slab);
        }

        slabs.emplace_back(new_slab_size);
        current_slab_idx++;
    }
    return slabs[current_slab_idx].allocate(size, alignment);
}

#endif //UDO_AST_CONTEXT_HPP