// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides bio::views::translate and bio::views::translate_single.
 */

#pragma once

#include <concepts>
#include <ranges>
#include <stdexcept>
#include <vector>

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/aminoacid/translation.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/container/concept.hpp>
#include <bio/ranges/container/small_string.hpp>
#include <bio/ranges/detail/random_access_iterator.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/detail.hpp>
#include <bio/ranges/views/translate_single.hpp>

// ============================================================================
//  translate_fn (adaptor definition )
// ============================================================================

namespace bio::ranges::detail
{

/*!\brief Definition of the range adaptor object type for bio::views::translate.
 */
struct translate_fn
{
    //!\brief The default frames parameter for the translation view adaptors.
    static constexpr alphabet::translation_frames default_frames = alphabet::translation_frames::SIX_FRAME;

    //!\brief Store the argument and return a range adaptor closure object.
    constexpr auto operator()(alphabet::translation_frames const tf = default_frames) const
    {
        return detail::adaptor_from_functor{*this, tf};
    }

    /*!\brief            Directly return an instance of the view, initialised with the given parameters.
     * \param[in] urange The underlying range.
     * \param[in] tf     The frame that should be used for translation.
     * \returns          A range of translated sequence(s).
     */
    template <std::ranges::range urng_t>
    constexpr auto operator()(urng_t && urange, alphabet::translation_frames const tf = default_frames) const
    {
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The range parameter to views::translate_single must be viewable.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The range parameter to views::translate_single must model std::ranges::sized_range.");
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The range parameter to views::translate_single must model std::ranges::random_access_range.");
        static_assert(alphabet::nucleotide<std::ranges::range_reference_t<urng_t>>,
                      "The range parameter to views::translate_single must be over elements of "
                      "bio::alphabet::alphabet::nucleotide.");

        /* frames */
        small_vector<alphabet::translation_frames, 6> selected_frames{};
        if ((tf & alphabet::translation_frames::FWD_FRAME_0) == alphabet::translation_frames::FWD_FRAME_0)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_0);
        if ((tf & alphabet::translation_frames::FWD_FRAME_1) == alphabet::translation_frames::FWD_FRAME_1)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_1);
        if ((tf & alphabet::translation_frames::FWD_FRAME_2) == alphabet::translation_frames::FWD_FRAME_2)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_2);
        if ((tf & alphabet::translation_frames::REV_FRAME_0) == alphabet::translation_frames::REV_FRAME_0)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_0);
        if ((tf & alphabet::translation_frames::REV_FRAME_1) == alphabet::translation_frames::REV_FRAME_1)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_1);
        if ((tf & alphabet::translation_frames::REV_FRAME_2) == alphabet::translation_frames::REV_FRAME_2)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_2);

        return detail::view_transform_by_pos{std::forward<urng_t>(urange),
                                             trans_fn{.selected_frames = selected_frames},
                                             selected_frames.size()};
    }

    //!\brief This adaptor is usable without setting the frames parameter in which case the default is chosen.
    template <std::ranges::range urng_t>
    constexpr friend auto operator|(urng_t && urange, translate_fn const & me)
    {
        return me(std::forward<urng_t>(urange));
    }

private:
    //!\brief Auxilliary functor.
    struct trans_fn
    {
        //!\brief The selected frames.
        small_vector<alphabet::translation_frames, 6> selected_frames{};

        //!\brief The operator invoked by the view on element access.
        auto operator()(auto && urange, size_t const n) const
        {
            assert(n < selected_frames.size());
            return urange | views::translate_single(selected_frames[n]);
        }
    };
};

} // namespace bio::ranges::detail

// ============================================================================
//  translate (adaptor object)
// ============================================================================

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that translates nucleotide into aminoacid alphabet with 1, 2, 3 or 6 frames.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \param[in] tf A value of bio::alphabet::tanslation_frames that indicates the desired frames. [optional, SIX_FRAME by default]
 * \returns A range of ranges containing frames with aminoacid sequence. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/translate.hpp}
 *
 * This view can be used to translate nucleotide sequences into aminoacid sequences (see alphabet::translation_frames for possible combination of frames).
 *
 * Note that the *dimension* of the input is always increased by one, i.e. if you pass a single sequence,
 * you will get multiple frames; if you pass muliple sequences, you get a as many ranges of frames.
 *
 * There are also two other views for creating translations:
 *
 * 1. bio::views::translate_single: 1 sequence → 1 frame [range → range OR range-of-ranges → range-of-ranges]
 * 2. bio::views::translate: 1 sequence → n frames [range → range-of-ranges OR range-of-ranges → ranges-of-ranges-of-ranges]]
 * 3. bio::views::translate_join: n sequences → n*m frames [range-of-ranges → range-of-ranges]
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                     |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                        |
 * | std::ranges::forward_range       | *required*                            | *preserved*                                        |
 * | std::ranges::bidirectional_range | *required*                            | *preserved*                                        |
 * | std::ranges::random_access_range | *required*                            | *preserved*                                        |
 * | std::ranges::contiguous_range    |                                       | *lost*                                             |
 * |                                  |                                       |                                                    |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                                       |
 * | std::ranges::view                |                                       | *guaranteed*                                       |
 * | std::ranges::sized_range         | *required*                            | *preserved*                                        |
 * | std::ranges::common_range        |                                       | *guaranteed*                                       |
 * | std::ranges::output_range        |                                       | *lost*                                             |
 * | bio::ranges::const_iterable_range     | *required*                            | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide            | std::ranges::view && std::ranges::random_access_range && std::ranges::sized_range |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Operating on a range of bio::alphabet::dna5:
 * \include test/snippet/ranges/views/translate_usage.cpp
 * \hideinitializer
 */
inline constexpr auto translate = deep{detail::translate_fn{}};
//!\}

} // namespace bio::ranges::views
