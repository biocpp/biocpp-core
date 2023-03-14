// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <concepts>
#include <forward_list>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/interleave.hpp>
#include <bio/ranges/views/take.hpp>
#include <bio/ranges/views/type_reduce.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_interleave, basic)
{
    std::string u{"FOOBARBAXBAT"};
    std::string i{"in"};
    size_t      s = 3;
    std::string cmp{"FOOinBARinBAXinBAT"};
    std::string cmp_rev{"TABni"};
    size_t      cmpsize = 18;

    // pipe notation
    // explicitly call bio::ranges::views::type_reduce
    auto v0 = bio::ranges::views::type_reduce(u) | bio::ranges::views::interleave(s, i);
    EXPECT_RANGE_EQ(cmp, v0);
    EXPECT_EQ(cmpsize, v0.size());
    // don't call bio::ranges::views::type_reduce
    auto v1 = u | bio::ranges::views::interleave(s, i);
    EXPECT_RANGE_EQ(cmp, v1);

    // function notation
    // explicitly call bio::ranges::views::type_reduce
    auto v2{bio::ranges::views::interleave(bio::ranges::views::type_reduce(u), s, i)};
    EXPECT_RANGE_EQ(cmp, v2);
    // don't call bio::ranges::views::type_reduce
    auto v3{bio::ranges::views::interleave(u, s, i)};
    EXPECT_RANGE_EQ(cmp, v3);

    //combinability
    // explicitly call bio::ranges::views::type_reduce
    auto v4 = bio::ranges::views::type_reduce(u) | bio::ranges::views::interleave(s, i) | std::views::reverse |
              std::views::take(5);
    EXPECT_RANGE_EQ(cmp_rev, v4);
    // don't call bio::ranges::views::type_reduce
    auto v5 = u | bio::ranges::views::interleave(s, i) | std::views::reverse | std::views::take(5);
    EXPECT_RANGE_EQ(cmp_rev, v5);
}

TEST(view_interleave, concepts)
{
    // random_access_range, viewable_range, sized_range
    std::string           u{"FOOBARBAXBAT"};
    std::string           i{"in"};
    size_t                s = 3;
    [[maybe_unused]] auto v1 =
      bio::ranges::views::interleave(bio::ranges::views::type_reduce(u), s, bio::ranges::views::type_reduce(i));

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

TEST(view_interleave, ref_t)
{
    {
        std::string           u{"FOOBARBAXBAT"};
        std::string           i{"in"};
        size_t                s  = 3;
        [[maybe_unused]] auto v1 = bio::ranges::views::interleave(u, s, i);
        EXPECT_TRUE((std::same_as<std::ranges::range_reference_t<decltype(v1)>, char &>));
        // capturing the inserted range as lvalue results in shallow const ref_t of "char &"
    }

#if __cpp_lib_ranges >= 202110L
    {
        std::string           u{"FOOBARBAXBAT"};
        size_t                s  = 3;
        [[maybe_unused]] auto v1 = bio::ranges::views::interleave(u, s, std::string{"in"});
        EXPECT_TRUE((std::same_as<std::ranges::range_reference_t<decltype(v1)>, char const &>));
        // capturing the inserted range as rvalue results in deep const ref_t of "char const &"
    }
#endif
}
