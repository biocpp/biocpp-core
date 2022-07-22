// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

//! [writable_alphabet]
#include <cassert>

#include <bio/alphabet/concept.hpp>                   // alphabet concept checks

struct dna2
{
    uint8_t rank{};

    /* Comparison operators */
    constexpr friend auto operator<=>(dna2 const & lhs, dna2 const & rhs) noexcept = default;

    /* Semialphabet */
    consteval friend size_t tag_invoke(bio::alphabet::cpo::size,
                                       dna2) noexcept
    {
        return 2;
    }

    constexpr friend uint8_t tag_invoke(bio::alphabet::cpo::to_rank,
                                        dna2 const & d) noexcept
    {
        return d.rank;
    }

    constexpr friend dna2 & tag_invoke(bio::alphabet::cpo::assign_rank_to,
                                       uint8_t rk,
                                       dna2 & d) noexcept
    {
        assert(rk < 2);
        d.rank = rk;
        return d;
    }

    /* Alphabet */
    constexpr friend char tag_invoke(bio::alphabet::cpo::to_char,
                                     dna2 const & d) noexcept
    {
        // map 0 => 'S' and 1 => 'W'
        return d.rank == 0 ? 'S' : 'W';
    }

    constexpr friend dna2 & tag_invoke(bio::alphabet::cpo::assign_char_to,
                                       char ch,
                                       dna2 & d) noexcept
    {
        switch (ch)
        {
            case 'W': case 'w':
                d.rank = 1; // allow assignment from uppercase and lowercase
                break;
            default:
                d.rank = 0; // unknown characters are mapped to 0 (=> 'S')
        }
        return d;
    }

    // Optional: make lower-case letters valid
    constexpr friend bool tag_invoke(bio::alphabet::cpo::char_is_valid_for,
                                     char ch,
                                     dna2) noexcept
    {
        switch (ch)
        {
            case 'W': case 'w': case 'S': case 's': return true;
            default:  return false;
        }
    }
};
//! [writable_alphabet]

//! [writable_alphabet_concept]
static_assert(bio::alphabet::alphabet<dna2>);               // ok
static_assert(bio::alphabet::writable_alphabet<dna2>);      // ok
//! [writable_alphabet_concept]
