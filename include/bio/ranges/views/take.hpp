// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::take.
 */

#pragma once

#include <algorithm>
#include <concepts>
#include <iterator>
#include <ranges>
#include <span>
#include <type_traits>

#include <bio/meta/type_traits/template_inspection.hpp>
#include <bio/meta/type_traits/transformation_trait_or.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/container/concept.hpp>
#include <bio/ranges/detail/inherited_iterator_base.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/detail.hpp>

namespace bio::ranges::detail
{

// ============================================================================
//  view_take
// ============================================================================

/*!\brief The type returned by bio::views::take and bio::views::take_or_throw.
 * \tparam urng_t    The type of the underlying ranges, must satisfy bio::views::concept.
 * \tparam exactly   Whether to expose sized'ness on the view.
 * \tparam or_throw  Whether to throw an exception when the input is exhausted before the end of line is reached.
 * \implements std::ranges::view
 * \implements std::ranges::random_access_range
 * \implements std::ranges::sized_range
 * \ingroup views
 *
 * \details
 *
 * Note that most members of this class are generated by ranges::view_interface which is not yet documented here.
 */
template <std::ranges::view urng_t, bool exactly, bool or_throw>
class view_take : public std::ranges::view_interface<view_take<urng_t, exactly, or_throw>>
{
private:
    //!\brief The underlying range.
    urng_t urange;

    //!\brief The desired target_size.
    size_t target_size;

    //!\brief The forward declared iterator type.
    template <typename rng_t>
    class basic_iterator;

private:
    /*!\name Associated types
     * \{
     */
    //!\brief The iterator type of this view (a random access iterator).
    using iterator       = basic_iterator<urng_t>;
    /*!\brief Note that this declaration does not give any compiler errors for non-const iterable ranges. Although
     * `basic_iterator` inherits from std::ranges::iterator_t which is not defined on a const-range, i.e. `urng_t const,
     *  if it is not const-iterable. We only just declare this type and never instantiate it, i.e. use this type within
     *  this class, if the underlying range is not const-iterable.
     */
    using const_iterator = basic_iterator<urng_t const>;
    //!\}

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    view_take()                                      = default; //!< Defaulted.
    view_take(view_take const & rhs)                 = default; //!< Defaulted.
    view_take(view_take && rhs) noexcept             = default; //!< Defaulted.
    view_take & operator=(view_take const & rhs)     = default; //!< Defaulted.
    view_take & operator=(view_take && rhs) noexcept = default; //!< Defaulted.
    ~view_take()                                     = default; //!< Defaulted.

    /*!\brief Construct from another View.
     * \param[in] _urange The underlying range.
     * \param[in] _size   The desired size (after which to stop returning elements).
     * \throws std::runtime_error If `exactly && or_throw && bio::alphabet::sized_range<urng_t>`.
     */
    constexpr view_take(urng_t _urange, size_t const _size) : urange{std::move(_urange)}, target_size{_size}
    {
        if constexpr (std::ranges::sized_range<urng_t>)
        {
            if (std::ranges::size(urange) < target_size)
            {
                if constexpr (exactly && or_throw)
                {
                    throw std::invalid_argument{
                      "You are trying to construct a views::take_exactly_or_throw from a range that is strictly "
                      "smaller."};
                }
                else
                {
                    target_size = std::ranges::size(urange);
                }
            }
        }
    }

    /*!\brief Construct from another viewable_range.
     * \tparam rng_t      Type of the passed range; `urng_t` must be constructible from this.
     * \param[in] _urange The underlying range.
     * \param[in] _size   The desired size (after which to stop returning elements).
     * \throws std::runtime_error If `exactly && or_throw && bio::alphabet::sized_range<urng_t>`.
     */
    template <std::ranges::viewable_range rng_t>
    //!\cond
        requires std::constructible_from<rng_t, std::views::all_t<rng_t>>
    //!\endcond
    constexpr view_take(rng_t && _urange, size_t const _size) :
      view_take{std::views::all(std::forward<rng_t>(_urange)), _size}
    {}
    //!\}

