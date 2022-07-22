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

} // namespace bio::alphabet::detail
