// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

//! [dna2]
#include <array>                                            // std::array
#include <bio/alphabet/base.hpp>                   // base
#include <bio/alphabet/concept.hpp>                         // alphabet concept checks

// derive from base
struct dna2 : public bio::alphabet::base<dna2, 2>
{
private:
    // map 0 => 'S' and 1 => 'W'
    static constexpr std::array<char_type, alphabet_size> rank_to_char {'S', 'W'};

    static constexpr std::array<rank_type, 256> char_to_rank =
    // initialise with an immediately evaluated lambda expression:
    [] () constexpr
    {
        std::array<rank_type, 256> ret{}; // initialise all values with 0 (=> 'S')
        ret['W'] = 1;                     // only 'W' and 'w' result in rank 1
        ret['w'] = 1;
        return ret;
    } ();

    // make the base class a friend so it can access the tables:
    friend bio::alphabet::base<dna2, 2>;
};

// check the concepts
static_assert(bio::alphabet::alphabet<dna2>);               // ok
static_assert(bio::alphabet::writable_alphabet<dna2>);      // ok
//! [dna2]
