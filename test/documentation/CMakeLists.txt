# Minimum cmake version
cmake_minimum_required(VERSION 3.7)

# Name of project
project(biocpp_documentation NONE)

### Find BioC++
include (../../build_system/biocpp-config-version.cmake)
set (BIOCPP_VERSION "${PACKAGE_VERSION}")

if (NOT EXISTS "${BIOCPP_INCLUDE_DIR}/bio/version.hpp")
    message (FATAL_ERROR "Could not find BioC++. Not building documentation.")
endif ()
message (STATUS "Found: ${BIOCPP_INCLUDE_DIR} (${BIOCPP_VERSION})")

set (BIOCPP_DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}")

include (biocpp-doxygen.cmake)

### Enable testing
enable_testing()

# doxygen does not show any warnings (doxygen prints warnings / errors to cerr)
set (BIOCPP_DOXYGEN_FAIL_ON_WARNINGS "
    ${DOXYGEN_EXECUTABLE} > doxygen.cout 2> doxygen.cerr;
    cat \"doxygen.cerr\";
    test ! -s \"doxygen.cerr\"")

# We search the HTML output to ensure that no `requires` clauses are at wrong places.
set (BIOCPP_DOXYGEN_FAIL_ON_UNCOND_REQUIRES
     "! find . -not -name \"*_source.html\" -name \"*.html\" -print0 | xargs -0 grep \"requires\" | grep \"memname\"")

if (BIOCPP_USER_DOC)
    message (STATUS "Add user doc test.")
    add_test(NAME doc_usr_no_warnings_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_WARNINGS}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_usr)

    add_test(NAME doc_usr_no_uncond_requires_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_UNCOND_REQUIRES}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_usr)
endif ()

if (BIOCPP_DEV_DOC)
    message (STATUS "Add dev doc test.")
    add_test(NAME doc_dev_no_warnings_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_WARNINGS}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_dev)

    add_test(NAME doc_dev_no_uncond_requires_test
             COMMAND bash -c "${BIOCPP_DOXYGEN_FAIL_ON_UNCOND_REQUIRES}"
             WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc_dev)
endif ()
