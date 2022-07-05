// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides bio::views::translate and bio::views::translate_single.
 */

#pragma once

#include <concepts>
#include <ranges>
#include <stdexcept>
#include <vector>

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/aminoacid/translation.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/container/concept.hpp>
#include <bio/ranges/container/small_string.hpp>
#include <bio/ranges/detail/random_access_iterator.hpp>
#include <bio/ranges/type_traits.hpp>
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/detail.hpp>

// ============================================================================
//  forwards
// ============================================================================

namespace bio::ranges::detail
{

template <std::ranges::view urng_t>
    //!\cond
    requires std::ranges::sized_range<urng_t> && std::ranges::random_access_range<urng_t> &&
      alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>
//!\endcond
class view_translate;

template <std::ranges::view urng_t>
    //!\cond
    requires std::ranges::sized_range<urng_t> && std::ranges::random_access_range<urng_t> &&
      alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>
//!\endcond
class view_translate_single;

// ============================================================================
//  translate_fn (adaptor definition for both views)
// ============================================================================

/*!\brief Definition of the range adaptor object type for bio::views::translate and bio::views::translate_single.
 * \tparam single Switch between bio::views::translate and bio::views::translate_single.
 */
template <bool single>
struct translate_fn
{
    //!\brief The default frames parameter for the translation view adaptors.
    static constexpr alphabet::translation_frames default_frames =
      single ? alphabet::translation_frames::FWD_FRAME_0 : alphabet::translation_frames::SIX_FRAME;

    //!\brief Store the argument and return a range adaptor closure object.
    constexpr auto operator()(alphabet::translation_frames const tf = default_frames) const
    {
        return detail::adaptor_from_functor{*this, tf};
    }

    /*!\brief            Directly return an instance of the view, initialised with the given parameters.
     * \param[in] urange The underlying range.
     * \param[in] tf     The frame that should be used for translation.
     * \returns          A range of translated sequence(s).
     */
    template <std::ranges::range urng_t>
    constexpr auto operator()(urng_t && urange, alphabet::translation_frames const tf = default_frames) const
    {
        static_assert(std::ranges::viewable_range<urng_t>,
                      "The range parameter to views::translate[_single] cannot be a temporary of a non-view range.");
        static_assert(std::ranges::sized_range<urng_t>,
                      "The range parameter to views::translate[_single] must model std::ranges::sized_range.");
        static_assert(std::ranges::random_access_range<urng_t>,
                      "The range parameter to views::translate[_single] must model std::ranges::random_access_range.");
        static_assert(alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>,
                      "The range parameter to views::translate[_single] must be over elements of "
                      "bio::alphabet::alphabet::nucleotide_alphabet.");

        if constexpr (single)
            return detail::view_translate_single{std::forward<urng_t>(urange), tf};
        else
            return detail::view_translate{std::forward<urng_t>(urange), tf};
    }

    //!\brief This adaptor is usable without setting the frames parameter in which case the default is chosen.
    template <std::ranges::range urng_t>
    constexpr friend auto operator|(urng_t && urange, translate_fn const & me)
    {
        return me(std::forward<urng_t>(urange));
    }
};

// ============================================================================
//  view_translate_single (range definition)
// ============================================================================

/*!\brief The return type of bio::views::translate_single.
 * \implements std::ranges::view
 * \implements std::ranges::sized_range
 * \implements std::ranges::random_access_range
 * \ingroup views
 */
template <std::ranges::view urng_t>
    //!\cond
    requires std::ranges::sized_range<urng_t> && std::ranges::random_access_range<urng_t> &&
      alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>
//!\endcond
class view_translate_single : public std::ranges::view_base
{
private:
    //!\brief The input range (of ranges).
    urng_t                        urange;
    //!\brief The frame that should be used for translation.
    alphabet::translation_frames  tf;
    //!\brief Error thrown if tried to be used with multiple frames.
    static constexpr small_string multiple_frame_error{
      "Error: Invalid type of frame. Choose one out of FWD_FRAME_0, "
      "REV_FRAME_0, FWD_FRAME_1, REV_FRAME_1, FWD_FRAME_2 and "
      "REV_FRAME_2."};

public:
    /*!\name Member types
     * \{
     */
    //!\brief The reference_type.
    using reference       = alphabet::aa27;
    //!\brief The const_reference type.
    using const_reference = alphabet::aa27;
    //!\brief The value_type (which equals the reference_type with any references removed).
    using value_type      = alphabet::aa27;
    //!\brief The size_type.
    using size_type       = std::ranges::range_size_t<urng_t>;
    //!\brief A signed integer type, usually std::ptrdiff_t.
    using difference_type = std::ranges::range_difference_t<urng_t>;
    //!\brief The iterator type of this view (a random access iterator).
    using iterator        = detail::random_access_iterator<view_translate_single>;
    //!\brief The const_iterator type is equal to the iterator type.
    using const_iterator  = detail::random_access_iterator<view_translate_single const>;
    //!\}

