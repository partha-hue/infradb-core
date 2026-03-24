# InfraDB Core

A high-performance C++ data infrastructure engine designed for
AI workloads, fast data transfer, and system-level optimization.

Part of the InframindAI ecosystem.

## Build Requirements
- CMake (3.20+)
- C++20 Compiler
- pybind11

## Architecture
- **Vectorized Execution**: Processes data in batches for L1/L2 cache locality.
- **Arena Memory Management**: Zero-deallocation overhead during query lifetimes.
- **Python Bridge**: Multi-threaded GIL-free execution for Django/AI integrations.
