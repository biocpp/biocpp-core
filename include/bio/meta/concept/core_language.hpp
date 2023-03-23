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

#include <concepts>
#include <type_traits>

#include <bio/core.hpp>

namespace bio::meta
{

/*!\addtogroup concept
 * \{
 */

/*!\tparam t1   The first type to compare.
 * \tparam t2   The second type to compare.
 * \brief       Requires the two operands to be comparable with `==` and `!=` in both directions.
 */
template <class T, class U>
concept weakly_equality_comparable_with =
  requires(std::remove_reference_t<T> const & t, std::remove_reference_t<U> const & u) {
      requires std::convertible_to<decltype(t == u), bool>;
      requires std::convertible_to<decltype(t != u), bool>;
      requires std::convertible_to<decltype(u == t), bool>;
      requires std::convertible_to<decltype(u != t), bool>;
  };

/*!\tparam t1   The first type to compare.
 * \tparam t2   The second type to compare.
 * \brief       Requires the two operands to be comparable with `<`, `<=`, `>` and `>=` in both directions.
 */
template <typename t1, typename t2>
concept weakly_ordered_with = requires(std::remove_reference_t<t1> const & v1, std::remove_reference_t<t2> const & v2) {
    requires std::convertible_to<decltype(v1 < v2), bool>;
    requires std::convertible_to<decltype(v1 <= v2), bool>;
    requires std::convertible_to<decltype(v1 > v2), bool>;
    requires std::convertible_to<decltype(v1 >= v2), bool>;

    requires std::convertible_to<decltype(v2 < v1), bool>;
    requires std::convertible_to<decltype(v2 <= v1), bool>;
    requires std::convertible_to<decltype(v2 > v1), bool>;
    requires std::convertible_to<decltype(v2 >= v1), bool>;
};

/*!\brief       A type that satisfies std::is_arithmetic_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_arithmetic
 */
template <typename t>
concept arithmetic = std::is_arithmetic_v<t>;

/*!\extends     bio::meta::arithmetic
 * \brief       An arithmetic type that also satisfies std::is_floating_point_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_floating_point
 */
template <typename t>
concept floating_point = arithmetic<t> && std::is_floating_point_v<t>;

/*!\brief       Resolves to (std::same_as<query_t, other_types> || ...).
 */
template <typename query_t, typename... other_types>
concept one_of = (BIOCPP_IS_SAME(query_t, other_types) || ...);

/*!\extends     std::integral
 * \brief       This concept encompasses exactly the types `char`, `signed char`, `unsigned char`, `wchar_t`,
 *              `char16_t` and `char32_t`.
 */
template <typename t>
concept builtin_character =
  std::integral<t> && one_of<t, char, unsigned char, signed char, char8_t, char16_t, char32_t, wchar_t>;

/*!\extends     std::integral
 * \brief       This concept encompasses exactly the types `char`, `wchar_t`,
 *              `char16_t` and `char32_t`.
 */
template <typename type>
concept nonint_character = one_of<type, char, char16_t, char32_t, wchar_t>;

/*!\extends     std::destructible
 * \brief       A type that satisfies std::is_trivially_destructible_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_destructible
 */
template <typename t>
concept trivially_destructible = std::destructible<t> && std::is_trivially_destructible_v<t>;

/*!\brief       A type that satisfies std::is_trivially_copyable_v<t>.
 * \extends     std::copyable
 * \sa          https://en.cppreference.com/w/cpp/types/is_trivially_copyable
 */
template <typename t>
concept trivially_copyable = std::copyable<t> && std::is_trivially_copyable_v<t>;

/*!\brief       A type that satisfies bio::meta::trivially_copyable and bio::meta::trivially_destructible.
 * \extends     bio::meta::trivially_copyable
 * \extends     bio::meta::trivially_destructible
 * \sa          https://en.cppreference.com/w/cpp/types/is_trivial
 */
template <typename t>
concept trivial = trivially_copyable<t> && trivially_destructible<t> && std::is_trivial_v<t>;

/*!\brief       Resolves to std::is_standard_layout_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_standard_layout
 */
template <typename t>
concept standard_layout = std::is_standard_layout_v<t>;

/*!\brief       Resolves to std::is_assignable_v<t>.
 * \sa          https://en.cppreference.com/w/cpp/types/is_assignable
 *
 * \details
 *
 * Note that this requires less than std::assignable_from, it simply tests if the expression
 * `std::declval<T>() = std::declval<U>()` is well-formed.
 */
template <typename t, typename u>
concept weakly_assignable_from = std::is_assignable_v<t, u>;

/*!\brief       Resolves to std::same_as<std::decay_t<t>, std::decay_t<u>>.
 */
template <typename from_t, typename to_t>
concept decays_to = std::same_as<std::decay_t<from_t>, std::decay_t<to_t>>;

/*!\brief       The negation of bio::meta::decays_to.
 */
template <typename from_t, typename to_t>
concept different_from = !decays_to<from_t, to_t>;

/*!\brief       A type that is std::default_initializable<t> at compile-time.
 */
template <typename t>
concept constexpr_default_initializable = std::default_initializable<t> && BIOCPP_IS_CONSTEXPR(t{});

//!\}

} // namespace bio::meta
