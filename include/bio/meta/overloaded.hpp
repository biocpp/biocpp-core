// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::overloaded.
 */

#pragma once

#include <bio/core.hpp>

namespace bio::meta
{

/*!\addtogroup meta
 * \{
 */

/*!\brief Wrapper to create an overload set of multiple functors.
 * \tparam functors The types of the functors (usually deduced).
 *
 * This helper allows creating a function object that statically dispatches between
 * other function objects (usually provided via lambda expressions).
 *
 * ### Example
 *
 * \include test/snippet/meta/overloaded.cpp
 */
template <typename... functors>
struct overloaded : functors...
{
    using functors::operator()...;
};

//!\brief Deduction guide for bio::meta::overloaded.
//!\relates bio::meta::overloaded
template <typename... functors>
overloaded(functors...) -> overloaded<functors...>;
//!\}

} // namespace bio::meta
