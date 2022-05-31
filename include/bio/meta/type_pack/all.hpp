// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 * \brief Meta-header for the \link core_type_pack Utility / Type Pack submodule \endlink.
 */

/*!\defgroup core_type_pack Type Pack
 * \brief Provides metaprogramming utilities for working on template parameter packs.
 * \ingroup core
 * \see core
 *
 * \details
 *
 * This module contains auxiliary functions, algorithms and traits on
 * [template parameter packs](https://en.cppreference.com/w/cpp/language/parameter_pack).
 * All traits on a template parameter pack are defined in the header `<seqan3/core/type_pack/traits.hpp>`.
 */

/*!\namespace bio::pack_traits
 * \brief Namespace containing traits for working on type packs.
 */

#pragma once

#include <bio/meta/type_pack/traits.hpp>
