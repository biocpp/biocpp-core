// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::detail::deferred_crtp_base.
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

#include <bio/meta/platform.hpp>

namespace bio::detail
{

/*!\brief An invocable wrapper that defers the instantiation of a crtp_base class.
 * \ingroup type_traits
 * \tparam crtp_base The crtp base class to be deferred. Must be a template template parameter.
 * \tparam args_t    A type template parameter pack used to augment the `crtp_base` class.
 *
 * \details
 *
 * This transformation trait wrapper allows to defer the template instantiation of crtp-base classes. This can be useful
 * if the crtp_base class should be augmented with traits or other templates, especially when using variadic
 * crtp_bases. The help function bio::detail::invoke_deferred_crtp_base can be used to instantiate the
 * deferred crtp base with the respective derived type.
 *
 * ### Example
 *
 * The following snippet demonstrates the use of the deferred crtp base class instantiation.
 *
 * \include test/snippet/meta/type_traits/deferred_crtp_base.cpp
 *
 * \see bio::detail::invoke_deferred_crtp_base
 * \see bio::detail::deferred_crtp_base_vargs
 */
template <template <typename ...> typename crtp_base, typename ...args_t>
struct deferred_crtp_base
{
    /*!\brief Invokes the deferred crtp_base with the corresponding derived type.
     * \tparam derived_t The derived type to instantiate the crtp_base with.
     */
    template <typename derived_t>
    using invoke = crtp_base<derived_t, args_t...>;
};

/*!\brief An invocable wrapper that defers the instantiation of a crtp_base class.
 * \ingroup type_traits
 * \tparam crtp_base The crtp base class to be deferred. Must be a template template parameter.
 * \tparam args      A non-type template parameter pack used to augment the `crtp_base` class.
 *
 * \details
 *
 * This transformation trait wrapper allows to defer the template instantiation of crtp-base classes. This can be useful
 * if the crtp_base class should be augmented with traits or other templates, especially when using variadic
 * crtp_bases. The help function bio::detail::invoke_deferred_crtp_base can be used to instantiate the
 * deferred crtp base with the respective derived type. This class wrapper can only be used to augment a crtp-base
 * class with non-type arguments. See bio::detail::deferred_crtp_base for a version that accepts type template
 * parameters.
 *
 * ### Example
 *
 * The following snippet demonstrates the use of the deferred crtp base class instantiation.
 *
 * \include test/snippet/meta/type_traits/deferred_crtp_base.cpp
 *
 * \see bio::detail::invoke_deferred_crtp_base
 * \see bio::detail::deferred_crtp_base
 */
template <template <typename, auto ...> typename crtp_base, auto ...args>
struct deferred_crtp_base_vargs
{
    /*!\brief Invokes the deferred crtp_base with the corresponding derived type.
     * \tparam derived_t The derived type to instantiate the crtp_base with.
     */
    template <typename derived_t>
    using invoke = crtp_base<derived_t, args...>;
};

/*!\brief Template alias to instantiate the deferred crtp base with the derived class.
 * \ingroup type_traits
 * \tparam deferred_crtp_base_t The deferred crtp base class.
 * \tparam derived_t            The derived type to instantiate the crtp base class with.
 *
 * \details
 *
 * Effectively declares the type resulting from `deferred_crtp_base_t::template invoke<derived_t>`.
 *
 * \see bio::detail::deferred_crtp_base
 */
template <typename deferred_crtp_base_t, typename derived_t>
//!\cond
    requires requires { typename deferred_crtp_base_t::template invoke<derived_t>; }
//!\endcond
using invoke_deferred_crtp_base = typename deferred_crtp_base_t::template invoke<derived_t>;

} // namespace bio::detail
