// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides various type traits on generic types.
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 */

#pragma once

#include <tuple>
#include <type_traits>
#include <type_traits>

#include <bio/meta/platform.hpp>

// ----------------------------------------------------------------------------
// is_constexpr
// ----------------------------------------------------------------------------

/*!\brief Returns true if the expression passed to this macro can be evaluated at compile time, false otherwise.
 * \ingroup type_traits
 * \returns true or false.
 */
#define BIOCPP_IS_CONSTEXPR(...) std::integral_constant<bool, __builtin_constant_p((__VA_ARGS__, 0))>::value

namespace bio
{

/*!\addtogroup type_traits
 * \{
 */

// ----------------------------------------------------------------------------
// remove_cvref_t
// ----------------------------------------------------------------------------

/*!\brief Return the input type with `const`, `volatile` and references removed (type trait).
 * \tparam t The type to operate on.
 */
template <typename t>
using remove_cvref_t BIOCPP_DEPRECATED_310 = std::remove_cv_t<std::remove_reference_t<t>>;

// ----------------------------------------------------------------------------
// remove_rvalue_reference
// ----------------------------------------------------------------------------

/*!\brief Return the input type with `&&` removed, but lvalue references preserved.
 * \implements bio::transformation_trait
 * \tparam t The type to operate on.
 * \see bio::remove_rvalue_reference_t
 */
template <typename t>
struct remove_rvalue_reference
{
    //!\brief The return type is the input type with any `&&` stripped.
    using type = std::conditional_t<std::is_rvalue_reference_v<t>, std::remove_reference_t<t>, t>;
};

/*!\brief Return the input type with `&&` removed, but lvalue references preserved (transformation_trait shortcut).
 * \tparam t The type to operate on.
 * \see bio::remove_rvalue_reference
 */
template <typename t>
using remove_rvalue_reference_t = typename remove_rvalue_reference<t>::type;

// ----------------------------------------------------------------------------
// is_constexpr_default_constructible
// ----------------------------------------------------------------------------

/*!\brief Whether a type std::is_default_constructible in `constexpr`-context.
 * \implements bio::unary_type_trait
 * \tparam t The type to operate on.
 */
template <typename t>
struct is_constexpr_default_constructible : std::false_type
{};

/*!\brief Whether a type std::is_default_constructible in `constexpr`-context (unary_type_trait specialisation).
 * \tparam t A type that std::is_default_constructible.
 * \see bio::is_constexpr_default_constructible
 */
template <typename t>
//!\cond
    requires std::is_default_constructible_v<t>
//!\endcond
struct is_constexpr_default_constructible<t> : std::integral_constant<bool, BIOCPP_IS_CONSTEXPR(t{})>
{};

/*!\brief Whether a type std::is_default_constructible in `constexpr`-context (unary_type_trait shortcut).
 * \tparam t The type to operate on.
 * \relates bio::is_constexpr_default_constructible
 */
template <typename t>
inline constexpr bool is_constexpr_default_constructible_v = is_constexpr_default_constructible<t>::value;

//!\}
} // namespace bio

namespace bio::detail
{

/*!\addtogroup type_traits
 * \{
 */

// ----------------------------------------------------------------------------
// deferred_type
// ----------------------------------------------------------------------------

/*!\brief Return the type identity; further arguments are ignored, but can make this type dependent if they are.
 * \implements bio::transformation_trait
 * \tparam t The type to operate on.
 * \tparam dependent_ts Any provided types are ignored.
 * \see bio::detail::deferred_type_t
 */
template <typename t, typename ...dependent_ts>
struct deferred_type
{
    //!\brief The type identity.
    using type = t;
};

/*!\brief Return the type identity; further arguments are ignored, but can make this type dependent if they are
 *        (transformation_trait shortcut).
 * \tparam t The type to operate on.
 * \tparam dependent_ts Any provided types are ignored.
 * \see bio::detail::deferred_type
 */
template <typename t, typename ...dependent_ts>
using deferred_type_t = typename deferred_type<t, dependent_ts...>::type;

// ----------------------------------------------------------------------------
// ignore_t
// ----------------------------------------------------------------------------

//!\brief Return the type of std::ignore with `const`, `volatile` and references removed (type trait).
using ignore_t = std::remove_cvref_t<decltype(std::ignore)>;

/*!\brief Return whether the input type with `const`, `volatile` and references removed is std::ignore's type.
 * (type trait).
 * \tparam t The type to operate on.
 */
template <typename t>
constexpr bool decays_to_ignore_v = std::is_same_v<std::remove_cvref_t<t>, ignore_t>;

//!\}

} // namespace bio::detail

// ----------------------------------------------------------------------------
// BIOCPP_IS_SAME
// ----------------------------------------------------------------------------

/*!\brief A macro that behaves like std::is_same_v, except that it doesn't need to instantiate the template on GCC and
 *        Clang.
 * \ingroup type_traits
 */
#if defined(__clang__)
#   define BIOCPP_IS_SAME(...)              __is_same(__VA_ARGS__)
#elif defined(__GNUC__)
#   define BIOCPP_IS_SAME(...)              __is_same_as(__VA_ARGS__)
#else
#   define BIOCPP_IS_SAME(...)              std::is_same_v<__VA_ARGS__>
#endif
