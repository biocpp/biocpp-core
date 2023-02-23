// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::ranges::dictionary.
 */

#pragma once

#include <array>
#include <concepts>
#include <initializer_list>
#include <ranges>
#include <type_traits>
#include <unordered_map>

#if __has_include(<cereal/types/vector.hpp>)
#    include <cereal/types/vector.hpp>
#endif
#if __has_include(<cereal/types/unordered_map.hpp>)
#    include <cereal/types/unordered_map.hpp>
#endif

#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/detail/int_types.hpp>
#include <bio/ranges/detail/random_access_iterator.hpp>

namespace bio::ranges
{

/*!\brief An associative container with contiguous, predictable storage.
 * \implements std::ranges::random_access_range
 * \implements std::ranges::sized_range
 * \implements std::ranges::common_range
 * \implements bio::ranges::back_insertable
 * \implements bio::cerealisable
 * \ingroup container
 * \tparam key_t   The key type; must be convertible to std::string_view (e.g. also std::string).
 * \tparam mapped_t The mapped type; must be an object type.
 *
 * This container behaves like a mixture of std::vector and std::unordered_map.
 *
 * It has the following properties:
 *   * The element type is `std::tuple<key_t, mapped_t>`, but the reference type is `std::tuple<key_t const &, mapped_t &>`.
 *   * Contiguous storage of elements inside a std::vector (internally).
 *   * O(1) access to elements via operator[] and ~O(1) push_back (like std::vector).
 *   * ~O(1) access to mapped value via the key (like std::unordered_map).
 *   * The size overhead compared to std::vector: every key stored twice + at least one uint64_t per element.
 *   * The key_type must be convertible to std::string_view, and every key must be unique.
 *   * There is no `insert()` member, only `assign()` and `push_back()` to add elements.
 *   * There is no `erase()` member, only `clear()` and `pop_pack()` to remove elements.
 *   * There is no `resize()` member, but there is a `reserve()` member.
 *
 * This data structure is used in several places where the order of elements must be preserved but fast random
 * access by key is still desirable. It usually makes sense when the key-string is short (less than 16 characters),
 * the value_type is large, and the data structure does not need many changes after construction.
 */
template <typename key_t, typename mapped_t>
class dictionary
{
public:
    static_assert(std::convertible_to<key_t, std::string_view>,
                  "The key type must be std::convertible_to<string_view>.");
    static_assert(std::is_object_v<mapped_t>, "The mapped type may not be a reference or have const.");

    /*!\name Associated types
     * \{
     */

    using value_type      = std::tuple<key_t, mapped_t>;                      //!< The value_type type.
    using reference       = std::tuple<key_t const &, mapped_t &>;            //!< The reference type.
    using const_reference = std::tuple<key_t const &, mapped_t const &>;      //!< The const_reference type.
    using difference_type = ptrdiff_t;                                        //!< The difference_type type.
    using size_type       = size_t;                                           //!< The size_type type.
    using iterator        = detail::random_access_iterator<dictionary>;       //!< The iterator type.
    using const_iterator  = detail::random_access_iterator<dictionary const>; //!< The const_iterator type.
    //!\}

    //!\cond
    // this signals to range-v3 that something is a container :|
    using allocator_type = void;
    //!\endcond

    /*!\name Constructors, destructor and assignment
     * \{
     */
    dictionary()                                   = default; //!< Defaulted.
    dictionary(dictionary const &)                 = default; //!< Defaulted.
    dictionary(dictionary &&) noexcept             = default; //!< Defaulted.
    dictionary & operator=(dictionary const &)     = default; //!< Defaulted.
    dictionary & operator=(dictionary &&) noexcept = default; //!< Defaulted.
    ~dictionary()                                  = default; //!< Defaulted.

