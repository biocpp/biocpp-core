# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2022 deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
# -----------------------------------------------------------------------------------------------------
#
# This CMake module will try to find BioC++ and its dependencies.  You can use
# it the same way you would use any other CMake module.
#
#   find_package (biocpp_core [REQUIRED] ...)
#
# Since this makes a difference for CMAKE, pay attention to the case.
#
# BioC++ has the following platform requirements:
#
#   C++20
#
# Once the search has been performed, the following variables will be set.
#
#   BIOCPP_CORE_FOUND            -- Indicate whether BioC++ was found and requirements met.
#
#   BIOCPP_CORE_VERSION          -- The version as string, e.g. "3.0.0"
#   BIOCPP_CORE_VERSION_MAJOR    -- e.g. 3
#   BIOCPP_CORE_VERSION_MINOR    -- e.g. 0
#   BIOCPP_CORE_VERSION_PATCH    -- e.g. 0
#
#   BIOCPP_CORE_INCLUDE_DIRS     -- to be passed to include_directories ()
#   BIOCPP_CORE_LIBRARIES        -- to be passed to target_link_libraries ()
#   BIOCPP_CORE_DEFINITIONS      -- to be passed to add_definitions ()
#   BIOCPP_CORE_CXX_FLAGS        -- to be added to CMAKE_CXX_FLAGS
#
# Additionally, the following [IMPORTED][IMPORTED] targets are defined:
#
#   biocpp::core          -- interface target where target_link_libraries(target biocpp::core)
#                              automatically sets
#                                  target_include_directories(target $BIOCPP_CORE_INCLUDE_DIRS),
#                                  target_link_libraries(target $BIOCPP_CORE_LIBRARIES),
#                                  target_compile_definitions(target $BIOCPP_CORE_DEFINITIONS) and
#                                  target_compile_options(target $BIOCPP_CORE_CXX_FLAGS)
#                              for a target.
#
#   [IMPORTED]: https://cmake.org/cmake/help/v3.10/prop_tgt/IMPORTED.html#prop_tgt:IMPORTED
#
# ============================================================================

cmake_minimum_required (VERSION 3.4...3.12)

# ----------------------------------------------------------------------------
# Set initial variables
# ----------------------------------------------------------------------------

# make output globally quiet if required by find_package, this effects cmake functions like `check_*`
set(CMAKE_REQUIRED_QUIET_SAVE ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ${${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY})

# ----------------------------------------------------------------------------
# Greeter
# ----------------------------------------------------------------------------

string (ASCII 27 Esc)
set (ColourBold "${Esc}[1m")
set (ColourReset "${Esc}[m")

if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message (STATUS "${ColourBold}BioC++ core library:${ColourReset}")
endif ()

# ----------------------------------------------------------------------------
# Includes
# ----------------------------------------------------------------------------

include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)
include (FindPackageHandleStandardArgs)

# ----------------------------------------------------------------------------
# Pretty printing and error handling
# ----------------------------------------------------------------------------

macro (biocpp_config_print text)
    if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        message (STATUS "  ${text}")
    endif ()
endmacro ()

