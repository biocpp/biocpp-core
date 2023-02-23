// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::views::transform_by_pos.
 */

#pragma once

#include <utility>

#include <bio/ranges/concept.hpp>
#include <bio/ranges/detail/random_access_iterator.hpp>
#include <bio/ranges/views/detail.hpp>

namespace bio::ranges::detail
{

/*!\brief View class for bio::ranges::views::transform_by_pos.
 * \tparam urng_t The underlying range type.
 * \tparam transform_by_pos_t Type of the invocable.
 */
template <std::ranges::view urng_t, typename transform_by_pos_t>
class view_transform_by_pos : public std::ranges::view_base
{
private:
    //!\brief The input range.
    urng_t             urange;
    //!\brief The invocable.
    transform_by_pos_t transform_by_pos;
    //!\brief The size of this range.
    size_t             _size = 0;

    static_assert(std::ranges::sized_range<urng_t>,
                  "The range parameter to views::transform_by_pos must model std::ranges::sized_range.");
    static_assert(std::ranges::random_access_range<urng_t>,
                  "The range parameter to views::transform_by_pos must model std::ranges::random_access_range.");

    //!\brief Helper to determine of the invocable is actually invocable.
    static constexpr bool invocable = std::invocable<transform_by_pos_t, urng_t &, std::ranges::range_size_t<urng_t>>;
    static_assert(invocable, "Both functors need to be invocable with the range type and an integral as parameters.");

    //!\brief Helper to determine of the invocable is invocable in const-context..
    static constexpr bool const_invocable =
      const_iterable_range<urng_t> &&
      std::invocable<transform_by_pos_t const, urng_t const &, std::ranges::range_size_t<urng_t>>;

    /*!\name Member types
     * \{
     */
    //!\brief The reference_type.
    using reference       = decltype(transform_by_pos(urange, 0ull));
    //!\brief The const_reference type.
    using const_reference = decltype(std::as_const(transform_by_pos)(std::as_const(urange), 0ull));
    //!\brief The value_type (which equals the reference_type with any cvref removed).
    using value_type      = std::remove_cvref_t<reference>;
    //!\brief A signed integer type, usually std::ptrdiff_t.
    using difference_type = std::ranges::range_difference_t<urng_t>;
    //!\brief The size type. We always fix this to size_t here.
    using size_type       = size_t;
    //!\brief The iterator type of this view (a random access iterator).
    using iterator        = detail::random_access_iterator<view_transform_by_pos>;
    //!\brief The iterator type of this view in const-context (a random access iterator).
    using const_iterator  = detail::random_access_iterator<view_transform_by_pos const>;
    //!\}

    //!\brief Befriend the iterators so they can instantiate.
    template <typename, template <typename...> typename>
    friend class detail::random_access_iterator_base;

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    view_transform_by_pos() noexcept                                                        = default; //!< Defaulted.
    constexpr view_transform_by_pos(view_transform_by_pos const & rhs) noexcept             = default; //!< Defaulted.
    constexpr view_transform_by_pos(view_transform_by_pos && rhs) noexcept                  = default; //!< Defaulted.
    constexpr view_transform_by_pos & operator=(view_transform_by_pos const & rhs) noexcept = default; //!< Defaulted.
    constexpr view_transform_by_pos & operator=(view_transform_by_pos && rhs) noexcept      = default; //!< Defaulted.
    ~view_transform_by_pos() noexcept                                                       = default; //!< Defaulted.

    /*!\brief Construct from another view.
     * \param[in] _urange           The underlying range.
     * \param[in] _transform_by_pos The invocable that transforms the elements.
     * \param[in] s                 Size of the returned view.
     */
    view_transform_by_pos(urng_t _urange, transform_by_pos_t _transform_by_pos, size_t const s) :
      urange{std::move(_urange)}, transform_by_pos{std::move(_transform_by_pos)}, _size{s}
    {}

    /*!\brief Construct from another range.
     * \param[in] _urange           The underlying range.
     * \param[in] _transform_by_pos The invocable that transforms the elements.
     * \param[in] s                 Size of the returned view.
     */
    template <typename rng_t>
    //!\cond
        requires(!meta::decays_to<rng_t, view_transform_by_pos> &&
                 (std::ranges::viewable_range<rng_t> &&
                  std::constructible_from<urng_t, std::ranges::ref_view<std::remove_reference_t<rng_t>>>))
    //!\endcond
    view_transform_by_pos(rng_t && _urange, transform_by_pos_t _transform_by_pos, size_t const s) :
      view_transform_by_pos{std::views::all(std::forward<rng_t>(_urange)), std::move(_transform_by_pos), s}
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
    auto begin() noexcept { return iterator{*this, 0}; }
    //!\overload
    auto begin() const noexcept
        requires const_invocable
    {
        return const_iterator{*this, 0};
    }

    /*!\brief Returns an iterator to the element following the last element of the container.
     * \returns Iterator to the first element.
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
    auto end() noexcept { return iterator{*this, size()}; }
    //!\overload
    auto end() const noexcept
        requires const_invocable
    {
        return const_iterator{*this, size()};
    }
    //!\}

    /*!\brief Returns the number of elements in the view.
     * \returns The number of elements in the view.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (never modifies data).
     */
    size_t size() const noexcept { return _size; }

