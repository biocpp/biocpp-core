// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::ranges::views::char_strictly_to.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <ranges>

#include <bio/alphabet/concept.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/ranges/views/validate_char_for.hpp>

namespace bio::ranges::views
{
/*!\name Alphabet related views
 * \{
 */

/*!\brief               A view over an alphabet, given a range of characters.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \tparam alphabet_t   The alphabet to convert to; must satisfy bio::alphabet::alphabet.
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of converted elements. See below for the properties of the returned range.
 * \throws bio::alphabet::invalid_char_assignment if an invalid character is encountered.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/char_strictly_to.hpp}
 *
 * This view differs from bio::ranges::views::char_to in that it throws an exception if an invalid character conversion
 * happens. See bio::alphabet::assign_char_strictly_to for more details.
 *
 * ### View properties
 *
 * This view is a **deep view**. Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
 *
 * | Concepts and traits               | `urng_t` (underlying range type)    | `rrng_t` (returned range type) |
 * |-----------------------------------|:-----------------------------------:|:------------------------------:|
 * | std::ranges::input_range          | *required*                          | *preserved*                    |
 * | std::ranges::forward_range        |                                     | *preserved*                    |
 * | std::ranges::bidirectional_range  |                                     | *preserved*                    |
 * | std::ranges::random_access_range  |                                     | *preserved*                    |
 * | std::ranges::contiguous_range     |                                     | *lost*                         |
 * |                                   |                                     |                                |
 * | std::ranges::viewable_range       | *required*                          | *guaranteed*                   |
 * | std::ranges::view                 |                                     | *guaranteed*                   |
 * | std::ranges::sized_range          |                                     | *preserved*                    |
 * | std::ranges::common_range         |                                     | *preserved*                    |
 * | std::ranges::output_range         |                                     | *lost*                         |
 * | std::semiregular                  |                                     | *preserved*                    |
 * | bio::ranges::const_iterable_range |                                     | *preserved*                    |
 * |                                   |                                     |                                |
 * | std::ranges::range_reference_t    | bio::alphabet::char_t<alphabet_t>   | `alphabet_t`                   |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/char_strictly_to.cpp
 * \hideinitializer
 */
template <alphabet::alphabet alphabet_type>
inline auto const char_strictly_to = views::validate_char_for<alphabet_type> | views::char_to<alphabet_type>;

//!\}

} // namespace bio::ranges::views
