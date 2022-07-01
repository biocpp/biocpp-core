// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides algorithms for meta programming, parameter packs and bio::meta::type_list.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include <bio/meta/type_list/type_list.hpp>

namespace bio::meta::detail
{

//-----------------------------------------------------------------------------
// all_of
//-----------------------------------------------------------------------------

/*!\brief Tests whether a given predicate evaluates to `true` for each type in a bio::meta::type_list.
 * \ingroup meta_type_list
 *
 * \tparam list_t A type list; must model bio::detail::template_specialisation_of a bio::meta::type_list
 * \tparam unary_predicate_t The function type, like function pointers, functors and lambdas;
 *                           must model std::predicate expanded on each argument type wrapped in std::type_identity.
 *
 * \param[in] fn The predicate called for every type in the bio::meta::type_list.
 *
 * \returns `true` if the predicate returns `true` for each type in the type list, `false` otherwise.
 *
 * \details
 *
 * This function operates on types instead of values.
 * The following steps are performed to call the passed predicate on the types contained in the type list:
 *
 *  * expand the types within the type list
 *  * wrap each type in std::identity and instantiate it (i.e. transform a type into a value)
 *  * call the functor on each of those (combined via `&&`)
 *
 * Note that wrapping the types in std::type_identity is a technical trick to make a type representable as a value.
 * Instantiating a type might not work because they might not be std::default_initializable.
 * In addition it is possible, to invoke the predicate on incomplete types.
 *
 * ### Example
 *
 * \include test/snippet/meta/type_list/detail/type_list_algorithm_all_of.cpp
 *
 * ### Complexity
 *
 * Linear in the number of types in the bio::meta::type_list.
 *
 * [Compile-time complexity: Linear number of template instantiations.]
 */
template <typename type_list_t, typename unary_predicate_t>
[[nodiscard]] constexpr bool all_of(unary_predicate_t && fn)
  //!\cond
  requires template_specialisation_of<type_list_t, type_list>
//!\endcond
{
    return [&]<typename... ts>(type_list<ts...>) { return (fn(std::type_identity<ts>{}) && ...); }(type_list_t{});
}

//-----------------------------------------------------------------------------
// for_each
//-----------------------------------------------------------------------------

/*!\brief Applies a function element wise to all types of a type list.
 * \ingroup meta_type_list
 *
 * \tparam list_t A type list; must model bio::detail::template_specialisation_of a bio::meta::type_list.
 * \tparam unary_function_t The function type, like function pointers, functors and lambdas; must model
 *                          std::invocable on each type of the type list wrapped in std::type_identity.
 *
 * \param[in] fn The function to call on every type contained in the list.
 *
 * \details
 *
 * This function operates on types instead of values.
 * The following steps are performed to call the passed unary function on the types contained in the type list:
 *
 *  * expand the types within the type list
 *  * wrap each type in std::identity and instantiate it (i.e. transform a type into a value)
 *  * call the parameter pack version of bio::meta::detail::for_each with the instances of std::identity (as a pack).
 *
 * Note that wrapping the types in std::type_identity is a technical trick to make a type representable as a value.
 * Instantiating a type might not work because they might not be std::default_initializable.
 * In addition, it is possible to invoke the unary function on incomplete types.
 *
 * ### Example
 *
 * \include test/snippet/meta/type_list/detail/type_list_algorithm_for_each.cpp
 *
 * ### Complexity
 *
 * Linear in the number of types in the bio::meta::type_list.
 *
 * [Compile-time complexity: Linear number of template instantiations.]
 *
 * \sa bio::meta::detail::for_each
 */
template <typename type_list_t, typename unary_function_t>
    //!\cond
    requires template_specialisation_of<type_list_t, bio::meta::type_list>
//!\endcond
constexpr void for_each(unary_function_t && fn)
{
    return [&]<typename... ts>(type_list<ts...>) { (fn(std::type_identity<ts>{}), ...); }(type_list_t{});
}

} // namespace bio::meta::detail
