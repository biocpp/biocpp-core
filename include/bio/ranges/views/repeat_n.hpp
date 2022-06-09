// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 * \brief Provides bio::views::repeat_n.
 */

#pragma once

#include <bio/ranges/views/repeat.hpp>
#include <bio/ranges/views/take_exactly.hpp>
#include <concepts>

namespace bio::detail
{

/*!\brief The underlying type of bio::views::repeat_n.
 * \ingroup views
 *
 * Under the hood this delegates to `views::repeat(value) | views::take_exactly(count)`.
 */
struct repeat_n_fn
{
    /*!\brief Creates a range of size `count`, where each element equals `value`.
     * \tparam    value_t The type of value to repeat; must be std::copy_constructible.
     * \param[in] value   The value to repeat.
     * \param[in] count   The number of times to repeat `value`.
     * \returns A range of size `count`, where each element equals `value`.
     */
    template <typename value_t>
    constexpr auto operator()(value_t && value, size_t const count) const
    {
        static_assert(std::copy_constructible<value_t>, "The value passed to repeat_n must be copy constructible.");

        return views::repeat(std::forward<value_t>(value)) | views::take_exactly(count);
    }
};

} // namespace bio::detail

namespace bio::views
{

/*!\name General purpose views
 * \{
 */
/*!\brief A view factory that repeats a given value `n` times.
 * \tparam    value_t The type of value to repeat; must be std::copy_constructible.
 * \param[in] value   The value to repeat.
 * \param[in] count   The number of times to repeat `value`.
 * \returns A range of size `count`, where each element equals `value`.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/repeat_n.hpp}
 *
 * ### View properties
 *
 * This view is **source-only**, it can only be at the beginning of a pipe of range transformations.
 *
 * | Concepts and traits              | `rrng_t` (returned range type)                     |
 * |----------------------------------|:--------------------------------------------------:|
 * | std::ranges::input_range         | *guaranteed*                                       |
 * | std::ranges::forward_range       | *guaranteed*                                       |
 * | std::ranges::bidirectional_range | *guaranteed*                                       |
 * | std::ranges::random_access_range | *guaranteed*                                       |
 * | std::ranges::contiguous_range    |                                                    |
 * |                                  |                                                    |
 * | std::ranges::viewable_range      | *guaranteed*                                       |
 * | std::ranges::view                | *guaranteed*                                       |
 * | std::ranges::sized_range         | *guaranteed*                                       |
 * | std::ranges::common_range        |                                                    |
 * | std::ranges::output_range        | *guaranteed*                                       |
 * | bio::const_iterable_range     | *guaranteed*                                       |
 * |                                  |                                                    |
 * | std::ranges::range_reference_t   | std::remove_reference_t<value_t> &                 |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * \attention The given value to repeat is always **copied** into the range.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/repeat_n.cpp
 *
 * \hideinitializer
 */
constexpr inline auto repeat_n = detail::repeat_n_fn{};
//!\}

} // namespace bio::views
