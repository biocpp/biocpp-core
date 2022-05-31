// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/move.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/ranges/views/to_lower.hpp>
#include <algorithm>
#include <ranges>
#include <bio/test/expect_range_eq.hpp>

using bio::operator""_dna5;

TEST(view_move, basic)
{
    std::string vec{"ACTTTGATA"};

    // pipe notation
    auto v = vec | bio::views::move;
    EXPECT_RANGE_EQ(vec, v); // equality comparison does not move

    // function notation
    auto v2(bio::views::move(vec));
    EXPECT_RANGE_EQ(vec, v2); // equality comparison does not move

    // combinability
    bio::dna5_vector vec2{"ACGTA"_dna5};
    bio::dna5_vector v3 = vec2
                           | bio::views::complement
                           | bio::views::move        // NOP, because already temporaries
                           | bio::views::to<std::vector>();
    EXPECT_EQ("TGCAT"_dna5, v3);
}

TEST(view_move, concepts)
{
    std::string vec{"ACTTTGATA"};
    auto v1 = vec | bio::views::move;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));

    EXPECT_TRUE((std::is_same_v<decltype(v1[0]), char &&>));

    auto v2 = vec | bio::views::to_lower | bio::views::move; // to_lower generates values
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));

    EXPECT_TRUE((std::is_same_v<decltype(v2[0]), char>)); // don't add const-ness to values
}
