// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <iostream>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/ranges/views/to_upper.hpp>
#include <ranges>

using bio::operator""_dna5;

TEST(view_to_upper, basic)
{
    std::string input_string{"IAmADnaString"};
    std::string cmp{"IAMADNASTRING"};

    // pipe notation string
    std::string s(input_string | bio::views::to_upper | bio::views::to<std::string>());
    EXPECT_EQ(cmp, s);

    // custom conversion operator
    std::string s2(bio::views::to_upper(input_string) | bio::views::to<std::string>());
    EXPECT_EQ(cmp, s2);
}

TEST(view_to_upper, combinability)
{
    std::string input_string{"IAmADnaString"};
    std::string cmp{"GNIRTSANDAMAI"};

    std::vector<bio::dna5> dna_vec{"aggcgt"_dna5};
    std::string            cmp2{"AGGCGT"};

    // output combinability
    std::string s(input_string | bio::views::to_upper | std::views::reverse | bio::views::to<std::string>());
    EXPECT_EQ(cmp, s);

    // input combinability
    std::string s2(dna_vec | bio::views::to_char | bio::views::to_upper | bio::views::to<std::string>());
    EXPECT_EQ(cmp2, s2);
}

TEST(view_to_upper, deep)
{
    std::vector<std::string> input_vec{"IAmADnaString", "IAmAProteinString"};
    std::vector<std::string> cmp{"IAMADNASTRING", "IAMAPROTEINSTRING"};

    std::vector<std::string> s(input_vec | bio::views::to_upper | bio::views::to<std::vector<std::string>>());
    EXPECT_EQ(cmp, s);
}

TEST(view_to_upper, concepts)
{
    std::string   input_string{"aeiou"};
    std::string & input_string_ref = input_string;
    auto          upper_view       = input_string | bio::views::to_upper;

    // Required
    EXPECT_TRUE(std::ranges::input_range<decltype(input_string)>);
    EXPECT_TRUE(std::ranges::viewable_range<decltype(input_string_ref)>);

    // Preserved
    EXPECT_EQ(std::ranges::input_range<decltype(input_string)>, std::ranges::input_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::forward_range<decltype(input_string)>, std::ranges::forward_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::bidirectional_range<decltype(input_string)>,
              std::ranges::bidirectional_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::random_access_range<decltype(input_string)>,
              std::ranges::random_access_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::random_access_range<decltype(input_string)>,
              std::ranges::random_access_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::viewable_range<decltype(input_string_ref)>,
              std::ranges::viewable_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::sized_range<decltype(input_string)>, std::ranges::sized_range<decltype(upper_view)>);
    EXPECT_EQ(std::ranges::common_range<decltype(input_string)>, std::ranges::common_range<decltype(upper_view)>);
    EXPECT_EQ(bio::const_iterable_range<decltype(input_string)>, bio::const_iterable_range<decltype(upper_view)>);
    EXPECT_TRUE((std::same_as<std::remove_reference_t<std::ranges::range_reference_t<decltype(input_string)>>,
                              std::remove_reference_t<std::ranges::range_reference_t<decltype(upper_view)>>>));

    // Guaranteed
    EXPECT_TRUE(std::ranges::viewable_range<decltype(upper_view)>);
    EXPECT_TRUE(std::ranges::view<decltype(upper_view)>);

    // Lost
    EXPECT_FALSE((std::ranges::output_range<decltype(upper_view), char>));
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(upper_view)>);
}