    /*!\name Constructors, destructor and assignment
     * \{
     */
    view_translate_single() noexcept                                                        = default; //!< Defaulted.
    constexpr view_translate_single(view_translate_single const & rhs) noexcept             = default; //!< Defaulted.
    constexpr view_translate_single(view_translate_single && rhs) noexcept                  = default; //!< Defaulted.
    constexpr view_translate_single & operator=(view_translate_single const & rhs) noexcept = default; //!< Defaulted.
    constexpr view_translate_single & operator=(view_translate_single && rhs) noexcept      = default; //!< Defaulted.
    ~view_translate_single() noexcept                                                       = default; //!< Defaulted.

    /*!\brief Construct from another view.
     * \param[in] _urange The underlying range.
     * \param[in] _tf The frame that should be used for translation.
     *
     * ### Exceptions
     *
     * Throws if multiple frames are given as _tf input argument.
     */
    view_translate_single(urng_t                             _urange,
                          alphabet::translation_frames const _tf = alphabet::translation_frames::FWD_FRAME_0) :
      urange{std::move(_urange)}, tf{_tf}
    {
        if (std::popcount(static_cast<uint8_t>(_tf)) > 1)
        {
            throw std::invalid_argument(multiple_frame_error.c_str());
        }
    }

    /*!\brief Construct from another range.
     * \param[in] _urange The underlying range.
     * \param[in] _tf The frame that should be used for translation.
     *
     * ### Exceptions
     *
     * Throws if multiple frames are given as _tf input argument.
     */
    template <typename rng_t>
        //!\cond
        requires(
          (!std::same_as<std::remove_cvref_t<rng_t>, view_translate_single>)&&std::ranges::viewable_range<rng_t> &&
            std::constructible_from<urng_t, std::ranges::ref_view<std::remove_reference_t<rng_t>>>)
    //!\endcond
    view_translate_single(rng_t &&                           _urange,
                          alphabet::translation_frames const _tf = alphabet::translation_frames::FWD_FRAME_0) :
      view_translate_single{std::views::all(std::forward<rng_t>(_urange)), _tf}
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
    iterator begin() noexcept { return {*this, 0}; }

    //!\overload
    const_iterator begin() const noexcept { return {*this, 0}; }

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
    iterator end() noexcept { return {*this, size()}; }

    //!\overload
    const_iterator end() const noexcept { return {*this, size()}; }
    //!\}

    /*!\brief Returns the number of elements in the view.
     * \returns The number of elements in the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (never modifies data).
     */
    size_type size()
    {
        switch (tf)
        {
            case alphabet::translation_frames::FWD_FRAME_0:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_0:
                return std::ranges::size(urange) / 3;
                break;
            case alphabet::translation_frames::FWD_FRAME_1:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_1:
                return (std::max<size_type>(std::ranges::size(urange), 1) - 1) / 3;
                break;
            case alphabet::translation_frames::FWD_FRAME_2:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_2:
                return (std::max<size_type>(std::ranges::size(urange), 2) - 2) / 3;
                break;
            default:
                throw std::invalid_argument(multiple_frame_error.c_str());
                break;
        }
    }

    //!\overload
    size_type size() const
    {
        switch (tf)
        {
            case alphabet::translation_frames::FWD_FRAME_0:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_0:
                return std::ranges::size(urange) / 3;
                break;
            case alphabet::translation_frames::FWD_FRAME_1:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_1:
                return (std::max<size_type>(std::ranges::size(urange), 1) - 1) / 3;
                break;
            case alphabet::translation_frames::FWD_FRAME_2:
                [[fallthrough]];
            case alphabet::translation_frames::REV_FRAME_2:
                return (std::max<size_type>(std::ranges::size(urange), 2) - 2) / 3;
                break;
            default:
                throw std::invalid_argument(multiple_frame_error.c_str());
                break;
        }
    }

