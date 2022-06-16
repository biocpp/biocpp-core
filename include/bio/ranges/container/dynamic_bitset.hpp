// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Enrico Seiler <enrico.seiler@fu-berlin.de>
 * \brief A constexpr bitset implementation with dynamic size at compile time.
 */

#pragma once

#include <bio/meta/bit_manipulation.hpp>
#include <bio/meta/concept/cereal.hpp>
#include <bio/ranges/views/interleave.hpp>
#include <bio/ranges/views/repeat_n.hpp>
#include <bio/ranges/views/to.hpp>

namespace bio::detail
{

//!\brief A bit field representing size and bit information stored in one `uint64_t`.
struct dynamic_bitset_bitfield
{
    //!\brief 6 bits representing the size information.
    uint64_t size : 6u;
    //!\brief 58 bits representing the bit information.
    uint64_t bits : 58u;
};

//!\brief Proxy data type returned by bio::dynamic_bitset as reference to the bit.
class dynamic_bitset_reference_proxy
{
public:
    /*!\name Constructors, destructor and assignment
        * \{
        */
    constexpr dynamic_bitset_reference_proxy() noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset_reference_proxy(dynamic_bitset_reference_proxy const &) noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset_reference_proxy(dynamic_bitset_reference_proxy &&) noexcept = default; //!< Defaulted.

    //!\brief Assign the value of the bit.
    constexpr dynamic_bitset_reference_proxy & operator=(dynamic_bitset_reference_proxy const rhs) noexcept
    {
        rhs ? set() : reset();
        return *this;
    }

    //!\brief Sets the referenced bit to `value`.
    constexpr dynamic_bitset_reference_proxy & operator=(bool const value) noexcept
    {
        value ? set() : reset();
        return *this;
    }

    ~dynamic_bitset_reference_proxy() noexcept = default; //!< Defaulted.
    //!\}

    //!\brief Initialise from bio::dynamic_bitset's underlying data and a bit position.
    constexpr dynamic_bitset_reference_proxy(dynamic_bitset_bitfield & internal_, size_t const pos) noexcept :
        internal{internal_}, mask{1ULL<<pos}
    {}

    //!\brief Returns the value of the referenced bit.
    constexpr operator bool() const noexcept
    {
        return static_cast<bool>(internal.bits & mask);
    }

    //!\brief Returns the inverted value of the referenced bit.
    constexpr bool operator~() const noexcept
    {
        return !static_cast<bool>(internal.bits & mask);
    }

    //!\brief Sets the referenced bit to the result of a binary OR with `value`.
    constexpr dynamic_bitset_reference_proxy & operator|=(bool const value)
    {
        if (value)
            set();

        return *this;
    }

    //!\brief Sets the referenced bit to the result of a binary AND with `value`.
    constexpr dynamic_bitset_reference_proxy & operator&=(bool const value)
    {
        if (!value)
            reset();

        return *this;
    }

    //!\brief Sets the referenced bit to the result of a binary XOR with `value`.
    constexpr dynamic_bitset_reference_proxy & operator^=(bool const value)
    {
        operator bool() && value ? reset() : set();
        return *this;
    }

private:
    //!\brief The proxy of the underlying data type.
    dynamic_bitset_bitfield & internal;
    //!\brief Bitmask to access one specific bit.
    uint64_t mask;

    //!\brief Sets the referenced bit to `1`.
    constexpr void set() noexcept
    {
        internal.bits |= mask;
    }

    //!\brief Sets the referenced bit to `0`.
    constexpr void reset() noexcept
    {
        internal.bits &= ~mask;
    }
};

} // namespace bio::detail

namespace bio
{

/*!\brief A constexpr bitset implementation with dynamic size at compile time.
 * \implements bio::reservible_container
 * \implements bio::cerealisable
 * \ingroup container
 * \tparam bit_capacity The capacity of the dynamic bitset
 *
 * \details
 *
 * This implementation of a bitset can be constructed, accessed and modified at compile time.
 * It has a fixed capacity but a dynamic size and provides all functionality of a sequence container. Note
 * that it also models a reservable sequence container but all associated member functions are no-op because the
 * capacity is fixed.
 *
 * ### Example
 *
 * \include test/snippet/ranges/container/dynamic_bitset_usage.cpp
 *
 * ### Thread safety
 *
 * This container provides no thread-safety beyond the promise given also by the STL that all
 * calls to `const` member functions are safe from multiple threads (as long as no thread calls
 * a non-`const` member function at the same time).
 */
template <size_t bit_capacity = 58>
class dynamic_bitset
{
private:
    //!\brief Befriend other template instantiations of dynamic_bitset.
    template <size_t>
    friend class dynamic_bitset;

    //!\brief Stores the actual data.
    detail::dynamic_bitset_bitfield data{0u, 0u}; // Specifying values prevents ICE on gcc < 9 when comparing to default constructed bitset

public:
    static_assert(bit_capacity <= 58, "The capacity of the dynamic_bitset exceeds the limit of 58.");