    /*!\brief Construct from a list of values of value_type.
     * \tparam value_type_ A parameter pack where each type is equal to value_type.
     * \param[in] args The values to construct from.
     *
     * ### Complexity
     *
     * Linear in the number of elements.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <typename... value_type_>
    //!\cond
        requires((std::convertible_to<value_type_, value_type> && ...) && sizeof...(value_type_) > 0)
    //!\endcond
    explicit dictionary(value_type_ &&... args)
    {
        assign(std::forward<value_type_>(args)...);
    }

    /*!\brief Construct from two iterators.
     * \tparam begin_it_type Must model std::forward_iterator and `value_type` must be constructible from
     *                             the reference type of begin_it_type.
     * \tparam   end_it_type Must satisfy std::sentinel_for.
     * \param[in]         begin_it Begin of range to construct/assign from.
     * \param[in]           end_it End of range to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <meta::different_from<iterator> begin_it_type, typename end_it_type>
    //!\cond
        requires(meta::different_from<const_iterator, begin_it_type> && std::forward_iterator<begin_it_type> &&
                 std::sentinel_for<end_it_type, begin_it_type> &&
                 std::convertible_to<std::iter_reference_t<begin_it_type>, value_type>)
    //!\endcond
    dictionary(begin_it_type const begin_it, end_it_type const end_it) : dictionary{}
    {
        assign(begin_it, end_it);
    }

    //!\overload
    dictionary(iterator const begin_it, iterator const end_it) : dictionary{} { assign(begin_it, end_it); }

    //!\overload
    dictionary(const_iterator const begin_it, const_iterator const end_it) : dictionary{} { assign(begin_it, end_it); }

    /*!\brief Construct from a different range.
     * \tparam other_range_t The type of range to be inserted; must satisfy std::ranges::input_range and `value_type`
     *                       must be constructible from std::ranges::range_reference_t<other_range_t>.
     * \param[in]      range The sequences to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <meta::different_from<dictionary> other_range_t>
    //!\cond
        requires(std::ranges::input_range<other_range_t>)
    //!\endcond
    explicit dictionary(other_range_t && range) : dictionary{std::ranges::begin(range), std::ranges::end(range)}
    {}

    /*!\brief Assign from multiple elements.
     * \param[in] args Multiple elements of value_type; must be at least one.
     *
     * This replaces the container's contents with the provided elements.
     *
     * ### Complexity
     *
     * Linear in the size of `args` and/or container size.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <typename... value_type_>
    //!\cond
        requires((meta::different_from<dictionary, value_type_> && ...) &&
                 (meta::different_from<iterator, value_type_> && ...) &&
                 (meta::different_from<const_iterator, value_type_> && ...) &&
                 (std::convertible_to<value_type_, value_type> && ...) && (sizeof...(value_type_) > 0))
    //!\endcond
    void assign(value_type_ &&... args)
    {
        storage.clear();
        storage.reserve(sizeof...(args));
        (storage.push_back(std::forward<value_type_>(args)), ...);
        recompute_hashes(); // this validates uniqueness of keys
    }

    /*!\brief Assign from a different range.
     * \tparam other_range_t The type of range to be inserted; must satisfy std::ranges::input_range and `value_type`
     *                       must be constructible from std::ranges::range_reference_t<other_range_t>.
     * \param[in]      range The sequences to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the size of `range`.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <std::ranges::input_range other_range_t>
    //!\cond
        requires std::convertible_to<std::ranges::range_reference_t<other_range_t>, value_type>
    //!\endcond
    void assign(other_range_t && range)
    {
        //TODO we could create move-iterators for
        assign(std::ranges::begin(range), std::ranges::end(range));
    }

    /*!\brief Assign from pair of iterators.
     * \tparam begin_it_type Must satisfy std::forward_iterator and the `value_type` must be constructible from
     *                       the reference type of begin_it_type.
     * \tparam   end_it_type Must satisfy std::sentinel_for.
     * \param[in]   begin_it Begin of range to construct/assign from.
     * \param[in]     end_it End of range to construct/assign from.
     *
     * ### Complexity
     *
     * Linear in the distance between `begin_it` and `end_it`.
     *
     * ### Exceptions
     *
     * Basic exception gurantee.
     */
    template <meta::different_from<iterator> begin_it_type, typename end_it_type>
    //!\cond
        requires(meta::different_from<const_iterator, begin_it_type> && std::forward_iterator<begin_it_type> &&
                 std::sentinel_for<end_it_type, begin_it_type> &&
                 std::convertible_to<std::iter_reference_t<begin_it_type>, value_type>)
    //!\endcond
    void assign(begin_it_type begin_it, end_it_type end_it)
    {
        assign_impl(begin_it, end_it);
    }