    /*!\name Element access
     * \{
     */
    /*!\brief Return the n-th element.
     * \param[in] n The element to retrieve.
     * \returns Either a writable proxy to the element or a copy (if called in const context).
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
    reference operator[](size_type const n)
    {
        assert(n < size());
        switch (tf)
        {
            case alphabet::translation_frames::FWD_FRAME_0:
                return alphabet::translate_triplet((urange)[n * 3], (urange)[n * 3 + 1], (urange)[n * 3 + 2]);
                break;
            case alphabet::translation_frames::REV_FRAME_0:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 1]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 2]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 3]));
                break;
            case alphabet::translation_frames::FWD_FRAME_1:
                return alphabet::translate_triplet((urange)[n * 3 + 1], (urange)[n * 3 + 2], (urange)[n * 3 + 3]);
                break;
            case alphabet::translation_frames::REV_FRAME_1:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 2]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 3]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 4]));
                break;
            case alphabet::translation_frames::FWD_FRAME_2:
                return alphabet::translate_triplet((urange)[n * 3 + 2], (urange)[n * 3 + 3], (urange)[n * 3 + 4]);
                break;
            case alphabet::translation_frames::REV_FRAME_2:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 3]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 4]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 5]));
                break;
            default:
                throw std::invalid_argument(multiple_frame_error.c_str());
                break;
        }
    }

    //!\overload
    const_reference operator[](size_type const n) const
    {
        assert(n < size());
        switch (tf)
        {
            case alphabet::translation_frames::FWD_FRAME_0:
                return alphabet::translate_triplet((urange)[n * 3], (urange)[n * 3 + 1], (urange)[n * 3 + 2]);
                break;
            case alphabet::translation_frames::REV_FRAME_0:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 1]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 2]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 3]));
                break;
            case alphabet::translation_frames::FWD_FRAME_1:
                return alphabet::translate_triplet((urange)[n * 3 + 1], (urange)[n * 3 + 2], (urange)[n * 3 + 3]);
                break;
            case alphabet::translation_frames::REV_FRAME_1:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 2]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 3]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 4]));
                break;
            case alphabet::translation_frames::FWD_FRAME_2:
                return alphabet::translate_triplet((urange)[n * 3 + 2], (urange)[n * 3 + 3], (urange)[n * 3 + 4]);
                break;
            case alphabet::translation_frames::REV_FRAME_2:
                return alphabet::translate_triplet(alphabet::complement((urange)[(urange).size() - n * 3 - 3]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 4]),
                                                   alphabet::complement((urange)[(urange).size() - n * 3 - 5]));
                break;
            default:
                throw std::invalid_argument(multiple_frame_error.c_str());
                break;
        }
    }
    //!\}
};

//!\brief Class template argument deduction for view_translate_single.
template <typename urng_t>
view_translate_single(urng_t &&, alphabet::translation_frames const)
  -> view_translate_single<std::views::all_t<urng_t>>;

//!\brief Class template argument deduction for view_translate_single with default alphabet::translation_frames.
template <typename urng_t>
view_translate_single(urng_t &&) -> view_translate_single<std::views::all_t<urng_t>>;

} // namespace bio::ranges::detail

// ============================================================================
//  translate_single (adaptor object)
// ============================================================================

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that translates nucleotide into aminoacid alphabet for one of the six frames.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \param[in] tf A value of bio::alphabet::alphabet::translation_frames that indicates the desired frames.
 * \returns A range containing frames with aminoacid sequence. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/translate.hpp}
 *
 * This view can be used to translate nucleotide sequences into aminoacid sequences (see alphabet::translation_frames for possible combination of frames).
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                     |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                        |
 * | std::ranges::forward_range       | *required*                            | *preserved*                                        |
 * | std::ranges::bidirectional_range | *required*                            | *preserved*                                        |
 * | std::ranges::random_access_range | *required*                            | *preserved*                                        |
 * | std::ranges::contiguous_range    |                                       | *lost*                                             |
 * |                                  |                                       |                                                    |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                                       |
 * | std::ranges::view                |                                       | *guaranteed*                                       |
 * | std::ranges::sized_range         | *required*                            | *preserved*                                        |
 * | std::ranges::common_range        |                                       | *guaranteed*                                       |
 * | std::ranges::output_range        |                                       | *lost*                                             |
 * | bio::ranges::const_iterable_range     | *required*                            | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide_alphabet            | bio::alphabet::aa27                                       |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Operating on a range of bio::alphabet::dna5:
 * \include test/snippet/ranges/views/translate_dna5.cpp
 * \hideinitializer
 */
inline constexpr auto translate_single = deep{detail::translate_fn<true>{}};

} // namespace bio::ranges::views

// ============================================================================
//  view_translate (range definition)
// ============================================================================

namespace bio::ranges::detail
{

/*!\brief The return type of bio::views::translate.
 * \implements std::ranges::view
 * \implements std::ranges::sized_range
 * \implements std::ranges::random_access_range
 * \tparam urng_t The type of the range being translated.
 * \param[in] tf Translation frames to be used.
 * \ingroup views
 */
template <std::ranges::view urng_t>
    //!\cond
    requires std::ranges::sized_range<urng_t> && std::ranges::random_access_range<urng_t> &&
      alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>
//!\endcond
class view_translate : public std::ranges::view_base
{
private:
    //!\brief The data members of view_translate_single.
    urng_t                                        urange;
    //!\brief The frames that should be used for translation.
    alphabet::translation_frames                  tf;
    //!\brief The selected frames corresponding to the frames required.
    small_vector<alphabet::translation_frames, 6> selected_frames{};

public:
    /*!\name Member types
     * \{
     */
    //!\brief The reference_type.
    using reference       = view_translate_single<urng_t>;
    //!\brief The const_reference type.
    using const_reference = reference;
    //!\brief The value_type (which equals the reference_type with any references removed).
    using value_type      = reference;
    //!\brief The size_type.
    using size_type       = std::ranges::range_size_t<urng_t>;
    //!\brief A signed integer type, usually std::ptrdiff_t.
    using difference_type = std::ranges::range_difference_t<urng_t>;
    //!\brief The iterator type of this view (a random access iterator).
    using iterator        = detail::random_access_iterator<view_translate>;
    //!\brief The const iterator type of this view (same as iterator, because it's a view).
    using const_iterator  = detail::random_access_iterator<view_translate const>;
    //!\}

protected:
    /*!\name Compatibility
     * \brief Static constexpr variables that emulate/encapsulate bio::ranges::range_compatible (which doesn't work for types during their definition).
     * \{
     */
    //!\cond
    // unfortunately we cannot specialise the variable template so we have to add an auxiliary here
    template <typename t>
        requires(range_dimension_v<t> == range_dimension_v<value_type> + 1)
    &&std::is_same_v<std::remove_cvref_t<range_innermost_value_t<value_type>>,
                     std::remove_cvref_t<range_innermost_value_t<t>>> static constexpr bool is_compatible_this_aux =
      true;
    //!\endcond
    //!\}

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    view_translate() noexcept                                                 = default; //!< Defaulted.
    constexpr view_translate(view_translate const & rhs) noexcept             = default; //!< Defaulted.
    constexpr view_translate(view_translate && rhs) noexcept                  = default; //!< Defaulted.
    constexpr view_translate & operator=(view_translate const & rhs) noexcept = default; //!< Defaulted.
    constexpr view_translate & operator=(view_translate && rhs) noexcept      = default; //!< Defaulted.
    ~view_translate() noexcept                                                = default; //!< Defaulted.