    /*!\name Associated types
     * \{
     */
    //!\brief Equals `bool`.
    using value_type      = bool;
    //!\brief A proxy type that enables assignment.
    using reference       = detail::dynamic_bitset_reference_proxy;
    //!\brief Equals the value_type.
    using const_reference = bool;
    //!\brief The iterator type of this container (a random access iterator).
    using iterator        = detail::random_access_iterator<dynamic_bitset>;
    //!\brief The `const_iterator` type of this container (a random access iterator).
    using const_iterator  = detail::random_access_iterator<dynamic_bitset const>;
    //!\brief A `std::ptrdiff_t`.
    using difference_type = ptrdiff_t;
    //!\brief An unsigned integer type (usually `std::size_t`).
    using size_type       = detail::min_viable_uint_t<bit_capacity>;
    //!\}

    //!\cond
    // this signals to range-v3 that something is a container :|
    using allocator_type    = void;
    //!\endcond

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr dynamic_bitset()                                   noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset(dynamic_bitset const &)             noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset(dynamic_bitset &&)                  noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset & operator=(dynamic_bitset const &) noexcept = default; //!< Defaulted.
    constexpr dynamic_bitset & operator=(dynamic_bitset &&)      noexcept = default; //!< Defaulted.
    ~dynamic_bitset()                                            noexcept = default; //!< Defaulted.

    /*!\brief Construct from an `uint64_t`.
     * \param[in] value The `uint64_t` to construct/assign from.
     * \throws std::invalid_argument if value has bits set outside of [0, 58).
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_construct_uint64_t.cpp
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Throws std::invalid_argument value has a set bit past the 58 one, i.e. only bits in [0,58) may be set.
     */
    constexpr dynamic_bitset(uint64_t const value)
    {
        if (detail::popcount(value >> 58) != 0)
            throw std::invalid_argument{"The dynamic_bitset can be at most 58 long."};
        data.bits |= value;
        data.size |= value ? detail::most_significant_bit_set(value) + 1 : 0u;
    }

    /*!\brief Construct from two iterators.
     * \tparam begin_it_type Must model std::forward_iterator and `value_type` must be constructible from
     *                       the reference type of `begin_it_type`.
     * \tparam end_it_type   Must model std::sentinel_for.
     * \param[in] begin_it Begin of range to construct/assign from.
     * \param[in] end_it   End of range to construct/assign from.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    template <std::forward_iterator begin_it_type, typename end_it_type>
    //!\cond
        requires (std::sentinel_for<end_it_type, begin_it_type> &&
                  std::constructible_from<value_type, std::iter_reference_t<begin_it_type>>)
    //!\endcond
    constexpr dynamic_bitset(begin_it_type begin_it, end_it_type end_it) noexcept:
        dynamic_bitset{}
    {
        assign(begin_it, end_it);
    }

    /*!\brief Construct from a different range.
     * \tparam other_range_t The type of range to be inserted; must satisfy std::ranges::input_range and `value_type`
     *                       must be constructible from `std::ranges::range_reference_t<other_range_t>`.
     * \param[in] range The sequence to construct/assign from.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    template <std::ranges::input_range other_range_t>
    //!\cond
        requires (!std::same_as<std::remove_cvref_t<other_range_t>, dynamic_bitset>)
    //!\endcond
    explicit constexpr dynamic_bitset(other_range_t && range) noexcept :
        dynamic_bitset{std::ranges::begin(range), std::ranges::end(range)}
    {}

    /*!\brief Construct with `n` times `value`.
     * \param[in] n     Number of elements.
     * \param[in] value The initial value to be assigned.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in `n`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr dynamic_bitset(size_type const n, value_type const value) noexcept :
        dynamic_bitset{}
    {
        assign(n, value);
    }

    /*!\brief Assign from `std::initializer_list`.
     * \param[in] ilist A `std::initializer_list` of `value_type`.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of `ilist`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr dynamic_bitset & operator=(std::initializer_list<value_type> const ilist) noexcept
    {
        assign(std::ranges::begin(ilist), std::ranges::end(ilist));
        return *this;
    }

    /*!\brief Construction from literal.
     * \param[in] lit The literal to construct the string for. May only contain <code>'0'</code> and <code>'1'</code>.
     * \throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     *
     * \details
     *
     * The `char` literal is expected to be null-terminated (asserted in debug-mode). If it is not, the last character
     * will be lost when copying to the instance of `dynamic_bitset`.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_construct_string.cpp
     *
     * ### Complexity
     *
     * Linear in the size of `lit`.
     *
     * ### Exceptions
     *
     * Throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     */
    template <size_t N>
    constexpr dynamic_bitset(char const (&lit)[N]) : dynamic_bitset{}
    {
        static_assert(N <= bit_capacity + 1, "Length of string literal exceeds capacity of dynamic_bitset.");
        assign(lit);
    }

