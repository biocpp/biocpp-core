// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/quality/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/ranges/views/trim_quality.hpp>
#include <ranges>

using bio::operator""_dna5;

TEST(view_trim, standalone)
{
    std::vector<bio::phred42> vec{ bio::phred42{40}, bio::phred42{40},
                                      bio::phred42{30}, bio::phred42{20}, bio::phred42{10}};
    std::vector<bio::phred42> cmp1{bio::phred42{40}, bio::phred42{40},
                                      bio::phred42{30}, bio::phred42{20}};
    std::vector<bio::phred42> cmp2{bio::phred42{40}, bio::phred42{40}};

    // trim by phred_value
    auto v1 = vec | bio::views::trim_quality(20u);                   // == ['I','I','?','5']
    EXPECT_EQ(v1 | bio::views::to<std::vector>(), cmp1);

    // trim by quality character
    auto v2 = vec | bio::views::trim_quality(bio::phred42{40});   // == ['I','I']
    EXPECT_EQ(v2 | bio::views::to<std::vector>(), cmp2);

    // function syntax
    auto v3 = bio::views::trim_quality(vec, 20u);                    // == ['I','I','?','5']
    EXPECT_EQ(v3 | bio::views::to<std::vector>(), cmp1);

    // combinability
    std::string v4 = bio::views::trim_quality(vec, 20u) | bio::views::to_char | bio::views::to<std::string>(); //=="II?5"
    EXPECT_EQ("II?5", v4);
}

TEST(view_trim, qualified)
{
    std::vector<bio::dna5q> vec{{'A'_dna5, bio::phred42{40}},
                                   {'G'_dna5, bio::phred42{40}},
                                   {'G'_dna5, bio::phred42{30}},
                                   {'A'_dna5, bio::phred42{20}},
                                   {'T'_dna5, bio::phred42{10}}};
    std::vector<bio::dna5q> cmp1{{'A'_dna5, bio::phred42{40}},
                                    {'G'_dna5, bio::phred42{40}},
                                    {'G'_dna5, bio::phred42{30}},
                                    {'A'_dna5, bio::phred42{20}}};
    std::vector<bio::dna5q> cmp2{{'A'_dna5, bio::phred42{40}},
                                    {'G'_dna5, bio::phred42{40}}};

    // trim by phred_value
    auto v1 = vec | bio::views::trim_quality(20u);
    EXPECT_EQ(v1 | bio::views::to<std::vector>(), cmp1);

    // trim by quality character
    auto v2 = vec | bio::views::trim_quality(bio::dna5q{'C'_dna5, bio::phred42{40}});
    EXPECT_EQ(v2 | bio::views::to<std::vector>(), cmp2);

    // function syntax
    auto v3 = bio::views::trim_quality(vec, 20u);
    EXPECT_EQ(v3 | bio::views::to<std::vector>(), cmp1);

    // combinability
    std::string v4 = bio::views::trim_quality(vec, 20u) | bio::views::to_char | bio::views::to<std::string>();
    EXPECT_EQ("AGGA", v4);
}

TEST(view_trim, concepts)
{
    std::vector<bio::dna5q> vec{{'A'_dna5, bio::phred42{40}},
                                   {'G'_dna5, bio::phred42{40}},
                                   {'G'_dna5, bio::phred42{30}},
                                   {'A'_dna5, bio::phred42{20}},
                                   {'T'_dna5, bio::phred42{10}}};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::dna5q>));
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);

    auto v1 = vec | bio::views::trim_quality(20u);
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_FALSE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v1), bio::dna5q>));
    EXPECT_TRUE(!std::ranges::sized_range<decltype(v1)>);
}