    //!\overload
    void assign(iterator begin_it, iterator end_it) { assign_impl(begin_it, end_it); }

    //!\overload
    void assign(const_iterator begin_it, const_iterator end_it) { assign_impl(begin_it, end_it); }
    //!\}

    /*!\name Iterators
     * \{
     */
    //!\brief Returns the begin iterator.
    iterator begin() noexcept { return iterator{*this}; }

    //!\copydoc bio::ranges::dictionary::begin()
    const_iterator begin() const noexcept { return const_iterator{*this}; }

    //!\copydoc bio::ranges::dictionary::begin()
    const_iterator cbegin() const noexcept { return const_iterator{*this}; }

    //!\brief Returns iterator past the end.
    iterator end() noexcept { return iterator{*this, size()}; }

    //!\copydoc bio::ranges::dictionary::end()
    const_iterator end() const noexcept { return const_iterator{*this, size()}; }

    //!\copydoc bio::ranges::dictionary::end()
    const_iterator cend() const noexcept { return const_iterator{*this, size()}; }
    //!\}

    /*!\name Element access
     * \{
     */
    /*!\brief Return the i-th element.
     * \param[in] i Index of the element to retrieve.
     * \throws std::out_of_range If you access an element behind the last.
     * \returns A reference to the value at position `i`.
     *
     * Note that this function returns the value_type, i.e. a key-value-pair.
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
        if (i >= size())
        {
            throw std::out_of_range{"Trying to access element behind the last in dictionary."};
        }
        return (*this)[i];
    }

    //!\copydoc bio::ranges::dictionary::at()
    const_reference at(size_type const i) const
    {
        if (i >= size())
        {
            throw std::out_of_range{"Trying to access element behind the last in dictionary."};
        }
        return (*this)[i];
    }

    /*!\brief Return the i-th element.
     * \param i The element to retrieve.
     * \returns A reference to the value at position `i`.
     *
     * Note that this function returns a key-value-pair (not the mapped value).
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
        return reference{get<0>(storage[i]), get<1>(storage[i])};
    }

    //!\copydoc bio::ranges::dictionary::operator[]()
    const_reference operator[](size_type const i) const noexcept
    {
        assert(i < size());
        return storage[i];
    }

    /*!\brief Return the first element. Calling front on an empty container is undefined.
     * \returns A reference to the value at the first position.
     *
     * Note that this function returns a key-value-pair (not the mapped value).
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

    //!\copydoc bio::ranges::dictionary::front()
    const_reference front() const noexcept
    {
        assert(size() > 0);
        return (*this)[0];
    }

    /*!\brief Return the last element.
     * \returns A reference to the value at the last position.
     *
     * Note that this function returns a key-value-pair (not the mapped value).
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

    //!\copydoc bio::ranges::dictionary::back()
    const_reference back() const noexcept
    {
        assert(size() > 0);
        return (*this)[size() - 1];
    }
    //!\}

    /*!\name Key-based element access
     * \{
     */
//TODO(GCC11): always use string_view after deprecating GCC10
#ifdef __cpp_lib_generic_unordered_lookup
    using het_key_t = std::string_view const; //!< Type used for key-based access. String-view supported for all keys.
#else
    using het_key_t = key_t const &; //!< Type used for key-based access.
#endif

