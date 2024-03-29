# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.7)
project (biocpp_test_performance CXX)

include (../biocpp-test.cmake)

macro (biocpp_benchmark benchmark_cpp)
    file (RELATIVE_PATH benchmark "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_LIST_DIR}/${benchmark_cpp}")
    biocpp_test_component (target "${benchmark}" TARGET_NAME)
    biocpp_test_component (test_name "${benchmark}" TEST_NAME)

    add_executable (${target} ${benchmark_cpp})
    target_link_libraries (${target} biocpp::test::performance)
    add_test (NAME "${test_name}" COMMAND ${target})

    unset (benchmark)
    unset (target)
    unset (test_name)
endmacro ()

biocpp_require_ccache ()
biocpp_require_benchmark ()

add_subdirectories ()
