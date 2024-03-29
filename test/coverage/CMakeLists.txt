# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2022, deCODE Genetics
# Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.10...3.22)
project (biocpp_test_coverage CXX)

include (../biocpp-test.cmake)

if (CMAKE_BUILD_TYPE AND NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    message (WARNING "Coverage test must be build in debug mode [build type = ${CMAKE_BUILD_TYPE}]")
endif ()

find_program (GCOVR_COMMAND NAMES gcovr)

if (NOT GCOVR_COMMAND)
    message (FATAL_ERROR "gcovr not found! Aborting...")
endif ()

# Holds all target's defined by biocpp_test
set_property (GLOBAL PROPERTY GLOBAL_TEST_COVERAGE_ALL_TESTS "")

set (BIOCPP_COVERAGE_PARALLEL_LEVEL
     "1"
     CACHE STRING "Number of threads to use for coverage report generation.")

# Arguments for gcovr.
# Each line is an argument, ';' will end up being a whitespace due to expansion by CMake.
# Special characters like '"', '\', ')' need to be escaped by a number of '\', depending how often they are evaluated.
# gcovr uses python regex expressions.
# See https://gcovr.com/en/5.0/guide.html#the-gcovr-command for an overview of parameters.
# See https://gcovr.com/en/5.0/faq.html#why-does-c-code-have-so-many-uncovered-branches for an explanation on branches.
set (BIOCPP_GCOVR_ARGUMENTS "")
# The directory of the CMakeLists.txt used for invoking cmake.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--root;${CMAKE_CURRENT_LIST_DIR}")
# The build directory.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "${PROJECT_BINARY_DIR}")
# Include all files whose path match '${BIOCPP_CORE_CLONE_DIR}/include/biocpp/.*'.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--filter;\'${BIOCPP_CORE_CLONE_DIR}/include/bio/.*\'")
# Include all files whose path match '${BIOCPP_CORE_CLONE_DIR}/test/include/biocpp/test/.*'.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--filter;\'${BIOCPP_CORE_CLONE_DIR}/test/include/bio/test/.*\'")
# Will exclude branches that are unreachable.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--exclude-unreachable-branches")
# Will exclude branches that are only generated for exception handling.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--exclude-throw-branches")
# Run up to this many gcov instances in parallel.
list (APPEND BIOCPP_GCOVR_ARGUMENTS "-j;${BIOCPP_COVERAGE_PARALLEL_LEVEL}")

set (BIOCPP_GCOVR_EXCLUDE_PATTERNS "")
list(APPEND BIOCPP_GCOVR_EXCLUDE_PATTERNS "^\\s*assert\\(.*$$")             # assert()
list(APPEND BIOCPP_GCOVR_EXCLUDE_PATTERNS "^.*GCOVR_EXCL_LINE.*$$")         # should be default but isnt
list(APPEND BIOCPP_GCOVR_EXCLUDE_PATTERNS "^\\s*BIOCPP_UNREACHABLE\\s*$$")  # unreachables

set(BIOCPP_GCOVR_EXCLUDE_PATTERNS_JOINED "")
list(JOIN BIOCPP_GCOVR_EXCLUDE_PATTERNS ")|(" BIOCPP_GCOVR_EXCLUDE_PATTERNS_JOINED)
list (APPEND BIOCPP_GCOVR_ARGUMENTS "--exclude-lines-by-pattern;'(${BIOCPP_GCOVR_EXCLUDE_PATTERNS_JOINED})'")

add_custom_command (OUTPUT ${PROJECT_BINARY_DIR}/biocpp_coverage.xml
                    # Run tests.
                    COMMAND ${CMAKE_CTEST_COMMAND} -j ${BIOCPP_COVERAGE_PARALLEL_LEVEL} --output-on-failure
                    # Run gcovr and create XML report.
                    COMMAND ${GCOVR_COMMAND} ${BIOCPP_GCOVR_ARGUMENTS} --xml --output
                            ${PROJECT_BINARY_DIR}/biocpp_coverage.xml
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                    COMMENT "Processing code coverage counters and generating XML report."
                    COMMAND_EXPAND_LISTS)

add_custom_target (coverage ALL DEPENDS ${PROJECT_BINARY_DIR}/biocpp_coverage.xml)

add_custom_command (OUTPUT ${PROJECT_BINARY_DIR}/html/index.html
                    # Run tests.
                    COMMAND ${CMAKE_CTEST_COMMAND} -j ${BIOCPP_COVERAGE_PARALLEL_LEVEL} --output-on-failure
                    # Create output directory.
                    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/html/
                    # Run gcovr and create HTML report.
                    COMMAND ${GCOVR_COMMAND} ${BIOCPP_GCOVR_ARGUMENTS} --html-details --output
                            ${PROJECT_BINARY_DIR}/html/index.html
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                    COMMENT "Processing code coverage counters and generating HTML report."
                    COMMAND_EXPAND_LISTS)

add_custom_target (coverage_html
                   DEPENDS ${PROJECT_BINARY_DIR}/html/index.html
                   COMMENT "Generate coverage report.")

add_custom_command (TARGET coverage_html
                    POST_BUILD
                    COMMAND ;
                    COMMENT "Open ${PROJECT_BINARY_DIR}/html/index.html in your browser to view the coverage report.")

macro (biocpp_test unit_test_cpp)
    file (RELATIVE_PATH unit_test "${CMAKE_SOURCE_DIR}/../unit" "${CMAKE_CURRENT_LIST_DIR}/${unit_test_cpp}")
    biocpp_test_component (target "${unit_test}" TARGET_NAME)
    biocpp_test_component (test_name "${unit_test}" TEST_NAME)

    add_executable (${target} ${unit_test_cpp})
    target_link_libraries (${target} biocpp::test::coverage)
    add_test (NAME "${test_name}" COMMAND ${target})

    # any change of a target will invalidate the coverage result;
    # NOTE that this is a GLOBAL variable, because a normal
    # `set(GLOBAL_TEST_COVERAGE_ALL_TESTS)` would not propagate the result when
    # CMakeLists.txt goes out of scope due to a `add_subdirectory`
    set_property (GLOBAL APPEND PROPERTY GLOBAL_TEST_COVERAGE_ALL_TESTS ${target})

    unset (unit_test)
    unset (target)
    unset (test_name)
endmacro ()

biocpp_require_ccache ()
biocpp_require_test ()

# add all unit tests
add_subdirectories_of ("${CMAKE_CURRENT_SOURCE_DIR}/../unit")

# add collected test cases as dependency
get_property (TEST_COVERAGE_ALL_TESTS GLOBAL PROPERTY GLOBAL_TEST_COVERAGE_ALL_TESTS)
add_custom_command (OUTPUT ${PROJECT_BINARY_DIR}/biocpp_coverage.xml
                    DEPENDS ${TEST_COVERAGE_ALL_TESTS}
                    APPEND)
