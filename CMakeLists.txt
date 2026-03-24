cmake_minimum_required(VERSION 3.20)
project(InfraDBCore VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Optimization for production
if(MSVC)
    add_compile_options(/O2 /arch:AVX2 /Oi /Ot /GL)
    add_link_options(/LTCG)
else()
    add_compile_options(-O3 -mavx2 -flto -pthread -fPIC)
endif()

# Fetch pybind11 automatically
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.13.1
)
FetchContent_MakeAvailable(pybind11)

include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES 
    "src/core/*.cpp"
    "src/memory/*.cpp"
    "src/execution/*.cpp"
    "src/python/*.cpp"
)

# Create the Python module
pybind11_add_module(infradb_core ${SOURCES})

target_include_directories(infradb_core PRIVATE include)

# Target-specific definitions
target_compile_definitions(infradb_core PRIVATE VERSION_INFO=${PROJECT_VERSION})
