// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides bio::views::add_reverse_complement.
 */

#pragma once

#include <ranges>

#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/transform_by_pos.hpp>

// ============================================================================
//  reverse_complement_or_not_fn (adaptor definition)
// ============================================================================

namespace bio::ranges::detail
{

/*!\brief Definition of the range adaptor object type for bio::views::reverse_complement_or_not.
 */
struct reverse_complement_or_not_fn
{
private:
    //!\brief Return original nucleotide at a given position.
    static auto func_nop(auto && urange, size_t pos) { return urange[pos]; }

    //!\brief Return nucleotide of reverse complemented range at a given position.
    static auto func_revcomp(auto && urange, size_t pos)
    {
        return bio::alphabet::complement(urange[std::ranges::size(urange) - pos - 1]);
    }

public:
    //!\brief Store the argument and return a range adaptor closure object.
    constexpr auto operator()(bool const transform) const { return detail::adaptor_from_functor{*this, transform}; }

    /*!\brief                 Directly return an instance of the view, initialised with the given parameters.
     * \param[in] urange      The underlying range.
     * \param[in] transform   Bool defining whether the range should be reverse complemented.
     * \returns               A reverse complemented nucleotide range or the original range.
     */
    template <std::ranges::range urng_t>
    constexpr auto operator()(urng_t && urange, bool const transform) const
    {
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The range parameter to views::reverse_complement_or_not cannot be a temporary of a non-view "
                      "range.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The range parameter to views::reverse_complement_or_not must model std::ranges::sized_range.");
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The range parameter to views::reverse_complement_or_not must model "
                      "std::ranges::random_access_range.");
        static_assert(alphabet::nucleotide<std::ranges::range_reference_t<urng_t>>,
                      "The range parameter to views::reverse_complement_or_not must be over elements of "
                      "bio::alphabet::nucleotide.");

        auto l = &func_nop<std::views::all_t<urng_t> const &>;
        if (transform)
            l = &func_revcomp<std::views::all_t<urng_t> const &>;
        return std::forward<urng_t>(urange) | views::transform_by_pos(l);
    }
};

// ============================================================================
//  reverse_complement_or_not (adaptor object)
// ============================================================================

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that reverse complements elements of a nucleotide alphabet or returns the elements itself depending on the `transform` parameter.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \param[in] transform A bool defining whether the range should be reverse complemented. [1 = reverse complement]
 * \returns A reverse complemented nucleotide range or the original range. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/add_reverse_complement.hpp}
 *
 * This view is used within bio::views::add_reverse_complement such that every second inner range is reverse complemented.
 *
 * ### View properties
 *
 * This view is a **deep view** Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
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
 * | bio::ranges::const_iterable_range     |  *required*                           | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide            | std::remove_reference_t<std::ranges::range_reference_t<urng_t>> |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 */

inline constexpr auto reverse_complement_or_not = views::deep{reverse_complement_or_not_fn{}};

// ============================================================================
//  add_reverse_complement_fn (adaptor definition)
// ============================================================================

/*!\brief Definition of the range adaptor object type for bio::views::add_reverse_complement.
 */
class add_reverse_complement_fn : public detail::adaptor_base<add_reverse_complement_fn>
{
private:
    //!\brief Type of the CRTP-base.
    using base_type = detail::adaptor_base<add_reverse_complement_fn>;

    //!\brief Befriend the base class so it can call impl().
    friend base_type;

    /*!\brief Delegate to bio::views::transform_by_pos that applies bio::views::reverse_complement_or_not
     * twice to every original element (once returning the original range and once returning the reverse complement).
     * \returns An instance of bio::views::transform_by_pos.
     */
    template <std::ranges::range urng_t>
    static auto impl(urng_t && urange)
    {
        static_assert(range_dimension_v<urng_t> == 2,
                      "This adaptor only handles range-of-range (two dimensions) as input.");
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The range parameter to views::add_reverse_complement cannot be a temporary of a non-view "
                      "range.");
        static_assert(std::ranges::viewable_range<std::ranges::range_reference_t<urng_t>>,
                      "The inner range of the range parameter to views::add_reverse_complement cannot be a temporary "
                      "of "
                      "a non-view range.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The range parameter to views::add_reverse_complement must model std::ranges::sized_range.");
        static_assert(std::ranges::sized_range<std::ranges::range_reference_t<urng_t>>,
                      "The inner range of the range parameter to views::add_reverse_complement must model "
                      "std::ranges::sized_range.");
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The range parameter to views::add_reverse_complement must model "
                      "std::ranges::random_access_range.");
        static_assert(std::ranges::random_access_range<std::ranges::range_reference_t<urng_t>>,
                      "The inner range of the range parameter to views::add_reverse_complement must model "
                      "std::ranges::random_access_range.");
        static_assert(alphabet::nucleotide<std::ranges::range_reference_t<std::ranges::range_reference_t<urng_t>>>,
                      "The range parameter to views::add_reverse_complement must be over elements of "
                      "bio::alphabet::nucleotide.");

        return std::forward<urng_t>(urange) |
               views::transform_by_pos([](auto && urange, size_t pos)
                                       { return urange[pos / 2] | reverse_complement_or_not(pos % 2 == 1); },
                                       [](auto && urange) { return std::ranges::size(urange) * 2; });
    }

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    //!\brief Defaulted.
    constexpr add_reverse_complement_fn()                                                       = default;
    //!\brief Defaulted.
    constexpr add_reverse_complement_fn(add_reverse_complement_fn const &) noexcept             = default;
    //!\brief Defaulted.
    constexpr add_reverse_complement_fn(add_reverse_complement_fn &&) noexcept                  = default;
    //!\brief Defaulted.
    constexpr add_reverse_complement_fn & operator=(add_reverse_complement_fn const &) noexcept = default;
    //!\brief Defaulted.
    constexpr add_reverse_complement_fn & operator=(add_reverse_complement_fn &&) noexcept      = default;
    //!\brief Defaulted.
    ~add_reverse_complement_fn() noexcept                                                       = default;

    //!\brief Inherit the base type's constructors.
    using base_type::base_type;
    //!\}
};

} // namespace bio::ranges::detail

// ============================================================================
//  add_reverse_complement (adaptor object)
// ============================================================================

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that adds the reverse complement of every inner range in a range-of-ranges after the respective inner range.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \returns A range that contains the original as well as the reverse complemented inner ranges. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/add_reverse_complement.hpp}
 *
 * This view adds the reverse complement of each inner range. The resulting output range is twice as large as the input range.
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
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide            | std::remove_reference_t<std::ranges::range_reference_t<urng_t>> |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Operating on a range of bio::alphabet::dna5:
 * \include test/snippet/ranges/views/add_reverse_complement.cpp
 * \hideinitializer
 */

inline constexpr auto add_reverse_complement = detail::add_reverse_complement_fn{};

} // namespace bio::ranges::views
