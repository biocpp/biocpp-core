// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/aminoacid/translation.hpp>
#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>

using bio::alphabet::operator""_aa27;
using bio::alphabet::operator""_dna4;
using bio::alphabet::operator""_dna15;

TEST(translate_triplets, dna4)
{
    bio::alphabet::dna4 n1{'C'_dna4};
    bio::alphabet::dna4 n2{'T'_dna4};
    bio::alphabet::dna4 n3{'A'_dna4};
    bio::alphabet::aa27 c{'L'_aa27};

    bio::alphabet::aa27 t1{bio::alphabet::translate_triplet<bio::alphabet::genetic_code::CANONICAL>(n1, n2, n3)};

    EXPECT_EQ(t1, c);
}

TEST(translate_triplets, dna15)
{
    bio::alphabet::dna15 n1{'C'_dna15};
    bio::alphabet::dna15 n2{'T'_dna15};
    bio::alphabet::dna15 n3{'A'_dna15};
    bio::alphabet::aa27  c{'L'_aa27};

    bio::alphabet::aa27 t1{
      bio::alphabet::translate_triplet<bio::alphabet::genetic_code::CANONICAL, bio::alphabet::dna15>(n1, n2, n3)};

    EXPECT_EQ(t1, c);
}
