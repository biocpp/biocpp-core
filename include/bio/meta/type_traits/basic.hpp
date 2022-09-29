// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides various type traits on generic types.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <tuple>
#include <type_traits>

#include <bio/meta/concept/core_language.hpp>

namespace bio::meta
{

/*!\addtogroup type_traits
 * \{
 */

// ----------------------------------------------------------------------------
// default_initialisable_wrap_t
// ----------------------------------------------------------------------------

/*!\brief Return t as t if it is noexcept- and constexpr-default-constructible; else wrap it in std::type_identity.
 * \tparam t The type to operate on.
 * \relates bio::meta::constexpr_default_initializable
 */
template <typename t>
using default_initialisable_wrap_t =
  std::conditional_t<std::is_nothrow_default_constructible_v<std::remove_cvref_t<t>> &&
                       constexpr_default_initializable<std::remove_cvref_t<t>>,
                     std::remove_cvref_t<t>,
                     std::type_identity<std::remove_cvref_t<t>>>;

// ----------------------------------------------------------------------------
// ignore_t
// ----------------------------------------------------------------------------

//!\brief Return the type of std::ignore with `const`, `volatile` and references removed (type trait).
using ignore_t = std::remove_cvref_t<decltype(std::ignore)>;

/*!\brief Return whether the input type with `const`, `volatile` and references removed is std::ignore's type.
 * (type trait).
 * \tparam t The type to operate on.
 */
template <typename t>
inline constexpr bool decays_to_ignore_v = std::is_same_v<std::remove_cvref_t<t>, ignore_t>;

//!\}

} // namespace bio::meta
