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
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/transform_by_pos.hpp>

// ============================================================================
//  translate_fn (adaptor definition for both views)
// ============================================================================

namespace bio::ranges::detail
{

/*!\brief Definition of the range adaptor object type for bio::views::translate_single.
 */
struct translate_single_fn
{
    //!\brief The default frames parameter for the translation view adaptors.
    static constexpr alphabet::translation_frames default_frames = alphabet::translation_frames::FWD_FRAME_0;

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
        static_assert(alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>,
                      "The range parameter to views::translate_single must be over elements of "
                      "bio::alphabet::alphabet::nucleotide_alphabet.");

        /* frames */
        if (size_t const num_frames = std::popcount(static_cast<uint8_t>(tf));
            num_frames > 1 ||
            static_cast<uint8_t>(tf) > static_cast<uint8_t>(alphabet::translation_frames::REV_FRAME_2))
        {
            throw std::invalid_argument{
              "Error: Invalid type of frame. Choose one out of FWD_FRAME_0, "
              "REV_FRAME_0, FWD_FRAME_1, REV_FRAME_1, FWD_FRAME_2 and "
              "REV_FRAME_2."};
        }

        /* size */
        size_t const old_size = std::ranges::size(urange);
        size_t const new_size = comp_new_size(old_size, tf);

        return detail::view_transform_by_pos{
          std::forward<urng_t>(urange),
          trans_fn{.old_size = old_size, .new_size = new_size, .tf = tf},
          new_size
        };
    }

    //!\brief This adaptor is usable without setting the frames parameter in which case the default is chosen.
    template <std::ranges::range urng_t>
    constexpr friend auto operator|(urng_t && urange, translate_single_fn const & me)
    {
        return me(std::forward<urng_t>(urange));
    }

private:
    //!\brief Compute the new size.
    static size_t comp_new_size(size_t const old_size, alphabet::translation_frames const tf)
    {
        switch (tf)
        {
            case alphabet::translation_frames::FWD_FRAME_0:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_0:
                return old_size / 3;
            case alphabet::translation_frames::FWD_FRAME_1:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_1:
                return (std::max<size_t>(old_size, 1) - 1) / 3;
            case alphabet::translation_frames::FWD_FRAME_2:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_2:
                return (std::max<size_t>(old_size, 2) - 2) / 3;
            default: /* GCOVR_EXCL_LINE */
                BIOCPP_UNREACHABLE
                break; /* GCOVR_EXCL_LINE */
        }

        BIOCPP_UNREACHABLE
    }

    //!\brief Auxilliary functor.
    struct trans_fn
    {
        //!\brief The size of the underlying range.
        size_t                       old_size{};
        //!\brief The size of the returned range.
        size_t                       new_size{};
        //!\brief The selected frame.
        alphabet::translation_frames tf{};

        //!\brief The operator invoked by the view on element access.
        auto operator()(auto && urange2, size_t const n) const
        {
            assert(n < new_size);
            switch (tf) //TODO create function ptr so we do not need to switch every time? → benchmark
            {
                case alphabet::translation_frames::FWD_FRAME_0:
                    return alphabet::translate_triplet(urange2[n * 3], urange2[n * 3 + 1], urange2[n * 3 + 2]);
                case alphabet::translation_frames::REV_FRAME_0:
                    return alphabet::translate_triplet(alphabet::complement(urange2[old_size - n * 3 - 1]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 2]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 3]));
                case alphabet::translation_frames::FWD_FRAME_1:
                    return alphabet::translate_triplet(urange2[n * 3 + 1], urange2[n * 3 + 2], urange2[n * 3 + 3]);
                case alphabet::translation_frames::REV_FRAME_1:
                    return alphabet::translate_triplet(alphabet::complement(urange2[old_size - n * 3 - 2]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 3]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 4]));
                case alphabet::translation_frames::FWD_FRAME_2:
                    return alphabet::translate_triplet(urange2[n * 3 + 2], urange2[n * 3 + 3], urange2[n * 3 + 4]);
                case alphabet::translation_frames::REV_FRAME_2:
                    return alphabet::translate_triplet(alphabet::complement(urange2[old_size - n * 3 - 3]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 4]),
                                                       alphabet::complement(urange2[old_size - n * 3 - 5]));
                default: /* GCOVR_EXCL_LINE */
                    BIOCPP_UNREACHABLE
                    break; /* GCOVR_EXCL_LINE */
            }
            BIOCPP_UNREACHABLE
        }
    };
};

} // namespace bio::ranges::detail

// ============================================================================
//  translate_single (adaptor object)
// ============================================================================

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that translates nucleotide into aminoacid alphabet for one of the six frames.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \param[in] tf A value of bio::alphabet::alphabet::translation_frames that indicates the desired frames. [optional, FWD_FRAME_0 by default]
 * \returns A range containing frames with aminoacid sequence. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/translate_single.hpp}
 *
 * This view can be used to translate nucleotide sequences into aminoacid sequences (see alphabet::translation_frames for possible frames).
 * Note that only single frames values are are valid here.
 *
 * There are also two other views for creating translations:
 *
 * 1. bio::views::translate_single: 1 sequence → 1 frame [range → range OR range-of-ranges → range-of-ranges]
 * 2. bio::views::translate: 1 sequence → `m` frames [range → range-of-ranges OR range-of-ranges → ranges-of-ranges-of-ranges]]
 * 3. bio::views::translate_join: `n` sequences → `n*m` frames [range-of-ranges → range-of-ranges]
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
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide_alphabet            | bio::alphabet::aa27                                       |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Operating on a range of bio::alphabet::dna5:
 * \include test/snippet/ranges/views/translate_dna5.cpp
 * \hideinitializer
 */
inline constexpr auto translate_single = deep{detail::translate_single_fn{}};

} // namespace bio::ranges::views
