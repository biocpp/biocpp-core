// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 * \brief Provides bio::debug_stream and related types.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/mask/mask.hpp>
#include <bio/meta/detail/debug_stream_type.hpp>

namespace bio
{
/*!\name Formatted output overloads
 * \{
 */
/*!\brief All alphabets can be printed to the bio::debug_stream by their char representation.
 * \tparam alphabet_t Type of the alphabet to be printed; must model bio::alphabet.
 * \param s The bio::debug_stream.
 * \param l The alphabet letter.
 * \relates bio::debug_stream_type
 */
template <typename char_t, alphabet alphabet_t>
inline debug_stream_type<char_t> & operator<<(debug_stream_type<char_t> & s, alphabet_t && l)
//!\cond
    requires (!requires { std::cout << l; })
//!\endcond
{
    return s << to_char(l);
}

/*!\brief Overload for the bio::mask alphabet.
 * \tparam char_t Type char type of the debug_stream.
 * \param s The bio::debug_stream.
 * \param l The mask alphabet letter.
 * \relates bio::debug_stream_type
 */
template <typename char_t>
inline debug_stream_type<char_t> & operator<<(debug_stream_type<char_t> & s, mask const l)
{
    return s << (l == mask::MASKED ? "MASKED" : "UNMASKED");
}

//!\}

} // namespace bio
