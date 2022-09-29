// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/char_strictly_to.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_char_strictly_to, basic)
{
    std::string                vec{"ACTTTGATA"};
    bio::alphabet::dna5_vector cmp{"ACTTTGATA"_dna5};

    // pipe notation
    EXPECT_RANGE_EQ(cmp, vec | bio::ranges::views::char_strictly_to<bio::alphabet::dna5>);

    // function notation
    EXPECT_RANGE_EQ(cmp, bio::ranges::views::char_strictly_to<bio::alphabet::dna5>(vec));

    // combinability
    bio::alphabet::dna5_vector cmp2{"ATAGTTTCA"_dna5};
    EXPECT_RANGE_EQ(cmp2, vec | bio::ranges::views::char_strictly_to<bio::alphabet::dna5> | std::views::reverse);
}

TEST(view_char_strictly_to, deep_view)
{
    std::vector<std::string> foo{"ACGTA", "TGCAT"};

    auto v = foo | bio::ranges::views::char_strictly_to<bio::alphabet::dna5>;

    ASSERT_EQ(std::ranges::size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ACGTA"_dna5);
    EXPECT_RANGE_EQ(v[1], "TGCAT"_dna5);
}

TEST(view_char_strictly_to, concepts)
{
    std::string vec{"ACTTTGATA"};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), char>));

    auto v1 = vec | bio::ranges::views::char_strictly_to<bio::alphabet::dna5>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));
}

TEST(view_char_strictly_to, exception)
{
    std::string foo = "ACGPTA";

    auto v = foo | bio::ranges::views::char_strictly_to<bio::alphabet::dna5>;
    EXPECT_THROW((std::ranges::equal(v, "ACGNTA"_dna5)), bio::alphabet::invalid_char_assignment);
}
