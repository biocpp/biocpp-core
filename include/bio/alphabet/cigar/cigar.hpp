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

#include <bio/alphabet/adaptation/uint.hpp>
#include <bio/alphabet/cigar/cigar_op.hpp>
#include <bio/alphabet/composite/alphabet_tuple_base.hpp>
#include <bio/ranges/container/small_string.hpp>

// ------------------------------------------------------------------
// cigar
// ------------------------------------------------------------------

namespace bio::alphabet
{

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
 * SAM and BAM formats. It consists of a number and a bio::alphabet::cigar_op symbol.
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
 * ### Example
 *
 * \include test/snippet/alphabet/cigar/cigar.cpp
 */
class cigar : public alphabet_tuple_base<cigar, uint32_t, cigar_op>
{
private:
    //!\brief The base class.
    using base_t = alphabet_tuple_base<cigar, uint32_t, cigar_op>;

    //!\cond \brief Befriend bio::alphabet::alphabet_tuple_base.
    friend base_t;
    //!\endcond

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

    // Inherit constructors from base
    using base_t::base_t;

    /*!\brief Construction via a value of one of the components.
     * \tparam component_type One of the component types; must be uniquely contained in the type list of the composite.
     * \param[in] alph        The value of a component that should be assigned.
     *
     * \include test/snippet/alphabet/cigar/cigar_value_construction.cpp
     */
    BIOCPP_DOXYGEN_ONLY((constexpr cigar(component_type const alph) noexcept {}))

    /*!\brief Assignment via a value of one of the components.
     * \tparam component_type One of the component types; must be uniquely contained in the type list of the composite.
     * \param[in] alph        The value of a component that should be assigned.
     *
     * \include test/snippet/alphabet/cigar/cigar_value_assignment.cpp
     */
    BIOCPP_DOXYGEN_ONLY((constexpr cigar & operator=(component_type const alph) noexcept {}))
    //!\}

    // Inherit operators from base
    using base_t::operator=;

    /*!\name Read functions
     * \{
     */
    //!\brief Return the string representation.
    ranges::small_string<11> to_string() const noexcept
    {
        ranges::small_string<11> ret{}; // maximum number of digits for uint32_t + 1 char for the cigar_op
        ret.resize(11);

        auto [ptr, errc] = std::to_chars(ret.data(), ret.data() + 10, get<0>(*this));

        *ptr = bio::alphabet::to_char(get<1>(*this));
        (void)errc;

        ret.resize(ptr - ret.data() + 1);
        return ret;
    }
    //!\}

    /*!\name Write functions
     * \{
     */
    /*!\brief Assign from the string representation.
     * \param s The string to assign from.
     * \throws std::runtime_error If the string does not begin with an integer.
     * \throws bio::alphabet::invalid_char_assignment If the string does not end on a valid character
     *         for bio::alphabet::cigar_op.
     */
    cigar & assign_string(std::string_view const s)
    {
        uint32_t num{};
        auto [ptr, errc] = std::from_chars(s.data(), s.data() + s.size() - 1, num);

        if ((errc != std::errc{}) || (ptr != s.data() + s.size() - 1))
        {
            throw std::runtime_error{std::string{"Illegal string assignment to CIGAR: "} + static_cast<std::string>(s)};
        }
        else
        {
            get<0>(*this) = num;
            cigar_op tmp;
            assign_char_strictly_to(*ptr, tmp);
            get<1>(*this) = tmp;
        }

        return *this;
    }
    //!\}

    /*!\name Get functions
     * \{
     */
    /*!\copydoc alphabet_tuple_base::get(alphabet_tuple_base & l)
     *
     * \include test/snippet/alphabet/cigar/cigar_get_index.cpp
     */
    BIOCPP_DOXYGEN_ONLY((template <size_t index> constexpr auto get(cigar & l) noexcept {}))

    /*!\copybrief get
     * \tparam type Return the element of specified type; only available if the type is unique in the set of components.
     * \returns A proxy to the contained element that models the same alphabet concepts and supports assignment.
     *
     * \include test/snippet/alphabet/cigar/cigar_get_type.cpp
     */
    BIOCPP_DOXYGEN_ONLY((template <typename type> constexpr auto get(cigar & l) noexcept {}))
    //!\}
};

} // namespace bio::alphabet

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
