// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/convert.hpp>

using namespace bio::alphabet::literals;

TEST(view_convert, basic)
{
    std::vector<int>  vec{7, 5, 0, 5, 0, 0, 4, 8, -3};
    std::vector<bool> cmp{1, 1, 0, 1, 0, 0, 1, 1, 1};

    // pipe notation
    std::vector<bool> v = vec | bio::ranges::views::convert<bool> | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    std::vector<bool> v2(bio::ranges::views::convert<bool>(vec) | bio::ranges::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    std::vector<bool> cmp2{1, 1, 1, 0, 0, 1, 0, 1, 1};
    std::vector<bool> v3 =
      vec | bio::ranges::views::convert<bool> | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_convert, explicit_conversion)
{
    bio::alphabet::dna5_vector vec{"ACGNTNGGN"_dna5};
    bio::alphabet::dna4_vector cmp{"ACGATAGGA"_dna4};

    // pipe notation
    bio::alphabet::dna4_vector v =
      vec | bio::ranges::views::convert<bio::alphabet::dna4> | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    bio::alphabet::dna4_vector v2(bio::ranges::views::convert<bio::alphabet::dna4>(vec) |
                                  bio::ranges::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    bio::alphabet::dna4_vector cmp2{"AGGATAGCA"_dna4};
    bio::alphabet::dna4_vector v3 =
      vec | bio::ranges::views::convert<bio::alphabet::dna4> | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_convert, concepts)
{
    bio::alphabet::dna5_vector vec{"ACGNTNGGN"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::alphabet::dna5>));

    auto v1 = vec | bio::ranges::views::convert<bio::alphabet::dna4>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna4>));
}
