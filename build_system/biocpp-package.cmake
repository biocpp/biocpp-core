# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2022 deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
# -----------------------------------------------------------------------------------------------------

# This file describes how BioC++ will be packaged.

cmake_minimum_required (VERSION 3.7)

set (CPACK_GENERATOR "TXZ")

set (CPACK_PACKAGE_VENDOR "biocpp")
# A description of the project, used in places such as the introduction screen of CPack-generated Windows installers.
# set (CPACK_PACKAGE_DESCRIPTION_FILE "") # TODO
set (CPACK_PACKAGE_CHECKSUM "SHA256")
set (CPACK_PACKAGE_ICON "${BIOCPP_CLONE_DIR}/test/documentation/biocpp_logo.png")
set (CPACK_RESOURCE_FILE_LICENSE "${BIOCPP_CLONE_DIR}/LICENSE.md")
set (CPACK_RESOURCE_FILE_README "${BIOCPP_CLONE_DIR}/README.md")

# Source Package
set (CPACK_SOURCE_GENERATOR "TXZ")
set (CPACK_SOURCE_IGNORE_FILES "\\\\.git($|/)")

include (CPack)
