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

# ----------------------------------------------------------------------------
# Core library
# ----------------------------------------------------------------------------

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config-version.cmake" AND
    EXISTS "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config-version.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/${CMAKE_FIND_PACKAGE_NAME}_core-config.cmake")
    if (NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
        message (STATUS "${ColourBold}Core library loaded.${ColourReset}")
    endif ()
else ()
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "Core library not found.")
    return()
endif ()

list(REMOVE_ITEM ${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS "core")

# ----------------------------------------------------------------------------
# I/O library
# ----------------------------------------------------------------------------

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
        include("${COMP_PATH}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config-version.cmake")
        include("${COMP_PATH}/${CMAKE_FIND_PACKAGE_NAME}_${_comp}-config.cmake")
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
