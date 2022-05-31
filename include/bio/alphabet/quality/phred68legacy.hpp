// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Marie Hoffmann <marie.hoffmann AT fu-berlin.de>
 * \brief Provides bio::phred68legacy quality scores.
 */

#pragma once

#include <bio/alphabet/quality/quality_base.hpp>

// ------------------------------------------------------------------
// phred68legacy
// ------------------------------------------------------------------

namespace bio
{

/*!\brief Quality type for Solexa and deprecated Illumina formats.
 * \implements bio::writable_quality_alphabet
 * \if DEV \implements bio::detail::writable_constexpr_alphabet \endif
 * \implements bio::trivially_copyable
 * \implements bio::standard_layout
 * \implements std::regular
 *
 * \ingroup quality
 *
 * \details
 *
 * The phred68legacy quality alphabet represents the -5-based phred score range
 * [-5..62] mapped to the ASCII range [';' .. '~']. It represents the Solexa and
 * the Illumina [1.0;1.8[ standard.
 *
 * \include test/snippet/alphabet/quality/phred68legacy.cpp
 */
class phred68legacy : public quality_base<phred68legacy, 68>
{
private:
    //!\brief The base class.
    using base_t = quality_base<phred68legacy, 68>;

    //!\brief Befriend bio::quality_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet_base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr phred68legacy()                                   noexcept = default; //!< Defaulted.
    constexpr phred68legacy(phred68legacy const &)              noexcept = default; //!< Defaulted.
    constexpr phred68legacy(phred68legacy &&)                   noexcept = default; //!< Defaulted.
    constexpr phred68legacy & operator=(phred68legacy const &)  noexcept = default; //!< Defaulted.
    constexpr phred68legacy & operator=(phred68legacy &&)       noexcept = default; //!< Defaulted.
    ~phred68legacy()                                            noexcept = default; //!< Defaulted.

    //!\brief Construct from phred value.
    constexpr phred68legacy(phred_type const p) : base_t{p} {}

    // Inherit converting constructor
    using base_t::base_t;
    //!\}

    /*!\name Member variables.
     * \{
     */
    //!\brief The projection offset between phred and rank score representation.
    static constexpr phred_type offset_phred{-5};

    //!\brief The projection offset between char and rank score representation.
    static constexpr char_type offset_char{';'};
    //!\}
};

/*!\name Literals
 * \{
 */
/*!\brief The bio::phred68legacy char literal.
 * \relates bio::phred68legacy
 * \returns bio::phred68legacy
 */
constexpr phred68legacy operator""_phred68legacy(char const c) noexcept
{
    return phred68legacy{}.assign_char(c);
}

/*!\brief The bio::phred68legacy string literal.
 * \param[in] s A pointer to the character sequence to assign from.
 * \param[in] n The length of the character sequence to assign from.
 * \relates bio::phred68legacy
 * \returns bio::std::vector<bio::phred68legacy>
 *
 * You can use this string literal to easily assign to std::vector<bio::phred68legacy>:
 *
 * \include test/snippet/alphabet/quality/phred68legacy_literal.cpp
 */
inline std::vector<phred68legacy> operator""_phred68legacy(char const * s, std::size_t n)
{
    std::vector<phred68legacy> r;
    r.resize(n);

    for (size_t i = 0; i < n; ++i)
        r[i].assign_char(s[i]);

    return r;
}
//!\}

} // namespace bio
