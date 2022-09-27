// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::ignore_t.
 */

#pragma once

#include <tuple>

#include <bio/core.hpp>

namespace bio::meta
{

//!\brief The type of std::ignore (with cvref removed).
//!\ingroup meta
using ignore_t = std::remove_cvref_t<decltype(std::ignore)>;

} // namespace bio::meta