    /*!\name Iterators
     * \{
     */
    /*!\brief Returns an iterator to the first element of the container.
     * \returns Iterator to the first element.
     *
     * If the container is empty, the returned iterator will be equal to end().
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
        if constexpr (std::ranges::random_access_range<urng_t> && (std::ranges::sized_range<urng_t> || exactly))
            return std::ranges::begin(urange);
        else
            return iterator{std::ranges::begin(urange), 0, target_size, this};
    }

    //!\copydoc begin()
    constexpr auto begin() const noexcept
        requires const_iterable_range<urng_t>
    {
        if constexpr (std::ranges::random_access_range<urng_t> && (std::ranges::sized_range<urng_t> || exactly))
            return std::ranges::begin(urange);
        else
            return const_iterator{std::ranges::begin(urange), 0, target_size};
    }

    /*!\brief Returns an iterator to the element following the last element of the range.
     * \returns Iterator to the end.
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
        if constexpr (std::ranges::random_access_range<urng_t> && (std::ranges::sized_range<urng_t> || exactly))
            return std::ranges::begin(urange) + target_size;
        else
            return std::ranges::end(urange);
    }

    //!\copydoc end()
    constexpr auto end() const noexcept
        requires const_iterable_range<urng_t>
    {
        if constexpr (std::ranges::random_access_range<urng_t> && (std::ranges::sized_range<urng_t> || exactly))
            return std::ranges::begin(urange) + target_size;
        else
            return std::ranges::end(urange);
    }
    //!\}

    /*!\brief Returns the number of elements in the view.
     * \returns The number of elements in the view.
     *
     * This overload is only available if the underlying range models std::ranges::sized_range (return type is
     * std::ranges::range_size_t<urng_type>) or for specialisation that have the `exactly` (return type is std::size_t)
     * template parameter set. If both conditions are true the return type is `std::size_t`.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr auto size() const noexcept
        requires exactly || std::ranges::sized_range<urng_t>
    {
        return target_size;
    }
};

//!\brief Template argument type deduction guide that strips references.
//!\relates bio::ranges::detail::view_take
template <typename urng_t, bool exactly = false, bool or_throw = false>
view_take(urng_t &&, size_t) -> view_take<std::views::all_t<urng_t>, exactly, or_throw>;

//!\brief The iterator for the view_take. It inherits from the underlying type, but overwrites several operators.
//!\tparam rng_t Should be `urng_t` for defining #iterator and `urng_t const` for defining #const_iterator.
template <std::ranges::view urng_t, bool exactly, bool or_throw>
template <typename rng_t>
class view_take<urng_t, exactly, or_throw>::basic_iterator :
  public inherited_iterator_base<basic_iterator<rng_t>, std::ranges::iterator_t<rng_t>>
{
private:
    //!\brief The iterator type of the underlying range.
    using base_base_t = std::ranges::iterator_t<rng_t>;
    //!\brief The CRTP wrapper type.
    using base_t      = inherited_iterator_base<basic_iterator, std::ranges::iterator_t<rng_t>>;

    //!\brief The sentinel type is identical to that of the underlying range.
    using sentinel_type = std::ranges::sentinel_t<urng_t>;

    //!\brief The current position.
    size_t pos{};

    //!\brief The size parameter to the view.
    size_t max_pos{};

    //!\brief A pointer to host, s.t. the size of the view can shrink on pure input ranges.
    std::conditional_t<exactly && !std::forward_iterator<base_base_t>, view_take *, meta::ignore_t> host_ptr;

public:
    /*!\name Constructors, destructor and assignment
     * \brief Exceptions specification is implicitly inherited.
     * \{
     */
    basic_iterator()                                           = default; //!< Defaulted.
    basic_iterator(basic_iterator const & rhs)                 = default; //!< Defaulted.
    basic_iterator(basic_iterator && rhs) noexcept             = default; //!< Defaulted.
    basic_iterator & operator=(basic_iterator const & rhs)     = default; //!< Defaulted.
    basic_iterator & operator=(basic_iterator && rhs) noexcept = default; //!< Defaulted.
    ~basic_iterator()                                          = default; //!< Defaulted.

