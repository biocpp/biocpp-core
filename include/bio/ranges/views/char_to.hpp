// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::char_to.
 */

#pragma once

#include <concepts>
#include <ranges>
#include <type_traits>

#include <bio/alphabet/cigar/cigar.hpp>
#include <bio/alphabet/custom/all.hpp>
#include <bio/meta/overloaded.hpp>
#include <bio/ranges/container/small_string.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/type_reduce.hpp>

#include "bio/alphabet/detail/to_lower.hpp"
#include "bio/alphabet/exception.hpp"
#include "bio/ranges/concept.hpp"

namespace bio::ranges::detail
{

// ---------------------------------------------------------------------------------------------------------------------
// char_to_cigar_view class
// ---------------------------------------------------------------------------------------------------------------------

/*!\brief The type returned by bio::views::char_to<bio::alphabet::cigar>
 * \tparam urng_t The type of the underlying range.
 * \implements std::ranges::view
 * \implements std::ranges::bidirectional_range
 * \ingroup views
 */
template <std::ranges::view urng_t>
    requires std::ranges::forward_range<urng_t>
class char_to_cigar_view : public std::ranges::view_interface<char_to_cigar_view<urng_t>>
{
private:
    //!\brief The underlying range.
    urng_t urange;

    //!\brief Iterator class.
    template <typename uit_t, typename usen_t>
    class iterator;

    //!\brief Type of the iterator.
    using it_t = iterator<std::ranges::iterator_t<urng_t>, std::ranges::sentinel_t<urng_t>>;
    //!\brief Type of the sentinel.
    using const_it_t =
      std::conditional_t<const_iterable_range<urng_t>,
                         iterator<std::ranges::iterator_t<urng_t const>, std::ranges::sentinel_t<urng_t const>>,
                         void>;

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    char_to_cigar_view()                                           = default; //!< Defaulted.
    char_to_cigar_view(char_to_cigar_view const &)                 = default; //!< Defaulted.
    char_to_cigar_view & operator=(char_to_cigar_view const &)     = default; //!< Defaulted.
    char_to_cigar_view(char_to_cigar_view &&) noexcept             = default; //!< Defaulted.
    char_to_cigar_view & operator=(char_to_cigar_view &&) noexcept = default; //!< Defaulted.
    ~char_to_cigar_view()                                          = default; //!< Defaulted.

    //!\brief Construct from any type (Note: the value will be copied into views::single).
    constexpr explicit char_to_cigar_view(urng_t _urange) : urange{std::move(_urange)} {}
    //!\}

    /*!\name Iterators
     * \{
     */
    /*!\brief Returns an iterator to the first element of the range.
     * \returns Iterator to the first element.
     *
     * \details
     *
     * This range is never empty so the returned iterator will never be equal to end().
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr auto begin() noexcept
    {
        return ++it_t{std::ranges::begin(urange), std::ranges::begin(urange), std::ranges::end(urange)};
    }

    //!\copydoc begin()
    constexpr auto begin() const noexcept requires const_iterable_range<urng_t>
    {
        return ++const_it_t{std::ranges::begin(urange), std::ranges::begin(urange), std::ranges::end(urange)};
    }

    /*!\brief Returns an iterator to the element following the last element of the range.
     * \returns Iterator to the end.
     *
     * \details
     *
     * This element acts as a placeholder; attempting to dereference it results in undefined behaviour.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr auto end() noexcept
    {
        if constexpr (std::ranges::common_range<urng_t>)
            return it_t{std::ranges::end(urange), std::ranges::begin(urange), std::ranges::end(urange)};
        else
            return std::ranges::end(urange);
    }

    //!\copydoc end()
    constexpr auto end() const noexcept requires const_iterable_range<urng_t>
    {
        if constexpr (std::ranges::common_range<urng_t>)
            return const_it_t{std::ranges::end(urange), std::ranges::begin(urange), std::ranges::end(urange)};
        else
            return std::ranges::end(urange);
    }
    //!\}
};

// ---------------------------------------------------------------------------------------------------------------------
// char_to_cigar_view::iterator class
// ---------------------------------------------------------------------------------------------------------------------

template <std::ranges::view urng_t>
    requires std::ranges::forward_range<urng_t>
template <typename uit_t, typename usen_t>
class char_to_cigar_view<urng_t>::iterator
{
private:
    //!\brief Placeholder value to mark "unset".
    static constexpr alphabet::cigar default_value{(2u << 28) - 1, alphabet::assign_char_to('B', alphabet::cigar_op{})};

    //!\brief Character is a digit.
    static constexpr bool is_digit(char const c) { return c >= '0' && c <= '9'; }

    //!\brief Character is alphaetical.
    static constexpr bool is_alpha(char const c)
    {
        return alphabet::detail::is_lower(c) || alphabet::detail::is_upper(c);
    }

    //!\brief Begin of get-area.
    uit_t           uit_left;
    //!\brief End of get-area.
    uit_t           uit_right;
    //!\brief Begin of underlying range. Conceptionally const.
    uit_t           ubeg;
    //!\brief End of underlying range. Conceptionally const.
    usen_t          usen;
    //!\brief A cache for the converted value.
    alphabet::cigar value = default_value;

    //!\brief Shortcuts for noexcept specifications.
    static constexpr bool nothrow_default_init =
      std::is_nothrow_default_constructible_v<uit_t> && std::is_nothrow_default_constructible_v<usen_t>;
    //!\brief Shortcuts for noexcept specifications.
    static constexpr bool nothrow_move_init =
      std::is_nothrow_move_constructible_v<uit_t> && std::is_nothrow_move_constructible_v<usen_t>;
    //!\brief Shortcuts for noexcept specifications.
    static constexpr bool nothrow_copy_init =
      std::is_nothrow_copy_constructible_v<uit_t> && std::is_nothrow_copy_constructible_v<usen_t>;
    //!\brief Shortcuts for noexcept specifications.
    static constexpr bool nothrow_move_assign =
      std::is_nothrow_move_assignable_v<uit_t> && std::is_nothrow_move_assignable_v<usen_t>;
    //!\brief Shortcuts for noexcept specifications.
    static constexpr bool nothrow_copy_assign =
      std::is_nothrow_copy_assignable_v<uit_t> && std::is_nothrow_copy_assignable_v<usen_t>;

public:
    /*!\name Associated types
     * \brief All are derived from the uit_t.
     * \{
     */

