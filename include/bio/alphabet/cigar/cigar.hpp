// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides the bio::alphabet::cigar alphabet.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <charconv>
#include <cstdint>
#include <stdexcept>

#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/composite/tuple_base.hpp>
#include <bio/alphabet/custom/uint.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/ranges/container/small_string.hpp>

namespace bio::alphabet
{

// ------------------------------------------------------------------
// cigar
// ------------------------------------------------------------------

/*!\brief The cigar semialphabet pairs a counter with a bio::alphabet::cigar_op letter.
 * \ingroup cigar
 * \implements bio::alphabet::writable_semialphabet
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 *
 * This semialphabet represents a unit in a CIGAR string, typically found in the
 * SAM and BAM formats. It consists of two components, representing a number and symbol.
 * These components are implemented as uint32_t and bio::alphabet::cigar_op, however, only
 * values `[0, 2^28)` are valid for the number component.
 *
 * It has a "visual representation", but since this is a string and not a char,
 * the type only models bio::alphabet::writable_semialphabet and not
 * bio::alphabet::writable_alphabet.
 * Members for reading/writing the string are provided.
 *
 * To avoid confusion between string and char literal, this alphabet has
 * no user defined literal operators. Always assign from a pair of
 * uint32_t and bio::alphabet::cigar_op.
 *
 * ### ABI
 *
 * Objects of this type always occupy exactly 32bits of memory.
 *
 * On little-endian CPU architectures (all platforms officially supported by BioC++), objects
 * of this type are guaranteed to have the same binary representation as CIGAR elements
 * in BAM files.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/cigar/cigar.cpp
 */
class cigar : public base<cigar, (1ul << 28) * size<cigar_op>,
                          void> // no char type, because this is only semi_alphabet
{
private:
    //!\brief The base class.
    using base_t = base<cigar, (1ul << 28) * size<cigar_op>, void>;

    //!\brief The number of values "wasted" due to cigar_op using it's own 4 bits
    static constexpr size_t wasted_rank = 16ull - size<cigar_op>;

    //!\brief Convert rank to rank of num-element.
    constexpr uint32_t to_num_rank() const noexcept { return rank >> 4; }

    //!\brief Convert rank to rank of op-element.
    constexpr uint8_t to_op_rank() const noexcept { return rank & 0b1111; }

    //!\brief Assign from the rank of the num-element.
    constexpr void assign_num_rank(uint32_t r) noexcept { rank = (rank & 0b1111) | (r << 4); }

    //!\brief Assign from the rank of the op-element.
    constexpr void assign_op_rank(uint8_t r) noexcept { rank = (rank & ~0b1111) | r; }

    //!\brief Proxy-type returned by get() on non-const lvalues.
    template <typename alphabet_type>
    class component_proxy : public proxy_base<component_proxy<alphabet_type>, alphabet_type>
    {
    private:
        //!\brief The base type.
        using base_t = proxy_base<component_proxy<alphabet_type>, alphabet_type>;
        //!\brief Befriend the base type so it can call our assign_rank() and to_rank().
        friend base_t;

        //!\brief Store a pointer to the parent object so we can update it.
        cigar * parent;

    public:
        //Import from base type:
        using base_t::operator=;

        /*!\name Constructors, destructor and assignment
        * \{
        */
        //!\brief Deleted, because using this proxy without parent would be undefined behaviour.
        component_proxy()                                           = delete;
        constexpr component_proxy(component_proxy const &) noexcept = default; //!< Defaulted.
        constexpr component_proxy(component_proxy &&) noexcept      = default; //!< Defaulted.
        ~component_proxy() noexcept                                 = default; //!< Defaulted.

        //!\brief Construct from a reference to the parent object.
        constexpr component_proxy(cigar & p) : parent{&p} {}

        //!\brief Assignment does not change `this`, instead it updates the referenced value.
        constexpr component_proxy & operator=(component_proxy const & rhs) // NOLINT(bugprone-unhandled-self-assignment)
        {
            return assign_rank(rhs.to_rank());
        }

        //!\brief Assignment does not change `this`, instead it updates the referenced value (also works on `const` objects).
        // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
        constexpr component_proxy const & operator=(component_proxy const & rhs) const
        {
            return assign_rank(rhs.to_rank());
        }
        //!\}

        //!\brief Retrieve the compressed representation.
        constexpr alphabet::rank_t<alphabet_type> to_rank() const noexcept
        {
            if constexpr (BIOCPP_IS_SAME(alphabet_type, uint32_t))
                return parent->to_num_rank();
            else
                return parent->to_op_rank();
        }

        //!\brief Update the compressed representation.
        constexpr component_proxy & assign_rank(alphabet::rank_t<alphabet_type> const r) noexcept
        {
            if constexpr (BIOCPP_IS_SAME(alphabet_type, uint32_t))
                parent->assign_num_rank(r);
            else
                parent->assign_op_rank(r);
            return *this;
        }

        //!\brief Update the compressed representation (also works on `const` objects).
        constexpr component_proxy const & assign_rank(alphabet::rank_t<alphabet_type> const r) const noexcept
        {
            if constexpr (BIOCPP_IS_SAME(alphabet_type, uint32_t))
                parent->assign_num_rank(r);
            else
                parent->assign_op_rank(r);
            return *this;
        }

        /*!\name Comparison operators (proxy type against parent)
        * \brief Comparison against the bio::alphabet::tuple_base that this proxy originates from (necessary
        *        to prevent recursive template instantiation in the tuple).
        * \{
        */
        /*!\brief Comparison against the alphabet tuple by casting self and tuple to the emulated type.
        * \param lhs Left-hand-side of comparison.
        * \param rhs Right-hand-side of comparison.
        */
        friend constexpr bool operator==(cigar const lhs, component_proxy const rhs) noexcept
        {
            return get<alphabet_type>(lhs) == static_cast<alphabet_type>(rhs);
        }

        //!\copydoc bio::alphabet::tuple_base::component_proxy::operator==(derived_type const, component_proxy const)
        friend constexpr auto operator<=>(cigar const lhs, component_proxy const rhs) noexcept
        {
            return get<alphabet_type>(lhs) <=> static_cast<alphabet_type>(rhs);
        }
        //!\}
    };

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr cigar() noexcept                          = default; //!< Defaulted.
    constexpr cigar(cigar const &) noexcept             = default; //!< Defaulted.
    constexpr cigar(cigar &&) noexcept                  = default; //!< Defaulted.
    constexpr cigar & operator=(cigar const &) noexcept = default; //!< Defaulted.
    constexpr cigar & operator=(cigar &&) noexcept      = default; //!< Defaulted.
    ~cigar() noexcept                                   = default; //!< Defaulted.

    //!\brief Construct from component types.
    constexpr cigar(uint32_t const count, cigar_op op) noexcept : base_t{(count << 4) | op.to_rank()} {}

    //!\brief Construct from one component.
    explicit constexpr cigar(uint32_t const count) noexcept : base_t{count << 4} {}

    //!\brief Construct from one component.
    explicit constexpr cigar(cigar_op const op) noexcept : base_t{op.to_rank()} {}

    //!\brief Assign from one component.
    constexpr cigar & operator=(uint32_t const count) noexcept
    {
        assign_num_rank(count);
        return *this;
    }

    //!\brief Assign from one component.
    constexpr cigar & operator=(cigar_op const op) noexcept
    {
        assign_op_rank(op.to_rank());
        return *this;
    }
    //!\}

    /*!\name Read functions
     * \{
     */

    /*!\brief Return the letter's numeric value (rank in the alphabet).
     *
     * \details
     *
     * This overload overrides the base class's function to ensure correct binary layout.
     *
     * ### Complexity
     *
     * Constant.
     *
     * In contrast to most alphabets, this function has more overhead than just returning
     * the stored value.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr uint32_t to_rank() const noexcept { return rank < 16u ? rank : rank - wasted_rank; }

    //!\brief Convert to number-component.
    explicit constexpr operator uint32_t() const { return to_num_rank(); }

    //!\brief Convert to bio::alphabet::cigar_op component.
    explicit constexpr operator cigar_op() const { return alphabet::assign_rank_to(to_op_rank(), cigar_op{}); }

    /*!\brief Convert to string representation.
     * \param[in,out] buffer The place the string will be created in.
     * \returns A string_view into the buffer with the string-representation.
     * \details
     *
     * This overload takes a character buffer that the string will be created in. The buffer is required to
     * be large enough to hold the string representation; **it is recommended to always pass a buffer of at
     * least 10 characters!** This is the maximum width of the string.
     *
     * A string_view into the buffer is returned. This is the only way to know the size of the "string", so you
     * should not discard the return-result. **No 0-terminator is written into the buffer.**
     */
    [[nodiscard("Always check the returned string_view for the size!")]] std::string_view to_string(
      std::span<char> buffer) const noexcept
    {
        auto [ptr, errc] = std::to_chars(buffer.data(), buffer.data() + 10, operator uint32_t());

        // I hope this is optimised down to a single table lookup
        *ptr = operator cigar_op().to_char();
        (void)errc;

        return std::string_view{buffer.data(), ptr + 1};
    }

    //!\brief Convert to string representation.
    ranges::small_string<10> to_string() const noexcept
    {
        ranges::small_string<10> ret{}; // maximum number of digits for uint32_t + 1 char for the cigar_op
        ret.resize(10);

        std::string_view s = to_string(ret);
        ret.resize(s.size());
        return ret;
    }
    //!\}

    /*!\name Write functions
     * \{
     */

    /*!\brief Assign from a numeric value.
     * \param r The rank to be assigned.
     *
     * \details
     *
     * This overload overrides the base class's function to ensure correct binary layout.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    constexpr cigar & assign_rank(uint32_t const r) noexcept
    {
        assert(r < alphabet_size);
        rank = r < 16u ? r : r + wasted_rank;
        return *this;
    }

    /*!\brief Assign from the string representation.
     * \param s The string to assign from.
     * \throws std::invalid_argument If the string does not begin with an integer.
     * \throws bio::alphabet::invalid_char_assignment If the string does not end on a valid character
     *         for bio::alphabet::cigar_op.
     */
    cigar & assign_string(std::string_view const s)
    {
        int64_t num{};
        auto [ptr, errc] = std::from_chars(s.data(), s.data() + s.size(), num);

        if ((errc != std::errc{}) || (ptr != s.data() + s.size() - 1))
        {
            throw std::invalid_argument{std::string{"Illegal string assignment to CIGAR: "} +
                                        static_cast<std::string>(s)};
        }
        else if (num < 0 || num > (2u << 28))
        {
            throw std::invalid_argument{"Only numbers between 0 and 2^28 are valid in cigars."};
        }
        else
        {
            assign_num_rank(num);
            assign_op_rank(assign_char_strictly_to(*ptr, cigar_op{}).to_rank());
        }

        return *this;
    }
    //!\}

    /*!\name Get functions
     * \{
     */
    /*!\brief Get one of the two components from the cigar element (by type).
     * \tparam type Return the element of specified type; eith `uint32_t` or bio::alphabet::cigar_op.
     * \returns A object of the requested type.
     *
     * ### Example
     *
     * \include test/snippet/alphabet/cigar/cigar_get_type.cpp
     */
    template <meta::one_of<uint32_t, cigar_op> type>
    friend constexpr type get(cigar const & l) noexcept
    {
        return l.operator type();
    }

    /*!\brief Get one of the two components from the cigar element (by type).
     * \tparam type Return the element of specified type; eith `uint32_t` or bio::alphabet::cigar_op.
     * \returns A proxy to the contained element that models the same alphabet concepts and supports assignment.
     *
     * ### Example
     *
     * \include test/snippet/alphabet/cigar/cigar_get_type.cpp
     */
    template <meta::one_of<uint32_t, cigar_op> type>
    friend constexpr auto get(cigar & l) noexcept
    {
        return component_proxy<type>{l};
    }

    /*!\brief Get one of the two components from the cigar element (by index).
     * \tparam index Return the element of specified index; either `0` or `1`.
     * \returns A object of the requested type.
     *
     * ### Example
     *
     * \include test/snippet/alphabet/cigar/cigar_get_index.cpp
     */
    template <size_t index>
    //!\cond
        requires(index == 0 || index == 1)
    //!\endcond
    friend constexpr auto get(cigar const & l) noexcept
    {
        if constexpr (index == 0)
            return get<uint32_t>(l);
        else if constexpr (index == 1)
            return get<cigar_op>(l);
    }

    /*!\brief Get one of the two components from the cigar element (by index).
     * \tparam index Return the element of specified index; either `0` or `1`.
     * \returns A proxy to the contained element that models the same alphabet concepts and supports assignment.
     *
     * ### Example
     *
     * \include test/snippet/alphabet/cigar/cigar_get_index.cpp
     */
    template <size_t index>
    //!\cond
        requires(index == 0 || index == 1)
    //!\endcond
    friend constexpr auto get(cigar & l) noexcept
    {
        if constexpr (index == 0)
            return get<uint32_t>(l);
        else if constexpr (index == 1)
            return get<cigar_op>(l);
    }
    //!\}
};

static_assert(sizeof(cigar) == 4, "Something wrong with binary layout of bio::alphabet::cigar. PLEASE REPORT BUG.");

} // namespace bio::alphabet

namespace std
{

/*!\brief Obtains the type of the specified component.
 * \implements bio::meta::transformation_trait
 * \ingroup cigar
 * \see [std::tuple_element](https://en.cppreference.com/w/cpp/utility/tuple/tuple_element)
 *
 */
template <std::size_t i>
struct tuple_element<i, bio::alphabet::cigar>
{
    //!\brief Element type.
    using type = std::conditional_t<i == 0, uint32_t, bio::alphabet::cigar_op>;
};

/*!\brief Provides access to the number of components in a tuple as a compile-time constant expression.
 * \implements bio::meta::unary_type_trait
 * \ingroup cigar
 * \see std::tuple_size_v
 *
 */
template <>
struct tuple_size<bio::alphabet::cigar> : public std::integral_constant<size_t, 2>
{};

} // namespace std

#if __has_include(<fmt/format.h>)

#    include <fmt/format.h>

template <>
struct fmt::formatter<bio::alphabet::cigar> : fmt::formatter<std::string_view>
{
    constexpr auto format(bio::alphabet::cigar const a, auto & ctx) const
    {
        auto             tmp = a.to_string();
        std::string_view v{tmp.data(), tmp.size()};
        return fmt::formatter<std::string_view>::format(v, ctx);
    }
};
#endif