    /*!\brief Check whether the container has an element with the given key.
     * \param[in] key The key to lookup.
     * \returns `true` if an element with the key exists, `false` otherwise.
     *
     * ### Complexity
     *
     * Amortized constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    bool contains(het_key_t key) const { return key_to_index.contains(key); }

    /*!\brief The number of elements in the container with the specified key.
     * \param[in] key The key to lookup.
     * \returns `1` if an element with the key exists, `0` otherwise.
     *
     * Since keys are required to be unique, this function can only return `0` or `1`.
     *
     * ### Complexity
     *
     * Amortized constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_t count(het_key_t key) const { return key_to_index.count(key); }

    /*!\brief Find an element with the given key.
     * \param[in] key The key to lookup.
     * \returns An iterator to the found element or end() if the key is not found.
     *
     * ### Complexity
     *
     * Amortized constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    iterator find(het_key_t key)
    {
        if (auto it = key_to_index.find(key); it == key_to_index.end())
            return end();
        else
            return begin() + get<1>(*it);
    }

    //!\overload
    const_iterator find(het_key_t key) const
    {
        if (auto it = key_to_index.find(key); it == key_to_index.end())
            return end();
        else
            return begin() + get<1>(*it);
    }

    /*!\brief Access element by key.
     * \param[in] key The key to lookup.
     * \returns The mapped value belonging to the key.
     * \throws std::out_of_range if no element is associated with the key.
     *
     * Note that this function returns a reference to the mapped object (not a key-value-pair).
     *
     * ### Complexity
     *
     * Amortized constant.
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    mapped_t & at(het_key_t key)
    {
        auto it = key_to_index.find(key);
        if (it == key_to_index.end())
            throw std::out_of_range{"Key not found in dictionary."};
        return get<1>(storage[it->second]);
    }

    //!\overload
    mapped_t const & at(het_key_t key) const
    {
        auto it = key_to_index.find(key);
        if (it == key_to_index.end())
            throw std::out_of_range{"Key not found in dictionary."};
        return get<1>(storage[it->second]);
    }

    /*!\brief Access element by key.
     * \param[in] key The key to lookup.
     * \returns The mapped value belonging to the key.
     * \throws std::out_of_range if no element is associated with the key.
     *
     * This function is identical to at(). It never inserts elements like the respective function
     * in std::unordered_map would do.
     *
     * ### Complexity
     *
     * Amortized constant.
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    mapped_t & operator[](het_key_t key) { return at(key); }

    //!\overload
    mapped_t const & operator[](het_key_t key) const { return at(key); }
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

    /*!\brief Returns the number of elements in the container.
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
    size_type size() const noexcept { return storage.size(); }

    /*!\brief Returns the maximum number of elements the container is able to hold.
     * \returns The number of elements in the container.
     *
     * This value typically reflects the theoretical limit on the size of the container. At runtime, the size
     * of the container may be limited to a value smaller than max_size() by the amount of RAM available.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_type max_size() const noexcept { return storage.max_size(); }

    /*!\brief Returns the number of elements that the container is able to hold without reallocating (*see below*).
     * \returns The capacity of the currently allocated storage.
     *
     * This returns the capacity of the internal storage vector. Depending on the implementation,
     * this may or may not be identical to the capacity of the internal hash-map.
     * Thus, push_back() and emplace_back() within the current capacity are guaranteed to not invalidate
     * any iterators, but may result in the hash-table allocating.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * No-throw guarantee.
     */
    size_type capacity() const noexcept { return storage.capacity(); }

    /*!\brief Reserve storage in the underlying data types to accomodate for future inserts.
     * \param[in] new_cap The desired capacity.
     *
     * This reserves the specified capacity in both internal data structures to prevent reallocation
     * on subsequent back insertions. It never reduces the capacity(); use shrink_to_fit() for that.
     *
     * ### Complexity
     *
     * Average case: linear in new_cap.
     * Worst case: quadratic in new_cap.
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    void reserve(size_type const new_cap)
    {
        storage.reserve(new_cap);
        key_to_index.reserve(new_cap);
    }

    /*!\brief Reduce capacity to current size() to free unused memory.
     *
     * This only reduces the capacity of the internal storage vector, not of the hash-map.
     *
     * ### Complexity
     *
     * Worst case: linear in size().
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    void shrink_to_fit() { storage.shrink_to_fit(); }
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
        storage.clear();
        key_to_index.clear();
    }

    /*!\brief Appends the given element value to the end of the container.
     * \param value The value to append.
     * \throws std::runtime_error If an element with the key already exists in the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Strong exception guarantee.
     */
    template <std::convertible_to<value_type> value_type_ = value_type>
    void push_back(value_type_ && value)
    {
        if (key_to_index.contains(get<0>(value)))
            throw std::runtime_error{"An element with this key already exists."};
        storage.push_back(std::forward<value_type_>(value));
        key_to_index[get<0>(storage.back())] = storage.size() - 1;
    }

