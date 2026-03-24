#include "infradb/execution/VectorBatch.hpp"
#include <iostream>

namespace infradb::execution {

void VectorBatch::add_column(DataType type, const std::string& name) {
    Column col;
    col.type = type;
    col.name = name;
    col.size = num_rows_;
    
    // Calculate byte size based on type
    size_t byte_size = 0;
    switch (type) {
        case DataType::INT32:   byte_size = sizeof(int32_t) * num_rows_; break;
        case DataType::FLOAT64: byte_size = sizeof(double) * num_rows_; break;
        case DataType::BOOL:    byte_size = sizeof(bool) * num_rows_; break;
        case DataType::STRING:  byte_size = sizeof(std::string) * num_rows_; break;
    }

    // Allocate contiguous memory from the pool
    if (byte_size > 0) {
        col.data = allocator_->allocate(byte_size);
        col.null_mask = static_cast<bool*>(allocator_->allocate(sizeof(bool) * num_rows_));
        
        // Initialize null mask to false (no nulls)
        for (size_t i = 0; i < num_rows_; ++i) {
            col.null_mask[i] = false;
        }
    } else {
        col.data = nullptr;
        col.null_mask = nullptr;
    }

    columns_.push_back(col);
}

} // namespace infradb::execution
