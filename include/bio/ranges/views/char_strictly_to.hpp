// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::ranges::views::char_strictly_to.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <ranges>

#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/custom/all.hpp>
#include <bio/meta/overloaded.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/type_reduce.hpp>

namespace bio::ranges::views
{
/*!\name Alphabet related views
 * \{
 */

/*!\brief               A view over an alphabet, given a range of characters.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \tparam alphabet_t   The alphabet to convert to; must satisfy bio::alphabet::alphabet.
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of converted elements. See below for the properties of the returned range.
 * \throws bio::alphabet::invalid_char_assignment if an invalid character is encountered.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/char_strictly_to.hpp}
 *
 * This adaptor returns a view that transforms strings into ranges over `alphabet_t`.
 *
 * This view differs from bio::ranges::views::char_to in that it throws an exception if an invalid character conversion
 * happens. See bio::alphabet::assign_char_strictly_to for more details.
 *
 * ### View properties (generic case)
 *
 * This view is a **deep view:** Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)   |
 * |----------------------------------|:---------------------------------:|:--------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                      |
 * | std::ranges::forward_range       |                                   | *preserved*                      |
 * | std::ranges::bidirectional_range |                                   | *preserved*                      |
 * | std::ranges::random_access_range |                                   | *preserved*                      |
 * | std::ranges::contiguous_range    |                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                     |
 * | std::ranges::view                |                                   | *guaranteed*                     |
 * | std::ranges::sized_range         |                                   | *preserved*                      |
 * | std::ranges::common_range        |                                   | *preserved*                      |
 * | std::ranges::output_range        |                                   | *lost*                           |
 * | std::ranges::borrowed_range      |                                   | *lost*                           |
 * | bio::ranges::const_iterable_range|                                   | *preserved*                      |
 * |                                  |                                   |                                  |
 * | std::ranges::range_reference_t   | bio::alphabet::char_t<alphabet_t> | `alphabet_t`                     |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### View properties (NOOP case)
 *
 * If a range is given whose value_type is the same its alphabet type (e.g. std::string), a simple view
 * to the range is returned and no transformation happens. See bio::ranges::views::type_reduce.
 *
 * ### View properties (bio::alphabet::cigar)
 *
 * This range adaptor can be applied to ranges over bio::alphabet::cigar (although that type  does not
 * model bio::alphabet::alphabet). The returned view is weaker than in the generic case, and
 * **it is not a deep view.**
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)   |
 * |----------------------------------|:---------------------------------:|:--------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                      |
 * | std::ranges::forward_range       | *required*                        | *preserved*                      |
 * | std::ranges::bidirectional_range |                                   | *preserved*                      |
 * | std::ranges::random_access_range |                                   | *lost*                           |
 * | std::ranges::contiguous_range    |                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                     |
 * | std::ranges::view                |                                   | *guaranteed*                     |
 * | std::ranges::sized_range         |                                   | *lost*                           |
 * | std::ranges::common_range        |                                   | *preserved*                      |
 * | std::ranges::output_range        |                                   | *lost*                           |
 * | std::ranges::borrowed_range      |                                   | *preserved*                      |
 * | bio::ranges::const_iterable_range|                                   | *preserved*                      |
 * |                                  |                                   |                                  |
 * | std::ranges::range_reference_t   | char                              | bio::alphabet::cigar             |
 *
 * Since this transformation comes with noticeable overhead anyway, this conversion is always strict, i.e.
 * bio::views::char_to<bio::alphabet::cigar> and bio::views::char_strictly_to<bio::alphabet::cigar> are identical.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/char_strictly_to.cpp
 * \hideinitializer
 */
template <typename alphabet_type>
    //!\cond
    requires(alphabet::alphabet<alphabet_type> || std::same_as<alphabet_type, alphabet::cigar>)
//!\endcond
inline auto const char_strictly_to = detail::adaptor_from_functor{meta::overloaded{
  // clang-format off
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(std::convertible_to<ranges::range_innermost_value_t<rng_t>, alphabet::char_t<alphabet_type>> &&
                 std::same_as<ranges::range_innermost_value_t<rng_t>, alphabet_type>)
    {
        return std::forward<rng_t>(range) | views::type_reduce; // NOP
    },
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(std::convertible_to<ranges::range_innermost_value_t<rng_t>, alphabet::char_t<alphabet_type>>)
    {
        auto fn = [] (alphabet::char_t<alphabet_type> const in) -> alphabet_type
        {
            return alphabet::assign_char_strictly_to(in, alphabet_type{});
        };
        return std::forward<rng_t>(range) | deep{std::views::transform(fn)};
    },
    []<typename rng_t>(rng_t &&)
    {
        static_assert(meta::always_false<rng_t>,
                      "The type you pass to bio::views::char_strictly_to must be a range of elements convertible "
                      "to the target alphabet's character type.");
    }}};
// clang-format on

template <>
inline constexpr auto char_strictly_to<bio::alphabet::cigar> = char_to<bio::alphabet::cigar>;

//!\}

//!\brief A shortcut for `decltype(std::string_view{} | bio::views::char_strictly_to<alph_t>)`.
//!\ingroup views
template <alphabet::alphabet alph_t>
using char_conversion_view_t = decltype(std::string_view{} | bio::views::char_strictly_to<alph_t>);

} // namespace bio::ranges::views
