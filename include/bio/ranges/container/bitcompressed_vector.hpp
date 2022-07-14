// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::ranges::bitcompressed_vector.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <climits>
#include <concepts>
#include <iterator>
#include <ranges>
#include <type_traits>

#include <bio/alphabet/detail/alphabet_proxy.hpp>
#include <bio/ranges/detail/random_access_iterator.hpp>
#include <bio/ranges/views/convert.hpp>
#include <bio/ranges/views/repeat_n.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/ranges/views/to_rank.hpp>

namespace bio::ranges
{

/*!\brief A space-optimised version of std::vector that compresses multiple letters into a single byte.
 * \tparam alphabet_type The value type of the container, must satisfy bio::alphabet::writable_semialphabet and std::regular.
 * \implements bio::ranges::detail::reservible_container
 * \implements bio::cerealisable
 * \ingroup container
 *
 * This class template behaves just like std::vector<alphabet_type> but has an internal representation where
 * multiple values are packed into a single byte/word to save space, e.g. bitcompressed_vector<bio::alphabet::dna4> uses a
 * quarter of of the memory that std::vector<bio::alphabet::dna4> uses, because a single bio::alphabet::dna4 letter can be represented
 * in two bits (instead of 8 which is the lower bound for a single object in C++).
 *
 * The disadvantages are slightly slower operations and unsafety towards parallel writes to adjacent positions
 * in the bitcompressed_vector.
 *
 * ### Example
 *
 * \include test/snippet/ranges/container/bitcompressed_vector.cpp
 *
 * ### Thread safety
 *
 * This container provides no thread-safety beyond the promise given also by the STL that all
 * calls to `const` member function are safe from multiple threads (as long as no thread calls
 * a non-`const` member function at the same time).
 *
 * An important difference to std::vector is that calling `vec[i] = value;` and `vec[j] = value2;` from two different
 * threads at the same time **is not safe** and will lead to corruption if both values are stored in the same
 * 64bit-block, i.e. if the distance between `i` and `j` is smaller than 64 / alphabet_size.
 */
template <alphabet::writable_semialphabet alphabet_type>
    //!\cond
    requires std::regular<alphabet_type>
//!\endcond
class bitcompressed_vector
{
private:
    //!\brief The number of bits needed to represent a single letter of the alphabet_type.
    static constexpr size_t bits_per_letter = std::bit_width(alphabet::alphabet_size<alphabet_type>);
    static_assert(bits_per_letter <= 64, "alphabet must be representable in at most 64bit.");

    //!\brief The element type of the underyling storage vector.
    using word_type                            = uint64_t;
    //!\brief Size in bits of the word_type.
    static constexpr size_t   word_size        = sizeof(word_type) * CHAR_BIT;
    //!\brief The number of letters that fit into a word.
    static constexpr size_t   letters_per_word = word_size / bits_per_letter;
    //!\brief A bitmask that has only the last #bits_per_letter bits set.
    static constexpr uint64_t mask             = (1ull << bits_per_letter) - 1ull;

    //!\brief Type of the underlying SDSL vector.
    using data_type = std::vector<uint64_t>;

    //!\brief The size!
    size_t size_ = 0;

    //!\brief The data storage.
    data_type data;

    //!\brief Decode a rank from compressed storage.
    static uint64_t get_rank(data_type const & vec, size_t const i) noexcept
    {
        assert(i / letters_per_word < vec.size());
        uint64_t const word   = vec[i / letters_per_word];
        size_t const   offset = (i % letters_per_word) * bits_per_letter;
        return (word >> offset) & mask;
    }

    //!\brief Store a rank to compressed storage.
    static void set_rank(data_type & vec, size_t const i, uint64_t const rank) noexcept
    {
        assert(i / letters_per_word < vec.size());
        uint64_t &   word   = vec[i / letters_per_word];
        size_t const offset = (i % letters_per_word) * bits_per_letter;
        word &= ~(mask << offset);
        word |= rank << offset;
    }

