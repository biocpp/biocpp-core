# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.7)
project (biocpp_test_snippet CXX)

include (../biocpp-test.cmake)

macro (biocpp_snippet test_name_prefix snippet snippet_base_path)
    biocpp_test_component (snippet_target_name "${snippet}" TARGET_NAME)
    biocpp_test_component (snippet_test_name "${snippet}" TEST_NAME)
    biocpp_test_component (snippet_target_path "${snippet}" TARGET_PATH)
    set (target "${snippet_target_name}_snippet")

    add_executable (${target} "${snippet_base_path}/${snippet}")
    target_link_libraries (${target} biocpp::test::unit sharg::sharg fmt::fmt-header-only)
    target_compile_options (${target} PUBLIC "-Wno-unused")
    set_target_properties(${target}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${snippet_target_path}"
    )

    add_test (NAME "${test_name_prefix}/${snippet_test_name}_snippet" COMMAND ${target})

    unset (snippet_target_name)
    unset (snippet_test_name)
    unset (snippet_target_path)
    unset (target)
endmacro ()

macro (biocpp_snippets test_name_prefix snippet_base_path)
    biocpp_test_files (snippet_files "${snippet_base_path}" "*.cpp")
    foreach (snippet ${snippet_files})
        biocpp_snippet("${test_name_prefix}" "${snippet}" "${snippet_base_path}")
    endforeach ()
endmacro ()

biocpp_require_ccache ()
biocpp_require_test ()
biocpp_require_sharg ()
biocpp_require_fmt ()

biocpp_snippets ("doc/snippet" "${CMAKE_SOURCE_DIR}/../../doc")
biocpp_snippets ("snippet" "${CMAKE_SOURCE_DIR}")
