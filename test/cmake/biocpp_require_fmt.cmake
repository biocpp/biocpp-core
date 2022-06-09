# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# Exposes the google-benchmark target `gbenchmark`.
macro (biocpp_require_fmt)
    enable_testing ()

    include (FetchContent)
    FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        # master TODO switch to release
        GIT_TAG 48b7e3dafb27ece02cd6addc8bd1041c79d59c2c
    )

    FetchContent_MakeAvailable(fmtlib)
endmacro ()