    /*!\brief Assign from literal.
     * \param[in] lit The literal to assign the string from. May only contain <code>'0'</code> and <code>'1'</code>.
     * \throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     *
     * \details
     *
     * The `char` literal is expected to be null-terminated (asserted in debug-mode). If it is not, the last character
     * will be lost when copying to the instance of `dynamic_bitset`.
     *
     * ### Complexity
     *
     * Linear in the size of `lit`.
     *
     * ### Exceptions
     *
     * Throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     */
    template <size_t N>
    constexpr dynamic_bitset & operator=(char const (&lit)[N])
    {
        static_assert(N <= bit_capacity + 1, "Length of string literal exceeds capacity of dynamic_bitset.");
        assign(lit);
        return *this;
    }

    /*!\brief Assign from literal.
     * \param[in] lit The literal to assign the string from. May only contain <code>'0'</code> and <code>'1'</code>.
     * \throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     *
     * \details
     *
     * The `char` literal is expected to be null-terminated (asserted in debug-mode). If it is not, the last character
     * will be lost when copying to the instance of `dynamic_bitset`.
     *
     * ### Complexity
     *
     * Linear in the size of `lit`.
     *
     * ### Exceptions
     *
     * Throws std::invalid_argument if any character is not <code>'0'</code> or <code>'1'</code>.
     */
    template <size_t N>
    constexpr void assign(char const (&lit)[N])
    {
        static_assert(N <= bit_capacity + 1, "Length of string literal exceeds capacity of dynamic_bitset.");
        assert(lit[N - 1] == '\0');
        uint64_t value{};

        for (size_t i = 0; i != N - 1; ++i)
        {
            if (lit[i] == '0')
            {
                value <<= 1;
            }
            else if (lit[i] == '1')
            {
                value <<= 1;
                value |= 1u;
            }
            else
            {
                throw std::invalid_argument{"The string to construct a dynamic_bitset from may only contain 0 and 1."};
            }
        }

        *this = value;
        resize(N - 1);
    }

    /*!\brief Assign from `std::initializer_list`.
     * \param[in] ilist A `std::initializer_list` of `value_type`.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of `ilist`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void assign(std::initializer_list<value_type> const ilist) noexcept
    {
        assign(std::ranges::begin(ilist), std::ranges::end(ilist));
    }

    /*!\brief Assign with `count` times `value`.
     * \param[in] count Number of elements.
     * \param[in] value The initial value to be assigned.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in `count`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void assign(size_type const count, value_type const value) noexcept
    {
        clear();
        auto tmp = views::repeat_n(value, count);
        assign(std::ranges::begin(tmp), std::ranges::end(tmp));
    }

    /*!\brief Assign from a different range.
     * \tparam other_range_t The type of range to be inserted; must satisfy std::ranges::input_range and `value_type`
     *                       must be constructible from `std::ranges::range_reference_t<other_range_t>`.
     * \param[in] range The sequences to construct/assign from.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    template <std::ranges::input_range other_range_t>
    //!\cond
        requires std::constructible_from<value_type, std::ranges::range_reference_t<other_range_t>>
    //!\endcond
    constexpr void assign(other_range_t && range) noexcept
    {
        assign(std::ranges::begin(range), std::ranges::end(range));
    }

    /*!\brief Assign from pair of iterators.
     * \tparam begin_it_type Must model std::forward_iterator and the `value_type` must be constructible from
     *                       the reference type of `begin_it_type`.
     * \tparam end_it_type   Must model std::sentinel_for.
     * \param[in] begin_it Begin of range to construct/assign from.
     * \param[in] end_it   End of range to construct/assign from.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    template <std::forward_iterator begin_it_type, typename end_it_type>
    //!\cond
        requires (std::sentinel_for<end_it_type, begin_it_type> &&
                 std::constructible_from<value_type, std::iter_reference_t<begin_it_type>>)
    //!\endcond
    constexpr void assign(begin_it_type begin_it, end_it_type end_it) noexcept
    {
        clear();
        insert(cbegin(), begin_it, end_it);
    }
    //!\}

    /*!\name Iterators
     * \{
     */
    /*!\brief Returns the begin to the `dynamic_bitset`.
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_begin.cpp
     */
    constexpr iterator begin() noexcept
    {
        return iterator{*this};
    }

    //!\copydoc begin()
    constexpr const_iterator begin() const noexcept
    {
        return const_iterator{*this};
    }

    //!\copydoc begin()
    constexpr const_iterator cbegin() const noexcept
    {
        return begin();
    }

    /*!\brief Returns iterator past the end of the `dynamic_bitset`.
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_begin.cpp
     */
    constexpr iterator end() noexcept
    {
        return iterator{*this, size()};
    }

    //!\copydoc end()
    constexpr const_iterator end() const noexcept
    {
        return const_iterator{*this, size()};
    }

    //!\copydoc end()
    constexpr const_iterator cend() const noexcept
    {
        return end();
    }
    //!\}

