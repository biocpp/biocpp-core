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

#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_complement, basic)
{
    std::vector<bio::alphabet::dna5> foo{"ACGTA"_dna5};

    // pipe notation
    std::vector<bio::alphabet::dna5> v = foo | bio::ranges::views::complement | bio::ranges::to<std::vector>();
    EXPECT_EQ(v, "TGCAT"_dna5);

    // function notation
    std::vector<bio::alphabet::dna5> v2(bio::ranges::views::complement(foo) | bio::ranges::to<std::vector>());
    EXPECT_EQ(v2, "TGCAT"_dna5);

    // combinability
    std::vector<bio::alphabet::dna5> v3 =
      foo | bio::ranges::views::complement | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(v3, "TACGT"_dna5);

    std::vector<bio::alphabet::dna5> const bar{"ACGTA"_dna5};

    // const pipe notation
    std::vector<bio::alphabet::dna5> v4 = bar | bio::ranges::views::complement | bio::ranges::to<std::vector>();
    EXPECT_EQ(v4, "TGCAT"_dna5);

    // const function notation
    std::vector<bio::alphabet::dna5> v5(bio::ranges::views::complement(bar) | bio::ranges::to<std::vector>());
    EXPECT_EQ(v5, "TGCAT"_dna5);

    // const combinability
    std::vector<bio::alphabet::dna5> v6 =
      bar | bio::ranges::views::complement | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(v6, "TACGT"_dna5);
}

TEST(view_complement, deep_view)
{
    std::vector<std::vector<bio::alphabet::dna5>> foo{"ACGTA"_dna5, "TGCAT"_dna5};

    auto v = foo | bio::ranges::views::complement;

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "TGCAT"_dna5);
    EXPECT_RANGE_EQ(v[1], "ACGTA"_dna5);

    std::vector<std::vector<bio::alphabet::dna5>> const bar{"ACGTA"_dna5, "TGCAT"_dna5};

    auto v2 = bar | bio::ranges::views::complement;

    ASSERT_EQ(size(v2), 2u);
    EXPECT_RANGE_EQ(v2[0], "TGCAT"_dna5);
    EXPECT_RANGE_EQ(v2[1], "ACGTA"_dna5);
}

TEST(view_complement, concepts)
{
    std::vector<bio::alphabet::dna5> vec{"ACGTA"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::alphabet::dna5>));

    auto v1 = vec | bio::ranges::views::complement;
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

    std::vector<bio::alphabet::dna5> const vec2{"ACGTA"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec2)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec2)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(vec2), bio::alphabet::dna5>));

    auto v2 = vec2 | bio::ranges::views::complement;
    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v2)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), char>));
}