    //!\brief The difference type.
    using difference_type   = std::iter_difference_t<uit_t>;
    //!\brief The value type.
    using value_type        = alphabet::cigar;
    //!\brief The reference type.
    using reference         = alphabet::cigar;
    //!\brief The pointer type.
    using pointer           = void *;
    //!\brief The iterator category tag.
    using iterator_category = std::input_iterator_tag;
    //!\brief The iterator concept tag.
    using iterator_concept  = std::conditional_t<std::random_access_iterator<uit_t>,
                                                std::bidirectional_iterator_tag,
                                                detail::iterator_concept_tag_t<uit_t>>;
    //!\}

    /*!\name Constructors, destructor and assignment
     * \brief The exception specification is explicitly "inherited" to also work for pointers as base.
     * \{
     */
    constexpr iterator() noexcept(nothrow_default_init)                                = default; //!< Defaulted.
    constexpr iterator(iterator const & rhs) noexcept(nothrow_copy_init)               = default; //!< Defaulted.
    constexpr iterator(iterator && rhs) noexcept(nothrow_move_init)                    = default; //!< Defaulted.
    constexpr iterator & operator=(iterator const & rhs) noexcept(nothrow_copy_assign) = default; //!< Defaulted.
    constexpr iterator & operator=(iterator && rhs) noexcept(nothrow_move_assign)      = default; //!< Defaulted.
    ~iterator() noexcept(std::is_nothrow_destructible_v<uit_t>)                        = default; //!< Defaulted.

    //!\brief Delegate to base class if inheriting from non-pointer iterator.
    constexpr iterator(uit_t const it, uit_t const beg, usen_t const sen) noexcept(nothrow_copy_init) :
      uit_left{it}, uit_right{it}, ubeg{beg}, usen{sen}
    {}
    //!\}

    /*!\name Comparison operators
     * \{
     */

    //!\brief Comparison for iterators.
    constexpr friend bool operator==(iterator const & lhs, iterator const & rhs) noexcept(
      noexcept(std::declval<uit_t &>() == std::declval<uit_t &>()))
    {
        return lhs.uit_left == rhs.uit_left;
    }

    //!\brief Comparison with sentinel (in case range is not common).
    constexpr friend bool operator==(iterator const & lhs, usen_t const & rhs) noexcept(
      noexcept(std::declval<uit_t &>() == std::declval<usen_t &>()))
      //!\cond
      requires(!std::same_as<uit_t, usen_t>)
    //!\endcond
    {
        return lhs.uit_left == rhs;
    }

    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr friend auto operator<=>(iterator const & lhs, iterator const & rhs) noexcept(
      noexcept(std::declval<uit_t &>() < std::declval<uit_t &>()))
      //!\cond
      requires std::totally_ordered<uit_t>
    //!\endcond
    {
        return lhs.uit_left <=> rhs.uit_left;
    }
    //!\}

