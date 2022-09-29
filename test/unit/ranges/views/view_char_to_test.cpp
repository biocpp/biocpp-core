// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_char_to, basic)
{
    std::string                vec{"ACTTTGATA"};
    bio::alphabet::dna5_vector cmp{"ACTTTGATA"_dna5};

    // pipe notation
    bio::alphabet::dna5_vector v =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    bio::alphabet::dna5_vector v2(bio::ranges::views::char_to<bio::alphabet::dna5>(vec) |
                                  bio::ranges::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    bio::alphabet::dna5_vector cmp2{"ATAGTTTCA"_dna5};
    bio::alphabet::dna5_vector v3 =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_char_to, deep_view)
{
    std::vector<std::string> foo{"ACGTA", "TGCAT"};

    std::vector<bio::alphabet::dna5_vector> v = foo | bio::ranges::views::char_to<bio::alphabet::dna5> |
                                                bio::ranges::to<std::vector<bio::alphabet::dna5_vector>>();

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ACGTA"_dna5);
    EXPECT_RANGE_EQ(v[1], "TGCAT"_dna5);
}

TEST(view_char_to, concepts)
{
    std::string vec{"ACTTTGATA"};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), char>));

    auto v1 = vec | bio::ranges::views::char_to<bio::alphabet::dna5>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));
}
