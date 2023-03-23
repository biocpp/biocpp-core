// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides various transformation traits used by the range module.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <iterator>
#include <ranges>
#include <type_traits>

#include <bio/core.hpp>
#include <bio/meta/type_traits/basic.hpp>

namespace bio::ranges::detail
{

// ----------------------------------------------------------------------------
// iterator_category_tag
// ----------------------------------------------------------------------------

#if BIOCPP_WORKAROUND_GCC_96070
//!\cond
template <typename it_t>
struct iterator_category_tag
{
    using type = void;
};

template <typename it_t>
    requires(requires { typename std::iterator_traits<it_t>::iterator_category; })
struct iterator_category_tag<it_t>
{
    using type = typename std::iterator_traits<it_t>::iterator_category;
};
//!\endcond
/*!\brief Exposes the
 * [iterator_category](https://en.cppreference.com/w/cpp/iterator/iterator_tags) from the modelled concept.
 * \implements bio::meta::transformation_trait
 * \tparam it_t The type to operate on.
 * \ingroup range
 *
 * \attention
 * If [std::iterator_traits<it_t>::iterator_category](https://en.cppreference.com/w/cpp/iterator/iterator_traits)
 * is defined for a type `it_t`, this transformation trait acts as an alias for it, if not it is `void`.
 */
template <typename it_t>
using iterator_category_tag_t = typename iterator_category_tag<it_t>::type;
#else  // ^^^ workaround / no workaround vvv
// TODO: Change the description / the definition of iterator_category_tag_t depending on how the standard resolves this
// https://github.com/seqan/product_backlog/issues/151

/*!\brief Exposes the
 * [iterator_category](https://en.cppreference.com/w/cpp/iterator/iterator_tags) from the modelled concept.
 * \tparam it_t The type to operate on.
 * \ingroup range
 *
 * \attention
 * If [std::iterator_traits<it_t>::iterator_category](https://en.cppreference.com/w/cpp/iterator/iterator_traits)
 * is defined for a type `it_t`, this transformation trait acts as an alias for it.
 *
 * If not, this means that `it_t` is no "legacy" iterator and this transformation trait will give a substitution error.
 */
template <typename it_t>
    requires(requires { typename std::iterator_traits<it_t>::iterator_category; })
using iterator_category_tag_t = typename std::iterator_traits<it_t>::iterator_category;
#endif // BIOCPP_WORKAROUND_GCC_96070

// ----------------------------------------------------------------------------
// iterator_concept_tag_
// ----------------------------------------------------------------------------

/*!\brief Exposes the
 * [iterator_concept](https://en.cppreference.com/w/cpp/iterator/iterator_tags) from the modelled concept.
 * \implements bio::meta::transformation_trait
 * \tparam it_t The type to operate on.
 * \ingroup range
 */
template <typename it_t>
    requires std::input_or_output_iterator<it_t>
// clang-format off
using iterator_concept_tag_t = std::conditional_t<std::contiguous_iterator<it_t>,    std::contiguous_iterator_tag,
                               std::conditional_t<std::random_access_iterator<it_t>, std::random_access_iterator_tag,
                               std::conditional_t<std::bidirectional_iterator<it_t>, std::bidirectional_iterator_tag,
                               std::conditional_t<std::forward_iterator<it_t>,       std::forward_iterator_tag,
                               std::conditional_t<std::input_iterator<it_t>,         std::input_iterator_tag,
                                                                                     std::output_iterator_tag>>>>>;
// clang-format on

// ----------------------------------------------------------------------------
// iter_pointer
// ----------------------------------------------------------------------------

/*!\brief This is like std::iter_value_t, but for the pointer type.
 * \implements bio::meta::transformation_trait
 * \tparam it_t The type to operate on.
 * \see bio::detail::iter_pointer_t
 * \ingroup range
 *
 * \attention
 * C++20 does not provide a `std::iter_pointer_t`, because the new C++20 iterators do not need to provide a pointer
 * type.
 */
template <typename it_t>
struct iter_pointer
{
    //!\brief The pointer type of std::iterator_traits or void.
    using type = void;
};

//!\cond
template <typename it_t>
    requires(requires { typename std::iterator_traits<it_t>::pointer; })
struct iter_pointer<it_t>
{
    //!\brief This is defined for every legacy input-iterator.
    //!\sa https://en.cppreference.com/w/cpp/iterator/iterator_traits
    using type = typename std::iterator_traits<it_t>::pointer;
};
//!\endcond

/*!\brief Return the `pointer` type of the input type (transformation_trait shortcut).
 * \tparam it_t The type to operate on.
 * \see bio::detail::iter_pointer
 * \ingroup range
 */
template <typename it_t>
using iter_pointer_t = typename iter_pointer<it_t>::type;

//!\cond
template <typename t>
concept has_range_value_type = requires { typename std::ranges::range_value_t<std::remove_cvref_t<t>>; };
//!\endcond

} // namespace bio::ranges::detail

