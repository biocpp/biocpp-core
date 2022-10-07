// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

//! [semialphabet]
#include <cassert>

#include <bio/alphabet/concept.hpp>                             // alphabet concept checks

struct dna2
{
    uint8_t rank{};

    /* Comparison operators */
    constexpr friend auto operator<=>(dna2 const & lhs, dna2 const & rhs) noexcept = default;

    /* Semialphabet */
    consteval friend size_t tag_invoke(bio::alphabet::custom::size,
                                       dna2) noexcept
    {
        return 2;
    }

    constexpr friend uint8_t tag_invoke(bio::alphabet::custom::to_rank,
                                        dna2 const & d) noexcept
    {
        return d.rank;
    }

    constexpr friend dna2 & tag_invoke(bio::alphabet::custom::assign_rank_to,
                                       uint8_t rk,
                                       dna2 & d) noexcept
    {
        assert(rk < 2);
        d.rank = rk;
        return d;
    }
};
//! [semialphabet]

//! [writable_semialphabet_concept]
static_assert(bio::alphabet::semialphabet<dna2>);               // ok
static_assert(bio::alphabet::writable_semialphabet<dna2>);      // ok
//! [writable_semialphabet_concept]

//! [free_functions]
int main ()
{
    dna2 chr{};
    bio::alphabet::assign_rank_to(1, chr);                      // chr is assigned rank 1
    uint8_t rnk = bio::alphabet::to_rank(chr);                  // query rank value  => 1
}
//! [free_functions]
