# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

macro (biocpp_require_cereal)
    enable_testing ()

    include (FetchContent)
    FetchContent_Declare(
        cereal-lib
        GIT_REPOSITORY https://github.com/USCiLab/cereal/
        # release v1.3.2
        GIT_TAG ebef1e929807629befafbb2918ea1a08c7194554
    )

    set(BUILD_SANDBOX OFF)
    set(BUILD_DOC OFF)
    set(BUILD_TESTS OFF)
    set(SKIP_PERFORMANCE_COMPARISON ON)
    set(CEREAL_INSTALL OFF)

    FetchContent_MakeAvailable(cereal-lib)
endmacro ()
