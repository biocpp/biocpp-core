# -----------------------------------------------------------------------------------------------------
# Copyright (c) 2022 deCODE Genetics
# Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
# Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
# This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
# shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
# -----------------------------------------------------------------------------------------------------
#
# This CMake module provides component-wise access to BioC++ libraries:
#
#   find_package (biocpp COMPONENT core io [...])
#
# ============================================================================

cmake_minimum_required (VERSION 3.12)

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
    message (STATUS "${ColourBold}The BioC++ libraries:${ColourReset}")
    if (NOT "core" IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
        message (STATUS "  The Core library was not requested but is assumed implicitly.")
        list(PREPEND ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS "core")
    endif ()
    message (STATUS "  Requested libraries: ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS}")
    message (STATUS "  Finding…")
endif ()

list(REMOVE_DUPLICATES ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)

set(QUIET_STRING "")
if (${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    set(QUIET_STRING "QUIET")
endif ()

set(REQUIRED_STRING "")
if (${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
    set(REQUIRED_STRING "REQUIRED")
endif ()

# ----------------------------------------------------------------------------
# Core library
# ----------------------------------------------------------------------------

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config-version.cmake" AND
    EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config.cmake")
    find_package(biocpp_core ${QUIET_STRING} ${REQUIRED_STRING} HINTS ${CMAKE_CURRENT_LIST_DIR})
else ()
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "Core library not found.")
    return()
endif ()

list(REMOVE_ITEM ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS "core")

# ----------------------------------------------------------------------------
# I/O library
# ----------------------------------------------------------------------------

# TODO make a loop out of this once we have more optional pkgs
set (_comp "io")
if (_comp IN_LIST ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
    set (COMP_PATH "")
    set (SIBLING_DIR "${CMAKE_CURRENT_LIST_DIR}/../../${CMAKE_FIND_PACKAGE_NAME}-${_comp}/build_system/")
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config.cmake" AND
        EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config-version.cmake")
        set (COMP_PATH "${CMAKE_CURRENT_LIST_DIR}/")
    elseif (EXISTS "${SIBLING_DIR}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config.cmake" AND
            EXISTS "${SIBLING_DIR}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config-version.cmake")
        set (COMP_PATH "${SIBLING_DIR}")
    endif ()

    if (COMP_PATH STREQUAL "")
        set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
        set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "ERROR: library '${_comp}' requested, but not found.")
        return()
    else ()
        find_package(biocpp_${_comp} ${QUIET_STRING} ${REQUIRED_STRING} HINTS ${COMP_PATH})
    endif ()
    list(REMOVE_ITEM ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS ${_comp})
endif ()

# ----------------------------------------------------------------------------
# Catch rest
# ----------------------------------------------------------------------------

if (${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "ERROR: the following requested library(s) are unknown: ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS} ")
    return()
endif ()

# ----------------------------------------------------------------------------
# end
# ----------------------------------------------------------------------------

if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message (STATUS "${ColourBold}All requested BioC++ libraries loaded.${ColourReset}")
endif ()