    //!\brief Constructor that delegates to the CRTP layer.
    constexpr basic_iterator(base_base_t const & it) noexcept(noexcept(base_t{it})) : base_t{std::move(it)} {}

    //!\brief Constructor that delegates to the CRTP layer and initialises the members.
    constexpr basic_iterator(base_base_t  it,
                             size_t const _pos,
                             size_t const _max_pos,
                             view_take *  host = nullptr) noexcept(noexcept(base_t{it})) :
      base_t{std::move(it)}, pos{_pos}, max_pos(_max_pos)
    {
        host_ptr = host;
    }
    //!\}

    /*!\name Associated types
     * \brief All are derived from the base_base_t.
     * \{
     */

    //!\brief The difference type.
    using difference_type   = std::iter_difference_t<base_base_t>;
    //!\brief The value type.
    using value_type        = std::iter_value_t<base_base_t>;
    //!\brief The reference type.
    using reference         = std::iter_reference_t<base_base_t>;
    //!\brief The pointer type.
    using pointer           = detail::iter_pointer_t<base_base_t>;
    //!\brief The iterator category tag.
    using iterator_category = detail::iterator_category_tag_t<base_base_t>;
    //!\brief The iterator concept tag.
    using iterator_concept  = detail::iterator_concept_tag_t<base_base_t>;
    //!\}

    /*!\name Arithmetic operators
     * \brief bio::ranges::detail::inherited_iterator_base operators are used unless specialised here.
     * \{
     */

    //!\brief Increments the iterator by one.
    constexpr basic_iterator & operator++() noexcept(noexcept(++std::declval<base_t &>()))
    {
        base_t::operator++();
        ++pos;
        if constexpr (exactly && !std::forward_iterator<base_base_t>)
            --host_ptr->target_size;
        return *this;
    }

    //!\brief Returns an iterator incremented by one.
    constexpr basic_iterator operator++(int) noexcept(noexcept(++std::declval<basic_iterator &>()) &&
                                                      std::is_nothrow_copy_constructible_v<basic_iterator>)
    {
        basic_iterator cpy{*this};
        ++(*this);
        return cpy;
    }

    //!\brief Decrements the iterator by one.
    constexpr basic_iterator & operator--() noexcept(noexcept(--std::declval<base_base_t &>()))
      //!\cond
        requires std::bidirectional_iterator<base_base_t>
    //!\endcond
    {
        base_t::operator--();
        --pos;
        return *this;
    }

    //!\brief Returns an iterator decremented by one.
    constexpr basic_iterator operator--(int) noexcept(noexcept(--std::declval<basic_iterator &>()) &&
                                                      std::is_nothrow_copy_constructible_v<basic_iterator>)
      //!\cond
        requires std::bidirectional_iterator<base_base_t>
    //!\endcond
    {
        basic_iterator cpy{*this};
        --(*this);
        return cpy;
    }

    //!\brief Advances the iterator by skip positions.
    constexpr basic_iterator & operator+=(difference_type const skip) noexcept(noexcept(std::declval<base_t &>() +=
                                                                                        skip))
      //!\cond
        requires std::random_access_iterator<base_base_t>
    //!\endcond
    {
        base_t::operator+=(skip);
        pos += skip;
        return *this;
    }

    //!\brief Advances the iterator by -skip positions.
    constexpr basic_iterator & operator-=(difference_type const skip) noexcept(noexcept(std::declval<base_t &>() -=
                                                                                        skip))
      //!\cond
        requires std::random_access_iterator<base_base_t>
    //!\endcond
    {
        base_t::operator-=(skip);
        pos -= skip;
        return *this;
    }
    //!\}

    /*!\name Comparison operators
     * \brief We define comparison against self and against the sentinel.
     * \{
     */

    //!\brief Checks whether `*this` is equal to `rhs`.
    constexpr bool operator==(basic_iterator const & rhs) const
      noexcept(!or_throw && noexcept(std::declval<base_base_t &>() == std::declval<base_base_t &>()))
      //!\cond
        requires std::forward_iterator<base_base_t>
    //!\endcond
    {
        return *base_t::this_to_base() == *rhs.this_to_base();
    }

