// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/test/cereal.hpp>

template <typename t>
using alphabet_cereal = ::testing::Test;

using test_types = ::testing::Types<seqan3::dna4,
                                    seqan3::qualified<seqan3::dna4, seqan3::phred42>,
                                    seqan3::gapped<seqan3::dna4>>;

TYPED_TEST_SUITE(alphabet_cereal, test_types, );

TYPED_TEST(alphabet_cereal, serialisation)
{
    TypeParam letter;

    seqan3::assign_rank_to(1 % seqan3::alphabet_size<TypeParam>, letter);
    seqan3::test::do_serialisation(letter);

    std::vector<TypeParam> vec;
    vec.resize(10);
    for (unsigned i = 0; i < 10; ++i)
        seqan3::assign_rank_to(i % seqan3::alphabet_size<TypeParam>, vec[i]);
    seqan3::test::do_serialisation(vec);
}