    /*!\name Bit manipulation
     * \{
     */
    /*!\brief Sets the bits to the result of binary AND on corresponding pairs of bits of `*this` and `rhs`.
     * \param[in] rhs dynamic_bitset to perform binary AND with.
     * \returns *this
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_binary_and_member.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & operator&=(dynamic_bitset const & rhs) noexcept
    {
        assert(size() == rhs.size());
        data.bits &= rhs.data.bits;
        return *this;
    }

    /*!\brief Sets the bits to the result of binary OR on corresponding pairs of bits of `*this` and `rhs`.
     * \param[in] rhs dynamic_bitset to perform binary OR with.
     * \returns *this
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_binary_or_member.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & operator|=(dynamic_bitset const & rhs) noexcept
    {
        assert(size() == rhs.size());
        data.bits |= rhs.data.bits;
        return *this;
    }

    /*!\brief Sets the bits to the result of binary XOR on corresponding pairs of bits of `*this` and `rhs`.
     * \param[in] rhs dynamic_bitset to perform binary XOR with.
     * \returns *this
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_binary_xor_member.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & operator^=(dynamic_bitset const & rhs) noexcept
    {
        assert(size() == rhs.size());
        data.bits ^= rhs.data.bits;
        return *this;
    }

    /*!\brief Returns a temporary copy of `*this` with all bits flipped (binary NOT).
     * \returns Copy of *this with all bits flipped.
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_flip.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Thread-safety
     *
     * Thread safe.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset operator~() const noexcept
    {
        dynamic_bitset tmp{*this};
        tmp.flip();
        return tmp;
    }

    /*!\brief Performs binary shift left on the current object.
     * \param[in] count Amount to shift to the left.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_shift_left_inplace.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & operator<<=(size_t const count) noexcept
    {
        assert(count > 0);
        assert(count < size());
        data.bits <<= count;
        data.bits &= (1ULL << size()) - 1ULL;
        return *this;
    }

    /*!\brief Performs binary shift right on the current object.
     * \param[in] count Amount to shift to the right.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_shift_right_inplace.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & operator>>=(size_t const count) noexcept
    {
        assert(count > 0);
        assert(count < size());
        data.bits >>= count;
        return *this;
    }

    /*!\brief Performs binary shift right.
     * \param[in] count Amount to shift to the right.
     * \returns Copy of *this with bits shifted count to the right.
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_shift_right_copy.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset operator>>(size_t const count) const noexcept
    {
        assert(count > 0);
        assert(count < size());
        dynamic_bitset tmp{*this};
        tmp >>= count;
        return tmp;
    }

    /*!\brief Performs binary shift left.
     * \param[in] count Amount to shift to the left.
     * \returns Copy of *this with bits shifted count to the left.
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_shift_left_copy.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset operator<<(size_t const count) const noexcept
    {
        assert(count > 0);
        assert(count < size());
        dynamic_bitset tmp{*this};
        tmp <<= count;
        return tmp;
    }

    /*!\brief Sets all bits to `1`.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_set_all.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & set() noexcept
    {
        data.bits |= (1ULL << size()) - 1ULL;
        return *this;
    }

    /*!\brief Sets the i'th bit to `value`.
     * \param[in] i     Index of the bit to set.
     * \param[in] value Value to set. Default true.
     * \throws std::out_of_range if you access an element behind the last.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_set_pos.cpp
     *
     * ### Exception
     *
     * Throws std::out_of_range if `i >= size()`.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & set(size_t const i, bool const value = true)
    {
        at(i) = value;
        return *this;
    }

    /*!\brief Sets all bits to `0`.
     * \returns *this
     *
     * \details
     *
     * \attention
     * In contrast to `clear()`, this method does not modify the size.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_reset_all.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & reset() noexcept
    {
        data.bits = 0u;
        return *this;
    }

    /*!\brief Sets the i'th bit to false.
     * \param[in] i Index of the bit to reset.
     * \throws std::out_of_range if you access an element behind the last.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_reset_pos.cpp
     *
     * ### Exception
     *
     * Throws std::out_of_range if `i >= size()`.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & reset(size_t const i)
    {
        set(i, false);
        return *this;
    }

    /*!\brief Flips all bits (binary NOT).
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_flip_all.cpp
     *
     * ### Exception
     *
     * No-throw guarantee.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & flip() noexcept
    {
        data.bits = ~data.bits;
        data.bits &= (1ULL << size()) - 1ULL;
        return *this;
    }

    /*!\brief Flips the i'th bit (binary NOT).
     * \param[in] i Index of the bit to flip.
     * \throws std::out_of_range if you access an element behind the last.
     * \returns *this
     *
     * \details
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_flip_pos.cpp
     *
     * ### Exception
     *
     * Throws std::out_of_range if `i >= size()`.
     *
     * ### Complexity
     *
     * Constant.
     */
    constexpr dynamic_bitset & flip(size_t const i)
    {
        at(i) ? reset(i) : set(i);
        return *this;
    }
    //!\}

    /*!\name Element Access
     * \{
     */
    /*!\brief Checks if all bit are set.
     * \returns `true` if all bits are set or the bitset is empty, `false` otherwise.
     */
    constexpr bool all() const noexcept
    {
        return count() == size();
    }

