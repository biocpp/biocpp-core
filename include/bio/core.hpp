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
#if !__has_include(<bio/core.hpp>)
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
//  Compiler Workarounds
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
//  Define our version
// ============================================================================

//!\brief The major version as MACRO.
#define BIOCPP_CORE_VERSION_MAJOR 0
//!\brief The minor version as MACRO.
#define BIOCPP_CORE_VERSION_MINOR 6
//!\brief The patch version as MACRO.
#define BIOCPP_CORE_VERSION_PATCH 0

//!\brief The full version as MACRO (number).
#define BIOCPP_CORE_VERSION                                                                                            \
    (BIOCPP_CORE_VERSION_MAJOR * 10000 + BIOCPP_CORE_VERSION_MINOR * 100 + BIOCPP_CORE_VERSION_PATCH)

#include <string>

/*!\namespace bio
 * \brief The main BioC++ namespace.
 */
namespace bio
{

//!\brief The major version.
inline constexpr size_t biocpp_core_version_major = BIOCPP_CORE_VERSION_MAJOR;
//!\brief The minor version.
inline constexpr size_t biocpp_core_version_minor = BIOCPP_CORE_VERSION_MINOR;
//!\brief The patch version.
inline constexpr size_t biocpp_core_version_patch = BIOCPP_CORE_VERSION_PATCH;

//!\brief The full version as `std::string`.
inline std::string const biocpp_core_version = std::to_string(biocpp_core_version_major) + "." +
                                               std::to_string(biocpp_core_version_minor) + "." +
                                               std::to_string(biocpp_core_version_patch);

} // namespace bio

// ============================================================================
//  Macros for general use
// ============================================================================

/*!\brief A macro that behaves like std::is_same_v, except that it doesn't need to instantiate the template on GCC and
 *        Clang.
 * \ingroup type_traits
 */
#if defined(__clang__)
#    define BIOCPP_IS_SAME(...) __is_same(__VA_ARGS__)
#elif defined(__GNUC__)
#    define BIOCPP_IS_SAME(...) __is_same_as(__VA_ARGS__)
#else
#    define BIOCPP_IS_SAME(...) std::is_same_v<__VA_ARGS__>
#endif

/*!\brief Returns true if the expression passed to this macro can be evaluated at compile time, false otherwise.
 * \ingroup type_traits
 * \returns true or false.
 */
#ifdef __clang__ // this doesn't work and is currently just a hack for clang-based tooling
#    define BIOCPP_IS_CONSTEXPR(...) true
#else
#    define BIOCPP_IS_CONSTEXPR(...) std::integral_constant<bool, __builtin_constant_p((__VA_ARGS__, 0))>::value
#endif

// ============================================================================
//  Alphabet namespaces
// ============================================================================

/*!\namespace bio::alphabet
 * \brief The alphabet module's namespace.
 * \ingroup alphabet
 * \see \ref biocpp_customisation
 */

namespace bio::alphabet
{}

/*!\namespace bio::alphabet::cpo
 * \brief A namespace for third party and standard library specialisations of BioC++ customisation points.
 * \ingroup alphabet
 * \see \ref biocpp_customisation
 */

namespace bio::alphabet::cpo
{}

/*!\namespace bio::alphabet::literals
 * \brief An inline namespace for alphabet literals. It exists to safely allow `using namespace`.
 * \ingroup alphabet
 */

namespace bio::alphabet
{
inline namespace literals
{}
} // namespace bio::alphabet

/*!\if DEV
 * \namespace bio::alphabet::detail
 * \brief The internal BioC++ namespace.
 * \ingroup alphabet
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */

namespace bio::alphabet::detail
{}

// ============================================================================
//  Meta namespaces
// ============================================================================

/*!\defgroup meta Meta
 * \brief Provides metaprogramming utilities, concepts and some helper classes.
 *
 * The meta module is used strongly by other modules, but the content is usually not relevant
 * to most users of the library.
 */

/*!\namespace bio::meta
 * \brief The Meta module's namespace.
 * \ingroup meta
 */

namespace bio::meta
{}

/*!\namespace bio::meta::literals
 * \brief An inline namespace for meta literals. It exists to safely allow `using namespace`.
 * \ingroup meta
 */

namespace bio::meta
{
inline namespace literals
{}
} // namespace bio::meta

/*!\if DEV
 * \namespace bio::meta::detail
 * \ingroup meta
 * \brief The internal BioC++ namespace.
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */
namespace bio::meta::detail
{}

// ============================================================================
//  Ranges namespaces
// ============================================================================

/*!\namespace bio::ranges
 * \brief The ranges module's namespace.
 * \ingroup range
 */
namespace bio::ranges
{}

/*!
 * \namespace bio::ranges::views
 * \brief The BioC++ namespace for views.
 * \ingroup views
 *
 * Since views often have name clashes with regular functions and ranges they are implemented in the sub
 * namespace `view`.
 *
 * See the \link views views submodule \endlink of the range module for more details.
 *
 * The namespace is aliased into `bio::` so you can write `bio::views::foo` instead of
 * `bio::ranges::views::foo`.
 */

namespace bio::ranges::views
{}

#ifndef BIOCPP_NO_NAMESPACE_ALIASES
namespace bio
{
namespace views = ranges::views;
} // namespace bio
#endif

/*!\if DEV
 * \namespace bio::ranges::detail
 * \brief The internal BioC++ namespace.
 * \ingroup alphabet
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */
namespace bio::ranges::detail
{}
