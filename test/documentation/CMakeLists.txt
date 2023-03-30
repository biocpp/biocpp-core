# Minimum cmake version
cmake_minimum_required(VERSION 3.7)

include (../../build_system/biocpp_core-config-version.cmake)

project (biocpp_core_doc LANGUAGES NONE VERSION "${BIOCPP_CORE_VERSION}")

if (NOT IS_DIRECTORY "${BIOCPP_CORE_CLONE_DIR}/doc")
    message (FATAL_ERROR "Could not find BioC++ Core. Not building documentation.")
endif ()
message (STATUS "BioC++ Core library: ${BIOCPP_CORE_CLONE_DIR} (${BIOCPP_CORE_VERSION})")

option(BIOCPP_DOXY_CORE "Build documentation for core module." ON)

if (BIOCPP_DOXY_CORE)
    include (biocpp-doxygen-module.cmake)
endif()

include (biocpp-doxygen.cmake)
