cmake_minimum_required (VERSION 3.10)

### Attention: this runs clang-tidy IN ADDITION to regular unit tests
### Also make sure to set your CXX_COMPILER to clang++

project(biocpp_clang_tidy NONE)

find_program(CLANG_TIDY "clang-tidy-16")

if (CLANG_TIDY STREQUAL "CLANG_TIDY-NOTFOUND")
    find_program(CLANG_TIDY "clang-tidy")
endif()

if (NOT CLANG_TIDY STREQUAL "CLANG_TIDY-NOTFOUND")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    add_subdirectory(../unit/ unit)
    enable_testing()
else ()
    message(FATAL_ERROR "clang-tidy not found. No targets made.")
endif()

