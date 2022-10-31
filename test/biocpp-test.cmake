# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2020-2022, deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# This file provides functionality common to the different test modules used by
# BioC++. To build tests, run cmake on one of the sub-folders in this directory
# which contain a CMakeLists.txt.

cmake_minimum_required (VERSION 3.7)

if (NOT BIOCPP_CORE_FOUND)
    find_package (biocpp_core REQUIRED HINTS ${CMAKE_CURRENT_LIST_DIR}/../build_system)
endif ()

include (CheckCXXSourceCompiles)
include (FindPackageHandleStandardArgs)
include (FindPackageMessage)

option (BIOCPP_TEST_BUILD_OFFLINE "Skip the update step of external projects." OFF)

# Force alignment of benchmarked loops so that numbers are reliable.
# For large loops and erratic seeming bench results the value might
# have to be adapted or the option deactivated.
option (BIOCPP_BENCHMARK_ALIGN_LOOPS "Pass -falign-loops=32 to the benchmark builds." ON)

# ----------------------------------------------------------------------------
# Paths to folders.
# ----------------------------------------------------------------------------

find_path (BIOCPP_TEST_INCLUDE_DIR NAMES bio/test/tmp_filename.hpp HINTS "${CMAKE_CURRENT_LIST_DIR}/include/")
find_path (BIOCPP_TEST_CMAKE_MODULE_DIR NAMES biocpp_test_component.cmake HINTS "${CMAKE_CURRENT_LIST_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${BIOCPP_TEST_CMAKE_MODULE_DIR}")

set (BIOCPP_BENCHMARK_CLONE_DIR "${PROJECT_BINARY_DIR}/vendor/benchmark")
set (BIOCPP_TEST_CLONE_DIR "${PROJECT_BINARY_DIR}/vendor/googletest")

# needed for add_library (biocpp::test::* INTERFACE IMPORTED)
# see cmake bug https://gitlab.kitware.com/cmake/cmake/issues/15052
file(MAKE_DIRECTORY ${BIOCPP_BENCHMARK_CLONE_DIR}/include/)
file(MAKE_DIRECTORY ${BIOCPP_TEST_CLONE_DIR}/googletest/include/)

# ----------------------------------------------------------------------------
# Interface targets for the different test modules in bio.
# ----------------------------------------------------------------------------

# biocpp::test exposes a base set of required flags, includes, definitions and
# libraries which are in common for **all** bio tests
add_library (biocpp_test INTERFACE)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options (biocpp_test INTERFACE "-pedantic"  "-fsized-deallocation" "-Wall" "-Wextra" "-Werror" "-Wno-missing-braces" "-Wno-unused-variable" "-Wno-unused-local-typedef")
else ()
    target_compile_options (biocpp_test INTERFACE "-pedantic"  "-Wall" "-Wextra" "-Werror")
endif ()
target_link_libraries (biocpp_test INTERFACE "biocpp::core" "pthread")
target_include_directories (biocpp_test INTERFACE "${BIOCPP_TEST_INCLUDE_DIR}")
add_library (biocpp::test ALIAS biocpp_test)

# biocpp::test::performance specifies required flags, includes and libraries
# needed for performance test cases in bio/test/performance
add_library (biocpp_test_performance INTERFACE)
target_link_libraries (biocpp_test_performance INTERFACE "biocpp::test" "gbenchmark")

if (BIOCPP_BENCHMARK_ALIGN_LOOPS)
    target_compile_options (biocpp_test_performance INTERFACE "-falign-loops=32")
endif ()

target_include_directories (biocpp_test_performance INTERFACE "${BIOCPP_BENCHMARK_CLONE_DIR}/include/")
add_library (biocpp::test::performance ALIAS biocpp_test_performance)

# biocpp::test::unit specifies required flags, includes and libraries
# needed for unit test cases in bio/test/unit
add_library (biocpp_test_unit INTERFACE)
target_link_libraries (biocpp_test_unit INTERFACE "biocpp::test" "gtest_main" "gtest")
target_include_directories (biocpp_test_unit INTERFACE "${BIOCPP_TEST_CLONE_DIR}/googletest/include/")
add_library (biocpp::test::unit ALIAS biocpp_test_unit)

# biocpp::test::coverage specifies required flags, includes and libraries
# needed for coverage test cases in bio/test/coverage
add_library (biocpp_test_coverage INTERFACE)
target_compile_options (biocpp_test_coverage INTERFACE "--coverage" "-fprofile-arcs" "-ftest-coverage")
target_link_libraries (biocpp_test_coverage INTERFACE "biocpp::test::unit" "gcov")
add_library (biocpp::test::coverage ALIAS biocpp_test_coverage)

# biocpp::test::header specifies required flags, includes and libraries
# needed for header test cases in bio/test/header
add_library (biocpp_test_header INTERFACE)
target_link_libraries (biocpp_test_header INTERFACE "biocpp::test::unit")
target_link_libraries (biocpp_test_header INTERFACE "biocpp::test::performance")
add_library (biocpp::test::header ALIAS biocpp_test_header)

# ----------------------------------------------------------------------------
# Commonly shared options for external projects.
# ----------------------------------------------------------------------------

set (BIOCPP_EXTERNAL_PROJECT_CMAKE_ARGS "")
list (APPEND BIOCPP_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
list (APPEND BIOCPP_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
list (APPEND BIOCPP_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}")
list (APPEND BIOCPP_EXTERNAL_PROJECT_CMAKE_ARGS "-DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}")

# ----------------------------------------------------------------------------
# Commonly used macros for the different test modules in bio.
# ----------------------------------------------------------------------------

include (biocpp_test_component)
include (biocpp_test_files)
include (biocpp_require_ccache)
include (biocpp_require_benchmark)
include (biocpp_require_cereal)
include (biocpp_require_fmt)
include (biocpp_require_sharg)
include (biocpp_require_test)
include (add_subdirectories)