    /*!\brief Checks if any bit is set.
     * \returns `true` if any bit is set, `false` otherwise.
     */
    constexpr bool any() const noexcept
    {
        return count() != 0;
    }

    /*!\brief Checks if no bit is set.
     * \returns `true` if no bit is set, `false` otherwise.
     */
    constexpr bool none() const noexcept
    {
        return count() == 0;
    }

    //!\brief Returns the number of set bits.
    constexpr size_type count() const noexcept
    {
        return detail::popcount(data.bits);
    }

    /*!\brief Returns the i-th element.
     * \param[in] i Index of the element to retrieve.
     * \throws std::out_of_range If you access an element behind the last.
     * \returns A reference to the value at position `i`.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Throws std::out_of_range if `i >= size()`.
     */
    constexpr reference at(size_t const i)
    {
        if (i >= size()) // [[unlikely]]
            throw std::out_of_range{"Trying to access position " + std::to_string(i) +
                                    " in a bio::dynamic_bitset of size " + std::to_string(size()) + "."};
        return (*this)[i];
    }

    //!\copydoc at()
    constexpr const_reference at(size_t const i) const
    {
        if (i >= size()) // [[unlikely]]
            throw std::out_of_range{"Trying to access position " + std::to_string(i) +
                                    " in a bio::dynamic_bitset of size " + std::to_string(size()) + "."};
        return (*this)[i];
    }

    //!\copydoc at()
    constexpr const_reference test(size_t const i) const
    {
        return at(i);
    }

