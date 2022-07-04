// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::trim_quality.
 */

#pragma once

#include <bio/alphabet/quality/qualified.hpp>
#include <bio/ranges/views/deep.hpp>
#include <ranges>

namespace bio::ranges::detail
{

/*!\brief The underlying type of bio::views::trim_quality.
 * \ingroup views
 *
 * Under the hood this delegates to views::take_until.
 */
struct trim_fn
{
    //!\brief Store the argument and return a range adaptor closure object.
    template <typename threshold_t>
    constexpr auto operator()(threshold_t const threshold) const
    {
        static_assert(quality_alphabet<threshold_t> || std::integral<threshold_t>,
                      "The threshold must either be a quality alphabet or an integral type "
                      "in which case it is compared with the underlying phred type.");

        return adaptor_from_functor{*this, threshold};
    }

    /*!\brief Trim based on minimum phred score.
     * \tparam irng_t The type of the range being processed. See bio::views::trim_quality for requirements.
     * \param irange The range being processed.
     * \param threshold The minimum quality as a phred score [integral type].
     */
    template <std::ranges::input_range irng_t, typename threshold_t>
    constexpr auto operator()(irng_t && irange, threshold_t const threshold) const
    {
        static_assert(quality_alphabet<std::remove_reference_t<std::ranges::range_reference_t<irng_t>>>,
                      "views::trim_quality can only operate on ranges over bio::quality_alphabet.");
        static_assert(
          std::same_as<std::remove_cvref_t<threshold_t>, std::remove_cvref_t<std::ranges::range_reference_t<irng_t>>> ||
            std::integral<std::remove_cvref_t<threshold_t>>,
          "The threshold must either be a letter of the underlying alphabet or an integral type "
          "in which case it is compared with the underlying phred type.");

        return std::views::take_while(
          std::forward<irng_t>(irange),
          [threshold](auto const value)
          {
              if constexpr (std::same_as<std::remove_cvref_t<threshold_t>,
                                         std::remove_cvref_t<std::ranges::range_reference_t<irng_t>>>)
              {
                  return to_phred(value) >= to_phred(threshold);
              }
              else
              {
                  using c_t = std::common_type_t<decltype(to_phred(value)), threshold_t>;
                  return static_cast<c_t>(to_phred(value)) >= static_cast<c_t>(threshold);
              }
          });
    }
};

} // namespace bio::ranges::detail

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief               A view that does quality-threshold trimming on a range of bio::quality_alphabet.
 * \tparam urng_t       The type of the range being processed. See below for requirements.
 * \tparam threshold_t  Either std::ranges::range_value_t<urng_t> or
 *                      bio::alphabet_phred_t<std::ranges::range_value_t<urng_t>>.
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \param[in] threshold The minimum quality.
 * \returns             A trimmed range. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/trim_quality.hpp}
 *
 * This view can be used to do easy quality based trimming of sequences.
 *
 * ### View properties
 *
 * This view is a **deep view** Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)         |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                            |
 * | std::ranges::forward_range       |                                       | *preserved*                            |
 * | std::ranges::bidirectional_range |                                       | *preserved*                            |
 * | std::ranges::random_access_range |                                       | *preserved*                            |
 * |                                  |                                       |                                        |
 * | std::ranges::view                |                                       | *guaranteed*                           |
 * | std::ranges::sized_range         |                                       | *lost*                                 |
 * | std::ranges::common_range        |                                       | *lost*                                 |
 * | std::ranges::output_range        |                                       | *preserved*                            |
 * | bio::ranges::const_iterable_range     |                                       | *preserved*                            |
 * |                                  |                                       |                                        |
 * | std::ranges::range_reference_t   | bio::quality_alphabet              | std::ranges::range_reference_t<urng_t> |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ###Example
 *
 * Operating on a range of bio::phred42:
 * \include test/snippet/ranges/views/trim_phred42.cpp
 *
 * Or operating on a range of bio::dna5q:
 * \include test/snippet/ranges/views/trim_dna5q.cpp
 * \hideinitializer
 */

inline constexpr auto trim_quality = deep{detail::trim_fn{}};

//!\}

} // namespace bio::ranges::views
