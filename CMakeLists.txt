cmake_minimum_required(VERSION 3.20)
project(InfraDBCore VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimization for production
if(MSVC)
    add_compile_options(/O2 /arch:AVX2 /Oi /Ot /GL)
    add_link_options(/LTCG)
else()
    add_compile_options(-O3 -mavx2 -flto -pthread -fPIC)
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/core/*.cpp"
    "src/memory/*.cpp"
    "src/execution/*.cpp"
)

# Build the main executable for Render deployment
add_executable(infradb_core_app src/main.cpp ${SOURCES})

# Also build the Python module if needed (Optional for standalone service)
# include(FetchContent)
# FetchContent_Declare(pybind11 GIT_REPOSITORY https://github.com/pybind/pybind11.git GIT_TAG v2.13.1)
# FetchContent_MakeAvailable(pybind11)
# pybind11_add_module(infradb_core src/python/bindings.cpp ${SOURCES})
