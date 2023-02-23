// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::dna5, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/detail/to_lower.hpp>
#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// dna5
// ------------------------------------------------------------------

namespace bio::alphabet
{

class rna5;

/*!\brief The five letter DNA alphabet of A,C,G,T and the unknown character N.
 * \ingroup nucleotide
 * \implements bio::alphabet::nucleotide
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * Note that you can assign 'U' as a character to dna5 and it will silently
 * be converted to 'T'.
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::dna5::assign_char().
 *
 *\include test/snippet/alphabet/nucleotide/dna5.cpp
 */
class dna5 : public nucleotide_base<dna5, 5>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<dna5, 5>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond
    //!\brief Befriend bio::alphabet::rna5 so it can copy #char_to_rank.
    friend rna5;

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr dna5() noexcept                         = default; //!< Defaulted.
    constexpr dna5(dna5 const &) noexcept             = default; //!< Defaulted.
    constexpr dna5(dna5 &&) noexcept                  = default; //!< Defaulted.
    constexpr dna5 & operator=(dna5 const &) noexcept = default; //!< Defaulted.
    constexpr dna5 & operator=(dna5 &&) noexcept      = default; //!< Defaulted.
    ~dna5() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    template <std::same_as<rna5> t> // Accept incomplete type
    constexpr dna5(t const & r) noexcept
    {
        assign_rank(r.to_rank());
    }
    //!\}

protected:
    //!\privatesection

    //!\copydoc bio::alphabet::dna4::rank_to_char
    static constexpr std::array<char_type, alphabet_size> rank_to_char{'A', 'C', 'G', 'N', 'T'};

    //!\copydoc bio::alphabet::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // initialize with UNKNOWN
        ret.fill(3); // == 'N'

        // reverse mapping for characters and their lowercase
        for (size_t rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[rank_to_char[rnk]]                   = rnk;
            ret[detail::to_lower(rank_to_char[rnk])] = rnk;
        }

        // set U equal to T
        ret['U'] = ret['T'];
        ret['u'] = ret['t'];

        // iupac characters are implicitly "UNKNOWN"
        return ret;
    }();

    //!\brief The complement table.
    static const std::array<dna5, alphabet_size> complement_table;
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

/*!\brief The bio::alphabet::dna5 char literal.
 * \relates bio::alphabet::dna5
 * \returns bio::alphabet::dna5
 */
consteval dna5 operator""_dna5(char const c)
{
    if (!char_is_valid_for<dna5>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return dna5{}.assign_char(c);
}

/*!\brief The bio::alphabet::dna5 string literal.
 * \relates bio::alphabet::dna5
 * \returns std::vector<bio::alphabet::dna5>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::dna5>:
 *
 * \include test/snippet/alphabet/nucleotide/dna5_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<dna5> operator""_dna5()
{
    return detail::string_literal<str, dna5>();
}
//!\}

} // namespace literals

} // namespace bio::alphabet

// ------------------------------------------------------------------
// dna5 (deferred definition)
// ------------------------------------------------------------------

namespace bio::alphabet
{

constexpr std::array<dna5, dna5::alphabet_size> dna5::complement_table{
  'T'_dna5, // complement of 'A'_dna5
  'G'_dna5, // complement of 'C'_dna5
  'C'_dna5, // complement of 'G'_dna5
  'N'_dna5, // complement of 'N'_dna5
  'A'_dna5  // complement of 'T'_dna5
};

} // namespace bio::alphabet
