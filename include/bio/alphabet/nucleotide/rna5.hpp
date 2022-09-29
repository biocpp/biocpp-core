// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::rna5, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/nucleotide/nucleotide_base.hpp>

// ------------------------------------------------------------------
// rna5
// ------------------------------------------------------------------

namespace bio::alphabet
{

/*!\brief The five letter RNA alphabet of A,C,G,U and the unknown character N.
 * \ingroup nucleotide
 * \implements bio::alphabet::nucleotide_alphabet
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * This alphabet has the same internal representation as bio::alphabet::dna5, the only difference is that it prints 'U' on
 * character conversion instead of 'T'. You can assign between values of bio::alphabet::dna5 and bio::alphabet::rna5.
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::rna5::assign_char().
 *
 *\include test/snippet/alphabet/nucleotide/rna5.cpp
 */
class rna5 : public nucleotide_base<rna5, 5>
{
private:
    //!\brief The base class.
    using base_t = nucleotide_base<rna5, 5>;

    //!\brief Befriend nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr rna5() noexcept                         = default; //!< Defaulted.
    constexpr rna5(rna5 const &) noexcept             = default; //!< Defaulted.
    constexpr rna5(rna5 &&) noexcept                  = default; //!< Defaulted.
    constexpr rna5 & operator=(rna5 const &) noexcept = default; //!< Defaulted.
    constexpr rna5 & operator=(rna5 &&) noexcept      = default; //!< Defaulted.
    ~rna5() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;

    //!\brief Allow implicit construction from dna/rna of the same size.
    constexpr rna5(dna5 const & r) noexcept
#if BIOCPP_WORKAROUND_GCC_90897
      requires true
#endif
    {
        assign_rank(r.to_rank());
    }
    //!\}

protected:
    //!\privatesection

    //!\copydoc bio::alphabet::dna4::rank_to_char
    static constexpr char_type rank_to_char[alphabet_size]{'A', 'C', 'G', 'N', 'U'};

    //!\copydoc bio::alphabet::dna4::char_to_rank
    static constexpr std::array<rank_type, 256> char_to_rank = dna5::char_to_rank;

    //!\brief The complement table.
    static const std::array<rna5, alphabet_size> complement_table;
};

// ------------------------------------------------------------------
// containers
// ------------------------------------------------------------------

//!\brief Alias for an std::vector of bio::alphabet::rna5.
//!\relates rna5
using rna5_vector = std::vector<rna5>;

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

/*!\brief The bio::alphabet::rna5 char literal.
 * \relates bio::alphabet::rna5
 * \returns bio::alphabet::rna5
 */
consteval rna5 operator""_rna5(char const c) noexcept
{
    return rna5{}.assign_char(c);
}

/*!\brief The bio::alphabet::rna5 string literal.
 * \relates bio::alphabet::rna5
 * \returns bio::alphabet::rna5_vector
 *
 * You can use this string literal to easily assign to rna5_vector:
 *
 * \include test/snippet/alphabet/nucleotide/rna5_literal.cpp
 *
 */
inline rna5_vector operator""_rna5(char const * s, std::size_t n)
{
    rna5_vector r;
    r.resize(n);

    for (size_t i = 0; i < n; ++i)
        r[i].assign_char(s[i]);

    return r;
}
//!\}

} // namespace literals

} // namespace bio::alphabet

// ------------------------------------------------------------------
// rna5 (deferred definition)
// ------------------------------------------------------------------

namespace bio::alphabet
{

constexpr std::array<rna5, rna5::alphabet_size> rna5::complement_table{
  'U'_rna5, // complement of 'A'_rna5
  'G'_rna5, // complement of 'C'_rna5
  'C'_rna5, // complement of 'G'_rna5
  'N'_rna5, // complement of 'N'_rna5
  'A'_rna5  // complement of 'U'_rna5
};

} // namespace bio::alphabet
