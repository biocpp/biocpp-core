# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.7)
project (biocpp_test_unit CXX)

include (../biocpp-test.cmake)
include (GoogleTest OPTIONAL)

option (BIOCPP_VERBOSE_TESTS "Run each test case individually" ON)

macro (biocpp_test unit_test_cpp)
    file (RELATIVE_PATH unit_test "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/${unit_test_cpp}")
    biocpp_test_component (target "${unit_test}" TARGET_NAME)
    biocpp_test_component (test_name "${unit_test}" TEST_NAME)

    add_executable (${target} ${unit_test_cpp})
    target_link_libraries (${target} biocpp::test::unit)
    if (BIOCPP_VERBOSE_TESTS AND NOT CMAKE_VERSION VERSION_LESS 3.10) # cmake >= 3.10
        gtest_discover_tests(${target} TEST_PREFIX "${test_name}::" PROPERTIES TIMEOUT "30")
    else ()
        add_test (NAME "${test_name}" COMMAND ${target})
    endif ()

    unset (unit_test)
    unset (target)
    unset (test_name)
endmacro ()

biocpp_require_ccache ()
biocpp_require_test ()

add_subdirectories ()
