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

// Self [required]
#if !__has_include(<bio/version.hpp>)
#    error BioC++ include directory not set correctly. Forgot to add -I ${INSTALLDIR}/include to your CXXFLAGS?
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
