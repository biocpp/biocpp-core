# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/master/LICENSE.md
# -----------------------------------------------------------------------------------------------------
#
# This CMake module will try to find BioC++ and its dependencies.  You can use
# it the same way you would use any other CMake module.
#
#   find_package (BioCpp [REQUIRED] ...)
#
# Since this makes a difference for CMAKE, pay attention to the case
# ("BioCpp", "BIOCPP" and "biocpp" are all valid, but other names not).
#
# BioC++ has the following platform requirements:
#
#   C++20
#
# BioC++ has the following optional dependencies:
#
#   Cereal    -- Serialisation library
#
# If you don't wish for these to be detected (and used), you may define BIOCPP_NO_ZLIB,
# BIOCPP_NO_BZIP2, BIOCPP_NO_CEREAL and BIOCPP_NO_LEMON respectively.
#
# If you wish to require the presence of ZLIB or BZip2, just check for the module before
# finding BioC++, e.g. "find_package (ZLIB REQUIRED)".
# If you wish to require the presence of CEREAL, you may define BIOCPP_CEREAL.
# If you wish to require the presence of LEMON, you may define BIOCPP_LEMON.
#
# Once the search has been performed, the following variables will be set.
#
#   BIOCPP_FOUND            -- Indicate whether BioC++ was found and requirements met.
#
#   BIOCPP_VERSION          -- The version as string, e.g. "3.0.0"
#   BIOCPP_VERSION_MAJOR    -- e.g. 3
#   BIOCPP_VERSION_MINOR    -- e.g. 0
#   BIOCPP_VERSION_PATCH    -- e.g. 0
#
#   BIOCPP_INCLUDE_DIRS     -- to be passed to include_directories ()
#   BIOCPP_LIBRARIES        -- to be passed to target_link_libraries ()
#   BIOCPP_DEFINITIONS      -- to be passed to add_definitions ()
#   BIOCPP_CXX_FLAGS        -- to be added to CMAKE_CXX_FLAGS
#
# Additionally, the following [IMPORTED][IMPORTED] targets are defined:
#
#   biocpp::core          -- interface target where
#                                  target_link_libraries(target biocpp::core)
#                              automatically sets
#                                  target_include_directories(target $BIOCPP_INCLUDE_DIRS),
#                                  target_link_libraries(target $BIOCPP_LIBRARIES),
#                                  target_compile_definitions(target $BIOCPP_DEFINITIONS) and
#                                  target_compile_options(target $BIOCPP_CXX_FLAGS)
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
    message (STATUS "${ColourBold}Finding BioC++ and checking requirements:${ColourReset}")
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

# Note that biocpp-config.cmake can be standalone and thus BIOCPP_CLONE_DIR might be empty.
# * `BIOCPP_CLONE_DIR` was already found in biocpp-config-version.cmake
# * `BIOCPP_INCLUDE_DIR` was already found in biocpp-config-version.cmake
find_path (BIOCPP_SUBMODULES_DIR NAMES submodules/sdsl-lite HINTS "${BIOCPP_CLONE_DIR}" "${BIOCPP_INCLUDE_DIR}/bio")

if (BIOCPP_INCLUDE_DIR)
    biocpp_config_print ("BioC++ include dir found:   ${BIOCPP_INCLUDE_DIR}")
else ()
    biocpp_config_error ("BioC++ include directory could not be found (BIOCPP_INCLUDE_DIR: '${BIOCPP_INCLUDE_DIR}')")
endif ()

# ----------------------------------------------------------------------------
# Options for CheckCXXSourceCompiles
# ----------------------------------------------------------------------------

# deactivate messages in check_*
set (CMAKE_REQUIRED_QUIET       1)
# use global variables in Check* calls
set (CMAKE_REQUIRED_INCLUDES    ${CMAKE_INCLUDE_PATH} ${BIOCPP_INCLUDE_DIR} ${BIOCPP_DEPENDENCY_INCLUDE_DIRS})
set (CMAKE_REQUIRED_FLAGS       ${CMAKE_CXX_FLAGS})

# ----------------------------------------------------------------------------
# Force-deactivate optional dependencies
# ----------------------------------------------------------------------------

# Cereal is auto-detected by default, i.e. used if found, not used if not found.
# You can optionally set a hard requirement so a build fails without cereal,
# or you can force-disable cereal even if present on the system.
option (BIOCPP_CEREAL    "Require cereal and fail if not present." OFF)
option (BIOCPP_NO_CEREAL "Don't use cereal, even if present." OFF)

