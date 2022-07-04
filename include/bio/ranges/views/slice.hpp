// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::slice.
 */

#pragma once

#include <stdexcept>

#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/detail.hpp>
#include <bio/ranges/views/type_reduce.hpp>
#include <concepts>
#include <iterator>
#include <ranges>
#include <span>
#include <type_traits>

namespace bio::ranges::detail
{

// ============================================================================
//  slice_fn (adaptor definition)
// ============================================================================

//!\brief View adaptor definition for views::slice.
struct slice_fn
{
    //!\brief Store the arguments and return a range adaptor closure object.
    constexpr auto operator()(ptrdiff_t begin_pos, ptrdiff_t end_pos) const noexcept
    {
        return detail::adaptor_from_functor{*this, begin_pos, end_pos};
    }

    /*!\brief       Call the view's constructor with the underlying view as argument.
     * \returns     An instance of bio::ranges::detail::view_slice.
     */
    template <std::ranges::viewable_range urng_t>
    constexpr auto operator()(urng_t && urange, ptrdiff_t begin_pos, ptrdiff_t end_pos) const
    {
        if constexpr (std::ranges::sized_range<urng_t>)
        {
            begin_pos = std::min(begin_pos, static_cast<ptrdiff_t>(std::ranges::size(urange)));
            end_pos   = std::min(end_pos, static_cast<ptrdiff_t>(std::ranges::size(urange)));
        }

        if (end_pos < begin_pos)
            throw std::invalid_argument{"end_pos argument to bio::views::slice must be >= the begin_pos argument."};

        return std::forward<urng_t>(urange) | std::views::drop(begin_pos) | std::views::take(end_pos - begin_pos) |
               views::type_reduce;
    }

    // does not require special overloads, because views::drop and views::take handle the flattening.
};

} // namespace bio::ranges::detail

// ============================================================================
//  views::slice (adaptor instance definition)
// ============================================================================

namespace bio::ranges::views
{

/*!\name General purpose views
 * \{
 */

/*!\brief               A view adaptor that returns a half-open interval on the underlying range.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \param[in] begin_pos The beginning of the interval (index of first element returned).
 * \param[in] end_pos   The end of the interval (index **behind** the last element returned).
 * \returns             Up to `end_pos - begin_pos` elements of the underlying range.
 * \throws std::invalid_argument If `end_pos < begin_pos`.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/slice.hpp}
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)         |
 * |----------------------------------|:---------------------------------:|:--------------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                            |
 * | std::ranges::forward_range       |                                   | *preserved*                            |
 * | std::ranges::bidirectional_range |                                   | *preserved*                            |
 * | std::ranges::random_access_range |                                   | *preserved*                            |
 * | std::ranges::contiguous_range    |                                   | *preserved*                            |
 * |                                  |                                   |                                        |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                           |
 * | std::ranges::view                |                                   | *guaranteed*                           |
 * | std::ranges::sized_range         |                                   | *preserved*                            |
 * | std::ranges::common_range        |                                   | *preserved*                            |
 * | std::ranges::output_range        |                                   | *preserved*                            |
 * | bio::ranges::const_iterable_range     |                                   | *preserved*                            |
 * |                                  |                                   |                                        |
 * | std::ranges::range_reference_t   |                                   | std::ranges::range_reference_t<urng_t> |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * This adaptor is a combination of std::views::drop, std::views::take and bio::views::type_reduce.
 *
 * If `begin_pos` is larger than the size of the underlying range an empty range is returned.
 * If `end_pos` is larger than the size of the underlying range all elements are returned.
 *
 * If `end_pos < begin_pos` an exception of type std::invalid_argument is thrown.
 *
 * ### Return type
 *
 * | `urng_t` (underlying range type)                                                              | `rrng_t` (returned range type)  |
 * |:---------------------------------------------------------------------------------------------:|:-------------------------------:|
 * | `std::basic_string const &` *or* `std::basic_string_view`                                     | `std::basic_string_view`        |
 * | `std::ranges::borrowed_range && std::ranges::sized_range && std::ranges::contiguous_range`    | `std::span`                     |
 * | `std::ranges::borrowed_range && std::ranges::sized_range && std::ranges::random_access_range` | `std::ranges::subrange`         |
 * | *else*                                                                                        | *implementation defined type*   |
 *
 * The adaptor returns exactly the type specified above.
 *
 * ### Complexity
 *
 * Construction of the returned view is in \f$ O(begin\_pos) \f$ for some views, see bio::views::drop.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/slice.cpp
 *
 * \hideinitializer
 */
inline constexpr auto slice = detail::slice_fn{};

//!\}

} // namespace bio::ranges::views
