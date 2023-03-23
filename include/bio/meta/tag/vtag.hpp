// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::meta::vtag.
 */

#pragma once

#include <numeric>
#include <tuple>
#include <type_traits>

#include <bio/core.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/ranges/container/small_string.hpp>

namespace bio::meta::detail
{

/*!\brief The same as ranges::small_string<capacity> except it only has one constructor.
 * \details
 *
 * When calling `1_vtag`, a numeric literal template is valid (as should be),
 * but also the string literal template is, because operator is invoked as
 * operator""_vtag<'c1'> and small_string is constructible from a single
 * character.
 *
 * To workaround this, we use a modified type that only has a constructor
 * for CString-literals.
 */
template <size_t capacity>
struct literal_buffer_string : ranges::small_string<capacity>
{
    //!\brief only one constructor to avoid ambiguity in the literal definitions
    template <size_t N>
    constexpr literal_buffer_string(char const (&_lit)[N]) noexcept : ranges::small_string<capacity>{_lit}
    {}
};

//!\brief Deduces literal_buffer_string from CString literal.
//!\relates literal_buffer_string
template <size_t N>
literal_buffer_string(char const (&)[N]) -> literal_buffer_string<N - 1>;

} // namespace bio::meta::detail

namespace bio::meta
{

/*!\brief The type of bio::meta::vtag. [Default "specialisation" for 0 arguments.]
 * \tparam more_vs Any number of values [only 0 arguments pick this specialisation].
 * \ingroup meta_tag
 * \see bio::meta::vtag
 */
template <auto... more_vs>
struct vtag_t
{
    //!\brief The number of values stored in the tag.
    static constexpr size_t size = 0;

    //!\brief The tag converted to a tuple.
    static constexpr auto as_tuple = std::tuple{};

    //!\brief A function that checks if a value is contained in the tag.
    static constexpr bool contains(auto &&) { return false; }

    //!\brief A function that returns the index of a value or ((size_t)-1) if the value is not found.
    static constexpr size_t index_of(auto &&) { return static_cast<size_t>(-1ULL); }

    //!\brief Equality comparison with other vtags.
    template <auto... v2s>
    consteval friend bool operator==(vtag_t, vtag_t<v2s...>) noexcept
    {
        return vtag_t::as_tuple == vtag_t<v2s...>::as_tuple;
    }

    //!\brief Ordered comparison with other vtags.
    template <auto... v2s>
    consteval friend auto operator<=>(vtag_t, vtag_t<v2s...>) noexcept
    {
        return vtag_t::as_tuple <=> vtag_t<v2s...>::as_tuple;
    }
};

/*!\brief The type of bio::meta::vtag. [Specialisation for 1 or more arguments]
 * \tparam v       First value.
 * \tparam more_vs More values.
 * \ingroup meta_tag
 * \see bio::meta::vtag
 */
template <auto v, auto... more_vs>
struct vtag_t<v, more_vs...>
{
    //!\brief The first value in the tag.
    static constexpr auto first_value = v;

    //!\copybrief bio::meta::vtag_t::size
    static constexpr size_t size = sizeof...(more_vs) + 1;

    //!\copybrief bio::meta::vtag_t::as_tuple
    static constexpr auto as_tuple = std::tuple{v, more_vs...};

    //!\brief Whether all values in the tag are unique.
    static constexpr bool unique_values()
        requires((weakly_equality_comparable_with<decltype(v), decltype(more_vs)> && ...))
    {
        return ((v != more_vs) && ...);
    }

    //!\copybrief bio::meta::vtag_t::contains
    static constexpr bool contains(auto && s)
        requires(weakly_equality_comparable_with<decltype(s), decltype(v)> &&
                 (weakly_equality_comparable_with<decltype(s), decltype(more_vs)> && ...))
    {
        return s == v || ((s == more_vs) || ...);
    }