    /*!\brief Constructs an element in-place at the end of the container.
     * \param args Arguments used to construct the element.
     * \throws std::runtime_error If an element with the key already exists in the container.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    void emplace_back(auto &&... args)
        requires(std::constructible_from<value_type, decltype(args)...>)
    {
        storage.emplace_back(std::forward<decltype(args)>(args)...);
        if (key_to_index.contains(get<0>(storage.back())))
            throw std::runtime_error{"An element with this key already exists."};
        key_to_index[get<0>(storage.back())] = storage.size() - 1;
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
     * No-throw guarantee.
     */
    void pop_back()
    {
        assert(!empty());
        key_to_index.erase(get<0>(storage.back()));
        storage.pop_back();
    }

    /*!\brief Removes and returns the last element of the container.
     *
     * Calling extract_back() on an empty container is undefined. In debug mode an assertion will be thrown.
     *
     * No iterators or references except for back() and end() are invalidated.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Basic exception guarantee.
     */
    value_type extract_back()
    {
        assert(!empty());
        value_type tmp = std::move(storage.back());
        pop_back();
        return tmp;
    }
    //!\}

    //!\name Comparison operators
    //!\{

    //!\brief Performs element-wise comparison.
    friend bool operator==(dictionary const & lhs, dictionary const & rhs) noexcept
    {
        return lhs.storage == rhs.storage;
    }

    friend bool operator<=>(dictionary const & lhs, dictionary const & rhs) noexcept
    {
        return lhs.storage <=> rhs.storage;
    }
    //!\}

private:
    //!\privatesection

    //!\brief Transparent hash functor.
    struct hash_string
    {
        //!\brief Mark as transparent.
        using is_transparent = void;

        //!\brief String_views operator is all that we need.
        std::size_t operator()(std::string_view const v) const { return std::hash<std::string_view>{}(v); }
    };

    //!\brief Transparent equality functor.
    struct eq_string
    {
        //!\brief Mark as transparent.
        using is_transparent = void;

        //!\brief String_views operator is all that we need.
        bool operator()(std::string_view const lhs, std::string_view const rhs) const
        {
            return std::ranges::equal(lhs, rhs);
        }
    };

    //!\brief Stores the elements.
    std::vector<value_type>                                   storage;
    //!\brief Map from key to index in storage.
    std::unordered_map<key_t, size_t, hash_string, eq_string> key_to_index;

    //!\brief Recompute the hash table.
    void recompute_hashes()
    {
        key_to_index.clear();
        key_to_index.reserve(storage.size());
        size_t i = 0;
        for (auto && [key, value] : storage)
            key_to_index[key] = i++;
        if (key_to_index.size() != storage.size())
            throw std::runtime_error{"When creating/assigning bio::ranges::dictionary, keys where not unique."};
    }

    //!\brief Implementation function
    void assign_impl(auto begin_it, auto end_it)
    {
        storage.clear();

        if constexpr (std::sized_sentinel_for<decltype(end_it), decltype(begin_it)>)
            storage.reserve(end_it - begin_it);

        for (; begin_it != end_it; ++begin_it)
            storage.push_back(*begin_it);
    }

public:
    //!\cond DEV
    /*!\brief Serialisation support function.
     * \tparam archive_t Type of `archive`; must satisfy bio::typename.
     * \param archive The archive being serialised from/to.
     *
     * \attention These functions are never called directly, see \ref howto_use_cereal for more details.
     */
    template <typename archive_t>
    void serialize(archive_t & archive)
    {
        archive(storage);
        archive(key_to_index);
    }
    //!\endcond
};

} // namespace bio::ranges
