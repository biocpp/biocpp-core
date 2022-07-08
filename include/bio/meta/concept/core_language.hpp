// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides concepts for core language types and relations that don't have concepts in C++20 (yet).
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <type_traits>

#include <bio/meta/platform.hpp>
#include <concepts>

namespace bio::meta
{

/*!\addtogroup concept
 * \{
 */

/*!\interface   bio::meta::weakly_equality_comparable_with <>
 * \tparam t1   The first type to compare.
 * \tparam t2   The second type to compare.
 * \brief       Requires the two operands to be comparable with `==` and `!=` in both directions.
 */
//!\cond
template <class T, class U>
concept weakly_equality_comparable_with = requires(std::remove_reference_t<T> const & t,
                                                   std::remove_reference_t<U> const & u)
{
    requires std::convertible_to<decltype(t == u), bool>;
    requires std::convertible_to<decltype(t != u), bool>;
    requires std::convertible_to<decltype(u == t), bool>;
    requires std::convertible_to<decltype(u != t), bool>;
};
//!\endcond

/*!\interface   bio::meta::weakly_ordered_with <>
 * \tparam t1   The first type to compare.
 * \tparam t2   The second type to compare.
 * \brief       Requires the two operands to be comparable with `<`, `<=`, `>` and `>=` in both directions.
 */
//!\cond
template <typename t1, typename t2>
concept weakly_ordered_with = requires(std::remove_reference_t<t1> const & v1, std::remove_reference_t<t2> const & v2)
{
    requires std::convertible_to<decltype(v1 < v2), bool>;
    requires std::convertible_to<decltype(v1 <= v2), bool>;
    requires std::convertible_to<decltype(v1 > v2), bool>;
    requires std::convertible_to<decltype(v1 >= v2), bool>;

    requires std::convertible_to<decltype(v2 < v1), bool>;
    requires std::convertible_to<decltype(v2 <= v1), bool>;
    requires std::convertible_to<decltype(v2 > v1), bool>;
    requires std::convertible_to<decltype(v2 >= v1), bool>;
};
//!\endcond

//TODO replace all occurrences of this with std::cosntructible_from
/*!\interface   bio::meta::implicitly_convertible_to <>
 * \brief       Resolves to `std::ranges::implicitly_convertible_to<type1, type2>()`.
 */
//!\cond
template <typename t, typename u>
concept implicitly_convertible_to = std::is_convertible_v<t, u>;
//!\endcond

//TODO replace all occurrences of this with std::convertible_to
/*!\interface   bio::meta::explicitly_convertible_to <>
 * \brief       Resolves to `std::ranges::explicitly_convertible_to<type1, type2>()`.
 */
//!\cond
template <typename t, typename u>
concept explicitly_convertible_to = requires(t vt)
{
    {static_cast<u>(vt)};
};
//!\endcond

/*!\interface   bio::meta::arithmetic <>
 * \brief       A type that satisfies std::is_arithmetic_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_arithmetic
 */
//!\cond
template <typename t>
concept arithmetic = std::is_arithmetic_v<t>;
//!\endcond

/*!\interface   bio::meta::floating_point <>
 * \extends     bio::meta::arithmetic
 * \brief       An arithmetic type that also satisfies std::is_floating_point_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_floating_point
 */
//!\cond
template <typename t>
concept floating_point = arithmetic<t> && std::is_floating_point_v<t>;
//!\endcond

/*!\interface   bio::meta::builtin_character <>
 * \extends     std::integral
 * \brief       This concept encompasses exactly the types `char`, `signed char`, `unsigned char`, `wchar_t`,
 *              `char16_t` and `char32_t`.
 */
//!\cond

template <typename t>
concept builtin_character = std::integral<t> &&
  (std::same_as<t, char> || std::same_as<t, unsigned char> || std::same_as<t, signed char> ||
#ifdef __cpp_char8_t
   std::same_as<t, char8_t> ||
#endif
   std::same_as<t, char16_t> || std::same_as<t, char32_t> || std::same_as<t, wchar_t>);
//!\endcond

/*!\interface   bio::meta::nonint_character <>
 * \extends     std::integral
 * \brief       This concept encompasses exactly the types `char`, `wchar_t`,
 *              `char16_t` and `char32_t`.
 */
//!\cond
template <typename type>
concept nonint_character = std::same_as<type, char> || std::same_as<type, char16_t> || std::same_as<type, char32_t> ||
  std::same_as<type, wchar_t>;
//!\endcond

/*!\interface   bio::meta::trivially_destructible <>
 * \extends     std::destructible
 * \brief       A type that satisfies std::is_trivially_destructible_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_destructible
 */
//!\cond
template <typename t>
concept trivially_destructible = std::destructible<t> && std::is_trivially_destructible_v<t>;
//!\endcond

/*!\interface   bio::meta::trivially_copyable
 * \brief       A type that satisfies std::is_trivially_copyable_v<t>.
 * \extends     std::copyable
 * \sa          https://en.cppreference.com/w/cpp/types/is_trivially_copyable
 */
//!\cond
template <typename t>
concept trivially_copyable = std::copyable<t> && std::is_trivially_copyable_v<t>;
//!\endcond

/*!\interface   bio::meta::trivial
 * \brief       A type that satisfies bio::meta::trivially_copyable and bio::meta::trivially_destructible.
 * \extends     bio::meta::trivially_copyable
 * \extends     bio::meta::trivially_destructible
 * \sa          https://en.cppreference.com/w/cpp/types/is_trivial
 */
//!\cond
template <typename t>
concept trivial = trivially_copyable<t> && trivially_destructible<t> && std::is_trivial_v<t>;
//!\endcond

/*!\interface   bio::meta::standard_layout
 * \brief       Resolves to std::is_standard_layout_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_standard_layout
 */
//!\cond
template <typename t>
concept standard_layout = std::is_standard_layout_v<t>;
//!\endcond

/*!\interface   bio::meta::weakly_assignable_from
 * \brief       Resolves to std::is_assignable_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_assignable
 *
 * \details
 *
 * Note that this requires less than std::assignable_from, it simply tests if the expression
 * `std::declval<T>() = std::declval<U>()` is well-formed.
 */
//!\cond
template <typename t, typename u>
concept weakly_assignable_from = std::is_assignable_v<t, u>;
//!\endcond

//!\}

} // namespace bio::meta
