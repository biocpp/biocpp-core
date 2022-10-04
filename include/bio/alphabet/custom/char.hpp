// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides alphabet adaptations for standard char types.
 * \details
 * This file provides function and type trait overloads so that the following types
 * fulfil the bio::alphabet::alphabet:
 *   * `char`
 *   * `char16_t`
 *   * `char32_t`
 *
 * You will likely not use these interfaces directly, they are, however, very helpful
 * for conversions between other alphabets and between other alphabets and characters.
 *
 * \attention
 *   * Note that `signed char` and `unsigned char` are absent from the list, because of
 * their type ambiguity with `int8_t` and `uint8_t`.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
#include <bio/meta/detail/int_types.hpp>

namespace bio::alphabet::custom
{

/*!\name Adapt builtin character types as alphabets.
 * \brief These overloads make `char`, `wchar_t`, `char16_t` and `char32_t` (but not `signed char` and `unsigned char`!) satisfy bio::alphabet::alphabet.
 * \{
 */

//!\brief The number of values the char type can take (e.g. 256 for `char`).
//!\ingroup alphabet_custom
template <bio::meta::nonint_character char_type>
consteval auto tag_invoke(size, char_type const) noexcept
{
    return meta::detail::min_viable_uint_v<meta::detail::size_in_values_v<char_type>>;
}

/*!\brief Convert char to rank by casting to an unsigned integral type of same size.
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] chr The alphabet letter that you wish to convert to rank.
 * \returns The letter's value in the alphabet's rank type (usually a `uint*_t`).
 */
template <bio::meta::nonint_character char_type>
constexpr auto tag_invoke(to_rank BIOCPP_DOXYGEN_ONLY(tag), char_type const chr) noexcept
{
    return static_cast<meta::detail::min_viable_uint_t<alphabet::size<char_type> - 1>>(chr);
}

/*!\brief Assigning a rank to a char is the same as assigning it a numeric value.
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] rank The `rank` value you wish to assign.
 * \param[in,out] chr The alphabet letter that you wish to assign to.
 * \returns A reference to the alphabet letter you passed in.
 */
template <bio::meta::nonint_character char_type>
constexpr char_type & tag_invoke(assign_rank_to                                     BIOCPP_DOXYGEN_ONLY(tag),
                                 decltype(tag_invoke(to_rank{}, char_type{})) const rank,
                                 char_type &                                        chr) noexcept
{
    return chr = rank;
}

/*!\brief Converting char to char is no-op (it will just return the value you pass in).
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] chr The alphabet letter that you wish to convert to char (no-op).
 * \returns `chr`.
 */
template <bio::meta::nonint_character char_type>
constexpr char_type tag_invoke(to_char BIOCPP_DOXYGEN_ONLY(tag), char_type const chr) noexcept
{
    return chr;
}

/*!\brief Assign a char to the char type (same as calling `=`).
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] chr2 The `char` value you wish to assign.
 * \param[in,out] chr The alphabet letter that you wish to assign to.
 * \returns A reference to the alphabet letter you passed in.
 */
template <bio::meta::nonint_character char_type>
constexpr char_type & tag_invoke(assign_char_to  BIOCPP_DOXYGEN_ONLY(tag),
                                 char_type const chr2,
                                 char_type &     chr) noexcept
{
    return chr = chr2;
}

//!\}

} // namespace bio::alphabet::custom
