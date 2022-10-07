// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides alphabet adaptations for standard uint types.
 * \details
 * This file provides function and type trait overloads so that the following types
 * fulfil the bio::alphabet::semialphabet:
 *   * `uint8_t`
 *   * `uint16_t`
 *   * `uint32_t`
 *
 * You will likely not use these interfaces directly, they are, however, very helpful
 * for conversions between other alphabets and between other alphabets and characters.
 *
 * \attention
 * Note that `uint64_t` is absent from the list, because there is no type that
 * can hold its alphabet size.
 */

#pragma once

#include <bio/alphabet/custom/tag.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/detail/int_types.hpp>

namespace bio::alphabet::detail
{
//!\brief Whether a type is `uint8_t`, `uint16_t` or `uint32_t`.
//!\ingroup alphabet_custom
//!\hideinitializer
template <typename type>
concept uint_adaptation = meta::one_of<type, uint8_t, uint16_t, uint32_t>;
} // namespace bio::alphabet::detail

namespace bio::alphabet::custom
{

/*!\name Adapt unsigned integral types as semialphabets.
 * \brief These overloads make `uint8_t`, `uint16_t` and `uint32_t` (but not `uint64_t`!) satisfy bio::alphabet::semialphabet.
 * \{
 */
//!\brief The number of values the uint type can take (e.g. 256 for `uint8_t`).
//!\ingroup alphabet_custom
template <alphabet::detail::uint_adaptation uint_type>
consteval auto tag_invoke(size BIOCPP_DOXYGEN_ONLY(tag), uint_type const) noexcept
{
    return meta::detail::min_viable_uint_v<meta::detail::size_in_values_v<uint_type>>;
}

/*!\brief Assign a rank to to the uint (same as calling `=`).
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] intgr2 The `rank` value you wish to assign.
 * \param[in,out] intgr The alphabet letter that you wish to assign to.
 * \returns A reference to the modified alphabet letter you passed in.
 */
template <alphabet::detail::uint_adaptation uint_type>
constexpr uint_type & tag_invoke(assign_rank_to  BIOCPP_DOXYGEN_ONLY(tag),
                                 uint_type const intgr2,
                                 uint_type &     intgr) noexcept
{
    return intgr = intgr2;
}

/*!\brief Converting uint to rank is a no-op (it will just return the value you pass in).
 * \ingroup alphabet_custom
 * \param[in] tag The tag for tag_invoke().
 * \param[in] intgr The alphabet letter that you wish to convert to rank.
 * \returns `intgr`.
 */
template <alphabet::detail::uint_adaptation uint_type>
constexpr auto tag_invoke(to_rank BIOCPP_DOXYGEN_ONLY(tag), uint_type const intgr) noexcept
{
    return intgr;
}

//!\}

} // namespace bio::alphabet::custom
