// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides my_tuple for testing tuple utility.
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

#include <tuple>
#include <utility>

#include <bio/meta/type_traits/basic.hpp>

namespace bio
{
struct my_tuple
{
    int   el0;
    float el1;

    constexpr friend auto operator<=>(my_tuple const &, my_tuple const &) = default;
};

template <size_t elem>
constexpr auto & get(bio::my_tuple & t)
{
    static_assert(elem < 2);

    if constexpr (elem == 0)
        return t.el0;
    else
        return t.el1;
}

template <size_t elem>
constexpr auto const & get(bio::my_tuple const & t)
{
    static_assert(elem < 2);

    if constexpr (elem == 0)
        return t.el0;
    else
        return t.el1;
}

template <size_t elem>
constexpr auto && get(bio::my_tuple && t)
{
    static_assert(elem < 2);

    if constexpr (elem == 0)
        return std::move(t.el0);
    else
        return std::move(t.el1);
}

template <size_t elem>
constexpr auto const && get(bio::my_tuple const && t)
{
    static_assert(elem < 2);

    if constexpr (elem == 0)
        return std::move(t.el0);
    else
        return std::move(t.el1);
}

} // namespace bio

namespace std
{

template <>
struct tuple_size<bio::my_tuple>
{
    static constexpr size_t value = 2;
};

template <size_t elem_no>
struct tuple_element<elem_no, bio::my_tuple>
{
    using type = std::remove_cvref_t<decltype(get<elem_no>(std::declval<bio::my_tuple>()))>;
};

} // namespace std
