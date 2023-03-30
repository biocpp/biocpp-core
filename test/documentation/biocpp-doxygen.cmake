# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.7)

function(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

macro(write_layout)
    file(WRITE ${BIOCPP_DOXYGEN_OUTPUT_DIR}/DoxygenLayout.xml "")
    cat(${BIOCPP_CORE_CLONE_DIR}/test/documentation/DoxygenLayout.xml.head ${BIOCPP_DOXYGEN_OUTPUT_DIR}/DoxygenLayout.xml)
    foreach(MODULE ${BIOCPP_DOXYGEN_MODULE_LAYOUT})
        cat("${MODULE}" "${BIOCPP_DOXYGEN_OUTPUT_DIR}/DoxygenLayout.xml")
    endforeach ()
    cat(${BIOCPP_CORE_CLONE_DIR}/test/documentation/DoxygenLayout.xml.tail ${BIOCPP_DOXYGEN_OUTPUT_DIR}/DoxygenLayout.xml)
endmacro()

### Find doxygen and dependency to DOT tool
message (STATUS "Searching for doxygen.")
find_package (Doxygen REQUIRED)

if (NOT ${DOXYGEN_FOUND})
    message (FATAL_ERROR "Could not find doxygen. Not building documentation.")
endif ()

if (NOT ${DOXYGEN_DOT_FOUND})
    message (STATUS "Could not find dot tool. Disabling dot support.")
    set (BIOCPP_DOXYGEN_HAVE_DOT "NO")
else ()
    message (STATUS "Found dot tool. Enabling dot support.")
    set (BIOCPP_DOXYGEN_HAVE_DOT "YES")
endif ()

### Configure doc/developer targets.
set(BIOCPP_DOXYFILE_IN ${BIOCPP_CORE_CLONE_DIR}/test/documentation/biocpp_doxygen_cfg.in)

option(BIOCPP_USER_DOC "Create build target and test for user documentation." ON)
option(BIOCPP_DEV_DOC "Create build target and test for developer documentation." ON)

### Download and extract cppreference-doxygen-web.tag.xml for std:: documentation links
set(BIOCPP_DOXYGEN_STD_TAGFILE "${PROJECT_BINARY_DIR}/cppreference-doxygen-web.tag.xml")
include(ExternalProject)
ExternalProject_Add (
    download-cppreference-doxygen-web-tag
    URL "https://github.com/PeterFeicht/cppreference-doc/releases/download/v20220201/html-book-20220201.tar.xz"
    URL_HASH SHA256=b41960e7ec9c5433b31b1b33db5854f97770ae49535c81e7647f618003102d1a
    TLS_VERIFY ON
    DOWNLOAD_DIR "${PROJECT_BINARY_DIR}"
    DOWNLOAD_NAME "html-book.tar.xz"
    DOWNLOAD_NO_EXTRACT YES
    BINARY_DIR "${PROJECT_BINARY_DIR}"
    CONFIGURE_COMMAND /bin/sh -c "xzcat html-book.tar.xz | tar -xf - cppreference-doxygen-web.tag.xml"
    BUILD_COMMAND rm "html-book.tar.xz"
    INSTALL_COMMAND ""
)

if (BIOCPP_USER_DOC)
    message (STATUS "Configuring user doc.")

    set (BIOCPP_DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/doc_usr")
    set (BIOCPP_DOXYGEN_EXCLUDE_SYMBOLS "bio::*::detail") #/""
    set (BIOCPP_DOXYGEN_PREDEFINED_NDEBUG "-NDEBUG") #/""
    set (BIOCPP_DOXYGEN_ENABLED_SECTIONS "") #/"DEV"
    set (BIOCPP_DOXYGEN_EXTRACT_PRIVATE "NO") #/"YES":

    configure_file (${BIOCPP_DOXYFILE_IN} ${BIOCPP_DOXYGEN_OUTPUT_DIR}/Doxyfile)
    write_layout()

    add_custom_target(doc_usr ALL
                      COMMAND ${DOXYGEN_EXECUTABLE}
                      WORKING_DIRECTORY ${BIOCPP_DOXYGEN_OUTPUT_DIR}
                      DEPENDS download-cppreference-doxygen-web-tag
                      COMMENT "Generating user API documentation with Doxygen"
                      VERBATIM)
endif ()

if (BIOCPP_DEV_DOC)
    message(STATUS "Configuring devel doc.")

    set(BIOCPP_DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DIR}/doc_dev")
    set(BIOCPP_DOXYGEN_EXCLUDE_SYMBOLS "")
    set(BIOCPP_DOXYGEN_PREDEFINED_NDEBUG "")
    set(BIOCPP_DOXYGEN_ENABLED_SECTIONS "DEV")
    set(BIOCPP_DOXYGEN_EXTRACT_PRIVATE "YES")

    configure_file(${BIOCPP_DOXYFILE_IN} ${BIOCPP_DOXYGEN_OUTPUT_DIR}/Doxyfile)
    write_layout()

    add_custom_target(doc_dev ALL
                      COMMAND ${DOXYGEN_EXECUTABLE}
                      WORKING_DIRECTORY ${BIOCPP_DOXYGEN_OUTPUT_DIR}
                      DEPENDS download-cppreference-doxygen-web-tag
                      COMMENT "Generating developer API documentation with Doxygen"
                      VERBATIM)
endif ()

### Enable testing
enable_testing()

# doxygen does not show any warnings (doxygen prints warnings / errors to cerr)
set (BIOCPP_DOXYGEN_FAIL_ON_WARNINGS "
    ${DOXYGEN_EXECUTABLE} > doxygen.cout 2> doxygen.cerr;
    cat \"doxygen.cerr\";
    test ! -s \"doxygen.cerr\"")

if (BIOCPP_USER_DOC)
    message (STATUS "Add user doc test.")
    add_test(NAME doc_usr_no_warnings_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_WARNINGS}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_usr)
endif ()

if (BIOCPP_DEV_DOC)
    message (STATUS "Add devel doc test.")
    add_test(NAME doc_dev_no_warnings_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_WARNINGS}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_dev)
endif ()
