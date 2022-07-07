// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

//! [semialphabet]
#include <cassert>

#include <bio/alphabet/concept.hpp>                   // alphabet concept checks

struct dna2
{
    uint8_t rank{};

    // semialphabet

    static constexpr size_t alphabet_size = 2;

    uint8_t to_rank() const noexcept
    {
        return rank;
    }

    dna2 & assign_rank(uint8_t const rk) noexcept
    {
        assert(rk < alphabet_size);
        rank = rk;
        return *this;
    }

    // Comparison operators
    friend auto operator<=>(dna2 const & lhs, dna2 const & rhs) noexcept = default;
};
//! [semialphabet]

//! [writable_semialphabet_concept]
//TODO(bio) fix this
// static_assert(bio::alphabet::semialphabet<dna2>);               // ok
// static_assert(bio::alphabet::writable_semialphabet<dna2>);       // ok
//! [writable_semialphabet_concept]

//! [free_functions]
int main ()
{
    dna2 chr{};
//     bio::alphabet::assign_rank_to(1, chr);                      // chr is assigned rank 1
//     uint8_t rnk = bio::alphabet::to_rank(chr);                  // query rank value  => 1
}
//! [free_functions]
