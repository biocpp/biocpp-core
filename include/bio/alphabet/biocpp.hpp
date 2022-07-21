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

/*!\namespace bio::alphabet
 * \brief The alphabet module's namespace.
 * \ingroup alphabet
 * \see \ref biocpp_customisation
 */

namespace bio::alphabet
{}

/*!\namespace bio::alphabet::cpo
 * \brief A namespace for third party and standard library specialisations of BioC++ customisation points.
 * \ingroup alphabet
 * \see \ref biocpp_customisation
 */

namespace bio::alphabet::cpo
{}

/*!\namespace bio::alphabet::literals
 * \brief An inline namespace for alphabet literals. It exists to safely allow `using namespace`.
 * \ingroup alphabet
 */

namespace bio::alphabet
{
inline namespace literals
{}
} // namespace bio::alphabet

/*!\if DEV
 * \namespace bio::alphabet::detail
 * \brief The internal BioC++ namespace.
 * \ingroup alphabet
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */

namespace bio::alphabet::detail
{}

#ifndef BIOCPP_NO_NAMESPACE_ALIASES
namespace bio
{
namespace a = alphabet;
} // namespace bio
#endif
