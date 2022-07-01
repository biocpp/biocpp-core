// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 * \brief Provides bio::views::get.
 */

#pragma once

#include <bio/meta/concept/tuple.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <ranges>

namespace bio::views
{
/*!\name General purpose views
 * \{
 */

/*!\brief               A view calling std::get on each element in a range.
 * \tparam size_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \tparam index        The index to get.
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of elements where every element is the result of calling std::get<index> on the underlying element.
                        See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/get.hpp}
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                                     |
 * |----------------------------------|:-------------------------------------:|:------------------------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                                        |
 * | std::ranges::forward_range       |                                       | *preserved*                                                        |
 * | std::ranges::bidirectional_range |                                       | *preserved*                                                        |
 * | std::ranges::random_access_range |                                       | *preserved*                                                        |
 * | std::ranges::contiguous_range    |                                       | *lost*                                                             |
 * |                                  |                                       |                                                                    |
 * | std::ranges::viewable_range      | *required*                            | *preserved*                                                        |
 * | std::ranges::view                |                                       | *preserved*                                                        |
 * | std::ranges::sized_range         |                                       | *preserved*                                                        |
 * | std::ranges::common_range        |                                       | *preserved*                                                        |
 * | std::ranges::output_range        |                                       | *preserved*                                                        |
 * | bio::const_iterable_range     |                                       | *preserved*                                                        |
 * |                                  |                                       |                                                                    |
 * | std::ranges::range_reference_t   | bio::meta::tuple_like                    | std::tuple_element_t<index, std::ranges::range_reference_t<urng_t>>|
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/get.cpp
 * \hideinitializer
 */
template <auto index>
inline auto const get = std::views::transform(
  [](auto && in) -> decltype(auto)
  {
      using std::get;
      using meta::get;
      static_assert(meta::tuple_like<decltype(in)>,
                    "You may only pass ranges to views::get whose reference_t models the tuple_like.");

      // we need to explicitly remove && around temporaries to return values as values (and not as rvalue references)
      // we cannot simply cast to std::tuple_element_t (or set that as return value), because some tuples, like
      // our alphabet_tuple_base alphabets do not return that type when get is called on them (they return a proxy)
      using ret_type = meta::remove_rvalue_reference_t<decltype(get<index>(std::forward<decltype(in)>(in)))>;
      return static_cast<ret_type>(get<index>(std::forward<decltype(in)>(in)));
  });

//!\}

} // namespace bio::views
