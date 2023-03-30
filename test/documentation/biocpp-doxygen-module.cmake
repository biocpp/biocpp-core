cmake_minimum_required (VERSION 3.7)

LIST(APPEND BIOCPP_DOXYGEN_MODULE_LAYOUT ${BIOCPP_CORE_CLONE_DIR}/test/documentation/DoxygenLayout.xml.module)

set (BIOCPP_DOXYGEN_INCLUDE_PATH "${BIOCPP_DOXYGEN_INCLUDE_PATH} ${BIOCPP_CORE_INCLUDE_DIR}")

set (BIOCPP_DOXYGEN_INPUT "${BIOCPP_CORE_CLONE_DIR}/include               \
                           ${BIOCPP_CORE_CLONE_DIR}/doc                   \
                           ${BIOCPP_CORE_CLONE_DIR}/LICENSE.md            \
                           ${BIOCPP_CORE_CLONE_DIR}/CHANGELOG.md          \
                           ${BIOCPP_CORE_CLONE_DIR}/CODE_OF_CONDUCT.md    \
                           ${BIOCPP_CORE_CLONE_DIR}/CONTRIBUTING.md")

set (BIOCPP_DOXYGEN_EXAMPLE_PATH "${BIOCPP_DOXYGEN_EXAMPLE_PATH} ${BIOCPP_CORE_CLONE_DIR}")

set(BIOCPP_VERSION "${BIOCPP_VERSION} core-${BIOCPP_CORE_VERSION}")
