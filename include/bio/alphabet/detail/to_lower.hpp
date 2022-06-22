// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#pragma once

#include <bio/meta/platform.hpp>

/*!\cond DEV
 * \file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::detail::to_lower and bio::detail::to_upper.
 * \endcond
 */

namespace bio::detail
{

//!\brief True if the character is within 'a' ... 'z'.
constexpr bool is_lower(char const c)
{
    return (c >= 'a' && c <= 'z');
}

//!\brief True if the character is within 'A' ... 'Z'.
constexpr bool is_upper(char const c)
{
    return (c >= 'A' && c <= 'Z');
}

/*!\brief Like std::tolower but locale independent and constexpr.
 * \ingroup alphabet
 * \param c The input character
 * \returns If c is between 'A' and 'Z', the respective value between 'a' and 'z'. Otherwise c is returned unchanged.
 */
constexpr char to_lower(char const c)
{
    return c | 0b0010'0000;
}

/*!\brief Like std::toupper but locale independent and constexpr.
 * \ingroup alphabet
 * \param c The input character
 * \returns If c is between 'a' and 'z', the respective value between 'A' and 'Z'. Otherwise c is returned unchanged.
 */
constexpr char to_upper(char const c)
{
    return c & 0b0101'1111;
}

} // namespace bio::detail
