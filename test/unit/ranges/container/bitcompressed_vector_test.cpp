// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/test/expect_range_eq.hpp>

#include "container_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(bitcompressed,
                               container_over_dna4_test,
                               bio::ranges::bitcompressed_vector<bio::alphabet::dna4>, );

using namespace bio::alphabet::literals;

TEST(bitcompressed_vector_test, issue1743_complement_on_proxy)
{ // https://github.com/seqan/seqan3/issues/1743
    bio::ranges::bitcompressed_vector<bio::alphabet::dna4> v{'A'_dna4};

    auto proxy      = *v.begin();
    auto complement = bio::alphabet::complement(proxy);

    EXPECT_TRUE((std::same_as<decltype(complement), bio::alphabet::dna4>));
    EXPECT_EQ(complement, 'T'_dna4);
}

TEST(bitcompressed_vector_test, issue1743_view_combinability)
{ // https://github.com/seqan/seqan3/issues/1743
    bio::ranges::bitcompressed_vector<bio::alphabet::dna4> v{'A'_dna4, 'C'_dna4, 'G'_dna4, 'T'_dna4};
    auto                                                   complement = v | bio::ranges::views::complement;

    EXPECT_EQ(v.size(), complement.size());
    EXPECT_RANGE_EQ(complement, (bio::alphabet::dna4_vector{'T'_dna4, 'G'_dna4, 'C'_dna4, 'A'_dna4}));
}

TEST(bitcompressed_vector_test, char_compatibility)
{
    bio::ranges::bitcompressed_vector<char> v;
    EXPECT_TRUE(bio::alphabet::writable_alphabet<decltype(v[0])>);
}

TEST(bitcompressed_vector_test, gap_compatibility)
{
    bio::ranges::bitcompressed_vector<bio::alphabet::gap> v;
    EXPECT_TRUE(bio::alphabet::writable_alphabet<decltype(v[0])>);
}

TEST(bitcompressed_vector_test, const_assignability)
{
    bio::ranges::bitcompressed_vector<bio::alphabet::dna4> v{"ACGT"_dna4};
    auto const                                             ref = *v.begin();
    ref                                                        = 'T'_dna4;
    EXPECT_RANGE_EQ(v, "TCGT"_dna4);
}

#include "../../alphabet/alphabet_proxy_test_template.hpp"

using namespace bio::alphabet::literals;
using ref_t = std::ranges::range_reference_t<bio::ranges::bitcompressed_vector<bio::alphabet::dna4>>;

template <>
struct proxy_fixture<ref_t> : public ::testing::Test
{
    bio::ranges::bitcompressed_vector<bio::alphabet::dna4> data{"AAC"_dna4};

    ref_t default_init{data[0]};
    ref_t t0{data[1]};
    ref_t t1{data[2]};
};

INSTANTIATE_TYPED_TEST_SUITE_P(proxy_test, proxy_fixture, ::testing::Types<ref_t>, );
