// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::type_list.
 */

#pragma once

#include <bio/meta/type_traits/template_inspection.hpp>

namespace bio::meta
{

// ----------------------------------------------------------------------------
// type_list class
// ----------------------------------------------------------------------------

/*!\brief Type that contains multiple types.
 * \ingroup meta_type_list
 */
template <typename... types>
struct type_list
{
    //!\brief The type list itself
    using type = type_list;

    //!\brief The number of types contained in the type list
    static constexpr size_t size() noexcept { return sizeof...(types); }
};

} // namespace bio::meta

// ----------------------------------------------------------------------------
// is_type_list trait
// ----------------------------------------------------------------------------

namespace bio::meta::detail
{

//!\brief Check whether a type is a specialisation of seqan3::type_list.
template <typename t>
inline constexpr bool is_type_list = false;

//!\brief Check whether a type is a specialisation of seqan3::type_list.
template <typename... ts>
inline constexpr bool is_type_list<type_list<ts...>> = true;

} // namespace bio::meta::detail
