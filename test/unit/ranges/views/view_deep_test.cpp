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
#include <bio/ranges/views/deep.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/test/expect_range_eq.hpp>
#include <ranges>

namespace bio::views
{
inline auto const deep_reverse = deep{std::views::reverse};
inline auto const deep_take    = deep{std::views::take};
inline auto const deep_take2   = deep{std::views::take(2)};
} // namespace bio::views

using bio::operator""_dna5;

// ------------------------------------------------------------------
// no parameters
// ------------------------------------------------------------------

TEST(view_deep_reverse, basic)
{
    bio::dna5_vector foo{"ACGTA"_dna5};

    // pipe notation, temporary
    bio::dna5_vector v0 = foo | bio::views::deep{std::views::reverse} | bio::views::to<std::vector>();
    EXPECT_EQ(v0, "ATGCA"_dna5);

    // pipe notation
    bio::dna5_vector v = foo | bio::views::deep_reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v, "ATGCA"_dna5);

    // function notation
    bio::dna5_vector v2(bio::views::deep_reverse(foo) | bio::views::to<std::vector>());
    EXPECT_EQ(v2, "ATGCA"_dna5);

    // combinability
    bio::dna5_vector v3 = foo | bio::views::deep_reverse | std::views::reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v3, "ACGTA"_dna5);
}

TEST(view_deep_reverse, deep)
{
    std::vector<bio::dna5_vector> foo{"ACGTA"_dna5, "TGCAT"_dna5};

    auto v = foo | bio::views::deep_reverse;

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ATGCA"_dna5);
    EXPECT_RANGE_EQ(v[1], "TACGT"_dna5);
}

TEST(view_deep_reverse, concepts)
{
    std::vector<bio::dna5_vector> vec{"ACGTA"_dna5, "TGCAT"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::dna5_vector>));

    auto v1 = vec | bio::views::deep_reverse;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::dna5_vector>)); // view temporary returned in deep case

    auto v_elem = v1[0];
    EXPECT_TRUE(std::ranges::input_range<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::view<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v_elem)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v_elem)>);
    EXPECT_TRUE(bio::const_iterable_range<decltype(v_elem)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v_elem), bio::dna5>));
}

// ------------------------------------------------------------------
// parameters preserved
// ------------------------------------------------------------------

TEST(view_deep_take, basic)
{
    bio::dna5_vector foo{"ACGTA"_dna5};

    // pipe notation, temporary
    bio::dna5_vector v0 = foo | bio::views::deep{std::views::take}(2) | bio::views::to<std::vector>();
    EXPECT_EQ(v0, "AC"_dna5);

    // pipe notation
    bio::dna5_vector v = foo | bio::views::deep_take(2) | bio::views::to<std::vector>();
    EXPECT_EQ(v, "AC"_dna5);

    // function notation
    bio::dna5_vector v2(bio::views::deep_take(foo, 2) | bio::views::to<std::vector>());
    EXPECT_EQ(v2, "AC"_dna5);

    // combinability
    bio::dna5_vector v3 = foo | bio::views::deep_take(2) | std::views::reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v3, "CA"_dna5);
}

TEST(view_deep_take, deep)
{
    std::vector<bio::dna5_vector> foo{"ACGTA"_dna5, "TGCAT"_dna5, "FOO"_dna5};

    auto v = foo | bio::views::deep_take(2);

    ASSERT_EQ(size(v), 3u);
    EXPECT_RANGE_EQ(v[0], "AC"_dna5);
    EXPECT_RANGE_EQ(v[1], "TG"_dna5);
    EXPECT_RANGE_EQ(v[2], "NN"_dna5);

    int  i  = 2;
    auto v2 = foo | bio::views::deep_take(i);

    ASSERT_EQ(size(v2), 3u);
    EXPECT_RANGE_EQ(v2[0], "AC"_dna5);
    EXPECT_RANGE_EQ(v2[1], "TG"_dna5);
    EXPECT_RANGE_EQ(v2[2], "NN"_dna5);
}

// ------------------------------------------------------------------
// parameters hardcoded
// ------------------------------------------------------------------

TEST(view_deep_take2, basic)
{
    bio::dna5_vector foo{"ACGTA"_dna5};

    // pipe notation, temporary
    bio::dna5_vector v0 = foo | bio::views::deep{std::views::take(2)} | bio::views::to<std::vector>();
    EXPECT_EQ(v0, "AC"_dna5);

    // pipe notation
    bio::dna5_vector v = foo | bio::views::deep_take2 | bio::views::to<std::vector>();
    EXPECT_EQ(v, "AC"_dna5);

    // function notation
    bio::dna5_vector v2(bio::views::deep_take2(foo) | bio::views::to<std::vector>());
    EXPECT_EQ(v2, "AC"_dna5);

    // combinability
    bio::dna5_vector v3 = foo | bio::views::deep_take2 | std::views::reverse | bio::views::to<std::vector>();
    EXPECT_EQ(v3, "CA"_dna5);
}

TEST(view_deep_take2, deep)
{
    std::vector<bio::dna5_vector> foo{"ACGTA"_dna5, "TGCAT"_dna5, "FOO"_dna5};

    auto v = foo | bio::views::deep_take2;

    ASSERT_EQ(size(v), 3u);
    EXPECT_RANGE_EQ(v[0], "AC"_dna5);
    EXPECT_RANGE_EQ(v[1], "TG"_dna5);
    EXPECT_RANGE_EQ(v[2], "NN"_dna5);
}