namespace bio::ranges
{

/*!\addtogroup range
 * \{
 */

// ----------------------------------------------------------------------------
// range_innermost_value
// ----------------------------------------------------------------------------

//NOTE(h-2): this could be moved to a separate file, because it also applies to iterators

/*!\brief Recursively determines the `value_type` on containers and/or iterators.
 * \implements bio::meta::transformation_trait
 * \tparam t The type to recurse on; must have `std::ranges::value_type_t<rng_t>`.
 *
 * \details
 *
 * Attention, this transformation trait implicitly removes cv-qualifiers on all value_types except the one returned.
 */
template <typename t>
    requires detail::has_range_value_type<t>
struct range_innermost_value
{
    //!\brief The return type (recursion not shown).
    using type = std::ranges::range_value_t<std::remove_cvref_t<t>>;
};

//!\cond
template <typename t>
    requires(detail::has_range_value_type<t> &&
             detail::has_range_value_type<std::ranges::range_value_t<std::remove_cvref_t<t>>>)
struct range_innermost_value<t>
{
    using type = typename range_innermost_value<std::ranges::range_value_t<std::remove_cvref_t<t>>>::type;
};
//!\endcond

//!\brief Shortcut for bio::ranges::range_innermost_value (transformation_trait shortcut).
//!\see bio::ranges::range_innermost_value
template <typename t>
using range_innermost_value_t = typename range_innermost_value<t>::type;

// ----------------------------------------------------------------------------
// range_dimension_v
// ----------------------------------------------------------------------------

/*!\brief Returns the number of times you can call `std::ranges::value_type_t` recursively on t (type trait).
 * \tparam t The type to be queried; must resolve `std::ranges::value_type_t` at least once.
 *
 * \details
 *
 * Attention, this type trait implicitly removes cv-qualifiers and reference from the types it recurses on and
 * returns.
 */
template <typename t>
    requires detail::has_range_value_type<t>
constexpr size_t range_dimension_v = 1;

//!\cond
template <typename t>
    requires(detail::has_range_value_type<t> &&
             detail::has_range_value_type<std::ranges::range_value_t<std::remove_cvref_t<t>>>)
constexpr size_t range_dimension_v<t> = range_dimension_v<std::ranges::range_value_t<std::remove_cvref_t<t>>> + 1;
//!\endcond

// ----------------------------------------------------------------------------
// range_compatible
// ----------------------------------------------------------------------------

/*!\interface bio::ranges::range_compatible <>
 * \brief Two types are "compatible" if their bio::ranges::range_dimension_v and their bio::ranges::range_innermost_value_t are
 * the same.
 *
 * \details
 *
 * \include test/snippet/meta/type_traits/range.cpp
 *
 * Attention, this concept implicitly removes cv-qualifiers and reference from the types it recurses on and
 * compares.
 */
//!\cond
template <typename t1, typename t2>
concept range_compatible = requires(t1, t2) {
    requires(range_dimension_v<t1> == range_dimension_v<t2>);

    requires std::is_same_v<range_innermost_value_t<t1>, range_innermost_value_t<t2>>;
};
//!\endcond

//!\}

} // namespace bio::ranges
