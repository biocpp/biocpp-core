// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Module header with module-wide definitions.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <bio/meta/platform.hpp> // TODO: change to meta/biocpp.hpp

namespace bio::ranges
{}

namespace bio::ranges::views
{}

#ifndef BIOCPP_NO_NAMESPACE_ALIASES
namespace bio::ranges
{
namespace v = views;
}

namespace bio
{
namespace r     = ranges;
namespace views = ranges::views;
} // namespace bio
#endif
