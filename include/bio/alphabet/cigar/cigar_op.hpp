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

#include <bio/alphabet/base.hpp>

// ------------------------------------------------------------------
// cigar_op
// ------------------------------------------------------------------

namespace bio::alphabet
{

/*!\brief The cigar operation alphabet.
 * \ingroup cigar
 * \implements bio::alphabet::writable_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \details
 *
 * The main purpose of the bio::alphabet::cigar_op alphabet is to be used in the
 * bio::alphabet::cigar composition, where a cigar operation is paired with a count
 * value.
 *
 * | Op | Rank |Description                                             | Consumes query    | Consumes reference    |
 * |:--:|:----:|:-------------------------------------------------------|:-----------------:|:---------------------:|
 * | M  | 0    | alignment match (can be a sequence match or mismatch)  | yes               | yes                   |
 * | I  | 1    | insertion to the reference                             | yes               | no                    |
 * | D  | 2    | deletion from the reference                            | no                | yes                   |
 * | N  | 3    | skipped region from the reference                      | no                | yes                   |
 * | S  | 4    | soft clipping (clipped sequences present in SEQ)       | yes               | no                    |
 * | H  | 5    | hard clipping (clipped sequences NOT present in SEQ)   | no                | no                    |
 * | P  | 6    | padding (silent deletion from padded reference)        | no                | no                    |
 * | =  | 7    | sequence match                                         | yes               | yes                   |
 * | X  | 8    | sequence mismatch                                      | yes               | yes                   |
 * | B  | 9    | ?                                                      | ?                 | ?                     |
 *
 * This table is reproduced from the SAM standard. The B operation is used internally and not documented.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/cigar/cigar_op.cpp
 */
class cigar_op : public base<cigar_op, 10, char>
{
private:
    //!\brief The base class.
    using base_t = base<cigar_op, 10, char>;

    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr cigar_op() noexcept                             = default; //!< Defaulted.
    constexpr cigar_op(cigar_op const &) noexcept             = default; //!< Defaulted.
    constexpr cigar_op(cigar_op &&) noexcept                  = default; //!< Defaulted.
    constexpr cigar_op & operator=(cigar_op const &) noexcept = default; //!< Defaulted.
    constexpr cigar_op & operator=(cigar_op &&) noexcept      = default; //!< Defaulted.
    ~cigar_op() noexcept                                      = default; //!< Defaulted.
    //!\}

protected:
    //!\privatesection

    //!\brief Value to char conversion table.
    static constexpr std::array<char_type, alphabet_size>
      rank_to_char{'M', 'I', 'D', 'N', 'S', 'H', 'P', '=', 'X', 'B'};

    //!\brief Char to value conversion table.
    static constexpr std::array<rank_type, 256> char_to_rank = []() constexpr
    {
        std::array<rank_type, 256> ret{};

        // reverse mapping for characters
        for (size_t rnk = 0u; rnk < alphabet_size; ++rnk)
        {
            ret[rank_to_char[rnk]] = rnk;
        }

        return ret;
    }();
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

/*!\brief The bio::alphabet::cigar_op char literal.
 * \relates bio::alphabet::cigar_op
 * \returns bio::alphabet::cigar_op
 */
consteval cigar_op operator""_cigar_op(char const c)
{
    if (!char_is_valid_for<cigar_op>(c))
        throw std::invalid_argument{"Illegal character in character literal."};
    return cigar_op{}.assign_char(c);
}
//!\}
} // namespace literals

} // namespace bio::alphabet
