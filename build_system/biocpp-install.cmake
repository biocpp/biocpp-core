# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2022 deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# This file describes where and which parts of BioC++ should be installed to.

cmake_minimum_required (VERSION 3.7)

include (GNUInstallDirs)

# install documentation files in /share/doc
install (
    FILES
    "${BIOCPP_CLONE_DIR}/CHANGELOG.md"
    "${BIOCPP_CLONE_DIR}/CODE_OF_CONDUCT.md"
    "${BIOCPP_CLONE_DIR}/CONTRIBUTING.md"
    "${BIOCPP_CLONE_DIR}/LICENSE.md"
    "${BIOCPP_CLONE_DIR}/README.md"
    TYPE DOC
)

# install cmake files in /share/cmake
install (
    FILES
    "${BIOCPP_CLONE_DIR}/build_system/biocpp-config.cmake"
    "${BIOCPP_CLONE_DIR}/build_system/biocpp-config-version.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/biocpp"
)

# install biocpp header files in /include/bio
install (DIRECTORY "${BIOCPP_INCLUDE_DIR}/bio" TYPE INCLUDE)
