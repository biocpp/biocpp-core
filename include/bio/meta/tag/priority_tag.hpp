// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief provides bio::meta::priority_tag.
 */

#pragma once

#include <bio/meta/platform.hpp>

namespace bio::meta
{

// ============================================================================
// priority_tag
// ============================================================================

//!\brief A tag that allows controlled overload resolution via implicit base conversion rules.
//!\ingroup meta_tag
template <size_t I>
struct priority_tag
  //!\cond
  // Doxygen fail
  : priority_tag<I - 1>
//!\endcond
{};

//!\brief Recursion anchor for bio::meta::priority_tag.
//!\ingroup meta_tag
template <>
struct priority_tag<0>
{};

} // namespace bio::meta