    //!\brief Zeros out the bits behind the last element in the last word.
    void clear_unused_bits_in_last_word()
    {
        if (data.empty() || (size() % letters_per_word == 0))
            return;

        size_t const offset = (size() % letters_per_word) * bits_per_letter;
        data.back() &= ((1ull << offset) - 1ull);
    }

    //!\brief Proxy data type returned by bio::ranges::bitcompressed_vector as reference to element unless the alphabet_type
    //!       is uint8_t, uint16_t, uint32_t or uint64_t (in which case a regular & is returned).
    class reference_proxy_type : public alphabet::alphabet_proxy<reference_proxy_type, alphabet_type>
    {
    private:
        //!\brief The base type.
        using base_t = alphabet::alphabet_proxy<reference_proxy_type, alphabet_type>;

        //!\brief Pointer to the host's storage container.
        data_type * data_ptr;
        //!\brief (Uncompressed) index of the current element.
        size_t      index;

    public:
        /*!\name Constructors, destructor and assignment
         * \{
         */
        //!\brief Deleted, because using this proxy without a parent would be undefined behaviour.
        reference_proxy_type()                                                = delete;
        constexpr reference_proxy_type(reference_proxy_type const &) noexcept = default; //!< Defaulted.
        constexpr reference_proxy_type(reference_proxy_type &&) noexcept      = default; //!< Defaulted.
        ~reference_proxy_type() noexcept                                      = default; //!< Defaulted.

        // Import from base:
        using base_t::operator=;

        //!\brief Assignment does not change `this`, instead it updates the referenced value.
        constexpr reference_proxy_type & operator=(reference_proxy_type const & rhs)
        {
            return assign_rank(rhs.to_rank());
        }

        //!\brief Assignment does not change `this`, instead it updates the referenced value (also works on `const` objects).
        constexpr reference_proxy_type const & operator=(reference_proxy_type const & rhs) const
        {
            return assign_rank(rhs.to_rank());
        }

        //!\brief The main constructor to create this object.
        reference_proxy_type(data_type * const data_ptr_, size_t const index_) noexcept :
          data_ptr{data_ptr_}, index{index_}
        {}
        //!\}

        //!\brief Retrieve the compressed representation.
        constexpr alphabet::alphabet_rank_t<alphabet_type> to_rank() const noexcept
        {
            return get_rank(*data_ptr, index);
        }

        //!\brief Update the compressed representation.
        constexpr reference_proxy_type & assign_rank(alphabet::alphabet_rank_t<alphabet_type> const r) noexcept
        {
            set_rank(*data_ptr, index, r);
            return *this;
        }

        //!\brief Update the compressed representation (also works on `const` objects).
        constexpr reference_proxy_type const & assign_rank(
          alphabet::alphabet_rank_t<alphabet_type> const r) const noexcept
        {
            set_rank(*data_ptr, index, r);
            return *this;
        }
    };

    static_assert(alphabet::writable_alphabet<reference_proxy_type>);
    //!\cond
    //NOTE(h-2): it is entirely unclear to me why we need this
    template <typename t>
        requires std::is_same_v<std::ranges::range_value_t<std::remove_cvref_t<t>>, alphabet_type>
    static constexpr bool has_same_value_type_v = true;
    //!\endcond

public:
    /*!\name Associated types
     * \{
     */
    //!\brief Equals the alphabet_type.
    using value_type      = alphabet_type;
    //!\brief A proxy type that enables assignment, if the underlying data structure also provides a proxy.
    using reference       = reference_proxy_type;
    //!\brief Equals the alphabet_type / value_type.
    using const_reference = alphabet_type;
    //!\brief The iterator type of this container (a random access iterator).
    using iterator        = detail::random_access_iterator<bitcompressed_vector>;
    //!\brief The const_iterator type of this container (a random access iterator).
    using const_iterator  = detail::random_access_iterator<bitcompressed_vector const>;
    //!\brief A signed integer type (usually std::ptrdiff_t)
    using difference_type = std::ranges::range_difference_t<data_type>;
    //!\brief An unsigned integer type (usually std::size_t)
    using size_type       = std::ranges::range_size_t<data_type>;
    //!\}

