// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 * \brief Provides bio::debug_stream and related types.
 */

#pragma once

#include <optional>

#include <bio/meta/detail/debug_stream_type.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <bio/meta/type_traits/template_inspection.hpp>

namespace bio
{
/*!\name Formatted output overloads
 * \{
 */
/*!\brief Make std::nullopt_t printable.
 * \tparam    optional_type This is std::nullopt_t.
 * \param[in] s             The bio::debug_stream.
 * \param[in] arg           This is std::nullopt.
 * \relates bio::debug_stream_type
 */
template <typename char_t>
inline debug_stream_type<char_t> & operator<<(debug_stream_type<char_t> & s, std::nullopt_t BIOCPP_DOXYGEN_ONLY(arg))
{
   s << "<VALUELESS_OPTIONAL>";
   return s;
}

/*!\brief A std::optional can be printed by printing its value or nothing if valueless.
 * \tparam    optional_type The type of the optional.
 * \param[in] s             The bio::debug_stream.
 * \param[in] arg           The std::optional.
 * \relates bio::debug_stream_type
 */
template <typename char_t, typename optional_type>
//!\cond
    requires detail::is_type_specialisation_of_v<std::remove_cvref_t<optional_type>, std::optional>
//!\endcond
inline debug_stream_type<char_t> & operator<<(debug_stream_type<char_t> & s, optional_type && arg)
{
    if (arg.has_value())
        s << *arg;
    else
        s << "<VALUELESS_OPTIONAL>";
    return s;
}

//!\}

} // namespace bio
