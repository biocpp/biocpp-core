// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::rna15, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// rna15
// ------------------------------------------------------------------

namespace bio::alphabet
{

/*!\brief The 15 letter RNA alphabet, containing all IUPAC smybols minus the gap.
 * \implements bio::alphabet::nucleotide
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \ingroup nucleotide
 *
 * \details
 *
 * This alphabet has the same internal representation as bio::alphabet::dna15, the only difference is that it prints 'U' on
 * character conversion instead of 'T'. You can assign between values of bio::alphabet::dna15 and bio::alphabet::rna15.
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::rna15::assign_char().
 *
 *\include test/snippet/alphabet/nucleotide/rna15.cpp
 */
class rna15 : public nucleotide_base<rna15, 15>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<rna15, 15>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr rna15() noexcept                          = default; //!< Defaulted.
    constexpr rna15(rna15 const &) noexcept             = default; //!< Defaulted.
    constexpr rna15(rna15 &&) noexcept                  = default; //!< Defaulted.
    constexpr rna15 & operator=(rna15 const &) noexcept = default; //!< Defaulted.
    constexpr rna15 & operator=(rna15 &&) noexcept      = default; //!< Defaulted.
    ~rna15() noexcept                                   = default; //!< Defaulted.

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    constexpr rna15(dna15 const & r) noexcept { assign_rank(r.to_rank()); }
    //!\}

protected:
    //!\privatesection

    //!\copydoc bio::alphabet::dna4::rank_to_char
    static constexpr std::array<char_type, alphabet_size>
      rank_to_char{'A', 'B', 'C', 'D', 'G', 'H', 'K', 'M', 'N', 'R', 'S', 'U', 'V', 'W', 'Y'};

    //!\copydoc bio::alphabet::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = dna15::char_to_rank;

    //!\brief The complement table.
    static const std::array<rna15, alphabet_size> complement_table;
};

} // namespace bio::alphabet

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

namespace bio::alphabet
{

inline namespace literals
{

/*!\name Literals
 * \{
 */

/*!\brief The bio::alphabet::rna15 char literal.
 * \relates bio::alphabet::rna15
 * \returns bio::alphabet::rna15
 */
consteval rna15 operator""_rna15(char const c)
{
    if (!char_is_valid_for<rna15>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return rna15{}.assign_char(c);
}

/*!\brief The bio::alphabet::rna15 string literal.
 * \relates bio::alphabet::rna15
 * \returns std::vector<bio::alphabet::rna15>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::rna15>:
 *
 * \include test/snippet/alphabet/nucleotide/rna15_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<rna15> operator""_rna15()
{
    return detail::string_literal<str, rna15>();
}
//!\}

} // namespace literals

} // namespace bio::alphabet

// ------------------------------------------------------------------
// rna15 (deferred definition)
// ------------------------------------------------------------------

namespace bio::alphabet
{

constexpr std::array<rna15, rna15::alphabet_size> rna15::complement_table{
  'U'_rna15, // complement of 'A'_rna15
  'V'_rna15, // complement of 'B'_rna15
  'G'_rna15, // complement of 'C'_rna15
  'H'_rna15, // complement of 'D'_rna15
  'C'_rna15, // complement of 'G'_rna15
  'D'_rna15, // complement of 'H'_rna15
  'M'_rna15, // complement of 'K'_rna15
  'K'_rna15, // complement of 'M'_rna15
  'N'_rna15, // complement of 'N'_rna15
  'Y'_rna15, // complement of 'R'_rna15
  'S'_rna15, // complement of 'S'_rna15
  'A'_rna15, // complement of 'U'_rna15
  'B'_rna15, // complement of 'V'_rna15
  'W'_rna15, // complement of 'W'_rna15
  'R'_rna15  // complement of 'Y'_rna15
};

} // namespace bio::alphabet