    /*!\brief Returns the i-th element.
     * \param[in] i The element to retrieve.
     * \returns A reference to the value at position `i`.
     *
     * \details
     *
     * Accessing an element behind the last causes undefined behaviour. In debug mode an assertion checks the size of
     * the container.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_subscript.cpp
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr reference operator[](size_t const i) noexcept
    {
        assert(i < size());
        return {data, i};
    }

    //!\copydoc operator[]()
    constexpr const_reference operator[](size_t const i) const noexcept
    {
        assert(i < size());
        return data.bits & 1ULL << i;
    }

    /*!\brief Returns the first element.
     * \returns A reference to the value at the first position.
     *
     * \details
     *
     * Calling `front()` on an empty container is undefined. In debug mode an assertion checks
     * the size of the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr reference front() noexcept
    {
        assert(size() > 0);
        return (*this)[0];
    }

    //!\copydoc front()
    constexpr const_reference front() const noexcept
    {
        assert(size() > 0);
        return (*this)[0];
    }

    /*!\brief Returns the last element.
     * \returns A reference to the value at the last position.
     *
     * \details
     *
     * Calling `back()` on an empty container is undefined. In debug mode an assertion checks the size of the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr reference back() noexcept
    {
        assert(size() > 0);
        return (*this)[size() - 1];
    }

    //!\copydoc back()
    constexpr const_reference back() const noexcept
    {
        assert(size() > 0);
        return (*this)[size() - 1];
    }

    //!\brief Direct access to the underlying bit field.
    constexpr detail::dynamic_bitset_bitfield * raw_data() noexcept
    {
        return &data;
    }

    //!\copydoc raw_data()
    constexpr detail::dynamic_bitset_bitfield const * raw_data() const noexcept
    {
        return &data;
    }
    //!\}

    /*!\name Capacity
     * \{
     */
    /*!\brief Checks whether the container is empty.
     * \returns `true` if the container is empty, `false` otherwise.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr bool empty() const noexcept
    {
        return size() == 0;
    }

    /*!\brief Returns the number of elements in the container, i.e. `std::distance(begin(), end())`.
     * \returns The number of elements in the container.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr size_type size() const noexcept
    {
        return data.size;
    }

    /*!\brief Returns the maximum number of elements the container is able to hold and resolves to `bit_capacity`.
     * \returns The number of elements in the container.
     *
     * \details
     *
     * This value typically reflects the theoretical limit on the size of the container. At runtime, the size
     * of the container may be limited to a value smaller than `max_size()` by the amount of RAM available.
     *
     * For the `dynamic_bitset` holds: `max_size() = capacity()`.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr size_type max_size() const noexcept
    {
        return capacity();
    }

    /*!\brief Returns the number of elements that the container is able to hold and resolves to `bit_capacity`.
     * \returns The capacity of the currently allocated storage.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr size_type capacity() const noexcept
    {
        return bit_capacity;
    }

    //!\brief Since the capacity is fixed on compile time, this is a no-op.
    constexpr void reserve(size_t) const noexcept
    {
        // no-op
    }

    //!\brief Since the capacity is fixed on compile time, this is a no-op.
    constexpr void shrink_to_fit() const noexcept
    {
        // no-op
    }
    //!\}

    /*!\name Modifiers
     * \{
     */
    /*!\brief Removes all elements from the container.
     *
     * \details
     *
     * \attention
     * In contrast to `reset()`, this method also sets the size to 0.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void clear() noexcept
    {
        data.size &= 0ULL;
        data.bits &= 0ULL;
    }

    /*!\brief Inserts `value` before `pos` in the container.
     * \param[in] pos   Iterator before which the content will be inserted. `pos` may be the `end()` iterator.
     * \param[in] value Element value to insert.
     * \returns Iterator pointing to the inserted value.
     *
     * \details
     *
     * Inserting a value although the maximum capacity is reached is undefined behaviour.
     *
     * ### Complexity
     *
     * Worst-case linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr iterator insert(const_iterator pos, value_type const value) noexcept
    {
        return insert(pos, 1, value);
    }

    /*!\brief Inserts `count` copies of `value` before position in the container.
     * \param[in] pos   Iterator before which the content will be inserted. `pos` may be the `end()` iterator.
     * \param[in] count Number of copies.
     * \param[in] value Element value to insert.
     * \returns Iterator pointing to the first element inserted, or `pos` if `count==0`.
     *
     * \details
     *
     * If `size()` + `count` > `capacity()` this function results in undefined behaviour.
     *
     * ### Complexity
     *
     * Worst-case linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr iterator insert(const_iterator pos, size_type const count, value_type const value) noexcept
    {
        auto tmp = views::repeat_n(value, count);
        return insert(pos, std::ranges::begin(tmp), std::ranges::end(tmp));
    }

    /*!\brief Inserts elements from range `[begin_it, end_it)` before `pos` in the container.
     * \tparam begin_it_type Must model std::forward_iterator and the `value_type` must be constructible from
     *                       the reference type of begin_it_type.
     * \tparam end_it_type   Must model std::sentinel_for.
     * \param[in] pos      Iterator before which the content will be inserted. `pos` may be the `end()` iterator.
     * \param[in] begin_it Begin of range to construct/assign from.
     * \param[in] end_it   End of range to construct/assign from.
     * \returns Iterator pointing to the first element inserted, or `pos` if `begin_it==end_it`.
     *
     * \details
     *
     * The behaviour is undefined if `begin_it` and `end_it` are iterators into `*this` or if, given the size `n` of the
     * range represented by [`begin_t`, `end_it`), `size()` + `n` > `capacity()`.
     *
     * ### Complexity
     *
     * Worst-case linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    template <std::forward_iterator begin_it_type, typename end_it_type>
    //!\cond
        requires (std::sentinel_for<end_it_type, begin_it_type> &&
                 std::constructible_from<value_type, std::iter_reference_t<begin_it_type>>)
    //!\endcond
    constexpr iterator insert(const_iterator pos, begin_it_type begin_it, end_it_type end_it) noexcept
    {
        auto const pos_as_num = std::ranges::distance(cbegin(), pos);
        auto const length = std::ranges::distance(begin_it, end_it);

        if (length == 0)
            return begin(); // nothing to insert

        size_type const tmp_size{size()};
        resize(tmp_size + length);

        for (size_type i = tmp_size + length - 1; i > pos_as_num + length - 1; --i)
            (*this)[i] = (*this)[i - length];

        // std::ranges::copy(begin_it, end_it, (*this)[pos_as_num]);
        for (auto i = pos_as_num; begin_it != end_it; ++i, ++begin_it)
            (*this)[i] = *begin_it;

        return begin() + pos_as_num;
    }

    /*!\brief Inserts elements from initializer list before `pos` in the container.
     * \param[in] pos   Iterator before which the content will be inserted. `pos` may be the `end()` iterator.
     * \param[in] ilist Initializer list with values to insert.
     * \returns Iterator pointing to the first element inserted, or `pos` if `ilist` is empty.
     *
     * \details
     *
     * Given the size `n` of `ilist`, this function results in undefined behaviour if `size()` + `n` > `capacity()`.
     *
     * ### Complexity
     *
     * Worst-case linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr iterator insert(const_iterator pos, std::initializer_list<value_type> const & ilist) noexcept
    {
        return insert(pos, ilist.begin(), ilist.end());
    }

    /*!\brief Removes specified elements from the container.
     * \param[in] begin_it Begin of range to erase.
     * \param[in] end_it   Behind the end of range to erase.
     * \returns Iterator following the last element removed. If the iterator `pos` refers to the last element,
     *          the `end()` iterator is returned.
     *
     * \details
     *
     * Invalidates iterators and references at or after the point of the erase, including the `end()` iterator.
     *
     * The iterator begin_it does not need to be dereferenceable if `begin_it==end_it`:
     * erasing an empty range is a no-op.
     *
     * ### Complexity
     *
     * Linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr iterator erase(const_iterator begin_it, const_iterator end_it) noexcept
    {
        if (begin_it >= end_it) // [[unlikely]]
            return begin() + std::ranges::distance(cbegin(), end_it);

        auto const length = std::ranges::distance(begin_it, end_it);
        auto out_it = begin() + std::ranges::distance(cbegin(), begin_it);

        while (end_it != cend())
            *(out_it++) = *(end_it++);

        resize(size() - length);
        return begin() + std::ranges::distance(cbegin(), begin_it);
    }

    /*!\brief Removes specified elements from the container.
     * \param[in] pos Remove the element at pos.
     * \returns Iterator following the last element removed. If the iterator `pos` refers to the last element,
     *          the `end()` iterator is returned.
     *
     * \details
     *
     * Invalidates iterators and references at or after the point of the erase, including the `end()` iterator.
     *
     * The iterator `pos` must be valid and dereferenceable. Thus the `end()` iterator (which is valid, but is not
     * dereferencable) cannot be used as a value for `pos`.
     *
     * ### Complexity
     *
     * Linear in `size()`.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr iterator erase(const_iterator pos) noexcept
    {
       return erase(pos, pos + 1);
    }

    /*!\brief Appends the given element `value` to the end of the container.
     * \param[in] value The value to append.
     *
     * \details
     *
     * If the new `size(`) is greater than `capacity()` this is undefined behaviour.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void push_back(value_type const value) noexcept
    {
        assert(size() < bit_capacity);
        resize(size() + 1);
        (*this)[size() - 1] = value;
    }

    /*!\brief Removes the last element of the container.
     *
    * \details
     *
     * Calling `pop_back()` on an empty container is undefined. In debug mode an assertion will be thrown.
     *
     * No iterators or references except for `back()` and `end()` are invalidated.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void pop_back() noexcept
    {
        assert(size() > 0);
        resize(size() - 1);
    }

    /*!\brief Resizes the container to contain count elements.
     * \param[in] count The new size.
     * \param[in] value Append copies of `value` when resizing, default = `false`.
     *
     * \details
     *
     * If `count` is greater than `capacity()` this is undefined behaviour.
     * If the `dynamic_bitset` is enlarged, bits in [0, `size()`) stay the same and bits in [`size()`, `count`)
     * are set to `value`.
     * If the `dynamic_bitset` is shrunk, bits in [0, `count`) stay the same and bits in [`count`, `size()`) are
     * set to `0`.
     *
     * ### Example
     *
     * \include test/snippet/ranges/container/dynamic_bitset_resize.cpp
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    constexpr void resize(size_type const count, value_type const value = false) noexcept
    {
        assert(count <= bit_capacity);
        // Enlarging.
        data.bits |= value && count > size() ? ((1ULL << (count - size())) - 1) << size() : 0ULL;
        // Set size bits.
        data.size = count;
        // Shrinking.
        data.bits &= (1ULL << size()) - 1ULL;
    }

    /*!\brief Swap contents with another instance.
     * \param[in] rhs The other instance to swap with.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of both containers.
     *
     * ### Exceptions
     *
     * No-throw guarantee if value_type is std::is_nothrow_copy_constructible.
     */
    constexpr void swap(dynamic_bitset & rhs) noexcept
    {
        detail::dynamic_bitset_bitfield tmp = std::move(data);
        data = std::move(rhs.data);
        rhs.data = std::move(tmp);
    }

