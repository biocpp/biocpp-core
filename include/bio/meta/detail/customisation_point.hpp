// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Helper utilities for defining customisation point objects.
 */

#pragma once

#include <bio/meta/platform.hpp>

namespace bio::meta::detail
{

// ============================================================================
// priority_tag
// ============================================================================

//!\brief A tag that allows controlled overload resolution via implicit base conversion rules.
template <size_t I>
struct priority_tag
  //!\cond
  // Doxygen fail
  : priority_tag<I - 1>
//!\endcond
{};

//!\brief Recursion anchor for bio::detail::priority_tag.
template <>
struct priority_tag<0>
{};

} // namespace bio::meta::detail

// ============================================================================
// BIOCPP_CPO_IMPL
// ============================================================================

// clang-format off
//!\brief A macro that helps defining the overload set of a customisation point.
#define BIOCPP_CPO_IMPL(PRIO, TERM)                                                                                  \
/*!\brief A customisation point overload.*/                                                                          \
template <typename t, typename ...arg_ts>                                                                            \
static constexpr decltype(auto) impl(meta::detail::priority_tag<PRIO>,                                               \
                                     [[maybe_unused]] t && v,                                                        \
                                     [[maybe_unused]] arg_ts && ... args)                                            \
    noexcept(noexcept(TERM))                                                                                         \
    requires (requires (meta::detail::priority_tag<PRIO> const &/*<- need for doxygen*/, t && v, arg_ts && ... args) \
    { { TERM }; })                                                                                                   \
{                                                                                                                    \
    return TERM;                                                                                                     \
}

// clang-format on
