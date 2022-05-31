# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
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
    DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/seqan3"
)

# install seqan3 header files in /include/seqan3
install (DIRECTORY "${BIOCPP_INCLUDE_DIR}/seqan3" TYPE INCLUDE)

# install submodule header files, e.g. all external dependencies in /home/user/seqan3/submodules/*,
# in /include/seqan3/submodules/<submodule>/include
foreach (submodule_dir ${BIOCPP_DEPENDENCY_INCLUDE_DIRS})
    # e.g. submodule_dir: (1) /home/user/seqan3/submodules/sdsl-lite/include or (2) /usr/include
    # strip /home/user/seqan3/submodules/ and /include part.
    file (RELATIVE_PATH submodule "${BIOCPP_SUBMODULES_DIR}/submodules" "${submodule_dir}/..")
    # submodule is either a single module name, like sdsl-lite or a relative path to a folder ../../../usr
    # skip relative folders and only keep submodules that reside in the submodules folder
    if (NOT submodule MATCHES "^\\.\\.") # skip relative folders
        install (DIRECTORY "${submodule_dir}" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/seqan3/submodules/${submodule}")
    endif ()
endforeach ()
