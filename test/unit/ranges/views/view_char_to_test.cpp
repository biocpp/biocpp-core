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
    std::string                      vec{"ACTTTGATA"};
    std::vector<bio::alphabet::dna5> cmp{"ACTTTGATA"_dna5};

    // pipe notation
    std::vector<bio::alphabet::dna5> v =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    std::vector<bio::alphabet::dna5> v2(bio::ranges::views::char_to<bio::alphabet::dna5>(vec) |
                                        bio::ranges::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    std::vector<bio::alphabet::dna5> cmp2{"ATAGTTTCA"_dna5};
    std::vector<bio::alphabet::dna5> v3 =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_char_to, deep_view)
{
    std::vector<std::string> foo{"ACGTA", "TGCAT"};

    std::vector<std::vector<bio::alphabet::dna5>> v = foo | bio::ranges::views::char_to<bio::alphabet::dna5> |
                                                      bio::ranges::to<std::vector<std::vector<bio::alphabet::dna5>>>();

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ACGTA"_dna5);
    EXPECT_RANGE_EQ(v[1], "TGCAT"_dna5);
}

TEST(view_char_to, preserve_string)
{
    std::string const s{"ACTTTGATA"};

    auto v = s | bio::ranges::views::char_to<char>;

    EXPECT_EQ(v, s);
    EXPECT_TRUE((std::same_as<decltype(v), std::string_view>));
}

TEST(view_char_to, concepts)
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

    auto v1 = vec | bio::ranges::views::char_to<bio::alphabet::dna5>;
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

    auto v2 = vec | bio::ranges::views::char_to<char>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v2)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), bio::alphabet::dna5>));
    EXPECT_TRUE((std::ranges::output_range<decltype(v2), char>));
}

TEST(view_char_to, exception)
{
    std::string foo = "ACGPTA";

    auto v = foo | bio::ranges::views::char_to<bio::alphabet::dna5>;
    EXPECT_NO_THROW((std::ranges::equal(v, "ACGNTA"_dna5)));
}
