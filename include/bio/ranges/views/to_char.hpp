// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::to_char.
 */

#pragma once

#include <ranges>

#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/custom/all.hpp>
#include <bio/meta/overloaded.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/type_reduce.hpp>

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief               A view that calls bio::alphabet::to_char() on each element in the input range.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of converted elements. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/to_char.hpp}
 *
 * ### View properties (generic case)
 *
 * This view is a **deep view** Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)                           |
 * |----------------------------------|:---------------------------------:|:--------------------------------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                                              |
 * | std::ranges::forward_range       |                                   | *preserved*                                              |
 * | std::ranges::bidirectional_range |                                   | *preserved*                                              |
 * | std::ranges::random_access_range |                                   | *preserved*                                              |
 * | std::ranges::contiguous_range    |                                   | *lost*                                                   |
 * |                                  |                                   |                                                          |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                                             |
 * | std::ranges::view                |                                   | *guaranteed*                                             |
 * | std::ranges::sized_range         |                                   | *preserved*                                              |
 * | std::ranges::common_range        |                                   | *preserved*                                              |
 * | std::ranges::output_range        |                                   | *lost*                                                   |
 * | bio::ranges::const_iterable_range|                                   | *preserved*                                              |
 * |                                  |                                   |                                                          |
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet           | bio::alphabet::char_t<std::ranges::range_value_t<urng_t>>|
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### View properties (NOOP case)
 *
 * If a range is given whose value_type is the same its alphabet type (e.g. std::string), a simple view
 * to the range is returned and no transformation happens. See bio::ranges::views::type_reduce.
 *
 *
 * ### View properties (bio::alphabet::cigar)
 *
 * This range adaptor can be applied to ranges over bio::alphabet::cigar (although that type does not
 * model bio::alphabet::alphabet). The returned view is weaker than in the generic case:
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)   |
 * |----------------------------------|:---------------------------------:|:--------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                      |
 * | std::ranges::forward_range       |                                   | *lost*                           |
 * | std::ranges::bidirectional_range |                                   | *lost*                           |
 * | std::ranges::random_access_range |                                   | *lost*                           |
 * | std::ranges::contiguous_range    |                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                     |
 * | std::ranges::view                |                                   | *guaranteed*                     |
 * | std::ranges::sized_range         |                                   | *lost*                           |
 * | std::ranges::common_range        |                                   | *lost*                           |
 * | std::ranges::output_range        |                                   | *lost*                           |
 * | bio::ranges::const_iterable_range|                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::range_reference_t   | bio::alphabet::cigar              | char                             |
 *
 * ### Example
 * \include test/snippet/ranges/views/range_view_to_char.cpp
 * \hideinitializer
 */
inline auto const to_char = detail::adaptor_from_functor{meta::overloaded{
  // clang-format off
    // cigar special case
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(std::same_as<ranges::range_innermost_value_t<rng_t>, bio::alphabet::cigar>)
    {
        //TODO we should mark small_strings <= 32 bytes as views and just return them here
        auto fn = [sbuf = small_string<10>{} ] (bio::alphabet::cigar const c) mutable
        {
            return c.to_string(sbuf);
        };
        return std::forward<rng_t>(range) | deep{std::views::transform(fn) | std::views::join};
    },
    // char2char special case
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(alphabet::alphabet<ranges::range_innermost_value_t<rng_t>> &&
                 std::same_as<ranges::range_innermost_value_t<rng_t>,
                              alphabet::char_t<ranges::range_innermost_value_t<rng_t>>>)
    {
        return std::forward<rng_t>(range) | views::type_reduce; // NOP
    },
    // generic case
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(alphabet::alphabet<ranges::range_innermost_value_t<rng_t>>)
    {
        return std::forward<rng_t>(range) | deep{std::views::transform(alphabet::to_char)};
    },
    // catch-all
    []<typename rng_t>(rng_t &&)
    {
        static_assert(meta::always_false<rng_t>,
                      "bio::views::to_char can only be invoked with ranges over an alphabet.");
    }}};
// clang-format on

//!\}

} // namespace bio::ranges::views
