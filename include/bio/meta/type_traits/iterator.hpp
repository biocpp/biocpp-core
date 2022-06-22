// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides various transformation traits for use on iterators.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <iterator>
#include <type_traits>

#include <bio/meta/platform.hpp>
#include <iterator>

namespace bio::detail
{
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
 * \implements bio::transformation_trait
 * \tparam it_t The type to operate on.
 * \ingroup type_traits
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
 * \ingroup type_traits
 *
 * \attention
 * If [std::iterator_traits<it_t>::iterator_category](https://en.cppreference.com/w/cpp/iterator/iterator_traits)
 * is defined for a type `it_t`, this transformation trait acts as an alias for it.
 *
 * If not, this means that `it_t` is no "legacy" iterator and this transformation trait will give a substitution error.
 */
template <typename it_t>
    //!\cond
    requires(requires { typename std::iterator_traits<it_t>::iterator_category; })
//!\endcond
using iterator_category_tag_t = typename std::iterator_traits<it_t>::iterator_category;
#endif // BIOCPP_WORKAROUND_GCC_96070

/*!\brief Exposes the
 * [iterator_concept](https://en.cppreference.com/w/cpp/iterator/iterator_tags) from the modelled concept.
 * \implements bio::transformation_trait
 * \tparam it_t The type to operate on.
 * \ingroup type_traits
 */
template <typename it_t>
    //!\cond
    requires std::input_or_output_iterator<it_t>
//!\endcond
using iterator_concept_tag_t = std::conditional_t<
  std::contiguous_iterator<it_t>,
  std::contiguous_iterator_tag,
  std::conditional_t<std::random_access_iterator<it_t>,
                     std::random_access_iterator_tag,
                     std::conditional_t<std::bidirectional_iterator<it_t>,
                                        std::bidirectional_iterator_tag,
                                        std::conditional_t<std::forward_iterator<it_t>,
                                                           std::forward_iterator_tag,
                                                           std::conditional_t<std::input_iterator<it_t>,
                                                                              std::input_iterator_tag,
                                                                              std::output_iterator_tag>>>>>;

} // namespace bio::detail

namespace bio::detail
{
// ----------------------------------------------------------------------------
// iter_pointer
// ----------------------------------------------------------------------------

/*!\brief This is like std::iter_value_t, but for the pointer type.
 * \implements bio::transformation_trait
 * \tparam it_t The type to operate on.
 * \see bio::detail::iter_pointer_t
 * \ingroup type_traits
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
 * \ingroup type_traits
 */
template <typename it_t>
using iter_pointer_t = typename iter_pointer<it_t>::type;

} // namespace bio::detail
