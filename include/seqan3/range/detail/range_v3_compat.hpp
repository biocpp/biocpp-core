// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Make sure we play well with range-v3.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <iterator>
#include <ranges>

// TODO(bio): make sure this is included automatically everywhere

#if __has_include(<range/v3/range/concepts.hpp>)

#include <range/v3/range/concepts.hpp>

namespace ranges
{
//!\brief std::ranges::views are valid range-v3 views
template<class T>
//!\cond
    requires ::std::derived_from<T, ::std::ranges::view_base>
//!\endcond
inline constexpr bool enable_view<T> = true;

// std::ranges::borrowed_range's are valid range-v3 borrowed_range's
//!\cond
template <::std::input_or_output_iterator I, ::std::sentinel_for<I> S, ::std::ranges::subrange_kind K>
inline constexpr bool enable_borrowed_range<::std::ranges::subrange<I, S, K>> = true;

template <class T>
inline constexpr bool enable_borrowed_range<::std::ranges::empty_view<T>> = true;

template <::std::weakly_incrementable W, ::std::semiregular Bound>
inline constexpr bool enable_borrowed_range<::std::ranges::iota_view<W, Bound>> = true;

template <class T>
inline constexpr bool enable_borrowed_range<::std::ranges::ref_view<T>> = true;
//!\endcond

} // namespace ranges

namespace std::ranges
{
//!\brief range-v3 views are valid std::ranges::views
template<class T>
//!\cond
    requires ::std::derived_from<T, ::ranges::view_base>
//!\endcond
inline constexpr bool enable_view<T> = true;

//!\brief std::ranges::borrowed_range's are valid range-v3 borrowed_range's
template<class T>
//!\cond
    requires ::ranges::enable_borrowed_range<T>
//!\endcond
inline constexpr bool enable_borrowed_range<T> = true;
} // namespace std::ranges

#endif
