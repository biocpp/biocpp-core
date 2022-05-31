// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <forward_list>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/interleave.hpp>
#include <bio/ranges/views/take.hpp>
#include <bio/ranges/views/type_reduce.hpp>
#include <ranges>
#include <bio/test/expect_range_eq.hpp>
#include <bio/test/pretty_printing.hpp>

#include <gtest/gtest.h>

using bio::operator""_dna4;

TEST(view_interleave, basic)
{
    std::string u{"FOOBARBAXBAT"};
    std::string i{"in"};
    size_t s = 3;
    std::string cmp{"FOOinBARinBAXinBAT"};
    std::string cmp_rev{"TABni"};
    size_t cmpsize = 18;

    // pipe notation
    // explicitly call bio::views::type_reduce
    auto v0 = bio::views::type_reduce(u) | bio::views::interleave(s, bio::views::type_reduce(i));
    EXPECT_RANGE_EQ(cmp, v0);
    EXPECT_EQ(cmpsize, v0.size());
    // don't call bio::views::type_reduce
    auto v1 = u | bio::views::interleave(s, i);
    EXPECT_RANGE_EQ(cmp, v1);

    // function notation
    // explicitly call bio::views::type_reduce
    auto v2{bio::views::interleave(bio::views::type_reduce(u), s, bio::views::type_reduce(i))};
    EXPECT_RANGE_EQ(cmp, v2);
    // don't call bio::views::type_reduce
    auto v3{bio::views::interleave(u, s, i)};
    EXPECT_RANGE_EQ(cmp, v3);

    //combinability
    // explicitly call bio::views::type_reduce
    auto v4 = bio::views::type_reduce(u)
            | bio::views::interleave(s, bio::views::type_reduce(i))
            | std::views::reverse
            | std::views::take(5);
    EXPECT_RANGE_EQ(cmp_rev, v4);
    // don't call bio::views::type_reduce
    auto v5 = u | bio::views::interleave(s, i) | std::views::reverse | std::views::take(5);
    EXPECT_RANGE_EQ(cmp_rev, v5);
}

TEST(view_interleave, concepts)
{
    // random_access_range, viewable_range, sized_range
    std::string u{"FOOBARBAXBAT"};
    std::string i{"in"};
    size_t s = 3;
    auto v1 = bio::detail::view_interleave(bio::views::type_reduce(u), s, bio::views::type_reduce(i));

    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v1), char>));

    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v1)>);
}
