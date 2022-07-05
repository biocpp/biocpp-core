// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/rank_to.hpp>
#include <bio/ranges/views/to.hpp>
#include <ranges>

TEST(view_rank_to, basic)
{
    using bio::alphabet::operator""_dna5;

    std::vector<unsigned>      vec{0, 1, 4, 4, 4, 2, 0, 4, 0};
    bio::alphabet::dna5_vector cmp{"ACTTTGATA"_dna5};

    // pipe notation
    bio::alphabet::dna5_vector v =
      vec | bio::ranges::views::rank_to<bio::alphabet::dna5> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    bio::alphabet::dna5_vector v2(bio::ranges::views::rank_to<bio::alphabet::dna5>(vec) |
                                  bio::ranges::views::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    bio::alphabet::dna5_vector cmp2{"ATAGTTTCA"_dna5};
    bio::alphabet::dna5_vector v3 = vec | bio::ranges::views::rank_to<bio::alphabet::dna5> | std::views::reverse |
                                    bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_rank_to, concepts)
{
    std::vector<unsigned> vec{0, 1, 3, 3, 3, 2, 0, 3, 0};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), unsigned>));

    auto v1 = vec | bio::ranges::views::rank_to<bio::alphabet::dna5>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), unsigned>));
}
