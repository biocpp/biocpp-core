cmake_minimum_required (VERSION 3.10)

project(biocpp_clang_format NONE)

find_program(CLANG_FORMAT "clang-format-16")

if (CLANG_FORMAT STREQUAL "CLANG_FORMAT-NOTFOUND")
    find_program(CLANG_FORMAT "clang-format")
endif()

if (NOT CLANG_FORMAT STREQUAL "CLANG_FORMAT-NOTFOUND")
    add_custom_target (check_library ALL "find" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/bio/" "-name" "'*.[ch]pp'" "-exec"
                       ${CLANG_FORMAT} "-style=file" "-n" "-Werror" "{}" "+"
                       COMMENT "Checking the library with clang-format.")

    add_custom_target (check_unit_tests ALL "find"
                       "${CMAKE_CURRENT_SOURCE_DIR}/../unit/"
                       "${CMAKE_CURRENT_SOURCE_DIR}/../integration/"
                       "-name" "'*.[ch]pp'" "-exec" ${CLANG_FORMAT} "-style=file" "-n" "-Werror" "{}" "+"
                       COMMENT "Checking the unit tests with clang-format.")

    add_custom_target (format_library "find" "${CMAKE_CURRENT_SOURCE_DIR}/../../include/bio/" "-name" "'*.[ch]pp'" "-exec"
                       ${CLANG_FORMAT} "-style=file" "-i" "{}" "+"
                       COMMENT "Format the library with clang-format.")

    add_custom_target (format_unit_tests "find"
                       "${CMAKE_CURRENT_SOURCE_DIR}/../unit/"
                       "${CMAKE_CURRENT_SOURCE_DIR}/../integration/"
                       "-name" "'*.[ch]pp'" "-exec" ${CLANG_FORMAT} "-style=file" "-i" "{}" "+"
                       COMMENT "Format the unit tests with clang-format.")
else ()
    message(FATAL_ERROR "clang-format not found. No targets made.")
endif()
