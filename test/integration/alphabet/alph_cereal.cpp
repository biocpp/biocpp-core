// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

#include "../cereal.hpp"

template <typename t>
using alphabet_cereal = ::testing::Test;

using test_types = ::testing::Types<bio::alphabet::dna4,
                                    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>,
                                    bio::alphabet::gapped<bio::alphabet::dna4>>;

TYPED_TEST_SUITE(alphabet_cereal, test_types, );

TYPED_TEST(alphabet_cereal, serialisation)
{
    TypeParam letter;

    bio::alphabet::assign_rank_to(1 % bio::alphabet::alphabet_size<TypeParam>, letter);
    do_serialisation(letter);

    std::vector<TypeParam> vec;
    vec.resize(10);
    for (unsigned i = 0; i < 10; ++i)
        bio::alphabet::assign_rank_to(i % bio::alphabet::alphabet_size<TypeParam>, vec[i]);
    do_serialisation(vec);
}