    //!\cond
    // this signals to range-v3 that something is a container :|
    using allocator_type = void;
    //!\endcond

    /*!\name Constructors, destructor and assignment
     * \{
     */
    bitcompressed_vector()                                                   = default; //!< Defaulted.
    constexpr bitcompressed_vector(bitcompressed_vector const &)             = default; //!< Defaulted.
    constexpr bitcompressed_vector(bitcompressed_vector &&)                  = default; //!< Defaulted.
    constexpr bitcompressed_vector & operator=(bitcompressed_vector const &) = default; //!< Defaulted.
    constexpr bitcompressed_vector & operator=(bitcompressed_vector &&)      = default; //!< Defaulted.
    ~bitcompressed_vector()                                                  = default; //!< Defaulted.

    /*!\brief Construct from a different range.
     * \tparam other_range_t The type of range to construct from; must satisfy std::ranges::input_range and
     *                       std::common_reference_with<std::ranges::range_value_t<other_range_t>, value_type>.
     * \param[in]      range The sequences to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    template <std::ranges::input_range other_range_t>
        //!\cond
        requires has_same_value_type_v<other_range_t>
    //!\endcond
    explicit bitcompressed_vector(other_range_t && range) :
      bitcompressed_vector{std::ranges::begin(range), std::ranges::end(range)}
    {}

    /*!\brief Construct with `count` times `value`.
     * \param[in] count Number of elements.
     * \param[in] value The initial value to be assigned.
     *
     * ### Complexity
     *
     * In \f$O(count)\f$.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    bitcompressed_vector(size_type const count, value_type const value) { insert(cend(), count, value); }

    /*!\brief Construct from pair of iterators.
     * \tparam begin_iterator_type Must model std::forward_iterator and
     *                             std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>.
     * \tparam   end_iterator_type Must model std::sentinel_for.
     * \param[in]         begin_it Begin of range to construct/assign from.
     * \param[in]           end_it End of range to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    template <std::forward_iterator begin_iterator_type, typename end_iterator_type>
    bitcompressed_vector(begin_iterator_type begin_it, end_iterator_type end_it)
      //!\cond
      requires(std::sentinel_for<end_iterator_type, begin_iterator_type> &&
                 std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>)
    //!\endcond
    {
        insert(cend(), begin_it, end_it);
    }

    /*!\brief Construct from `std::initializer_list`.
     * \param[in] ilist A `std::initializer_list` of value_type.
     *
     * ### Complexity
     *
     * Linear in the size of `ilist`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    bitcompressed_vector(std::initializer_list<value_type> ilist) :
      bitcompressed_vector(std::begin(ilist), std::end(ilist))
    {}

    /*!\brief Assign from `std::initializer_list`.
     * \param[in] ilist A `std::initializer_list` of value_type.
     *
     * ### Complexity
     *
     * Linear in the size of `ilist`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    bitcompressed_vector & operator=(std::initializer_list<value_type> ilist)
    {
        assign(std::begin(ilist), std::end(ilist));
        return *this;
    }

    /*!\brief Assign from a different range.
     * \tparam other_range_t The type of range to be inserted; must satisfy std::ranges::input_range and
     *                       std::common_reference_with<std::ranges::range_value_t<other_range_t>, value_type>.
     * \param[in]      range The sequences to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    template <std::ranges::input_range other_range_t>
    void assign(other_range_t && range)
      //!\cond
      requires std::common_reference_with<std::ranges::range_value_t<other_range_t>, value_type>
    //!\endcond
    {
        bitcompressed_vector rhs{std::forward<other_range_t>(range)};
        swap(rhs);
    }

    /*!\brief Assign with `count` times `value`.
     * \param[in] count Number of elements.
     * \param[in] value The initial value to be assigned.
     *
     * ### Complexity
     *
     * In \f$O(count)\f$.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    void assign(size_type const count, value_type const value)
    {
        bitcompressed_vector rhs{count, value};
        swap(rhs);
    }

    /*!\brief Assign from pair of iterators.
     * \tparam begin_iterator_type Must satisfy std::forward_iterator and
     *                             std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>.
     * \tparam   end_iterator_type Must satisfy std::sentinel_for.
     * \param[in]         begin_it Begin of range to construct/assign from.
     * \param[in]           end_it End of range to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    template <std::forward_iterator begin_iterator_type, typename end_iterator_type>
    void assign(begin_iterator_type begin_it, end_iterator_type end_it)
      //!\cond
      requires(std::sentinel_for<end_iterator_type, begin_iterator_type> &&
                 std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>)
    //!\endcond
    {
        bitcompressed_vector rhs{begin_it, end_it};
        swap(rhs);
    }

    /*!\brief Assign from `std::initializer_list`.
     * \param[in] ilist A `std::initializer_list` of value_type.
     *
     * ### Complexity
     *
     * Linear in the size of `ilist`.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    void assign(std::initializer_list<value_type> ilist) { assign(std::begin(ilist), std::end(ilist)); }

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
    iterator begin() noexcept { return iterator{*this}; }

    //!\copydoc begin()
    const_iterator begin() const noexcept { return const_iterator{*this}; }

    //!\copydoc begin()
    const_iterator cbegin() const noexcept { return const_iterator{*this}; }

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
    iterator end() noexcept { return iterator{*this, size()}; }

    //!\copydoc end()
    const_iterator end() const noexcept { return const_iterator{*this, size()}; }

    //!\copydoc end()
    const_iterator cend() const noexcept { return const_iterator{*this, size()}; }
    //!\}

    /*!\name Element access
     * \{
     */
    /*!\brief Return the i-th element.
     * \param[in]              i Index of the element to retrieve.
     * \throws std::out_of_range If you access an element behind the last.
     * \returns                  Either a writable proxy to the element or a copy (if called in const context).
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Throws std::out_of_range if `i >= size()`.
     */
    reference at(size_type const i)
    {
        if (i >= size()) // [[unlikely]]
        {
            throw std::out_of_range{"Trying to access element behind the last in bitcompressed_vector."};
        }
        return (*this)[i];
    }

