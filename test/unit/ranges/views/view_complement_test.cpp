// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <algorithm>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/test/expect_range_eq.hpp>
#include <ranges>

using bio::operator""_dna5;

TEST(view_complement, basic)
{
    bio::dna5_vector foo{"ACGTA"_dna5};

    // pipe notation
    bio::dna5_vector v = foo | bio::views::complement | bio::views::to<std::vector>();
    EXPECT_EQ(v, "TGCAT"_dna5);

    // function notation
    bio::dna5_vector v2(bio::views::complement(foo) | bio::views::to<std::vector>());
    EXPECT_EQ(v2, "TGCAT"_dna5);

    // combinability
    bio::dna5_vector v3 = foo | bio::views::complement | std::views::reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v3, "TACGT"_dna5);

    bio::dna5_vector const bar{"ACGTA"_dna5};

    // const pipe notation
    bio::dna5_vector v4 = bar | bio::views::complement | bio::views::to<std::vector>();
    EXPECT_EQ(v4, "TGCAT"_dna5);

    // const function notation
    bio::dna5_vector v5(bio::views::complement(bar) | bio::views::to<std::vector>());
    EXPECT_EQ(v5, "TGCAT"_dna5);

    // const combinability
    bio::dna5_vector v6 = bar | bio::views::complement | std::views::reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v6, "TACGT"_dna5);
}

TEST(view_complement, deep_view)
{
    std::vector<bio::dna5_vector> foo{"ACGTA"_dna5, "TGCAT"_dna5};

    auto v = foo | bio::views::complement;

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "TGCAT"_dna5);
    EXPECT_RANGE_EQ(v[1], "ACGTA"_dna5);

    std::vector<bio::dna5_vector> const bar{"ACGTA"_dna5, "TGCAT"_dna5};

    auto v2 = bar | bio::views::complement;

    ASSERT_EQ(size(v2), 2u);
    EXPECT_RANGE_EQ(v2[0], "TGCAT"_dna5);
    EXPECT_RANGE_EQ(v2[1], "ACGTA"_dna5);
}

TEST(view_complement, concepts)
{
    bio::dna5_vector vec{"ACGTA"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::dna5>));

    auto v1 = vec | bio::views::complement;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));

    bio::dna5_vector const vec2{"ACGTA"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec2)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec2)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(vec2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(vec2), bio::dna5>));

    auto v2 = vec2 | bio::views::complement;
    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v2)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), bio::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), char>));
}
