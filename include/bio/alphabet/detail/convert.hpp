// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\cond DEV
 * \file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::detail::convert_through_char_representation.
 * \endcond
 */

#pragma once

#include <array>

#include <bio/alphabet/concept.hpp>
#include <bio/meta/tag/vtag.hpp>

// ============================================================================
// conversion to/from char/rank types
// ============================================================================

namespace bio::alphabet::detail
{

/*!\brief A precomputed conversion table for two alphabets based on their char representations.
 * \ingroup alphabet
 * \tparam out_t The type of the output, must satisfy bio::alphabet::alphabet.
 * \tparam in_t The type of the input, must satisfy bio::alphabet::alphabet.
 * \hideinitializer
 */
template <alphabet out_t, alphabet in_t>
constexpr std::array<out_t, size<in_t>> convert_through_char_representation = []() constexpr
{
    std::array<out_t, size<in_t>> ret{};
    // for (decltype(alphabet_size<in_t>) i = 0; ...) causes indefinite compilation :(
    for (auto i = decltype(size<in_t>){0}; i < size<in_t>; ++i)
        assign_char_to(to_char(assign_rank_to(i, in_t{})), ret[i]);
    return ret;
}
();

/*!\brief Helper function for defining string literal operators.
 * \ingroup alphabet
 * \tparam str The literal string.
 * \tparam alph_t The target alphabet type.
 */
template <meta::detail::literal_buffer_string str, typename alphabet_t>
constexpr std::vector<alphabet_t> string_literal()
{
    auto illegal_char_at = []() consteval->int32_t
    {
        for (int32_t i = 0; i < (int32_t)str.size(); ++i)
            if (!char_is_valid_for<alphabet_t>(str[i]))
                return i;
        return -1;
    };
    static_assert(illegal_char_at() == -1, "Illegal character in string literal.");

    std::vector<alphabet_t> r;
    r.resize(str.size());

    for (size_t i = 0; i < str.size(); ++i)
        r[i].assign_char(str[i]);

    return r;
}

} // namespace bio::alphabet::detail
