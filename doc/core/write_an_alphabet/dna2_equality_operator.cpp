// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

//! [equality]
#include <bio/alphabet/concept.hpp>                   // alphabet concept checks

struct dna2
{
    uint8_t rank{};

    // Equality operator

    friend bool operator==(dna2 const & lhs, dna2 const & rhs) noexcept
    {
        return lhs.rank == rhs.rank;
    }
};
//! [equality]
