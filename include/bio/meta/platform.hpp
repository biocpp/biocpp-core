// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#pragma once

#include <cinttypes>
#include <ciso646> // makes _LIBCPP_VERSION available
#include <cstddef> // makes __GLIBCXX__ available

/*!\file
 * \brief Provides platform and dependency checks.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

// ============================================================================
//  C++ standard and features
// ============================================================================

// C++ standard [required]
#ifdef __cplusplus
static_assert(__cplusplus >= 201709, "BioCpp-Core requires C++20, make sure that you have set -std=c++20.");
#else
#    error "This is not a C++ compiler."
#endif

#if __has_include(<version>)
#    include <version>
#endif

// ============================================================================
//  Dependencies
// ============================================================================

// BioC++ [required]
#if !__has_include(<bio/version.hpp>)
#    error BioC++ include directory not set correctly. Forgot to add -I ${INSTALLDIR}/include to your CXXFLAGS?
#endif

// SDSL [required]
#if __has_include(<sdsl/version.hpp>)
#    include <sdsl/version.hpp>
static_assert(sdsl::sdsl_version_major == 3, "Only version 3 of the SDSL is supported by BioC++.");
#else
#    error The sdsl library was not included correctly. Forgot to add -I ${INSTALLDIR}/include to your CXXFLAGS?
#endif

// Cereal [optional]
/*!\def BIOCPP_WITH_CEREAL
 * \brief Whether CEREAL support is available or not.
 * \ingroup meta
 */
#ifndef BIOCPP_WITH_CEREAL
#    if __has_include(<cereal/cereal.hpp>)
#        define BIOCPP_WITH_CEREAL 1
#    else
#        define BIOCPP_WITH_CEREAL 0
#    endif
#elif BIOCPP_WITH_CEREAL != 0
#    if !__has_include(<cereal/cereal.hpp>)
#        error Cereal was marked as required, but not found!
#    endif
#endif

#if !BIOCPP_WITH_CEREAL
/*!\cond DEV
     * \name Cereal function macros
     * \ingroup meta
     * \brief These can be changed by apps so we used the macros instead of the values internally.
     * \{
     */

//! \brief Macro for Cereal's serialize function.
#    define CEREAL_SERIALIZE_FUNCTION_NAME    serialize
//! \brief Macro for Cereal's load function.
#    define CEREAL_LOAD_FUNCTION_NAME         load
//! \brief Macro for Cereal's save function.
#    define CEREAL_SAVE_FUNCTION_NAME         save
//! \brief Macro for Cereal's load_minimal function.
#    define CEREAL_LOAD_MINIMAL_FUNCTION_NAME load_minimal
//! \brief Macro for Cereal's save_minimal function.
#    define CEREAL_SAVE_MINIMAL_FUNCTION_NAME save_minimal
/*!\}
     * \endcond
     */
#endif

// ============================================================================
//  Documentation
// ============================================================================

// Doxygen related
// this macro is a NO-OP unless doxygen parses it, in which case it resolves to the argument
#ifndef BIOCPP_DOXYGEN_ONLY
#    define BIOCPP_DOXYGEN_ONLY(x)
#endif

// ============================================================================
//  Workarounds
// ============================================================================

/*!\brief Warn about gcc 10.0 and gcc 10.1
 * Known Issues:
 * * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93983
 * * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95371
 * * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95578
 */
#if defined(__GNUC__) && (__GNUC__ == 10 && __GNUC_MINOR__ <= 1)
#    pragma GCC warning                                                                                                \
      "Be aware that gcc 10.0 and 10.1 are known to have several bugs that might make BioC++ fail to compile. Please use gcc >= 10.2."
#endif // defined(__GNUC__) && (__GNUC__ == 10 && __GNUC_MINOR__ <= 1)

//!\brief See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=96070 and https://github.com/seqan/product_backlog/issues/151
#ifndef BIOCPP_WORKAROUND_GCC_96070 // fixed since gcc10.4
#    if defined(__GNUC__) && (__GNUC__ == 10 && __GNUC_MINOR__ < 4)
#        define BIOCPP_WORKAROUND_GCC_96070 1
#    else
#        define BIOCPP_WORKAROUND_GCC_96070 0
#    endif
#endif

#if BIOCPP_DOXYGEN_ONLY(1) 0
//!\brief This disables the warning you would get if -D_GLIBCXX_USE_CXX11_ABI=0 is set.
#    define BIOCPP_DISABLE_LEGACY_STD_DIAGNOSTIC
#endif // BIOCPP_DOXYGEN_ONLY(1)0

#if defined(_GLIBCXX_USE_CXX11_ABI) && _GLIBCXX_USE_CXX11_ABI == 0
#    ifndef BIOCPP_DISABLE_LEGACY_STD_DIAGNOSTIC
#        pragma GCC warning                                                                                            \
          "Your compiler has set -D_GLIBCXX_USE_CXX11_ABI=0. Please switch to a compiler that does not. You can disable this warning by setting -DBIOCPP_DISABLE_LEGACY_STD_DIAGNOSTIC."
#    endif // BIOCPP_DISABLE_LEGACY_STD_DIAGNOSTIC
#endif     // _GLIBCXX_USE_CXX11_ABI == 0

// ============================================================================
//  Backmatter
// ============================================================================
