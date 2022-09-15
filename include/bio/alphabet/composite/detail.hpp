// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides implementation detail for bio::alphabet::alphabet_variant and bio::alphabet::alphabet_tuple_base.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
// #include <bio/alphabet/detail/concept.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/type_list/type_list.hpp>
#include <bio/meta/type_traits/lazy.hpp>

namespace bio::alphabet::detail
{

// ------------------------------------------------------------------
// alphabet_tuple_like
// ------------------------------------------------------------------

/*!\interface bio::alphabet::detail::alphabet_tuple_like <>
 * \brief bio::alphabet::alphabet_tuple_base and its derivates model this concept.
 * \ingroup alphabet_composite
 *
 * \details
 *
 * This concept is necessary/helpful, because CRTP-specialisations cannot easily be tracked via regular inheritance or
 * specialisation mechanisms.
 */
//!\cond
template <typename t>
concept alphabet_tuple_like = requires
{
    requires t::biocpp_alphabet_tuple_like;
};
//!\endcond

// ------------------------------------------------------------------
// required_types
// ------------------------------------------------------------------

/*!\brief A bio::meta::type_list with types that the given type depends on.
 * \implements bio::meta::transformation_trait
 * \ingroup alphabet_composite
 *
 * \details
 *
 * The list is empty by default. This trait maybe used in metaprogramming to indicate that certain types need to be
 * complete and not depend on the given type to avoid recursive template instantiation.
 */
template <typename t>
struct required_types
{
    //!\brief The returned type.
    using type = meta::type_list<>;
};

/*!\brief A bio::meta::type_list with types that the given type depends on.
 *        [specialisation for bio::alphabet::alphabet_variant and derivates of bio::alphabet::alphabet_tuple_base].
 * \implements bio::meta::transformation_trait
 * \ingroup alphabet_composite
 *
 * \details
 *
 * Exposes for bio::alphabet::alphabet_tuple_base its components and for bio::alphabet::alphabet_variant its alternatives.
 */
template <typename t>
    //!\cond
    requires(requires { typename t::biocpp_required_types; })
//!\endcond
struct required_types<t>
{
    //!\brief The returned type.
    using type = typename t::biocpp_required_types;
};

/*!\brief A bio::meta::type_list with types that the given type depends on. [Trait shortcut]
 * \relates bio::alphabet::detail::required_types
 */
template <typename t>
using required_types_t = typename required_types<t>::type;

// ------------------------------------------------------------------
// recursive_required_types
// ------------------------------------------------------------------

//TODO: This can be replaced with metaprogramming magic once a few more functions land in list_traits.

/*!\brief Like bio::alphabet::detail::required_types, but recursive.
 * \implements bio::meta::transformation_trait
 * \ingroup alphabet_composite
 */
template <typename t>
struct recursive_required_types
{
    //!\brief The returned type.
    using type = meta::type_list<>;
};

/*!\brief Like bio::alphabet::detail::required_types, but recursive.
 * \implements bio::meta::transformation_trait
 * \ingroup alphabet_composite
 */
template <typename t>
    //!\cond
    requires(requires { typename t::biocpp_recursive_required_types; })
//!\endcond
struct recursive_required_types<t>
{
    //!\brief The returned type.
    using type = typename t::biocpp_recursive_required_types;
};

/*!\brief Shortcut for bio::alphabet::detail::recursive_required_types.
 * \relates bio::alphabet::detail::recursive_required_types
 */
template <typename t>
using recursive_required_types_t = typename recursive_required_types<t>::type;

// ------------------------------------------------------------------
// Callable concept helpers for meta::invoke
// ------------------------------------------------------------------

/*!\brief 'Callable' helper class that is invokable by meta::invoke.
 * \ingroup alphabet_composite
 * Returns a std::true_type if the `type` is constructable from `T`.
 */
template <typename T>
struct constructible_from
{
    //!\brief The returned type when invoked.
    template <typename type>
    using invoke = std::integral_constant<bool, std::is_constructible_v<type, T>>;
};

/*!\brief 'Callable' helper class that is invokable by meta::invoke.
 * \ingroup alphabet_composite
 * Returns a std::true_type if the `T` is implicitly convertible to `type`.
 */
template <typename T>
struct implicitly_convertible_from
{
    //!\brief The returned type when invoked.
    template <typename type>
    using invoke = std::integral_constant<bool, std::is_convertible_v<T, type>>;
};

/*!\brief 'Callable' helper class that is invokable by meta::invoke.
 * \ingroup alphabet_composite
 * Returns a std::true_type if the `type` is assignable from `T`.
 */
template <typename T>
struct assignable_from
{
    //!\brief The returned type when invoked.
    template <typename type>
    using invoke = std::integral_constant<bool, meta::weakly_assignable_from<type, T>>;
};

/*!\brief 'Callable' helper class that is invokable by meta::invoke.
 * \ingroup alphabet_composite
 * Returns a std::true_type if the `type` is weakly equality comparable to `T`.
 */
template <typename T>
struct weakly_equality_comparable_with_
{
    //!\brief The returned type when invoked.
    template <typename type>
    using invoke = std::integral_constant<bool, meta::weakly_equality_comparable_with<type, T>>;
};

/*!\brief 'Callable' helper class that is invokable by meta::invoke.
 * \ingroup alphabet_composite
 * Returns a std::true_type if the `type` is comparable via <,<=,>,>= to `T`.
 */
template <typename T>
struct weakly_ordered_with_
{
    //!\brief The returned type when invoked.
    template <typename type>
    using invoke = std::integral_constant<bool, meta::weakly_ordered_with<type, T>>;
};

// ------------------------------------------------------------------
// Concept traits helper
// ------------------------------------------------------------------

/*!\brief Binary type trait that behaves like the bio::meta::weakly_equality_comparable_with concept.
 * \ingroup alphabet_composite
 */
template <typename lhs_t, typename rhs_t>
struct weakly_equality_comparable_with_trait :
  std::integral_constant<bool, meta::weakly_equality_comparable_with<lhs_t, rhs_t>>
{};

/*!\brief Binary type trait that behaves like the bio::meta::weakly_ordered_with concept.
 * \ingroup alphabet_composite
 */
template <typename lhs_t, typename rhs_t>
struct weakly_ordered_with_trait : std::integral_constant<bool, meta::weakly_ordered_with<lhs_t, rhs_t>>
{};

} // namespace bio::alphabet::detail

// ------------------------------------------------------------------
// Forwards
// ------------------------------------------------------------------

namespace bio::alphabet
{

// forward
template <typename... alternative_types>
    //!\cond
    requires((detail::writable_constexpr_alphabet<alternative_types> && ...) &&
             (std::regular<alternative_types> && ...) && (sizeof...(alternative_types) >= 2))
//!\endcond
class alphabet_variant;

template <typename derived_type, typename... component_types>
    //!\cond
    requires((detail::writable_constexpr_semialphabet<component_types> && ...) &&
             (std::regular<component_types> && ...))
//!\endcond
class alphabet_tuple_base;

} // namespace bio::alphabet
