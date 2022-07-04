// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Meta-header for the \link meta meta module \endlink.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

// ============================================================================
// External concept implementations
// ============================================================================

#include <bio/meta/add_enum_bitwise_operators.hpp>
#include <bio/meta/concept/all.hpp>
#include <bio/meta/detail/all.hpp>
#include <bio/meta/platform.hpp>
#include <bio/meta/pod_tuple.hpp>
#include <bio/meta/tuple_utility.hpp>
#include <bio/meta/type_list/type_list.hpp>
#include <bio/meta/type_traits/all.hpp>

/*!\defgroup meta Meta
 * \brief Provides metaprogramming utilities, concepts and some helper classes.
 *
 * The meta module is used strongly by other modules, but the content is usually not relevant
 * to most users of the library.
 */

/*!\namespace bio
 * \brief The main BioC++ namespace.
 */

/*!\namespace bio::meta
 * \brief The Meta module's namespace.
 */

/*!\namespace bio::custom
 * \brief A namespace for third party and standard library specialisations of BioC++ customisation points.
 * \see \ref biocpp_customisation
 */

/*!\if DEV
 * \namespace bio::detail
 * \brief The internal BioC++ namespace.
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */

/*!\if DEV
 * \namespace bio::meta::detail
 * \brief The internal BioC++ namespace.
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \sa https://github.com/biocpp/biocpp-core/wiki/Documentation
 * \endif
 */