    //!\overload
    constexpr void swap(dynamic_bitset && rhs) noexcept
    {
        data = std::move(rhs.data);
    }

    //!\}

    /*!\brief Swap contents with another instance.
     * \param[in] lhs The first instance.
     * \param[in] rhs The other instance to swap with.
     *
     * \details
     *
     * ### Complexity
     *
     * Linear in the size of both containers.
     *
     * ### Exceptions
     *
     * No-throw guarantee if value_type is std::is_nothrow_copy_constructible.
     */
    friend constexpr void swap(dynamic_bitset & lhs, dynamic_bitset & rhs) noexcept
    {
        lhs.swap(rhs);
    }

    /*!\name Binary operators
     * \{
     */
    /*!\brief Returns dynamic_bitset containing the result of binary AND on corresponding pairs of bits of
     *        `lhs` and `rhs`.
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     */
    template <size_t cap>
    //!\cond
        requires (cap <= bit_capacity)
    //!\endcond
    friend constexpr dynamic_bitset operator&(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        assert(lhs.size() == rhs.size());
        dynamic_bitset tmp{lhs};
        tmp &= rhs;
        return tmp;
    }

    /*!\brief Returns dynamic_bitset containing the result of binary XOR on corresponding pairs of bits of
     *        `lhs` and `rhs`.
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     */
    template <size_t cap>
    //!\cond
        requires (cap <= bit_capacity)
    //!\endcond
    friend constexpr dynamic_bitset operator^(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        assert(lhs.size() == rhs.size());
        dynamic_bitset tmp{lhs};
        tmp ^= rhs;
        return tmp;
    }

    /*!\brief Returns dynamic_bitset containing the result of binary OR on corresponding pairs of bits of
     *        `lhs` and `rhs`.
     *
     * \details
     *
     * \attention
     * Both dynamic_bitsets must have the same size. In debug mode an assertion checks this constraint.
     */
    template <size_t cap>
    //!\cond
        requires (cap <= bit_capacity)
    //!\endcond
    friend constexpr dynamic_bitset operator|(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        assert(lhs.size() == rhs.size());
        dynamic_bitset tmp{lhs};
        tmp |= rhs;
        return tmp;
    }
    //!\}

