// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides bio::alphabet::aa27, container aliases and string literals.
 */

#pragma once

#include <vector>

#include <bio/alphabet/aminoacid/aminoacid_base.hpp>
#include <bio/alphabet/aminoacid/concept.hpp>
#include <bio/alphabet/detail/to_lower.hpp>

namespace bio::alphabet
{
/*!\brief The twenty-seven letter amino acid alphabet.
 * \ingroup aminoacid
 * \implements bio::alphabet::aminoacid
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 * The alphabet consists of letters A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X,
 * Y, Z, *
 *
 * Like most alphabets, this alphabet cannot be initialised directly from its character representation.
 * Instead initialise/assign from the character literal or use the
 * function bio::alphabet::aa27::assign_char().
 *
 * \include test/snippet/alphabet/aminoacid/aa27_construction.cpp
 */

class aa27 : public aminoacid_base<aa27, 27>
{
private:
    //!\brief The base class.
    using base_t = aminoacid_base<aa27, 27>;

    //!\brief Befriend bio::alphabet::nucleotide_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr aa27() noexcept                         = default; //!< Defaulted.
    constexpr aa27(aa27 const &) noexcept             = default; //!< Defaulted.
    constexpr aa27(aa27 &&) noexcept                  = default; //!< Defaulted.
    constexpr aa27 & operator=(aa27 const &) noexcept = default; //!< Defaulted.
    constexpr aa27 & operator=(aa27 &&) noexcept      = default; //!< Defaulted.
    ~aa27() noexcept                                  = default; //!< Defaulted.

    using base_t::base_t;
    //!\}

protected:
    //!\brief Value to char conversion table.
    static constexpr std::array<char_type, alphabet_size> rank_to_char{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                                                       'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                                                                       'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '*'};

    //!\brief Char to value conversion table.
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // initialize with UNKNOWN
        ret.fill(23); // value of 'X'

        // reverse mapping for characters and their lowercase
        for (rank_type rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[static_cast<rank_type>(rank_to_char[rnk])]                   = rnk;
            ret[static_cast<rank_type>(detail::to_lower(rank_to_char[rnk]))] = rnk;
        }

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

/*!\brief The bio::alphabet::aa27 char literal.
 * \relates bio::alphabet::aa27
 * \returns bio::alphabet::aa27
 */
consteval aa27 operator""_aa27(char const c)
{
    if (!char_is_valid_for<aa27>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return aa27{}.assign_char(c);
}

/*!\brief The bio::alphabet::aa27 string literal.
 * \relates bio::alphabet::aa27
 * \returns std::vector<bio::alphabet::aa27>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::aa27>:
 *
 * \include test/snippet/alphabet/aminoacid/aa27_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<aa27> operator""_aa27()
{
    return detail::string_literal<str, aa27>();
}
//!\}

} // namespace literals

} // namespace bio::alphabet
