#include <ast/ASTContext.hpp>
#include <memory>
#include <cstdint>
#include <algorithm>

ASTContext::Slab::Slab(const std::size_t size) {
    buffer = new char[size];
    current = buffer;
    capacity = size;
}

ASTContext::Slab::~Slab() {
    delete[] buffer;
}

ASTContext::Slab::Slab(Slab&& other) noexcept
    : buffer(other.buffer), current(other.current), capacity(other.capacity) {
    other.buffer = nullptr;
    other.current = nullptr;
    other.capacity = 0;
}

ASTContext::Slab& ASTContext::Slab::operator=(Slab&& other) noexcept {
    if (this != &other) {
        delete[] buffer;
        buffer = other.buffer;
        current = other.current;
        capacity = other.capacity;
        other.buffer = nullptr;
        other.current = nullptr;
        other.capacity = 0;
    }
    return *this;
}

bool ASTContext::Slab::is_full(const std::size_t size, const std::size_t align) const {
    std::size_t remaining = capacity - (current - buffer);
    if (remaining < size) return true;

    // Calculate padding needed to align 'current'
    auto curr_addr = reinterpret_cast<std::uintptr_t>(current);
    std::size_t padding = (align - (curr_addr % align)) % align;

    return remaining < (size + padding);
}

std::size_t ASTContext::Slab::get_remaining_capacity() const {
    return capacity - (current - buffer);
}

void* ASTContext::Slab::allocate(const std::size_t size, const std::size_t align) {
    std::size_t space_left = capacity - (current - buffer);

    void* alignPtr = std::align(align, size, reinterpret_cast<void*&>(current), space_left);
    if (!alignPtr) return nullptr; // allocation failed

    current += size;
    return alignPtr;
}

void ASTContext::Slab::reset() {
    current = buffer; // overwrite all data
}