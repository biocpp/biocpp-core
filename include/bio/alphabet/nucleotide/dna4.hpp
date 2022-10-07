// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::dna4, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// dna4
// ------------------------------------------------------------------

namespace bio::alphabet
{

class rna4;

/*!\brief The four letter DNA alphabet of A,C,G,T.
 * \ingroup nucleotide
 * \implements bio::alphabet::nucleotide_alphabet
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * Note that you can assign 'U' as a character to dna4 and it will silently
 * be converted to 'T'.
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::dna4::assign_char().
 *
 * \include test/snippet/alphabet/nucleotide/dna4.cpp
 */
class dna4 : public nucleotide_base<dna4, 4>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<dna4, 4>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond
    //!\brief Befriend bio::alphabet::rna4 so it can copy #char_to_rank.
    friend rna4;

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr dna4() noexcept                         = default; //!< Defaulted.
    constexpr dna4(dna4 const &) noexcept             = default; //!< Defaulted.
    constexpr dna4(dna4 &&) noexcept                  = default; //!< Defaulted.
    constexpr dna4 & operator=(dna4 const &) noexcept = default; //!< Defaulted.
    constexpr dna4 & operator=(dna4 &&) noexcept      = default; //!< Defaulted.
    ~dna4() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    template <std::same_as<rna4> t> // Accept incomplete type
    constexpr dna4(t const & r) noexcept
    {
        assign_rank(r.to_rank());
    }
    //!\}

    /*!\brief Convert to complement. Faster implementation than the default-generated.
     * \private
     */
    constexpr dna4 complement() const noexcept { return dna4{}.assign_rank(to_rank() ^ 0b11); }

protected:
    //!\privatesection

    //!\brief Value to char conversion table.
    static constexpr char_type rank_to_char[alphabet_size]{'A', 'C', 'G', 'T'};

    //!\brief Char to value conversion table.
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // reverse mapping for characters and their lowercase
        for (size_t rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[rank_to_char[rnk]]                   = rnk;
            ret[detail::to_lower(rank_to_char[rnk])] = rnk;
        }

        // set U equal to T
        ret['U'] = ret['T'];
        ret['u'] = ret['t'];

        // iupac characters get special treatment, because there is no N
        ret['R'] = ret['A'];
        ret['r'] = ret['A']; // or G
        ret['Y'] = ret['C'];
        ret['y'] = ret['C']; // or T
        ret['S'] = ret['C'];
        ret['s'] = ret['C']; // or G
        ret['W'] = ret['A'];
        ret['w'] = ret['A']; // or T
        ret['K'] = ret['G'];
        ret['k'] = ret['G']; // or T
        ret['M'] = ret['A'];
        ret['m'] = ret['A']; // or T
        ret['B'] = ret['C'];
        ret['b'] = ret['C']; // or G or T
        ret['D'] = ret['A'];
        ret['d'] = ret['A']; // or G or T
        ret['H'] = ret['A'];
        ret['h'] = ret['A']; // or C or T
        ret['V'] = ret['A'];
        ret['v'] = ret['A']; // or C or G

        return ret;
    }
    ();
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

/*!\brief The bio::alphabet::dna4 char literal.
 * \relates bio::alphabet::dna4
 * \returns bio::alphabet::dna4
 */
consteval dna4 operator""_dna4(char const c)
{
    if (!char_is_valid_for<dna4>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return dna4{}.assign_char(c);
}

/*!\brief The bio::alphabet::dna4 string literal.
 * \relates bio::alphabet::dna4
 * \returns std::vector<bio::alphabet::dna4>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::dna4>:
 *
 * \include test/snippet/alphabet/nucleotide/dna4_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<dna4> operator""_dna4()
{
    return detail::string_literal<str, dna4>();
}
//!\}

} // namespace literals

} // namespace bio::alphabet
