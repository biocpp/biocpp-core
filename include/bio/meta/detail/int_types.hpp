// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <limits>
#include <type_traits>

#include <bio/meta/platform.hpp>

/*!\file
 * \brief Provides metaprogramming utilities for integer types.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

namespace bio::meta::detail
{

// ------------------------------------------------------------------
// min_viable_uint_t
// ------------------------------------------------------------------

// clang-format off
//!\brief Given a value, return the smallest unsigned integer that can hold it.
template <uint64_t value>
using min_viable_uint_t = std::conditional_t<value <= 255ull,        uint8_t,
                          std::conditional_t<value <= 65535ull,      uint16_t,
                          std::conditional_t<value <= 4294967295ull, uint32_t, uint64_t>>>;
// clang-format on

//!\brief Given a value, cast the value as the smallest unsigned integer that can hold it.
//!\sa bio::meta::min_viable_uint_t
template <uint64_t value>
inline constexpr auto min_viable_uint_v = static_cast<min_viable_uint_t<value>>(value);

// ------------------------------------------------------------------
// size_in_values_v
// ------------------------------------------------------------------

//!\brief Return the number of values an integral type can have, i.e. the different between min and max.
template <std::integral int_t>
inline constexpr size_t size_in_values_v =
  static_cast<size_t>(std::numeric_limits<int_t>::max()) - std::numeric_limits<int_t>::lowest() + 1;

} // namespace bio::meta::detail