    /*!\brief Construct from another view.
     * \param[in] _urange The underlying range (of ranges).
     * \param[in] _tf The frames that should be used for translation.
     */
    view_translate(urng_t _urange, alphabet::translation_frames const _tf = alphabet::translation_frames::SIX_FRAME) :
      urange{std::move(_urange)}, tf{_tf}
    {
        if ((_tf & alphabet::translation_frames::FWD_FRAME_0) == alphabet::translation_frames::FWD_FRAME_0)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_0);
        if ((_tf & alphabet::translation_frames::FWD_FRAME_1) == alphabet::translation_frames::FWD_FRAME_1)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_1);
        if ((_tf & alphabet::translation_frames::FWD_FRAME_2) == alphabet::translation_frames::FWD_FRAME_2)
            selected_frames.push_back(alphabet::translation_frames::FWD_FRAME_2);
        if ((_tf & alphabet::translation_frames::REV_FRAME_0) == alphabet::translation_frames::REV_FRAME_0)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_0);
        if ((_tf & alphabet::translation_frames::REV_FRAME_1) == alphabet::translation_frames::REV_FRAME_1)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_1);
        if ((_tf & alphabet::translation_frames::REV_FRAME_2) == alphabet::translation_frames::REV_FRAME_2)
            selected_frames.push_back(alphabet::translation_frames::REV_FRAME_2);
    }

    /*!\brief Construct from another range.
     * \param[in] _urange The underlying range (of ranges).
     * \param[in] _tf The frames that should be used for translation.
     */
    template <typename rng_t>
        //!\cond
        requires((!std::same_as<std::remove_cvref_t<rng_t>, view_translate>)&&std::ranges::viewable_range<rng_t> &&
                   std::constructible_from<urng_t, std::ranges::ref_view<std::remove_reference_t<rng_t>>>)
    //!\endcond
    view_translate(rng_t && _urange, alphabet::translation_frames const _tf = alphabet::translation_frames::SIX_FRAME) :
      view_translate{std::views::all(std::forward<rng_t>(_urange)), _tf}
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
    iterator begin() noexcept { return {*this, 0}; }

    //!\overload
    const_iterator begin() const noexcept { return {*this, 0}; }

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
    iterator end() noexcept { return {*this, size()}; }

    //!\overload
    const_iterator end() const noexcept { return {*this, size()}; }

    /*!\brief Returns the number of elements in the view.
     * \returns The number of elements in the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_type size() noexcept { return (size_type)selected_frames.size(); }

    //!\overload
    size_type size() const noexcept { return (size_type)selected_frames.size(); }

    /*!\name Element access
     * \{
     */
    /*!\brief Return the n-th element.
     * \param[in] n The element to retrieve.
     * \returns Either a writable proxy to the element or a copy (if called in const context).
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
    reference operator[](size_type const n)
    {
        assert(n < size());
        return urange | views::translate_single(selected_frames[n]);
    }

    //!\overload
    const_reference operator[](size_type const n) const
    {
        assert(n < size());
        return urange | views::translate_single(selected_frames[n]);
    }
    //!\}
};

//!\brief Class template argument deduction for view_translate.
template <typename urng_t>
    //!\cond
    requires std::ranges::sized_range<urng_t> && std::ranges::random_access_range<urng_t> &&
      alphabet::nucleotide_alphabet<std::ranges::range_reference_t<urng_t>>
      //!\endcond
      view_translate(urng_t &&, alphabet::translation_frames const = alphabet::translation_frames{})
->view_translate<std::views::all_t<urng_t>>;

} // namespace bio::ranges::detail

// ============================================================================
//  translate (adaptor object)
// ============================================================================

namespace bio::ranges::views
{

/*!\name Alphabet related views
 * \{
 */

