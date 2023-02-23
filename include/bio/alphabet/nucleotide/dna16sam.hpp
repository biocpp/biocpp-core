// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Svenja Mehringer <svenja.mehringer AT fu-berlin.de>
 * \brief Provides bio::alphabet::dna16sam.
 */

#pragma once

#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

namespace bio::alphabet
{

/*!\brief A 16 letter DNA alphabet, containing all IUPAC symbols minus the gap and plus an equality sign ('=').
 * \ingroup nucleotide
 * \implements bio::alphabet::writable_alphabet
 * \implements bio::alphabet::nucleotide
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 *
 * \details
 *
 * The bio::alphabet::dna16sam alphabet is the nucleotide alphabet used inside the SAM, BAM and CRAM formats.
 * It has all the letters of the bio::alphabet::dna15 alphabet and the extra alphabet character '=' which denotes a
 * nucleotide character identical to the reference.
 * Without the context of this reference sequence, no assumptions can be made about the actual value of '=' letter.
 *
 * Note that you can assign 'U' as a character to dna16sam and it will silently
 * be converted to 'T'.
 * Lower case letters are accepted when assigning from char (just like bio::alphabet::dna15) and unknown characters are
 * silently converted to 'N'.
 *
 * The complement is the same as for bio::alphabet::dna15, with the addition that the complement of '=' is unknown and
 * therefore set to 'N'.
 *
 * The binary representation of an object of this type corresponds to the binary representation in BAM/CRAM.
 *
 * \include test/snippet/alphabet/nucleotide/dna16sam.cpp
 */
class dna16sam : public nucleotide_base<dna16sam, 16>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<dna16sam, 16>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr dna16sam() noexcept                             = default; //!< Defaulted.
    constexpr dna16sam(dna16sam const &) noexcept             = default; //!< Defaulted.
    constexpr dna16sam(dna16sam &&) noexcept                  = default; //!< Defaulted.
    constexpr dna16sam & operator=(dna16sam const &) noexcept = default; //!< Defaulted.
    constexpr dna16sam & operator=(dna16sam &&) noexcept      = default; //!< Defaulted.
    ~dna16sam() noexcept                                      = default; //!< Defaulted.

    using base_t::base_t;
    //!\}

protected:
    //!\privatesection

    //!\brief The representation is the same as in the SAM specifications (which is NOT in alphabetical order).
    static constexpr std::array<char_type, alphabet_size>
      rank_to_char{'=', 'A', 'C', 'M', 'G', 'R', 'S', 'V', 'T', 'W', 'Y', 'H', 'K', 'D', 'B', 'N'};

    //!\copydoc bio::alphabet::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // initialize with UNKNOWN
        ret.fill(15); // rank of 'N'

        // reverse mapping for characters and their lowercase
        for (size_t rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[rank_to_char[rnk]]                   = rnk;
            ret[detail::to_lower(rank_to_char[rnk])] = rnk;
        }

        // set U equal to T
        ret['U'] = ret['T'];
        ret['u'] = ret['t'];

        return ret;
    }();

    //!\brief The complement table.
    static const std::array<dna16sam, alphabet_size> complement_table;
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

/*!\brief The bio::alphabet::dna16sam char literal.
 * \relates bio::alphabet::dna16sam
 * \returns bio::alphabet::dna16sam
 */
consteval dna16sam operator""_dna16sam(char const c)
{
    if (!char_is_valid_for<dna16sam>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return dna16sam{}.assign_char(c);
}

/*!\brief The bio::alphabet::dna16sam string literal.
 * \relates bio::alphabet::dna16sam
 * \returns std::vector<bio::alphabet::dna16sam>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::dna16sam>:
 *
 * \include test/snippet/alphabet/nucleotide/dna16sam_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<dna16sam> operator""_dna16sam()
{
    return detail::string_literal<str, dna16sam>();
}
//!\}

} // namespace literals

} // namespace bio::alphabet

// ------------------------------------------------------------------
// complement deferred definition
// ------------------------------------------------------------------

namespace bio::alphabet
{

constexpr std::array<dna16sam, dna16sam::alphabet_size> dna16sam::complement_table{
  'N'_dna16sam, // complement of '='_dna16sam
  'T'_dna16sam, // complement of 'A'_dna16sam
  'G'_dna16sam, // complement of 'C'_dna16sam
  'K'_dna16sam, // complement of 'M'_dna16sam
  'C'_dna16sam, // complement of 'G'_dna16sam
  'Y'_dna16sam, // complement of 'R'_dna16sam
  'S'_dna16sam, // complement of 'S'_dna16sam
  'B'_dna16sam, // complement of 'V'_dna16sam
  'A'_dna16sam, // complement of 'T'_dna16sam
  'W'_dna16sam, // complement of 'W'_dna16sam
  'R'_dna16sam, // complement of 'Y'_dna16sam
  'D'_dna16sam, // complement of 'H'_dna16sam
  'M'_dna16sam, // complement of 'K'_dna16sam
  'H'_dna16sam, // complement of 'D'_dna16sam
  'V'_dna16sam, // complement of 'B'_dna16sam
  'N'_dna16sam  // complement of 'N'_dna16sam
};

}
