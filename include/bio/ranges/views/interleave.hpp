// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 * \brief Provides bio::views::interleave.
 */

#pragma once

#include <cmath>
#include <concepts>
#include <ranges>

#include <bio/ranges/views/transform_by_pos.hpp>

namespace bio::ranges::detail
{

// ============================================================================
//  interleave_fn (adaptor definition)
// ============================================================================

/*!\brief views::interleave's range adaptor object type (non-closure).
 */
struct interleave_fn
{
private:
    //!\brief Functor definition.
    template <typename irange_t>
    struct func
    {
        //!\brief The inserted range, view-wrapped.
        std::views::all_t<irange_t> irange;
        //!\brief Frequency of insertion.
        size_t                      step_size = 0;

        //!\brief Operator definition invoked on access.
        template <typename urng_t>
        constexpr auto operator()(urng_t && urange, size_t const i) const
          -> std::common_reference_t<std::ranges::range_reference_t<urng_t>,
                                     std::ranges::range_reference_t<decltype(std::as_const(irange))>>
        {
            size_t const div  = i / (step_size + std::ranges::size(irange));
            size_t const rest = i % (step_size + std::ranges::size(irange));
            return (rest < step_size) ? urange[i - div * std::ranges::size(irange)] : irange[rest - step_size];
        }
    };

public:
    //!\brief Store the argument and return a range adaptor closure object.
    template <std::ranges::forward_range inserted_rng_t>
    constexpr auto operator()(size_t const size, inserted_rng_t && i) const noexcept
    {
        return detail::adaptor_from_functor{*this, size, std::views::all(std::forward<inserted_rng_t>(i))};
    }

    /*!\brief            Call the view's constructor with the underlying view as argument.
     * \param[in] urange The input range to process. Must model std::ranges::forward_range and std::ranges::viewable_range.
     * \param[in] irange      The inserted range to process. Must model std::ranges::forward_range.
     * \param[in] step_size   The step size for insertion into the input range.
     * \returns          A range of with the inserted range interleaved into the underlying range at the specified intervals.
     */
    template <std::ranges::range urng_t, std::ranges::range inserted_rng_t>
    constexpr auto operator()(urng_t && urange, size_t const step_size, inserted_rng_t && irange) const noexcept
    {
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The underlying range parameter in views::interleave must model "
                      "std::ranges::random_access_range.");
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The underlying range parameter in views::interleave must model std::ranges::viewable_range.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The underlying range parameter in views::interleave must model std::ranges::sized_range.");

        static_assert(std::ranges::random_access_range<inserted_rng_t>,
                      "The range to be inserted by views::interleave must model std::ranges::forward_range.");
        static_assert(std::ranges::sized_range<inserted_rng_t>,
                      "The range to be inserted by views::interleave must model std::ranges::sized_range.");

        static_assert(std::common_reference_with<std::ranges::range_reference_t<urng_t>,
                                                 std::ranges::range_reference_t<inserted_rng_t const>>,
                      "The underlying range and the CONST inserted range must have reference types that have a "
                      "common_reference type.");

        size_t const size =
          std::ranges::size(urange) +
          (((std::ranges::size(urange) / step_size) - (std::ranges::size(urange) % step_size == 0 ? 1 : 0)) *
           std::ranges::size(irange));

        func<std::views::all_t<inserted_rng_t>> fn{.irange    = std::views::all(std::forward<inserted_rng_t>(irange)),
                                                   .step_size = step_size};

        return view_transform_by_pos{std::forward<urng_t>(urange), std::move(fn), size};
    }
};

} // namespace bio::ranges::detail

// ============================================================================
//  views::interleave (adaptor instance definition)
// ============================================================================

namespace bio::ranges::views
{

/*!\name General purpose views
 * \{
 */

/*!\brief A view that interleaves a given range into another range at regular intervals.
 * \tparam urng_t The type of the range being processed.
 * \tparam inserted_rng_t The type of the range being inserted.
 * \param[in] urange The range being processed.
 * \param[in] inserted_range The range being inserted.
 * \param[in] step_size A value of size_t which indicates the interval to insert the inserted_range.
 * \returns A range with the second range inserted at regular intervals. See below for properties of said range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/interleave.hpp}
 *
 * This view can be used to insert one range into another range at regular intervals. It behaves essentially like
 * `| std::views::chunk(step_size) | std::views::join_with(inserted_range)` (avaiable in C++23), except that
 * it requires the underlying range and the inserted range to model std::ranges::random_access_range and
 * std::ranges::sized_range; and that the view itself also models these concepts.
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)         |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                            |
 * | std::ranges::forward_range       | *required*                            | *preserved*                            |
 * | std::ranges::bidirectional_range | *required*                            | *preserved*                            |
 * | std::ranges::random_access_range | *required*                            | *preserved*                            |
 * | std::ranges::contiguous_range    |                                       | *lost*                                 |
 * |                                  |                                       |                                        |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                           |
 * | std::ranges::view                |                                       | *guaranteed*                           |
 * | std::ranges::sized_range         | *required*                            | *preserved*                            |
 * | std::ranges::common_range        |                                       | *guaranteed*                           |
 * | std::ranges::output_range        |                                       | see below                              |
 * | bio::ranges::const_iterable_range|                                       | *preserved*                            |
 * |                                  |                                       |                                        |
 * | std::ranges::range_reference_t   |                                       | see below                              |
 *
 *
 * The `range_reference_t` of the view is the common reference type of the reference types of `urng_t` and
 * `std::views::all_t<inserted_rng_t> const`, e.g. if urange is a std::string, and inserted_range is a std::string
 * lvalue, the reference type of this view is `char &`; if inserted_range is in rvalue, the reference type of this
 * view is `char const &` (because of weird implementation detail of views).
 * As a rule-of-thumb, assume that `T &` will turn to `T const &` and the output_range property is lost.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/interleave.cpp
 * \hideinitializer
 */
inline constexpr auto interleave = detail::interleave_fn{};

//!\}

} // namespace bio::ranges::views