    /*!\name Arithmetic operators
     * \brief Unless specialised in iteratorype, all operators perform uit_t's operator and cast to iterator.
     * \{
    */
    //!\brief Pre-increment, return updated iterator.
    constexpr iterator & operator++()
    {
        value = default_value;

        if (uit_left != uit_right) // every increment except the first
            uit_left = uit_right;

        if (uit_left == usen) // at end
            return *this;

        while (!is_alpha(*uit_right))
        {
            assert(is_digit(*uit_right));
            ++uit_right;
            assert(uit_right != usen);
        }

        ++uit_right; // go behind alpha

        return *this;
    }

    //!\brief Post-increment..
    constexpr auto operator++(int)
    {
        auto copy = *this;
        ++(*this);
        return copy;
    }

    //!\brief Pre-decrement, return updated iterator.
    constexpr iterator & operator--()
      //!\cond
      requires(requires(uit_t i) { --i; })
    //!\endcond
    {
        assert(uit_left != ubeg);

        value = default_value;

        uit_right = uit_left;

        --uit_left;
        assert(is_alpha(*uit_left));

        do
        {
            --uit_left;
        }
        while (is_digit(*uit_left) && uit_left != ubeg);

        if (uit_left != ubeg)
            ++uit_left;

        return *this;
    }

    //!\brief Post-decrement, return previous iterator state.
    constexpr auto operator--(int)
      //!\cond
      requires(requires(uit_t i) { --i; })
    //!\endcond
    {
        auto copy = *this;
        --(*this);
        return copy;
    }
    //!\}

    /*!\name Reference/Dereference operators
     * \{
     */
    //!\brief Dereference operator returns element currently pointed at.
    constexpr value_type operator*()
    {
        if (value == default_value)
        {
            if constexpr (std::contiguous_iterator<uit_t>)
            {
                std::string_view v{uit_left, uit_right};
                value.assign_string(v);
            }
            else
            {
                std::string s;
                std::ranges::copy(uit_left, uit_right, std::back_inserter(s));
                value.assign_string(s);
            }
        }
        return value;
    }

    //!\brief Dereference operator returns element currently pointed at.
    constexpr value_type operator*() const
    {
        alphabet::cigar c;

        if constexpr (std::contiguous_iterator<uit_t>)
        {
            std::string_view v{uit_left, uit_right};
            c.assign_string(v);
        }
        else
        {
            std::string s;
            std::ranges::copy(uit_left, uit_right, std::back_inserter(s));
            c.assign_string(s);
        }
        return c;
    }

    //!\brief Return pointer to this iterator.
    constexpr pointer operator->() noexcept(noexcept(*std::declval<uit_t &>()))
      //!\cond
      requires std::input_iterator<uit_t>
    //!\endcond
    {
        return this;
    }

    //!\brief Return pointer to this iterator.
    constexpr decltype(auto) operator->() const noexcept(noexcept(*std::declval<uit_t const &>()))
      //!\cond
      requires std::input_iterator<uit_t>
    //!\endcond
    {
        return this;
    }
    //!\}
};

} // namespace bio::ranges::detail

// ---------------------------------------------------------------------------------------------------------------------
// enable_borrowed_range
// ---------------------------------------------------------------------------------------------------------------------

//!\cond
// Mark char_to_cigar_view as conditionally borrowed.
template <typename urng_t>
inline constexpr bool std::ranges::enable_borrowed_range<bio::ranges::detail::char_to_cigar_view<urng_t>> =
  std::ranges::borrowed_range<urng_t>;
//!\endcond

