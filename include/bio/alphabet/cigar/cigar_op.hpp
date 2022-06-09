// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Introduces the cigar_op alphabet.
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 */

#pragma once

#include <bio/alphabet/alphabet_base.hpp>

// ------------------------------------------------------------------
// cigar_op
// ------------------------------------------------------------------

namespace bio
{

/*!\brief The (extended) cigar operation alphabet of M,D,I,H,N,P,S,X,=.
 * \ingroup cigar
 * \implements bio::writable_alphabet
 * \if DEV \implements bio::detail::writable_constexpr_alphabet \endif
 * \implements bio::trivially_copyable
 * \implements bio::standard_layout
 * \implements std::regular
 *
 * \details
 *
 * The CIGAR string can be either basic or extended. The only difference in the
 * extended cigar alphabet is that aligned bases are classified as an actual
 * match ('=') or mismatch ('X'). In contrast, the basic cigar alphabet
 * only indicated the aligned status with an 'M', without further
 * information if the bases are actually equal or not.
 *
 * The main purpose of the bio::cigar_op alphabet is to be used in the
 * bio::cigar composition, where a cigar operation is paired with a count
 * value.
 *
 * Example usage:
 * \include test/snippet/alphabet/cigar/cigar_op.cpp
 *
 * \note Usually you do not want to manipulate cigar elements and vectors on
 *       your own but convert an alignment to a cigar and back. See
 *       bio::get_cigar_vector for how to convert two aligned sequences into
 *       a cigar_vector.
 */
class cigar_op : public alphabet_base<cigar_op, 9, char>
{
private:
    //!\brief The base class.
    using base_t = alphabet_base<cigar_op, 9, char>;

    //!\cond \brief Befriend bio::alphabet_base.
    friend base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr cigar_op()                             noexcept = default; //!< Defaulted.
    constexpr cigar_op(cigar_op const &)             noexcept = default; //!< Defaulted.
    constexpr cigar_op(cigar_op &&)                  noexcept = default; //!< Defaulted.
    constexpr cigar_op & operator=(cigar_op const &) noexcept = default; //!< Defaulted.
    constexpr cigar_op & operator=(cigar_op &&)      noexcept = default; //!< Defaulted.
    ~cigar_op()                                      noexcept = default; //!< Defaulted.
    //!\}

protected:
    //!\privatesection

    //!\brief Value to char conversion table.
    static constexpr char_type rank_to_char[alphabet_size]
    {
        'M',
        'D',
        'I',
        'S',
        'H',
        'N',
        'P',
        'X',
        '='
    };

    //!\brief Char to value conversion table.
    static constexpr std::array<rank_type, 256> char_to_rank
    {
        [] () constexpr
        {
            std::array<rank_type, 256> ret{};

            // reverse mapping for characters
            for (size_t rnk = 0u; rnk < alphabet_size; ++rnk)
            {
                ret[rank_to_char[rnk] ] = rnk;
            }

            return ret;
        }()
    };
};

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

/*!\name Literals
 * \{
 */

/*!\brief The bio::cigar_op char literal.
 * \relates bio::cigar_op
 * \returns bio::cigar_op
 */
inline cigar_op operator""_cigar_op(char const c) noexcept
{
    return cigar_op{}.assign_char(c);
}
//!\}
} // namespace bio
