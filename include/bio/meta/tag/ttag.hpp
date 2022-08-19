// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::ttag.
 */

#pragma once

#include <bio/meta/type_list/type_list.hpp>

namespace bio::meta
{

/*!\brief A type-tag template.
 * \tparam type The first type to store.
 * \tparam more_types More types to store (optional).
 * \ingroup meta_tag
 * \see bio::meta::type_list
 *
 * \details
 *
 * Using this template, you can easily turn a type into a compile-time constant (value).
 *
 * ### Example
 *
 * \snippet test/snippet/meta/tag/ttag.cpp ttag
 */
template <typename type, typename... more_types>
inline constinit type_list<type, more_types...> ttag{};

} // namespace bio::meta