    //!\copybrief bio::meta::vtag_t::index_of
    static constexpr size_t index_of(auto && s)
        requires(weakly_equality_comparable_with<decltype(s), decltype(v)> &&
                 (weakly_equality_comparable_with<decltype(s), decltype(more_vs)> && ...))
    {
        size_t c = 0;
        ((v != s && ++c) && ((more_vs != s && ++c) && ...));
        return c >= size ? static_cast<size_t>(-1ULL) : c;
    }

    /*!\brief Turns `-vtag<3>` into `vtag<-3>`.
     *
     * \details
     *
     * This function is only available for vtags of size 1 where the value
     * type is a number. The operator is necessary to allow negativ
     * literals.
     */
    consteval auto operator-()
        requires(arithmetic<decltype(v)> && size == 1)
    {
        return vtag_t<-v>{};
    }

    //!\copybrief bio::meta::vtag_t::operator==
    template <auto... v2s>
    consteval friend bool operator==(vtag_t, vtag_t<v2s...>) noexcept
    {
        return vtag_t::as_tuple == vtag_t<v2s...>::as_tuple;
    }

    //!\copybrief bio::meta::vtag_t::operator<=>
    template <auto... v2s>
    consteval friend auto operator<=>(vtag_t, vtag_t<v2s...>) noexcept
    {
        return vtag_t::as_tuple <=> vtag_t<v2s...>::as_tuple;
    }
};

/*!\brief A value-tag template.
 * \tparam vs The values to store in the tag.
 * \ingroup meta_tag
 * \details
 *
 * Using this template, you can easily turn a value, e.g. a literal value, into a compile-time constant with a unique
 * type.
 *
 * ### Example
 *
 * Dispatching between two overloads:
 * \snippet test/snippet/meta/tag/vtag.cpp dispatch
 *
 */
template <auto... vs>
inline constexpr vtag_t<vs...> vtag{};

inline namespace literals
{

/*!\brief String literal operator for creating vtags.
 * \tparam str A buffer for the literal.
 * \ingroup meta_tag
 * \details
 *
 * Creates bio::meta::vtag from a string literal.
 *
 * The returned tag will be of type
 * `bio::meta::vtag_t<bio::ranges::small_string<str.size()>{str}>`.
 *
 * ### Example
 *
 * The tags allow compile-time comparisons of the the string-literals:
 * \snippet test/snippet/meta/tag/vtag.cpp string_literal
 *
 * The allows very self-descriptive dispatching.
 *
 */
template <detail::literal_buffer_string str>
consteval auto operator""_vtag() noexcept
{
    return vtag<ranges::small_string<str.size()>{str}>;
}

/*!\brief Integer literal operator for creating vtags.
 * \tparam chrs The literal buffer.
 * \ingroup meta_tag
 * \details
 *
 * Creates bio::meta::vtag from a numeric literal.
 *
 * The returned tag will be of type
 * `bio::meta::vtag<int64_t{NUMBER}>`, where NUMBER is the integer you passed.
 *
 * ### Example
 *
 * Reduces the verbosity of tag-dispatching:
 * \snippet test/snippet/meta/tag/vtag.cpp int_literal
 *
 * Negative literals are supported:
 * \snippet test/snippet/meta/tag/vtag.cpp negative_int_literal
 */
template <char... chrs>
consteval auto operator""_vtag() noexcept
{
    static_assert(((chrs >= '0' && chrs <= '9') && ...), "Only integral digits supported.");

    constexpr int64_t val = []() constexpr
    {
        ranges::small_string str{chrs...};
        int64_t              ret  = 0;
        int64_t              mult = 1;
        // std::views::reverse is not constexpr on GCC10 :|
        for (int64_t i = str.size() - 1; i >= 0; --i)
        {
            ret += (str[i] - '0') * mult;
            mult *= 10;
        }
        return ret;
    }();
    return vtag<val>;
}

} // namespace literals

} // namespace bio::meta
