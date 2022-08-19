// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <deque>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <algorithm>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/container/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/single_pass_input.hpp>
#include <bio/ranges/views/slice.hpp>
#include <bio/test/expect_range_eq.hpp>
#include <bio/test/expect_same_type.hpp>
#include <concepts>
#include <ranges>

TEST(view_slice, regular)
{
    using namespace std::literals;

    std::string const vec{"foobar"};

    // pipe notation
    EXPECT_RANGE_EQ("oob"sv, vec | bio::ranges::views::slice(1, 4));

    // function notation
    EXPECT_RANGE_EQ("oob"sv, bio::ranges::views::slice(vec, 1, 4));

    // combinability
    EXPECT_RANGE_EQ("o"sv,
                    vec | bio::ranges::views::slice(0, 4) | bio::ranges::views::slice(1, 3) | std::views::take(1));
    EXPECT_RANGE_EQ("abo"sv, vec | std::views::reverse | bio::ranges::views::slice(1, 4) | std::views::take(3));

    // store arg
    auto a0 = bio::ranges::views::slice(1, 4);
    EXPECT_RANGE_EQ("oob"sv, vec | a0);

    // store combined
    auto a1 = bio::ranges::views::slice(0, 4) | bio::ranges::views::slice(1, 3) | std::views::take(1);
    EXPECT_RANGE_EQ("o"sv, vec | a1);

    // store combined in middle
    auto a2 = std::views::reverse | bio::ranges::views::slice(1, 4) | std::views::take(3);
    EXPECT_RANGE_EQ("abo"sv, vec | a2);
}

TEST(view_slice, concepts)
{
    std::vector vec{1, 2, 3};

    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), int>));

    auto v1 = vec | bio::ranges::views::slice(1, 4);

    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v1), int>));

    auto v2 = vec | bio::ranges::views::single_pass_input | bio::ranges::views::slice(1, 4);

    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_FALSE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_FALSE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_EQ(std::ranges::sized_range<decltype(v2)>, false);
    EXPECT_FALSE(std::ranges::common_range<decltype(v2)>);
    EXPECT_FALSE(bio::ranges::const_iterable_range<decltype(v2)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v2), int>));
}

TEST(view_slice, underlying_is_shorter)
{
    using namespace std::literals;

    std::string vec{"foobar"};
    EXPECT_NO_THROW((bio::ranges::views::slice(vec, 1, 4))); // no parsing

    // full parsing on conversion
    EXPECT_RANGE_EQ("oob"sv, vec | bio::ranges::views::single_pass_input | bio::ranges::views::slice(1, 4));
}

TEST(view_slice, end_before_begin)
{
    std::string vec{"foobar"};
    EXPECT_THROW(bio::ranges::views::slice(vec, 4, 1), std::invalid_argument);
}

TEST(view_slice, type_erasure)
{
    { // string overload
        std::string const urange{"foobar"};

        auto v = bio::ranges::views::slice(urange, 1, 4);

        EXPECT_SAME_TYPE(decltype(v), std::string_view);
        EXPECT_RANGE_EQ(v, urange.substr(1, 3));
    }

    { // stringview overload
        std::string_view urange{"foobar"};

        auto v = bio::ranges::views::slice(urange, 1, 4);

        EXPECT_SAME_TYPE(decltype(v), std::string_view);
        EXPECT_RANGE_EQ(v, urange.substr(1, 3));
    }

    { // contiguous overload
        std::vector<int> urange{1, 2, 3, 4, 5, 6};

        auto v = bio::ranges::views::slice(urange, 1, 4);

        EXPECT_SAME_TYPE(decltype(v), (std::span<int, std::dynamic_extent>));
        EXPECT_RANGE_EQ(v, (std::vector{2, 3, 4}));
    }

    { // contiguous overload
        std::array<int, 6> urange{1, 2, 3, 4, 5, 6};

        auto v = bio::ranges::views::slice(urange, 1, 4);

        EXPECT_SAME_TYPE(decltype(v), (std::span<int, std::dynamic_extent>));
        EXPECT_RANGE_EQ(v, (std::vector{2, 3, 4}));
    }

    { // random-access overload
        std::deque<int> urange{1, 2, 3, 4, 5, 6};

        auto v = bio::ranges::views::slice(urange, 1, 4);

        EXPECT_TRUE((
          std::same_as<decltype(v),
                       std::ranges::subrange<typename std::deque<int>::iterator, typename std::deque<int>::iterator>>));
        EXPECT_RANGE_EQ(v, (std::vector{2, 3, 4}));
    }
}