    //!\brief Checks whether `lhs` is equal to `rhs`.
    constexpr friend bool operator==(basic_iterator const & lhs, sentinel_type const & rhs) noexcept(
      !or_throw && noexcept(std::declval<base_base_t const &>() == std::declval<sentinel_type const &>()))
    {
        if (lhs.pos >= lhs.max_pos)
            return true;

        if (static_cast<base_base_t>(lhs) == rhs)
        {
            if constexpr (or_throw)
                throw std::runtime_error{"Reached end of input before designated size."};

            return true;
        }
        else
        {
            return false;
        }
    }
    //!\}

    /*!\name Reference/Dereference operators
     * \brief bio::ranges::detail::inherited_iterator_base operators are used unless specialised here.
     * \{
     */

    /*!\brief Accesses an element by index.
     * \param n Position relative to current location.
     * \return A reference to the element at relative location.
     */
    constexpr reference operator[](std::make_unsigned_t<difference_type> const n) const
      noexcept(noexcept(std::declval<base_base_t &>()[0]))
      //!\cond
        requires std::random_access_iterator<base_base_t>
    //!\endcond
    {
        return base_base_t::operator[](n);
    }
    //!\}
};

// ============================================================================
//  take_fn (adaptor definition)
// ============================================================================

/*!\brief View adaptor definition for views::take and views::take_or_throw.
 * \tparam or_throw Whether to throw an exception when the input is exhausted before the end is reached.
 */
template <bool exactly, bool or_throw>
struct take_fn
{
    //!\brief Store the arguments and return a range adaptor closure object.
    constexpr auto operator()(size_t const size) const { return adaptor_from_functor{*this, size}; }

    /*!\brief Type erase if possible and return view_take if not.
     * \returns An instance of std::span, std::basic_string_view, std::ranges::subrange or bio::ranges::detail::view_take.
     */
    template <std::ranges::range urng_t>
    constexpr auto operator()(urng_t && urange, size_t target_size) const
    {
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The views::take adaptor can only be passed viewable_ranges, i.e. Views or &-to-non-View.");

        // safeguard against wrong size
        if constexpr (std::ranges::sized_range<urng_t>)
        {
            if constexpr (or_throw)
            {
                if (target_size > std::ranges::size(urange))
                {
                    throw std::invalid_argument{
                      "You are trying to construct a views::take_exactly_or_throw from a "
                      "range that is strictly smaller."};
                }
            }
            else
            {
                target_size = std::min<size_t>(target_size, std::ranges::size(urange));
            }
        }

        // string_view
        if constexpr (meta::is_type_specialisation_of_v<std::remove_cvref_t<urng_t>, std::basic_string_view>)
        {
            return urange.substr(0, target_size);
        }
        // string const &
        else if constexpr (meta::is_type_specialisation_of_v<std::remove_cvref_t<urng_t>, std::basic_string> &&
                           std::is_const_v<std::remove_reference_t<urng_t>>)
        {
            return std::basic_string_view{std::ranges::data(urange), target_size};
        }
        // contiguous
        else if constexpr (std::ranges::borrowed_range<urng_t> && std::ranges::contiguous_range<urng_t> &&
                           std::ranges::sized_range<urng_t>)
        {
            return std::span{std::ranges::data(urange), target_size};
        }
        // random_access
        else if constexpr (std::ranges::borrowed_range<urng_t> && std::ranges::random_access_range<urng_t> &&
                           std::ranges::sized_range<urng_t>)
        {
            return std::ranges::subrange<std::ranges::iterator_t<urng_t>, std::ranges::iterator_t<urng_t>>{
              std::ranges::begin(urange),
              std::ranges::begin(urange) + target_size,
              target_size};
        }
        // our type
        else
        {
            return view_take<std::views::all_t<urng_t>, exactly, or_throw>{std::forward<urng_t>(urange), target_size};
        }
    }
};

} // namespace bio::ranges::detail
