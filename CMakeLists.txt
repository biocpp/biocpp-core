# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2021-2022, deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# To build tests, run cmake on one of the sub-folders in the test/ subdirectory.

cmake_minimum_required (VERSION 3.4)

find_path (BIOCPP_MODULE_PATH "biocpp-config.cmake" HINTS "${CMAKE_CURRENT_LIST_DIR}/build_system/")
list (APPEND CMAKE_MODULE_PATH "${BIOCPP_MODULE_PATH}")

include (biocpp-config-version)

if (CMAKE_VERSION VERSION_LESS 3.12)
    project (biocpp_core LANGUAGES CXX VERSION "${PACKAGE_VERSION}")
else ()
    project (
        biocpp_core LANGUAGES CXX VERSION "${PACKAGE_VERSION}"
        DESCRIPTION "SeqAn3 -- the modern C++ library for sequence analysis" # since cmake 3.9
        HOMEPAGE_URL "https://github.com/biocpp/biocpp-core" # since cmake 3.12
    )
endif ()

find_package (SeqAn3 3.0 REQUIRED HINTS ${BIOCPP_MODULE_PATH})

option(INSTALL_BIOCPP "Enable installation of BioC++ (Projects embedding biocpp may want to turn this OFF.)" ON)

if (INSTALL_BIOCPP)
    include (biocpp-install)
    include (biocpp-package)
endif ()
