// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::as_const.
 */

#pragma once

#include <ranges>

#include <bio/meta/type_traits/function.hpp>
#include <bio/ranges/biocpp.hpp>

namespace bio::ranges::detail
{

//!\brief Function object for bio::views::as_const.
struct as_const_fn
{
    //!\brief Operator that returns rvalues as rvalues.
    template <typename t>
    t operator()(t const && arg) const
    {
        return std::move(arg);
    }

    //!\brief Operator that returns lvalue references as lvalue-to-const-references.
    template <typename t>
    t const & operator()(t const & arg) const
    {
        return arg;
    }
};

} // namespace bio::ranges::detail

namespace bio::ranges::views
{

/*!\name General purpose views
 * \{
 */

/*!\brief               A view that provides only `const &` to elements of the underlying range.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of `const`-protected elements.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/as_const.hpp}
 *
 * ### View properties
 *
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                     |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                        |
 * | std::ranges::forward_range       |                                       | *preserved*                                        |
 * | std::ranges::bidirectional_range |                                       | *preserved*                                        |
 * | std::ranges::random_access_range |                                       | *preserved*                                        |
 * | std::ranges::contiguous_range    |                                       | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                                       |
 * | std::ranges::view                |                                       | *guaranteed*                                       |
 * | std::ranges::sized_range         |                                       | *preserved*                                        |
 * | std::ranges::common_range        |                                       | *preserved*                                        |
 * | std::ranges::output_range        |                                       | *lost*                                             |
 * | bio::ranges::const_iterable_range     |                                       | *preserved*                                        |
 * | std::semiregular                 |                                       | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   |                                       | `t &` -> `t const &` but `t` -> `t`                |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/as_const.cpp
 * \hideinitializer
 */
inline auto const as_const = std::views::transform(detail::as_const_fn{});
//!\}

} // namespace bio::ranges::views