macro (biocpp_config_error text)
    if (${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
        message (FATAL_ERROR ${text})
    else ()
        if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
            message (WARNING ${text})
        endif ()
        return ()
    endif ()
endmacro ()

# ----------------------------------------------------------------------------
# Find BioC++ include path
# ----------------------------------------------------------------------------

if (BIOCPP_CORE_INCLUDE_DIR)
    biocpp_config_print ("Include dir found:          ${BIOCPP_CORE_INCLUDE_DIR}")
else ()
    biocpp_config_error ("Include directory could not be found (BIOCPP_CORE_INCLUDE_DIR: '${BIOCPP_CORE_INCLUDE_DIR}')")
endif ()

# ----------------------------------------------------------------------------
# Options for CheckCXXSourceCompiles
# ----------------------------------------------------------------------------

# deactivate messages in check_*
set (CMAKE_REQUIRED_QUIET       1)
# use global variables in Check* calls
set (CMAKE_REQUIRED_INCLUDES    ${CMAKE_INCLUDE_PATH} ${BIOCPP_CORE_INCLUDE_DIR} ${BIOCPP_CORE_DEPENDENCY_INCLUDE_DIRS})
set (CMAKE_REQUIRED_FLAGS       ${CMAKE_CXX_FLAGS})

# ----------------------------------------------------------------------------
# Require C++20
# ----------------------------------------------------------------------------

set (CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})

set (CXXSTD_TEST_SOURCE
    "#if !defined (__cplusplus) || (__cplusplus < 201709L)
    #error NOCXX20
    #endif
    int main() {}")

check_cxx_source_compiles ("${CXXSTD_TEST_SOURCE}" CXX20_BUILTIN)

if (CXX20_BUILTIN)
    biocpp_config_print ("C++ Standard-20 support:    builtin")
else ()
    set (CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS_SAVE} -std=c++20")

    check_cxx_source_compiles ("${CXXSTD_TEST_SOURCE}" CXX20_FLAG)

    if (CXX20_FLAG)
        biocpp_config_print ("C++ Standard-20 support:    via -std=c++20")
    else ()
        biocpp_config_print ("BioC++ requires C++20, but your compiler does not support it.")
    endif ()

    set (BIOCPP_CORE_CXX_FLAGS "${BIOCPP_CORE_CXX_FLAGS} -std=c++20")
endif ()

# ----------------------------------------------------------------------------
# thread support (pthread, windows threads)
# ----------------------------------------------------------------------------

set (THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads QUIET)

if (Threads_FOUND)
    set (BIOCPP_CORE_LIBRARIES ${BIOCPP_CORE_LIBRARIES} Threads::Threads)
    if ("${CMAKE_THREAD_LIBS_INIT}" STREQUAL "")
        biocpp_config_print ("Thread support:             builtin.")
    else ()
        biocpp_config_print ("Thread support:             via ${CMAKE_THREAD_LIBS_INIT}")
    endif ()
else ()
    biocpp_config_print ("Thread support:             not found.")
endif ()

# ----------------------------------------------------------------------------
# System dependencies
# ----------------------------------------------------------------------------

# librt
if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "kFreeBSD") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "GNU"))
    set (BIOCPP_CORE_LIBRARIES ${BIOCPP_CORE_LIBRARIES} rt)
endif ()