if (BIOCPP_CEREAL AND BIOCPP_NO_CEREAL)
    # this is always a user error, therefore we always error-out, even if BioC++ is not required
    message (FATAL_ERROR "You may not specify BIOCPP_CEREAL and BIOCPP_NO_CEREAL at the same time.\n\
                          You can specify neither (use auto-detection), or specify either to force on/off.")
    return ()
endif ()

if (BIOCPP_CEREAL)
    set (BIOCPP_DEFINITIONS ${BIOCPP_DEFINITIONS} "-DBIOCPP_WITH_CEREAL=1")
elseif (BIOCPP_NO_CEREAL)
    set (BIOCPP_DEFINITIONS ${BIOCPP_DEFINITIONS} "-DBIOCPP_WITH_CEREAL=0")
endif ()

# These two are "opt-in", because detected by CMake
# If you want to force-require these, just do find_package (zlib REQUIRED) before find_package (seqan3)
option (BIOCPP_NO_ZLIB  "Don't use ZLIB, even if present." OFF)
option (BIOCPP_NO_BZIP2 "Don't use BZip2, even if present." OFF)

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

    set (BIOCPP_CXX_FLAGS "${BIOCPP_CXX_FLAGS} -std=c++20")
endif ()

# ----------------------------------------------------------------------------
# thread support (pthread, windows threads)
# ----------------------------------------------------------------------------

set (THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads QUIET)

if (Threads_FOUND)
    set (BIOCPP_LIBRARIES ${BIOCPP_LIBRARIES} Threads::Threads)
    if ("${CMAKE_THREAD_LIBS_INIT}" STREQUAL "")
        biocpp_config_print ("Thread support:             builtin.")
    else ()
        biocpp_config_print ("Thread support:             via ${CMAKE_THREAD_LIBS_INIT}")
    endif ()
else ()
    biocpp_config_print ("Thread support:             not found.")
endif ()

# ----------------------------------------------------------------------------
# Cereal dependency is optional, but may set as required
# ----------------------------------------------------------------------------

if (NOT BIOCPP_NO_CEREAL)
    check_include_file_cxx (cereal/cereal.hpp _BIOCPP_HAVE_CEREAL)

    if (_BIOCPP_HAVE_CEREAL)
        if (BIOCPP_CEREAL)
            biocpp_config_print ("Required dependency:        Cereal found.")
        else ()
            biocpp_config_print ("Optional dependency:        Cereal found.")
        endif ()
    else ()
        if (BIOCPP_CEREAL)
            biocpp_config_error ("The (optional) cereal library was marked as required, but wasn't found.")
        else ()
            biocpp_config_print ("Optional dependency:        Cereal not found.")
        endif ()
    endif ()
endif ()

# ----------------------------------------------------------------------------
# System dependencies
# ----------------------------------------------------------------------------

# librt
if ((${CMAKE_SYSTEM_NAME} STREQUAL "Linux") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "kFreeBSD") OR
    (${CMAKE_SYSTEM_NAME} STREQUAL "GNU"))
    set (BIOCPP_LIBRARIES ${BIOCPP_LIBRARIES} rt)
endif ()