    //!\copydoc at()
    const_reference at(size_type const i) const
    {
        if (i >= size()) // [[unlikely]]
        {
            throw std::out_of_range{"Trying to access element behind the last in bitcompressed_vector."};
        }
        return (*this)[i];
    }

    /*!\brief Return the i-th element.
     * \param i The element to retrieve.
     * \returns Either a writable proxy to the element or a copy (if called in const context).
     *
     * Accessing an element behind the last causes undefined behaviour. In debug mode an assertion checks the size of
     * the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    reference operator[](size_type const i) noexcept
    {
        assert(i < size());
        return {&data, i};
    }

    //!\copydoc operator[]()
    const_reference operator[](size_type const i) const noexcept
    {
        assert(i < size());
        return alphabet::assign_rank_to(get_rank(data, i), alphabet_type{});
    }

    /*!\brief Return the first element. Calling front on an empty container is undefined.
     * \returns Either a writable proxy to the element or a copy (if called in const context).
     *
     * Calling front on an empty container is undefined. In debug mode an assertion checks the size of the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    reference front() noexcept
    {
        assert(size() > 0);
        return (*this)[0];
    }

    //!\copydoc front()
    const_reference front() const noexcept
    {
        assert(size() > 0);
        return (*this)[0];
    }

    /*!\brief Return the last element.
     * \returns Either a writable proxy to the element or a copy (if called in const context).
     *
     * Calling back on an empty container is undefined. In debug mode an assertion checks the size of the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    reference back() noexcept
    {
        assert(size() > 0);
        return (*this)[size() - 1];
    }

    //!\copydoc back()
    const_reference back() const noexcept
    {
        assert(size() > 0);
        return (*this)[size() - 1];
    }

    /*!\brief Provides direct, unsafe access to underlying data structures.
     * \returns A reference to a std::vector
     *
     * \details
     *
     * The exact representation of the data is implementation defined. Do not rely on it for API stability.
     */
    constexpr data_type & raw_data() noexcept { return data; }