    /*!\name Element access
     * \{
     */
    /*!\brief Return the n-th element.
     * \param[in] n The element to retrieve.
     * \returns The element (possibly reference or value).
     *
     * Accessing an element behind the last causes undefined behaviour. In debug mode an assertion checks the size of
     * the container.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (never modifies data).
     *
     * ### Complexity
     *
     * Constant.
     */
    decltype(auto) operator[](size_t const n)
    {
        assert(n < size());
        return transform_by_pos(urange, n);
    }

    //!\overload
    decltype(auto) operator[](size_t const n) const
        requires const_invocable
    {
        assert(n < size());
        return transform_by_pos(urange, n);
    }
    //!\}
};

//!\brief Class template argument deduction for view_transform_by_pos.
template <typename urng_t, typename transform_by_pos_t>
view_transform_by_pos(urng_t &&, transform_by_pos_t, size_t)
  -> view_transform_by_pos<std::views::all_t<urng_t>, transform_by_pos_t>;

// ============================================================================
//  transform_by_pos_fn (adaptor object)
// ============================================================================

//!\brief Adaptor object definition for bio::ranges::views::transform_by_pos.
struct transform_by_pos_fn
{
    //!\brief Store the argument and return a range adaptor closure object.
    template <typename transform_by_pos_t, typename size_transform_t = std::remove_cvref_t<decltype(std::ranges::size)>>
    constexpr auto operator()(transform_by_pos_t transform_by_pos,
                              size_transform_t   size_transform = std::ranges::size) const
    {
        return detail::adaptor_from_functor{*this, std::move(transform_by_pos), std::move(size_transform)};
    }

    /*!\brief Directly return an instance of the view, initialised with the given parameters.
     * \param[in] urange            The underlying range.
     * \param[in] transform_by_pos  The invocable that performs the transformation.
     * \param[in] size_transform    The invocable that computes the new size. [optional]
     * \returns The transformed range.
     */
    template <std::ranges::range urng_t,
              typename transform_by_pos_t,
              typename size_transform_t = std::remove_cvref_t<decltype(std::ranges::size)>>
    constexpr auto operator()(urng_t &&          urange,
                              transform_by_pos_t transform_by_pos,
                              size_transform_t   size_transform = std::ranges::size) const
    {
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The range parameter to views::transform_by_pos must viewable.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The range parameter to views::transform_by_pos must model std::ranges::sized_range.");
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The range parameter to views::transform_by_pos must model std::ranges::random_access_range.");
        static_assert(std::convertible_to<std::invoke_result_t<size_transform_t, urng_t &>, size_t>,
                      "The size_transform must be invocable with `urng_t &` and return sth. convertible to size_t.");

        return view_transform_by_pos{std::forward<urng_t>(urange), std::move(transform_by_pos), size_transform(urange)};
    }
};

} // namespace bio::ranges::detail

// A view that applies a transformation function to an element of a range using its positional index.
namespace bio::ranges::views
{

/*!\name General purpose views
 * \{
 */

/*!\brief A view adaptor similar to std::views::transform. Calls the invocable with two arguments: underlying range and position.
 * \tparam urng_t     The type of the range being processed.
 * \param[in] urange  The range being processed.
 * \param[in] fn      Invocable that takes `(urng_t &, size_t)` as arguments and returns transformed element.
 * \param[in] size_fn Invocable that takes `(urng_t &)` as argument and returns size of new range. [optional]
 * \returns A range of the transformed elements.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/transform_by_pos.hpp}
 *
 * This view can be used to create more elaborate transformations than are possible with std::views::transform, but
 * it only works on sized, random-access ranges.
 *
 * If you want the returned range to be const-iterable, the `fn` that you pass should be able to
 * handle both `urng_t &` and `urng_t const &` as arguments.
 *
 * The `size_fn` parameter is optional; if it is omitted, the size of the returned range will be the same as
 * the size of the underlying range. Note that the size is computed only once on construction of the returned range
 * (and then cached).
 *
 * ### View properties
 *
 * | Concepts and traits               | `urng_t` (underlying range type) | `rrng_t` (returned range type)                              |
 * |-----------------------------------|:--------------------------------:|:-----------------------------------------------------------:|
 * | std::ranges::input_range          | *required*                       | *preserved*                                                 |
 * | std::ranges::forward_range        | *required*                       | *preserved*                                                 |
 * | std::ranges::bidirectional_range  | *required*                       | *preserved*                                                 |
 * | std::ranges::random_access_range  | *required*                       | *preserved*                                                 |
 * | std::ranges::contiguous_range     |                                  | *lost*                                                      |
 * |                                   |                                  |                                                             |
 * | std::ranges::viewable_range       | *required*                       | *guaranteed*                                                |
 * | std::ranges::view                 |                                  | *guaranteed*                                                |
 * | std::ranges::sized_range          | *required*                       | *preserved*                                                 |
 * | std::ranges::common_range         |                                  | *guaranteed*                                                |
 * | std::ranges::output_range         |                                  | depends on `fn`                                             |
 * | bio::ranges::const_iterable_range |                                  | depends on `fn`                                             |
 * |                                   |                                  |                                                             |
 * | std::ranges::range_reference_t    |                                  | depends on `fn`                                             |
 * | std::ranges::range_value_t        |                                  | std::remove_cvref_t<std::ranges::range_reference_t<rrng_t>> |
 *
 * * `urng_t` is the type of the range adapted by this view (input).
 * * `rrng_t` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Implementation of a "reverse view":
 * \include test/snippet/ranges/views/transform_by_pos_reverse.cpp
 * \hideinitializer
 */

inline constexpr auto transform_by_pos = detail::transform_by_pos_fn{};
//!\}
} // namespace bio::ranges::views
