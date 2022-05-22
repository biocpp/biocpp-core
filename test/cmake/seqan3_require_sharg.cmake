# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# Exposes the google-benchmark target `gbenchmark`.
macro (seqan3_require_sharg)
    enable_testing ()

    include (FetchContent)
    FetchContent_Declare(
        sharg_parser
        GIT_REPOSITORY https://github.com/seqan/sharg-parser.git
        GIT_TAG        d52f26bcaef54f9c334dba1c5900a1e6e234824b
    )

    FetchContent_MakeAvailable(sharg_parser)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/_deps/sharg_parser-src/build_system")
    find_package(SHARG REQUIRED)

endmacro ()