    //!\copydoc raw_data()
    constexpr data_type const & raw_data() const noexcept { return data; }
    //!\}

    /*!\name Capacity
     * \{
     */
    /*!\brief Checks whether the container is empty.
     * \returns `true` if the container is empty, `false` otherwise.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    bool empty() const noexcept { return size() == 0; }

    /*!\brief Returns the number of elements in the container, i.e. std::distance(begin(), end()).
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
    size_type size() const noexcept { return size_; }

    /*!\brief Returns the maximum number of elements the container is able to hold due to system or library
     * implementation limitations, i.e. std::distance(begin(), end()) for the largest container.
     * \returns The number of elements in the container.
     *
     * This value typically reflects the theoretical limit on the size of the container. At runtime, the size
     * of the container may be limited to a value smaller than max_size() by the amount of RAM available.
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_type max_size() const noexcept
    {
        // this protects against underflow in the multiplication
        return std::max<size_type>(data.max_size(), data.max_size() * letters_per_word);
    }

    /*!\brief Returns the number of elements that the container has currently allocated space for.
     * \returns The capacity of the currently allocated storage.
     *
     * \attention
     *
     * This does not operate on underlying concat container, see concat_capacity().
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_type capacity() const noexcept { return data.capacity() * letters_per_word; }

    /*!\brief Increase the capacity to a value that's greater or equal to new_cap.
     * \param            new_cap The new capacity.
     * \throws std::length_error If new_cap > max_size().
     * \throws    std::exception Any exception thrown by `Allocator::allocate()` (typically `std::bad_alloc`).
     *
     * Increase the capacity of the vector to a value that's greater or equal to new_cap.
     * If new_cap is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
     * If new_cap is greater than capacity(), all iterators, including the past-the-end iterator, and all references
     * to the elements are invalidated. Otherwise, no iterators or references are invalidated.
     *
     * ### Complexity
     *
     * At most linear in the size() of the container.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    void reserve(size_type const new_cap)
    {
        data.reserve(new_cap / letters_per_word + (new_cap % letters_per_word != 0));
    }

    /*!\brief Requests the removal of unused capacity.
     *
     * It is a non-binding request to reduce capacity() to size() and concat_capacity() to concat_size().
     * It depends on the implementation if the request is fulfilled.
     * If reallocation occurs, all iterators, including the past the end iterator, and all references to the elements
     * are invalidated. If no reallocation takes place, no iterators or references are invalidated.
     *
     * ### Complexity
     *
     * At most linear in the size() of the container.
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    void shrink_to_fit() { data.shrink_to_fit(); }
    //!\}

    /*!\name Modifiers
     * \{
     */
    /*!\brief Removes all elements from the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    void clear() noexcept
    {
        data.clear();
        size_ = 0;
    }

    /*!\brief Inserts value before position in the container.
     * \param   pos Iterator before which the content will be inserted. `pos` may be the end() iterator.
     * \param value Element value to insert.
     * \returns     Iterator pointing to the inserted value.
     *
     * \details
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    iterator insert(const_iterator pos, value_type const value) { return insert(pos, 1, value); }

    /*!\brief Inserts count copies of value before position in the container.
     * \param   pos Iterator before which the content will be inserted. `pos` may be the end() iterator.
     * \param count Number of copies.
     * \param value Element value to insert.
     * \returns     Iterator pointing to the first element inserted, or `pos` if `count==0`.
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    iterator insert(const_iterator pos, size_type const count, value_type const value)
    {
        auto v = views::repeat_n(value, count);
        return insert(pos, v.begin(), v.end());
    }

    /*!\brief Inserts elements from range `[begin_it, end_it)` before position in the container.
     * \tparam begin_iterator_type Must satisfy std::forward_iterator and
     *                             std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>.
     * \tparam   end_iterator_type Must satisfy std::sentinel_for.
     * \param[in]              pos Iterator before which the content will be inserted. `pos` may be the end() iterator.
     * \param[in]         begin_it Begin of range to construct/assign from.
     * \param[in]           end_it End of range to construct/assign from.
     * \returns                    Iterator pointing to the first element inserted, or `pos` if `begin_it==end_it`.
     *
     * The behaviour is well-defined, even if begin_it and end_it are iterators into `*this`.
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    template <std::forward_iterator begin_iterator_type, typename end_iterator_type>
    iterator insert(const_iterator pos, begin_iterator_type begin_it, end_iterator_type end_it)
      //!\cond
      requires(std::sentinel_for<end_iterator_type, begin_iterator_type> &&
                 std::common_reference_with<std::iter_value_t<begin_iterator_type>, value_type>)
    //!\endcond
    {
        //TODO UPDATE DOCUMENTATION TO REFLECT THIS
        //TODO this is not ideal, always linear
        size_t const         pos_as_num     = std::distance(cbegin(), pos);
        size_t const         size_of_insert = std::distance(begin_it, end_it);
        bitcompressed_vector tmp;
        tmp.resize(size() + size_of_insert);
        //TODO use constrained algorithms here once alphabet_proxy is out-iterator-compatible
        std::copy(cbegin(), pos, tmp.begin());
        std::copy(begin_it, end_it, tmp.begin() + pos_as_num);
        std::copy(cbegin() + pos_as_num, cend(), tmp.begin() + pos_as_num + size_of_insert);

        std::swap(*this, tmp);
        return begin() + pos_as_num;
    }

    /*!\brief Inserts elements from initializer list before position in the container.
     * \param   pos Iterator before which the content will be inserted. `pos` may be the end() iterator.
     * \param ilist Initializer list with values to insert.
     * \returns     Iterator pointing to the first element inserted, or `pos` if `ilist` is empty.
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    iterator insert(const_iterator pos, std::initializer_list<value_type> const & ilist)
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /*!\brief Removes specified elements from the container.
     * \param begin_it Begin of range to erase.
     * \param   end_it Behind the end of range to erase.
     * \returns        Iterator following the last element removed. If the iterator `begin_it` refers to the last
     *                 element or begin_it == end_it, the end() iterator is returned.
     *
     * \details
     *
     * The iterator begin_it does not need to be dereferenceable if begin_it==end_it: erasing an empty range is a no-op.
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    iterator erase(const_iterator begin_it, const_iterator end_it)
    {
        if (begin_it == end_it)
            return end();

        //TODO this is not ideal, always linear
        size_t const         begin_pos_of_removal = std::distance(cbegin(), begin_it);
        size_t const         size_of_removal      = std::distance(begin_it, end_it);
        bitcompressed_vector tmp;
        tmp.resize(size() - size_of_removal);
        //TODO use constrained algorithms here once alphabet_proxy is out-iterator-compatible
        std::copy(cbegin(), begin_it, tmp.begin());
        std::copy(end_it, cend(), tmp.begin() + begin_pos_of_removal);

        std::swap(*this, tmp);
        return begin() + begin_pos_of_removal + size_of_removal;
    }

    /*!\brief Removes specified elements from the container.
     * \param   pos Remove the element at pos.
     * \returns     Iterator following the last element removed. If the iterator `pos` refers to the last element,
     *              the end() iterator is returned.
     *
     * This function always reallocates, so all iterators and references are invalidated.
     *
     * ### Complexity
     *
     * Linear in the new size().
     *
     * ### Exceptions
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }

    /*!\brief Appends the given element value to the end of the container.
     * \param value The value to append.
     *
     * If the new size() is greater than capacity() then all iterators and references (including the past-the-end
     * iterator) are invalidated. Otherwise only the past-the-end iterator is invalidated.
     *
     * ### Complexity
     *
     * Amortised constant, worst-case linear in size().
     *
     * ### Exceptions
     *
     * Basic exception guarantee, i.e. guaranteed not to leak, but container may contain invalid data after exception is
     * thrown.
     */
    void push_back(value_type const value)
    {
        if (data.size() * letters_per_word == size_) // the last word is "full"
            data.emplace_back();

        set_rank(data, size(), alphabet::to_rank(value));
        ++size_;
    }

