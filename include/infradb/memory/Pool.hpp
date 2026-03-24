#pragma once
#include <memory_resource>
#include <mutex>

namespace infradb::memory {

/**
 * GlobalMemoryPool manages a large pre-allocated slab of memory.
 * It uses std::pmr::monotonic_buffer_resource for zero-deallocation overhead
 * during query lifetimes.
 */
class GlobalMemoryPool {
public:
    static GlobalMemoryPool& instance() {
        static GlobalMemoryPool inst;
        return inst;
    }

    std::pmr::memory_resource* get_resource() {
        return &res_;
    }

    // Reset the pool (use with caution, typically at the end of a query)
    void release() {
        res_.release();
    }

private:
    GlobalMemoryPool() : buffer_(1024 * 1024 * 512), res_(buffer_.data(), buffer_.size()) {}
    
    std::vector<std::byte> buffer_;
    std::pmr::monotonic_buffer_resource res_;

    // Prevent copying
    GlobalMemoryPool(const GlobalMemoryPool&) = delete;
    GlobalMemoryPool& operator=(const GlobalMemoryPool&) = delete;
};

} // namespace infradb::memory