/*!\brief A view that translates nucleotide into aminoacid alphabet with 1, 2, 3 or 6 frames.
 * \tparam urng_t The type of the range being processed.
 * \param[in] urange The range being processed.
 * \param[in] tf A value of bio::alphabet::tanslation_frames that indicates the desired frames.
 * \returns A range of ranges containing frames with aminoacid sequence. See below for the properties of the returned range.
 * \ingroup views
 *
 * \details
 *
 * \header_file{bio/ranges/views/translate.hpp}
 *
 * This view can be used to translate nucleotide sequences into aminoacid sequences (see alphabet::translation_frames for possible combination of frames).
 *
 * ### View properties
 *
 * | Concepts and traits              | `urng_t` (underlying range type)      | `rrng_t` (returned range type)                     |
 * |----------------------------------|:-------------------------------------:|:--------------------------------------------------:|
 * | std::ranges::input_range         | *required*                            | *preserved*                                        |
 * | std::ranges::forward_range       | *required*                            | *preserved*                                        |
 * | std::ranges::bidirectional_range | *required*                            | *preserved*                                        |
 * | std::ranges::random_access_range | *required*                            | *preserved*                                        |
 * | std::ranges::contiguous_range    |                                       | *lost*                                             |
 * |                                  |                                       |                                                    |
 * | std::ranges::viewable_range      | *required*                            | *guaranteed*                                       |
 * | std::ranges::view                |                                       | *guaranteed*                                       |
 * | std::ranges::sized_range         | *required*                            | *preserved*                                        |
 * | std::ranges::common_range        |                                       | *guaranteed*                                       |
 * | std::ranges::output_range        |                                       | *lost*                                             |
 * | bio::ranges::const_iterable_range     | *required*                            | *preserved*                                        |
 * |                                  |                                       |                                                    |
 * | std::ranges::range_reference_t   | bio::alphabet::alphabet::nucleotide_alphabet            | std::ranges::view && std::ranges::random_access_range && std::ranges::sized_range |
 *
 * * `urng_t` is the type of the range modified by this view (input).
 * * `rrng_type` is the type of the range returned by this view.
 * * for more details, see \ref views.
 *
 * ### Example
 *
 * Operating on a range of bio::alphabet::dna5:
 * \include test/snippet/ranges/views/translate_usage.cpp
 * \hideinitializer
 */
inline constexpr auto translate = deep{detail::translate_fn<false>{}};
//!\}

} // namespace bio::ranges::views
