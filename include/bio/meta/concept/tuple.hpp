// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::tuple_like.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>

#include <bio/meta/pod_tuple.hpp>
#include <bio/meta/type_list/type_list.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <bio/meta/type_traits/template_inspection.hpp>

namespace bio::detail
{

/*!\interface bio::detail::tuple_size <>
 * \ingroup utility_tuple
 * \brief Subconcept definition for bio::tuple_like to test for std::tuple_size-interface.
 * \see bio::tuple_like
 */
//!\cond
template <typename tuple_t>
concept tuple_size = requires(tuple_t v)
{
    {
        std::tuple_size<tuple_t>::value
        } -> std::convertible_to<size_t>;
};
//!\endcond

/*!\interface bio::detail::tuple_get <>
 * \ingroup utility_tuple
 * \brief Subconcept definition for bio::tuple_like to test for std::get-interface.
 * \see bio::tuple_like
 */
//!\cond
template <typename tuple_t>
concept tuple_get = requires(tuple_t & v, tuple_t const & v_c)
{
    requires std::tuple_size_v<tuple_t> >
    0;

    typename std::tuple_element<0, tuple_t>::type;

    {
        get<0>(v)
        } -> std::convertible_to<typename std::tuple_element<0, tuple_t>::type>;
    //     requires weakly_assignable_from<decltype(get<0>(v)), typename std::tuple_element<0, tuple_t>::type>;
    //TODO check that the previous returns something that can be assigned to
    // unfortunately std::assignable_from requires lvalue-reference, but we want to accept xvalues too (returned
    // proxies)
    {
        get<0>(v_c)
        } -> std::convertible_to<typename std::tuple_element<0, tuple_t>::type>;
    {
        get<0>(std::move(v))
        } -> std::convertible_to<typename std::tuple_element<0, tuple_t>::type>;
    {
        get<0>(std::move(v_c))
        } -> std::convertible_to<typename std::tuple_element<0, tuple_t>::type const &&>;
};
//!\endcond

/*!\brief Transformation trait to expose the tuple element types as bio::type_list
 * \ingroup utility_tuple
 * \tparam tuple_t The tuple to extract the element types from.
 *
 * \returns A bio::type_list over the element types of the given tuple.
 * \see bio::detail::tuple_type_list_t
 */
template <detail::tuple_size tuple_t>
struct tuple_type_list
{
protected:
    //!\brief Helper function to extract the types using the tuple elements.
    template <size_t... Is>
    static constexpr auto invoke_to_type_list(std::index_sequence<Is...>)
    {
        return type_list<std::tuple_element_t<Is, tuple_t>...>{};
    }

public:
    //!\brief The generated bio::type_list.
    using type = decltype(invoke_to_type_list(std::make_index_sequence<std::tuple_size<tuple_t>::value>{}));
};

/*!\brief Helper type for bio::detail::tuple_type_list
 * \ingroup utility_tuple
 *
 * \see bio::detail::tuple_type_list
 */
template <detail::tuple_size tuple_t>
using tuple_type_list_t = typename tuple_type_list<tuple_t>::type;

/*!\brief Helper type function to check for std::totally_ordered on all elements of the given tuple type.
 * \ingroup utility_tuple
 */
template <typename... elements_t>
inline constexpr auto all_elements_model_totally_ordered(bio::type_list<elements_t...>)
  -> std::bool_constant<(std::totally_ordered<elements_t> && ... && true)>;

/*!\brief Helper type trait function to check for std::totally_ordered on all elements of the given tuple type.
 * \tparam tuple_t The tuple to check if all elements model std::totally_ordered.
 * \ingroup utility_tuple
 */
template <typename tuple_t>
    //!\cond
    requires(requires { {detail::all_elements_model_totally_ordered(tuple_type_list_t<tuple_t>{})}; })
//!\endcond
static constexpr bool all_elements_model_totally_ordered_v =
  decltype(detail::all_elements_model_totally_ordered(tuple_type_list_t<tuple_t>{}))::value;
} // namespace bio::detail

namespace bio
{

// ----------------------------------------------------------------------------
// tuple_like
// ----------------------------------------------------------------------------

/*!\interface bio::tuple_like
 * \extends std::totally_ordered
 * \ingroup utility_tuple
 * \brief Whether a type behaves like a tuple.
 *
 * \details
 *
 * Types that meet this concept are for example std::tuple, std::pair, std::array, bio::pod_tuple, bio::record.
 * The std::totally_ordered will only be required if all types contained in the tuple-like
 * data structure are themselves strict totally ordered.
 */
/*!\name Requirements for bio::tuple_like
 * \brief You can expect these (meta-)functions on all types that implement bio::tuple_like.
 * \{
 */
/*!\var         size_t std::tuple_size_v<type>
 * \brief       A unary type trait that holds the number of elements in the tuple.
 * \tparam      type The tuple-like type.
 * \relates     bio::tuple_like
 *
 * \details
 * \attention This is a concept requirement, not an actual function (however types satisfying this concept
 * will provide an implementation).
 */
/*!\typedef     std::tuple_elment_t<i, type>
 * \brief       A transformation trait that holds the type of elements in the tuple.
 * \tparam      i Index of the queried element type.
 * \tparam      type The tuple-like type.
 * \relates     bio::tuple_like
 *
 * \details
 * \attention This is a concept requirement, not an actual function (however types satisfying this concept
 * will provide an implementation).
 * \attention This constraint is **not enforced** since empty tuples are valid.
 */
/*!\fn              auto && std::get<i>(type && val)
 * \brief           Return the i-th element of the tuple.
 * \relates         bio::tuple_like
 * \tparam          i The index of the element to return (of type `size_t`).
 * \param[in,out]   val The tuple-like object to operate on.
 * \returns         The i-th value in the tuple.
 *
 * \details
 * \attention This is a concept requirement, not an actual function (however types satisfying this concept
 * will provide an implementation).
 * \attention This constraint is **not enforced** since empty tuples are valid.
 */
//!\}
//!\cond
template <typename t>
concept tuple_like = detail::tuple_size<std::remove_reference_t<t>> && requires(t v)
{
    typename detail::tuple_type_list<std::remove_cvref_t<t>>::type;

    // NOTE(rrahn): To check the full tuple_concept including the get interface and the std::totally_ordered
    //              we need to make some assumptions. In general these checks can only be executed if the tuple is not
    //              empty. Furthermore, the std::totally_ordered can only be checked if all elements in the
    //              tuple are strict_totally_ordered. This is done, by the fold expression in the second part.
    requires(std::tuple_size<std::remove_reference_t<t>>::value == 0) ||
      (detail::tuple_get<std::remove_cvref_t<t>> &&
       (!detail::all_elements_model_totally_ordered_v<std::remove_cvref_t<t>> ||
        std::totally_ordered<std::remove_cvref_t<t>>));
};
//!\endcond

/*!\interface bio::pair_like
 * \extends bio::tuple_like
 * \ingroup utility_tuple
 * \brief Whether a type behaves like a tuple with exactly two elements.
 *
 * \details
 *
 * Types that meet this concept are for example std::tuple, std::pair, std::array, bio::pod_tuple,
 * iff std::tuple_size equals `2`.
 */
//!\cond
template <typename t>
concept pair_like = tuple_like<t> && std::tuple_size_v<std::remove_reference_t<t>>
== 2;
//!\endcond

} // namespace bio
