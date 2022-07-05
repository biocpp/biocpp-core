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

/*!\namespace bio::ranges
 * \brief The ranges module's namespace.
 * \ingroup range
 */
namespace bio::ranges
{}

#ifndef BIOCPP_NO_NAMESPACE_ALIASES
namespace bio
{
namespace r = ranges;
} // namespace bio
#endif

/*!
 * \namespace bio::ranges::views
 * \brief The BioC++ namespace for views.
 * \ingroup views
 *
 * Since views often have name clashes with regular functions and ranges they are implemented in the sub
 * namespace `view`.
 *
 * See the \link views views submodule \endlink of the range module for more details.
 *
 * The namespace is aliased into `bio::` so you can write `bio::views::foo` instead of
 * `bio::ranges::views::foo`.
 */

namespace bio::ranges::views
{}

#ifndef BIOCPP_NO_NAMESPACE_ALIASES
namespace bio
{
namespace views = ranges::views;
} // namespace bio
#endif

/*!\if DEV
 * \namespace bio::ranges::detail
 * \brief The internal BioC++ namespace.
 * \ingroup alphabet
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */
namespace bio::ranges::detail
{}
