// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides unary type traits on a set of types, usually provided as template argument pack.
 */

#pragma once

#include <type_traits>

#include <bio/meta/platform.hpp>

namespace bio::meta::detail
{

//!\addtogroup type_traits
//!\{

//!\brief Indicates whether the first template argument is contained in the remaining.
//!\implements bio::meta::unary_type_trait
template <typename target_t, typename... pack>
struct type_in_pack : std::false_type
{};

//!\cond
template <typename target_t, typename... pack>
struct type_in_pack<target_t, target_t, pack...> : std::true_type
{};

template <typename target_t, typename pack1, typename... pack>
struct type_in_pack<target_t, pack1, pack...> : type_in_pack<target_t, pack...>
{};
//!\endcond

//!\brief Shortcut for bio::meta::detail::type_in_pack (unary_type_trait shortcut).
//!\relates bio::meta::detail::type_in_pack
template <typename target_t, typename... pack>
inline constexpr bool type_in_pack_v = type_in_pack<target_t, pack...>::value;

//!\}

} // namespace bio::meta::detail