    /*!\brief Removes the last element of the container.
     *
     * Calling pop_back() on an empty container is undefined. In debug mode an assertion will be thrown.
     *
     * No iterators or references except for back() and end() are invalidated.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No exception is thrown in release mode.
     *
     * Strong exception guarantee (no data is modified in case an exception is thrown).
     */
    void pop_back()
    {
        assert(size() > 0);
        if ((size_--) % letters_per_word == 1) // the last word contains only one letter
            data.pop_back();
        else
            clear_unused_bits_in_last_word();
    }

    /*!\brief Resizes the container to contain count elements.
     * \param              count The new size.
     * \throws std::length_error If count > max_size().
     * \throws    std::exception Any exception thrown by `Allocator::allocate()` (typically `std::bad_alloc`).
     *
     * Increase the size() of the vector to count.
     *
     * If the current capacity() is smaller than count, new storage is allocated and all iterators, including
     * the past-the-end iterator, and all references to the elements are invalidated.
     * Otherwise only the past-the-end iterator is invalidated.
     *
     * If the current size is greater than count, the container is reduced to its first count elements.
     * Capacity is never reduced when resizing to smaller size because that would invalidate all iterators, rather
     * than only the ones that would be invalidated by the equivalent sequence of pop_back() calls.
     *
     * ### Complexity
     *
     * At most linear in the size() of the container.
     *
     * ### Exceptions
     *
     * Only new size: Strong exception guarantee (no data is modified in case an exception is thrown).
     *
     * New default value: Basic exception guarantee, i.e. guaranteed not to leak, but container my contain bogus data
     * after exceptions is thrown.
     */
    void resize(size_type const count)
    {
        assert(count < max_size());
        data.resize(count / letters_per_word + (count % letters_per_word != 0));
        size_ = count;
        clear_unused_bits_in_last_word();
    }