// ---------------------------------------------------------------------------------------------------------------------
// views::char_to
// ---------------------------------------------------------------------------------------------------------------------

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief               A view over an alphabet, given a range of characters.
 * \tparam urng_t       The type of the range being processed. See below for requirements. [template parameter is
 *                      omitted in pipe notation]
 * \tparam alphabet_t   The alphabet to convert to; must satisfy bio::alphabet::alphabet.
 * \param[in] urange    The range being processed. [parameter is omitted in pipe notation]
 * \returns             A range of converted elements. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/char_to.hpp}
 *
 * This adaptor returns a view that transforms strings into ranges over `alphabet_t`.
 *
 * ### View properties (generic case)
 *
 * This view is a **deep view:** Given a range-of-range as input (as opposed to just a range), it will apply
 * the transformation on the innermost range (instead of the outermost range).
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)   |
 * |----------------------------------|:---------------------------------:|:--------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                      |
 * | std::ranges::forward_range       |                                   | *preserved*                      |
 * | std::ranges::bidirectional_range |                                   | *preserved*                      |
 * | std::ranges::random_access_range |                                   | *preserved*                      |
 * | std::ranges::contiguous_range    |                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                     |
 * | std::ranges::view                |                                   | *guaranteed*                     |
 * | std::ranges::sized_range         |                                   | *preserved*                      |
 * | std::ranges::common_range        |                                   | *preserved*                      |
 * | std::ranges::output_range        |                                   | *lost*                           |
 * | std::ranges::borrowed_range      |                                   | *lost*                           |
 * | bio::ranges::const_iterable_range|                                   | *preserved*                      |
 * |                                  |                                   |                                  |
 * | std::ranges::range_reference_t   | bio::alphabet::char_t<alphabet_t> | `alphabet_t`                     |
 *
 * See the \link views views submodule documentation \endlink for detailed descriptions of the view properties.
 *
 * ### View properties (NOOP case)
 *
 * If a range is given whose value_type is the same its alphabet type (e.g. std::string), a simple view
 * to the range is returned and no transformation happens. See bio::ranges::views::type_reduce.
 *
 * ### View properties (bio::alphabet::cigar)
 *
 * This range adaptor can be applied to ranges over bio::alphabet::cigar (although that type  does not
 * model bio::alphabet::alphabet). The returned view is weaker than in the generic case, and
 * **it is not a deep view.**
 *
 * | Concepts and traits              | `urng_t` (underlying range type)  | `rrng_t` (returned range type)   |
 * |----------------------------------|:---------------------------------:|:--------------------------------:|
 * | std::ranges::input_range         | *required*                        | *preserved*                      |
 * | std::ranges::forward_range       | *required*                        | *preserved*                      |
 * | std::ranges::bidirectional_range |                                   | *preserved*                      |
 * | std::ranges::random_access_range |                                   | *lost*                           |
 * | std::ranges::contiguous_range    |                                   | *lost*                           |
 * |                                  |                                   |                                  |
 * | std::ranges::viewable_range      | *required*                        | *guaranteed*                     |
 * | std::ranges::view                |                                   | *guaranteed*                     |
 * | std::ranges::sized_range         |                                   | *lost*                           |
 * | std::ranges::common_range        |                                   | *preserved*                      |
 * | std::ranges::output_range        |                                   | *lost*                           |
 * | std::ranges::borrowed_range      |                                   | *preserved*                      |
 * | bio::ranges::const_iterable_range|                                   | *preserved*                      |
 * |                                  |                                   |                                  |
 * | std::ranges::range_reference_t   | char                              | bio::alphabet::cigar             |
 *
 * Since this transformation comes with noticeable overhead anyway, this conversion is always strict, i.e.
 * bio::views::char_to<bio::alphabet::cigar> and bio::views::char_strictly_to<bio::alphabet::cigar> are identical.
 *
 * ### Example
 *
 * \include test/snippet/ranges/views/char_to.cpp
 * \hideinitializer
 */
template <typename alphabet_type>
    //!\cond
    requires(alphabet::alphabet<alphabet_type> || std::same_as<alphabet_type, alphabet::cigar>)
//!\endcond
inline constexpr auto char_to = detail::adaptor_from_functor{meta::overloaded{
  // clang-format off
    // cigar special case below
    // char2char special case
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(std::convertible_to<ranges::range_innermost_value_t<rng_t>, alphabet::char_t<alphabet_type>> &&
                 std::same_as<ranges::range_innermost_value_t<rng_t>, alphabet_type>)
    {
        return std::forward<rng_t>(range) | views::type_reduce; // NOP
    },
    // generic case
    []<std::ranges::input_range rng_t>(rng_t && range)
        requires(std::convertible_to<ranges::range_innermost_value_t<rng_t>, alphabet::char_t<alphabet_type>>)
    {
        auto fn = [] (alphabet::char_t<alphabet_type> const in) -> alphabet_type
        {
            return alphabet::assign_char_to(in, alphabet_type{});
        };
        return std::forward<rng_t>(range) | deep{std::views::transform(fn)};
    },
    // catch-all
    []<typename rng_t>(rng_t &&)
    {
        static_assert(meta::always_false<rng_t>,
                      "The type you pass to bio::views::char_to must be a range of elements convertible "
                      "to the target alphabet's character type.");
    }}};
// clang-format on

template <>
inline constexpr auto char_to<alphabet::cigar> = detail::adaptor_from_functor{
  // clang-format off
    []<typename rng_t>(rng_t && range)
    {
        static_assert(std::ranges::forward_range<rng_t> && std::same_as<std::ranges::range_value_t<rng_t>, char>,
                      "The type you pass to bio::views::char_to<bio::alphabet::cigar> must be a forward_range "
                      "over char.");
        return detail::char_to_cigar_view{views::type_reduce(std::forward<rng_t>(range))};
    }};
// clang-format on

//!\}

} // namespace bio::ranges::views
