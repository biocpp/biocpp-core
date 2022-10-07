// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Marie Hoffmann <marie.hoffmann AT fu-berlin.de>
 * \brief Provides bio::alphabet::phred63 quality scores.
 */

#pragma once

#include <bio/alphabet/quality/quality_base.hpp>

// ------------------------------------------------------------------
// phred63
// ------------------------------------------------------------------

namespace bio::alphabet
{

/*!\brief Quality type for traditional Sanger and modern Illumina Phred scores (full range).
 * \implements bio::alphabet::writable_quality_alphabet
 * \if DEV \implements bio::alphabet::detail::writable_constexpr_alphabet \endif
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 * \implements std::regular
 *
 * \ingroup quality
 *
 * \details
 *
 * The phred63 quality alphabet represents the zero-based phred score range
 * [0..62] mapped to the ASCII range ['!' .. '~']. It represents the Sanger and
 * Illumina 1.8+ standard beyond the typical range of raw reads (0 to 41).
 *
 * \include test/snippet/alphabet/quality/phred63.cpp
 */
class phred63 : public quality_base<phred63, 63>
{
private:
    //!\brief The base class.
    using base_t = quality_base<phred63, 63>;

    //!\brief Befriend bio::alphabet::quality_base.
    friend base_t;
    //!\cond \brief Befriend bio::alphabet::base.
    friend base_t::base_t;
    //!\endcond

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr phred63() noexcept                            = default; //!< Defaulted.
    constexpr phred63(phred63 const &) noexcept             = default; //!< Defaulted.
    constexpr phred63(phred63 &&) noexcept                  = default; //!< Defaulted.
    constexpr phred63 & operator=(phred63 const &) noexcept = default; //!< Defaulted.
    constexpr phred63 & operator=(phred63 &&) noexcept      = default; //!< Defaulted.
    ~phred63() noexcept                                     = default; //!< Defaulted.

    //!\brief Construct from phred value.
    constexpr phred63(phred_type const p) : base_t{p} {}

    // Inherit converting constructor
    using base_t::base_t;
    //!\}

    /*!\name Member variables.
     * \{
     */
    //!\brief The projection offset between phred and rank score representation.
    static constexpr phred_type offset_phred{0};

    //!\brief The projection offset between char and rank score representation.
    static constexpr char_type offset_char{'!'};
    //!\}
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

/*!\brief The bio::alphabet::phred63 char literal.
 * \relates bio::alphabet::phred63
 * \returns bio::alphabet::phred63
 */
consteval phred63 operator""_phred63(char const c)
{
    if (!char_is_valid_for<phred63>(c))
        throw std::invalid_argument{"Illegal character in character literal."};

    return phred63{}.assign_char(c);
}

/*!\brief The bio::alphabet::phred63 string literal.
 * \relates bio::alphabet::phred63
 * \returns std::vector<bio::alphabet::phred63>
 *
 * You can use this string literal to easily create a std::vector<bio::alphabet::phred63>:
 *
 * \include test/snippet/alphabet/quality/phred63_literal.cpp
 *
 */
template <meta::detail::literal_buffer_string str>
constexpr std::vector<phred63> operator""_phred63()
{
    return detail::string_literal<str, phred63>();
}
//!\}

} // namespace literals
} // namespace bio::alphabet
