// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>
#include <iterator>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

TEST(view_to_char, basic)
{
    std::vector<bio::alphabet::dna5> vec{"ACTTTGATA"_dna5};
    std::string                      cmp{"ACTTTGATA"};

    // pipe notation
    std::string v = vec | bio::ranges::views::to_char | bio::ranges::to<std::string>();
    EXPECT_EQ(cmp, v);

    // function notation
    std::string v2(bio::ranges::views::to_char(vec) | bio::ranges::to<std::string>());
    EXPECT_EQ(cmp, v2);

    // combinability
    std::string cmp2{"ATAGTTTCA"};
    std::string v3 = vec | bio::ranges::views::to_char | std::views::reverse | bio::ranges::to<std::string>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_to_char, preserve_string)
{
    {
        std::string const s{"ACTTTGATA"};

        auto v = s | bio::ranges::views::to_char;

        EXPECT_EQ(v, s);
        EXPECT_TRUE((std::same_as<decltype(v), std::string_view>));
    }

    /* deep behaviour */
    {
        std::vector<std::string> vec{"ACGT", "AGGA"};
        auto                     v = vec | bio::ranges::views::to_char;

        EXPECT_RANGE_EQ(v, vec);
        EXPECT_TRUE((std::same_as<decltype(v), std::span<std::string>>));
    }
}

TEST(view_to_char, cigars)
{
#if !BIOCPP_WORKAROUND_CLANG_59501
    {
        std::vector<bio::alphabet::cigar> vec{
          {23, 'I'_cigar_op},
          {42, 'M'_cigar_op},
          {42, 'D'_cigar_op}
        };

        std::string_view comp = "23I42M42D";

        auto v = vec | bio::ranges::views::to_char;

        static_assert(std::ranges::input_range<decltype(v)>);
        EXPECT_RANGE_EQ(v, comp);
    }

    /* deep */
    {
        std::vector<std::vector<bio::alphabet::cigar>> vec{
          {{23, 'I'_cigar_op}, {42, 'M'_cigar_op}, {42, 'D'_cigar_op}},
          { {1, 'I'_cigar_op}, {22, 'M'_cigar_op}, {33, 'D'_cigar_op}}
        };

        std::vector<std::string_view> comp = {"23I42M42D", "1I22M33D"};

        auto v = vec | bio::ranges::views::to_char;

        ASSERT_EQ(v.size(), 2ull);
        EXPECT_RANGE_EQ(v[0], comp[0]);
        EXPECT_RANGE_EQ(v[1], comp[1]);
    }
#endif
}

TEST(view_to_char, concepts)
{
    std::vector<bio::alphabet::dna5> vec{"ACTTTGATA"_dna5};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), bio::alphabet::dna5>));

    auto v1 = vec | bio::ranges::views::to_char;
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

    std::string s;
    auto        v2 = s | bio::ranges::views::to_char;
    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v2)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v2)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(v2), char>));

#if !BIOCPP_WORKAROUND_CLANG_59501
    std::vector<bio::alphabet::cigar> cig;
    auto                              v3 = cig | bio::ranges::views::to_char;
    EXPECT_TRUE(std::ranges::input_range<decltype(v3)>);
    EXPECT_FALSE(std::ranges::forward_range<decltype(v3)>);       // TODO should be TRUE
    EXPECT_FALSE(std::ranges::bidirectional_range<decltype(v3)>); // TODO should be TRUE
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v3)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v3)>);
    EXPECT_TRUE(std::ranges::view<decltype(v3)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v3)>);
    EXPECT_FALSE(std::ranges::common_range<decltype(v3)>);
    EXPECT_FALSE(bio::ranges::const_iterable_range<decltype(v3)>); // TODO should be TRUE
    EXPECT_FALSE((std::ranges::output_range<decltype(v3), char>));
#endif
}