# libexecinfo -- implicit
check_include_file_cxx (execinfo.h _BIOCPP_HAVE_EXECINFO)
mark_as_advanced (_BIOCPP_HAVE_EXECINFO)
if (_BIOCPP_HAVE_EXECINFO)
    biocpp_config_print ("Optional dependency:        libexecinfo found.")
    if ((${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD") OR (${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD"))
        set (BIOCPP_LIBRARIES ${BIOCPP_LIBRARIES} execinfo elf)
    endif ()
else ()
    biocpp_config_print ("Optional dependency:        libexecinfo not found.")
endif ()

# ----------------------------------------------------------------------------
# Perform compilability test of platform.hpp (tests some requirements)
# ----------------------------------------------------------------------------

set (CXXSTD_TEST_SOURCE
     "#include <bio/meta/platform.hpp>
     int main() {}")

# using try_compile instead of check_cxx_source_compiles to capture output in case of failure
file (WRITE "${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx" "${CXXSTD_TEST_SOURCE}\n")

try_compile (BIOCPP_PLATFORM_TEST
             ${CMAKE_BINARY_DIR}
             ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/src.cxx
             CMAKE_FLAGS         "-DCOMPILE_DEFINITIONS:STRING=${CMAKE_CXX_FLAGS} ${BIOCPP_CXX_FLAGS}"
                                 "-DINCLUDE_DIRECTORIES:STRING=${CMAKE_INCLUDE_PATH};${BIOCPP_INCLUDE_DIR};${BIOCPP_DEPENDENCY_INCLUDE_DIRS}"
             COMPILE_DEFINITIONS ${BIOCPP_DEFINITIONS}
             LINK_LIBRARIES      ${BIOCPP_LIBRARIES}
             OUTPUT_VARIABLE     BIOCPP_PLATFORM_TEST_OUTPUT)

if (BIOCPP_PLATFORM_TEST)
    biocpp_config_print ("BioC++ platform.hpp build:  passed.")
else ()
    biocpp_config_error ("BioC++ platform.hpp build:  failed!\n\
                        ${BIOCPP_PLATFORM_TEST_OUTPUT}")
endif ()

# ----------------------------------------------------------------------------
# Export targets
# ----------------------------------------------------------------------------

separate_arguments (BIOCPP_CXX_FLAGS_LIST UNIX_COMMAND "${BIOCPP_CXX_FLAGS}")

add_library (biocpp_core INTERFACE)
target_compile_definitions (biocpp_core INTERFACE ${BIOCPP_DEFINITIONS})
target_compile_options (biocpp_core INTERFACE ${BIOCPP_CXX_FLAGS_LIST})
target_link_libraries (biocpp_core INTERFACE "${BIOCPP_LIBRARIES}")
# include bio/include/ as -I, because BioC++ should never produce warnings.
target_include_directories (biocpp_core INTERFACE "${BIOCPP_INCLUDE_DIR}")
# include everything except bio/include/ as -isystem, i.e.
# a system header which suppresses warnings of external libraries.
target_include_directories (biocpp_core SYSTEM INTERFACE "${BIOCPP_DEPENDENCY_INCLUDE_DIRS}")
add_library (biocpp::core ALIAS biocpp_core)

# propagate BIOCPP_INCLUDE_DIR into BIOCPP_INCLUDE_DIRS
set (BIOCPP_INCLUDE_DIRS ${BIOCPP_INCLUDE_DIR} ${BIOCPP_DEPENDENCY_INCLUDE_DIRS})

# ----------------------------------------------------------------------------
# Finish find_package call
# ----------------------------------------------------------------------------

find_package_handle_standard_args (${CMAKE_FIND_PACKAGE_NAME} REQUIRED_VARS BIOCPP_INCLUDE_DIR)

# Set BIOCPP_* variables with the content of ${CMAKE_FIND_PACKAGE_NAME}_(FOUND|...|VERSION)
# This needs to be done, because `find_package(BioC++)` might be called in any case-sensitive way and we want to
# guarantee that BIOCPP_* are always set.
foreach (package_var FOUND DIR ROOT CONFIG VERSION VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_TWEAK VERSION_COUNT)
    set (BIOCPP_${package_var} "${${CMAKE_FIND_PACKAGE_NAME}_${package_var}}")
endforeach ()

set (CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_SAVE})

if (BIOCPP_FIND_DEBUG)
  message ("Result for ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt")
  message ("")
  message ("  CMAKE_BUILD_TYPE            ${CMAKE_BUILD_TYPE}")
  message ("  CMAKE_SOURCE_DIR            ${CMAKE_SOURCE_DIR}")
  message ("  CMAKE_INCLUDE_PATH          ${CMAKE_INCLUDE_PATH}")
  message ("  BIOCPP_INCLUDE_DIR          ${BIOCPP_INCLUDE_DIR}")
  message ("")
  message ("  ${CMAKE_FIND_PACKAGE_NAME}_FOUND                ${${CMAKE_FIND_PACKAGE_NAME}_FOUND}")
  message ("  BIOCPP_HAS_ZLIB             ${ZLIB_FOUND}")
  message ("  BIOCPP_HAS_BZIP2            ${BZIP2_FOUND}")
  message ("")
  message ("  BIOCPP_INCLUDE_DIRS         ${BIOCPP_INCLUDE_DIRS}")
  message ("  BIOCPP_LIBRARIES            ${BIOCPP_LIBRARIES}")
  message ("  BIOCPP_DEFINITIONS          ${BIOCPP_DEFINITIONS}")
  message ("  BIOCPP_CXX_FLAGS            ${BIOCPP_CXX_FLAGS}")
  message ("")
  message ("  BIOCPP_VERSION              ${BIOCPP_VERSION}")
  message ("  BIOCPP_VERSION_MAJOR        ${BIOCPP_VERSION_MAJOR}")
  message ("  BIOCPP_VERSION_MINORG       ${BIOCPP_VERSION_MINOR}")
  message ("  BIOCPP_VERSION_PATCH        ${BIOCPP_VERSION_PATCH}")
endif ()