    /*!\copybrief resize()
     * \param value Append copies of value when resizing.
     * \copydetails resize()
     */
    void resize(size_type const count, value_type const value)
    {
        assert(count < max_size());
        size_t old_size = size_;
        resize(count);
        for (size_t i = old_size; i < size(); ++i)
            set_rank(data, i, alphabet::to_rank(value));
    }

    /*!\brief Swap contents with another instance.
     * \param rhs The other instance to swap with.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void swap(bitcompressed_vector & rhs) noexcept { std::swap(*this, rhs); }

    //!\copydoc swap()
    constexpr void swap(bitcompressed_vector && rhs) noexcept { std::swap(*this, rhs); }

    /*!\brief Swap contents with another instance.
     * \param lhs The first instance.
     * \param rhs The other instance to swap with.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    friend constexpr void swap(bitcompressed_vector & lhs, bitcompressed_vector & rhs) noexcept { std::swap(lhs, rhs); }

    //!\overload
    friend constexpr void swap(bitcompressed_vector && lhs, bitcompressed_vector && rhs) noexcept
    {
        std::swap(lhs, rhs);
    }
    //!\}

    //!\brief Comparison operators.
    friend auto operator<=>(bitcompressed_vector const & lhs, bitcompressed_vector const & rhs) noexcept = default;

    /*!\cond DEV
     * \brief Serialisation support function.
     * \tparam archive_t Type of `archive`; must satisfy bio::typename.
     * \param archive The archive being serialised from/to.
     *
     * \attention These functions are never called directly, see \ref howto_use_cereal for more details.
     */
    template <typename archive_t>
    void serialize(archive_t & archive)
    {
        archive(size_);
        archive(data);
    }
    //!\endcond
};

} // namespace bio::ranges
