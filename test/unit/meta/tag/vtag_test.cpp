// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <list>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <bio/meta/tag/vtag.hpp>

TEST(vtag, size0)
{
    auto v = bio::meta::vtag<>;

    {
        constexpr auto _ = v.size;
        EXPECT_EQ(_, 0ull);
    }
    {
        constexpr auto _ = v.as_tuple;
        EXPECT_EQ(_, std::tuple{});
    }
}

TEST(vtag, size1)
{
    auto v = bio::meta::vtag<42>;

    {
        constexpr auto _ = v.first_value;
        EXPECT_EQ(_, 42);
    }
    {
        constexpr auto _ = v.size;
        EXPECT_EQ(_, 1ull);
    }
    {
        constexpr auto _ = v.as_tuple;
        EXPECT_EQ(_, std::tuple<int>{42});
    }
    {
        constexpr auto _ = v.unique_values();
        EXPECT_EQ(_, true);
    }
    {
        constexpr auto _ = v.contains(42);
        EXPECT_EQ(_, true);
    }
    {
        constexpr auto _ = v.contains(2);
        EXPECT_EQ(_, false);
    }
    {
        constexpr auto _ = v.index_of(42);
        EXPECT_EQ(_, 0ull);
    }
    {
        constexpr auto _ = v.index_of(2);
        EXPECT_EQ(_, -1ull);
    }
    {
        constexpr bool a = v < bio::meta::vtag<43>;
        EXPECT_TRUE(a);
        constexpr bool b = v < bio::meta::vtag<41>;
        EXPECT_FALSE(b);
        constexpr bool c = v == bio::meta::vtag<42>;
        EXPECT_TRUE(c);
    }
}

TEST(vtag, size2_sametype)
{
    auto v = bio::meta::vtag<42, 23>;

    {
        constexpr auto _ = v.first_value;
        EXPECT_EQ(_, 42);
    }
    {
        constexpr auto _ = v.size;
        EXPECT_EQ(_, 2ull);
    }
    {
        constexpr auto _ = v.as_tuple;
        EXPECT_EQ(_, (std::tuple<int, int>{42, 23}));
    }
    {
        constexpr auto _ = v.unique_values();
        EXPECT_EQ(_, true);
    }
    {
        constexpr auto _ = v.contains(42);
        EXPECT_EQ(_, true);
    }
    {
        constexpr auto _ = v.contains(2);
        EXPECT_EQ(_, false);
    }
    {
        constexpr auto _ = v.index_of(23);
        EXPECT_EQ(_, 1ull);
    }
    {
        constexpr auto _ = v.index_of(2);
        EXPECT_EQ(_, -1ull);
    }
}

TEST(vtag, size2_difftype)
{
    auto v = bio::meta::vtag<42, bio::ranges::small_string{"foo"}>;

    {
        constexpr auto _ = v.first_value;
        EXPECT_EQ(_, 42);
    }
    {
        constexpr auto _ = v.size;
        EXPECT_EQ(_, 2ull);
    }
    {
        constexpr auto _ = v.as_tuple;
        EXPECT_EQ(_, (std::tuple<int, bio::ranges::small_string<3>>{42, "foo"}));
    }
}

TEST(vtag, string_literal)
{
    using namespace bio::meta::literals;

    auto v = "foo"_vtag;

    EXPECT_TRUE((std::same_as<decltype(v), bio::meta::vtag_t<bio::ranges::small_string{"foo"}>>));
    constexpr bool eq = v == bio::meta::vtag<bio::ranges::small_string{"foo"}>;
    EXPECT_TRUE(eq);
}

TEST(vtag, int_literal)
{
    using namespace bio::meta::literals;

    {
        auto v = 42_vtag;
        EXPECT_TRUE((std::same_as<decltype(v), bio::meta::vtag_t<int64_t{42}>>));
    }
    {
        auto v = -42_vtag;
        EXPECT_TRUE((std::same_as<decltype(v), bio::meta::vtag_t<int64_t{-42}>>));
    }
    {
        // single digit; this caused ambiguity originally; see meta::detail::literal_buffer_string
        auto v = 1_vtag;
        EXPECT_TRUE((std::same_as<decltype(v), bio::meta::vtag_t<int64_t{1}>>));
    }
}