# libexecinfo -- implicit
check_include_file_cxx (execinfo.h _BIOCPP_CORE_HAVE_EXECINFO)
mark_as_advanced (_BIOCPP_CORE_HAVE_EXECINFO)
if (_BIOCPP_CORE_HAVE_EXECINFO)
    biocpp_config_print ("Optional dependency:        libexecinfo found.")
    if ((${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD") OR (${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD"))
        set (BIOCPP_CORE_LIBRARIES ${BIOCPP_CORE_LIBRARIES} execinfo elf)
    endif ()
else ()
    biocpp_config_print ("Optional dependency:        libexecinfo not found.")
endif ()

# ----------------------------------------------------------------------------
# Perform compilability test of core.hpp (tests some requirements)
# ----------------------------------------------------------------------------

set (CXXSTD_TEST_SOURCE
     "#include <bio/core.hpp>
     int main() {}")

# using try_compile instead of check_cxx_source_compiles to capture output in case of failure
file (WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx" "${CXXSTD_TEST_SOURCE}\n")

try_compile (BIOCPP_CORE_PLATFORM_TEST
             ${CMAKE_BINARY_DIR}
             ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx
             CMAKE_FLAGS         "-DCOMPILE_DEFINITIONS:STRING=${CMAKE_CXX_FLAGS} ${BIOCPP_CORE_CXX_FLAGS}"
                                 "-DINCLUDE_DIRECTORIES:STRING=${CMAKE_INCLUDE_PATH};${BIOCPP_CORE_INCLUDE_DIR};${BIOCPP_CORE_DEPENDENCY_INCLUDE_DIRS}"
             COMPILE_DEFINITIONS ${BIOCPP_CORE_DEFINITIONS}
             LINK_LIBRARIES      ${BIOCPP_CORE_LIBRARIES}
             OUTPUT_VARIABLE     BIOCPP_CORE_PLATFORM_TEST_OUTPUT)

if (BIOCPP_CORE_PLATFORM_TEST)
    biocpp_config_print ("Minimal build test:         passed.")
else ()
    biocpp_config_error ("Minimal build test:         failed!\n\
                        ${BIOCPP_CORE_PLATFORM_TEST_OUTPUT}")
endif ()

# ----------------------------------------------------------------------------
# Export targets
# ----------------------------------------------------------------------------

separate_arguments (BIOCPP_CORE_CXX_FLAGS_LIST UNIX_COMMAND "${BIOCPP_CORE_CXX_FLAGS}")

add_library (biocpp_core INTERFACE)
target_compile_definitions (biocpp_core INTERFACE ${BIOCPP_CORE_DEFINITIONS})
target_compile_options (biocpp_core INTERFACE ${BIOCPP_CORE_CXX_FLAGS_LIST})
target_link_libraries (biocpp_core INTERFACE "${BIOCPP_CORE_LIBRARIES}")
# include bio/include/ as -I, because BioC++ should never produce warnings.
target_include_directories (biocpp_core INTERFACE "${BIOCPP_CORE_INCLUDE_DIR}")
# include everything except bio/include/ as -isystem, i.e.
# a system header which suppresses warnings of external libraries.
target_include_directories (biocpp_core SYSTEM INTERFACE "${BIOCPP_CORE_DEPENDENCY_INCLUDE_DIRS}")
add_library (biocpp::core ALIAS biocpp_core)

# propagate BIOCPP_CORE_INCLUDE_DIR into BIOCPP_CORE_INCLUDE_DIRS
set (BIOCPP_CORE_INCLUDE_DIRS ${BIOCPP_CORE_INCLUDE_DIR} ${BIOCPP_CORE_DEPENDENCY_INCLUDE_DIRS})

# ----------------------------------------------------------------------------
# Finish find_package call
# ----------------------------------------------------------------------------

find_package_handle_standard_args (${CMAKE_FIND_PACKAGE_NAME} REQUIRED_VARS BIOCPP_CORE_INCLUDE_DIR)

# Set BIOCPP_CORE_* variables with the content of ${CMAKE_FIND_PACKAGE_NAME}_(FOUND|...|VERSION)
# This needs to be done, because `find_package(biocpp_core)` might be called in any case-sensitive way and we want to
# guarantee that BIOCPP_CORE_* are always set.
foreach (package_var FOUND DIR ROOT CONFIG VERSION VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_TWEAK VERSION_COUNT)
    set (BIOCPP_CORE_${package_var} "${${CMAKE_FIND_PACKAGE_NAME}_${package_var}}")
endforeach ()

set (CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_SAVE})

if (BIOCPP_CORE_FIND_DEBUG)
  message ("Result for ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
  message ("")
  message ("  CMAKE_BUILD_TYPE            ${CMAKE_BUILD_TYPE}")
  message ("  CMAKE_SOURCE_DIR            ${CMAKE_SOURCE_DIR}")
  message ("  CMAKE_INCLUDE_PATH          ${CMAKE_INCLUDE_PATH}")
  message ("  BIOCPP_CORE_INCLUDE_DIR          ${BIOCPP_CORE_INCLUDE_DIR}")
  message ("")
  message ("  ${CMAKE_FIND_PACKAGE_NAME}_FOUND                ${${CMAKE_FIND_PACKAGE_NAME}_FOUND}")
  message ("")
  message ("  BIOCPP_CORE_INCLUDE_DIRS         ${BIOCPP_CORE_INCLUDE_DIRS}")
  message ("  BIOCPP_CORE_LIBRARIES            ${BIOCPP_CORE_LIBRARIES}")
  message ("  BIOCPP_CORE_DEFINITIONS          ${BIOCPP_CORE_DEFINITIONS}")
  message ("  BIOCPP_CORE_CXX_FLAGS            ${BIOCPP_CORE_CXX_FLAGS}")
  message ("")
  message ("  BIOCPP_CORE_VERSION              ${BIOCPP_CORE_VERSION}")
  message ("  BIOCPP_CORE_VERSION_MAJOR        ${BIOCPP_CORE_VERSION_MAJOR}")
  message ("  BIOCPP_CORE_VERSION_MINORG       ${BIOCPP_CORE_VERSION_MINOR}")
  message ("  BIOCPP_CORE_VERSION_PATCH        ${BIOCPP_CORE_VERSION_PATCH}")
endif ()