    /*!\name Comparison operators
     * \{
     */
    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator==(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return lhs.data.size == rhs.raw_data()->size && lhs.data.bits == rhs.raw_data()->bits;
    }

    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator!=(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return !(lhs == rhs);
    }

    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator<(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return lhs.data.bits < rhs.raw_data()->bits;
    }

    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator>(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return lhs.data.bits > rhs.raw_data()->bits;
    }

    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator<=(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return !(lhs > rhs);
    }

    //!\brief Performs element-wise comparison.
    template <size_t cap>
    friend constexpr bool operator>=(dynamic_bitset const & lhs, dynamic_bitset<cap> const & rhs) noexcept
    {
        return !(lhs < rhs);
    }
    //!\}

    /*!\name Conversions
     * \{
     */
    /*!\brief Converts the `dynamic_bitset` to an `unsigned long` integer.
     * \throws std::overflow_error if the value cannot be represented in `unsigned long`.
     * \returns A `unsigned long` representing the `dynamic_bitset`.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Throws std::overflow_error if the value cannot be represented in `unsigned long`.
     */
    inline constexpr unsigned long to_ulong() const
    {
        if constexpr (std::numeric_limits<unsigned long>::max() < std::numeric_limits<size_t>::max())
        {
            if (data.bits > std::numeric_limits<unsigned long>::max())
                throw std::overflow_error{"bio::dynamic_bitset cannot be represented as unsigned long."};
        }

        return static_cast<unsigned long>(data.bits);
    }

    /*!\brief Converts the `dynamic_bitset` to an `unsigned long long` integer.
     * \throws std::overflow_error if the value cannot be represented in `unsigned long long`.
     * \returns A `unsigned long long` representing the `dynamic_bitset`.
     *
     * \details
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Throws std::overflow_error if the value cannot be represented in `unsigned long long`.
     */
    inline constexpr unsigned long long to_ullong() const
    {
        if constexpr (std::numeric_limits<unsigned long long>::max() < std::numeric_limits<size_t>::max())
        {
            if (data.bits >= std::numeric_limits<unsigned long long>::max())
                throw std::overflow_error{"bio::dynamic_bitset cannot be represented as unsigned long long."};
        }

        return static_cast<unsigned long long>(data.bits);
    }
    //!\}

    //!\cond DEV
    /*!\brief Serialisation support function.
     * \tparam archive_t Type of `archive`; must satisfy bio::cereal_archive.
     * \param[in] archive The archive being serialised from/to.
     *
     * \details
     *
     * \attention
     * These functions are never called directly, see \ref howto_use_cereal for more details.
     */
    template <cereal_archive archive_t>
    void CEREAL_SERIALIZE_FUNCTION_NAME(archive_t & archive)
    {
        uint64_t size = data.size;
        archive(size);
        data.size = size;
        uint64_t bits = data.bits;
        archive(bits);
        data.bits = bits;
    }
    //!\endcond
};

} // namespace bio

namespace std
{

/*!\brief Struct for hashing a `bio::dynamic_bitset`.
 * \ingroup container
 * \tparam cap Capacity of the `bio::dynamic_bitset`.
 */
template <size_t cap>
struct hash<bio::dynamic_bitset<cap>>
{
    /*!\brief Compute the hash for a `bio::dynamic_bitset`.
     * \param[in] arg The `bio::dynamic_bitset` to process.
     * \returns `size_t`.
     * \sa bio::dynamic_bitset.to_ullong().
     */
    size_t operator()(bio::dynamic_bitset<cap> const arg) const noexcept
    {
        return static_cast<size_t>(arg.to_ullong());
    }
};

} //namespace std


#if __has_include(<fmt/format.h>)

#include <fmt/ranges.h>

template <>
struct fmt::formatter<bio::detail::dynamic_bitset_reference_proxy> : fmt::formatter<bool>
{
    constexpr auto format(bio::detail::dynamic_bitset_reference_proxy const a, auto & ctx) const
    {
        return fmt::formatter<bool>::format(static_cast<bool>(a), ctx);
    }
};

template <size_t bit_capacity>
struct fmt::is_range<bio::dynamic_bitset<bit_capacity>, char> : std::false_type
{};

template <size_t bit_capacity>
struct fmt::formatter<bio::dynamic_bitset<bit_capacity>> : fmt::formatter<std::string>
{
    constexpr auto format(bio::dynamic_bitset<bit_capacity> const s, auto & ctx) const
    {
        std::string str{"0b"};
        str.reserve(s.size()); // TODO fix me
        auto v = s
               | std::views::reverse
               | std::views::transform([] (bool const bit) { return bit ? '1' : '0'; })
               | bio::views::interleave(4, std::string_view{"'"});
        std::ranges::copy(v, std::back_inserter(str));
        return fmt::formatter<std::string>::format(str, ctx);
    }
};

#endif
