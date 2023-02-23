// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>
#include <ranges>
#include <vector>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/add_reverse_complement.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_add_reverse_complement, basic)
{
    std::vector<std::vector<bio::alphabet::dna5>> vec{{"ACGTACGTACGTA"_dna5}, {"TCGAGAGCTTTAGC"_dna5}};

    std::vector<std::vector<bio::alphabet::dna5>> cmp1{{"ACGTACGTACGTA"_dna5},
                                                       {"TACGTACGTACGT"_dna5},
                                                       {"TCGAGAGCTTTAGC"_dna5},
                                                       {"GCTAAAGCTCTCGA"_dna5}};

    std::vector<std::vector<bio::alphabet::dna5>> cmp2{{"TGCATGCATGCAT"_dna5},
                                                       {"ATGCATGCATGCA"_dna5},
                                                       {"AGCTCTCGAAATCG"_dna5},
                                                       {"CGATTTCGAGAGCT"_dna5}};
    // pipe notation
    auto                                          v1 = vec | bio::ranges::views::add_reverse_complement;
    EXPECT_EQ(v1.size(), cmp1.size());
    for (unsigned i = 0; i < v1.size(); i++)
        EXPECT_RANGE_EQ(v1[i], cmp1[i]);

    // function syntax
    auto v2 = bio::ranges::views::add_reverse_complement(vec);
    EXPECT_EQ(v2.size(), cmp1.size());
    for (unsigned i = 0; i < v2.size(); i++)
        EXPECT_RANGE_EQ(v2[i], cmp1[i]);

    // combinability
    auto v3 = vec | bio::ranges::views::complement | bio::ranges::views::add_reverse_complement;
    EXPECT_EQ(v3.size(), cmp2.size());
    for (unsigned i = 0; i < v3.size(); i++)
        EXPECT_RANGE_EQ(v3[i], cmp2[i]);

    // combinability and function syntax
    auto v5 = bio::views::add_reverse_complement(bio::ranges::views::complement(vec));
    EXPECT_EQ(v5.size(), cmp2.size());
    for (unsigned i = 0; i < v5.size(); i++)
        EXPECT_RANGE_EQ(v5[i], cmp2[i]);
}

TEST(view_add_reverse_complement, concepts)
{
    std::vector<std::vector<bio::alphabet::dna5>> vec{{"ACGTACGTACGTA"_dna5}, {"TCGAGAGCTTTAGC"_dna5}};
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);

    [[maybe_unused]] auto v1 = vec | bio::ranges::views::add_reverse_complement;

    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::sized_range<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::view<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::random_access_range<std::ranges::range_reference_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::sized_range<std::ranges::range_reference_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::view<std::ranges::range_reference_t<decltype(v1)>>);
}
