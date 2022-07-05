// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/mask/masked.hpp>
#include <bio/alphabet/quality/aliases.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/get.hpp>
#include <bio/ranges/views/to.hpp>
#include <bio/ranges/views/to_char.hpp>
#include <bio/ranges/views/zip.hpp>
#include <ranges>

using bio::alphabet::operator""_dna4;

TEST(view_get, basic)
{
    // TODO remove const-ness from input vector once alphabet_proxy's complement doesnt cause ICE
    std::vector<bio::alphabet::dna4q> const qv{
      {'A'_dna4, bio::alphabet::phred42{0}},
      {'C'_dna4, bio::alphabet::phred42{1}},
      {'G'_dna4, bio::alphabet::phred42{2}},
      {'T'_dna4, bio::alphabet::phred42{3}}
    };
    bio::alphabet::dna4_vector          cmp0{'A'_dna4, 'C'_dna4, 'G'_dna4, 'T'_dna4};
    std::vector<bio::alphabet::phred42> cmp1{bio::alphabet::phred42{0},
                                             bio::alphabet::phred42{1},
                                             bio::alphabet::phred42{2},
                                             bio::alphabet::phred42{3}};

    //functor
    bio::alphabet::dna4_vector functor0 = bio::ranges::views::get<0>(qv) | bio::ranges::views::to<std::vector>();
    std::vector<bio::alphabet::phred42> functor1 =
      bio::ranges::views::get<1>(qv) | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp0, functor0);
    EXPECT_EQ(cmp1, functor1);

    // pipe notation
    bio::alphabet::dna4_vector          pipe0 = qv | bio::ranges::views::get<0> | bio::ranges::views::to<std::vector>();
    std::vector<bio::alphabet::phred42> pipe1 = qv | bio::ranges::views::get<1> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp0, pipe0);
    EXPECT_EQ(cmp1, pipe1);

    // combinability
    bio::alphabet::dna4_vector cmp2{"TGCA"_dna4};
    bio::alphabet::dna4_vector comp =
      qv | bio::ranges::views::get<0> | bio::ranges::views::complement | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp2, comp);

    std::string cmp3{"TGCA"};
    std::string to_char_test = comp | bio::ranges::views::to_char | bio::ranges::views::to<std::string>();
    EXPECT_EQ(cmp3, to_char_test);

    // reference return check
    functor1[0] = bio::alphabet::phred42{4};
    std::vector<bio::alphabet::phred42> cmp4{bio::alphabet::phred42{4},
                                             bio::alphabet::phred42{1},
                                             bio::alphabet::phred42{2},
                                             bio::alphabet::phred42{3}};
    EXPECT_EQ(cmp4, functor1);
}

TEST(view_get, advanced)
{
    // TODO remove const-ness from input vector once alphabet_proxy inherits it's alphabet
    std::vector<bio::alphabet::qualified<bio::alphabet::masked<bio::alphabet::dna4>, bio::alphabet::phred42>> const t{
      {  {'A'_dna4, bio::alphabet::mask::MASKED}, bio::alphabet::phred42{0}},
      {{'C'_dna4, bio::alphabet::mask::UNMASKED}, bio::alphabet::phred42{1}},
      {  {'G'_dna4, bio::alphabet::mask::MASKED}, bio::alphabet::phred42{2}},
      {{'T'_dna4, bio::alphabet::mask::UNMASKED}, bio::alphabet::phred42{3}}
    };

    // functor notation
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> cmp0{
      {'A'_dna4,   bio::alphabet::mask::MASKED},
      {'C'_dna4, bio::alphabet::mask::UNMASKED},
      {'G'_dna4,   bio::alphabet::mask::MASKED},
      {'T'_dna4, bio::alphabet::mask::UNMASKED}
    };
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> functor0 =
      bio::ranges::views::get<0>(t) | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp0, functor0);

    std::vector<bio::alphabet::phred42> cmp1{bio::alphabet::phred42{0},
                                             bio::alphabet::phred42{1},
                                             bio::alphabet::phred42{2},
                                             bio::alphabet::phred42{3}};
    std::vector<bio::alphabet::phred42> functor1 =
      bio::ranges::views::get<1>(t) | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp1, functor1);

    bio::alphabet::dna4_vector cmp00{'A'_dna4, 'C'_dna4, 'G'_dna4, 'T'_dna4};
    bio::alphabet::dna4_vector functor00 =
      bio::ranges::views::get<0>(bio::ranges::views::get<0>(t)) | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp00, functor00);

    // pipe notation
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> pipe0 =
      t | bio::ranges::views::get<0> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp0, pipe0);

    std::vector<bio::alphabet::phred42> pipe1 = t | bio::ranges::views::get<1> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp1, pipe1);

    bio::alphabet::dna4_vector pipe00 =
      t | bio::ranges::views::get<0> | bio::ranges::views::get<0> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp00, pipe00);

    // combinability
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> cmprev{
      {'T'_dna4, bio::alphabet::mask::UNMASKED},
      {'G'_dna4,   bio::alphabet::mask::MASKED},
      {'C'_dna4, bio::alphabet::mask::UNMASKED},
      {'A'_dna4,   bio::alphabet::mask::MASKED}
    };
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> revtest =
      t | bio::ranges::views::get<0> | std::views::reverse | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmprev, revtest);

    bio::alphabet::dna4_vector cmprev2{'T'_dna4, 'G'_dna4, 'C'_dna4, 'A'_dna4};
    bio::alphabet::dna4_vector revtest2 = t | bio::ranges::views::get<0> | bio::ranges::views::get<0> |
                                          std::views::reverse | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmprev2, revtest2);

    // reference check
    functor0[0] = bio::alphabet::masked<bio::alphabet::dna4>{'T'_dna4, bio::alphabet::mask::UNMASKED};
    std::vector<bio::alphabet::masked<bio::alphabet::dna4>> cmpref{
      {'T'_dna4, bio::alphabet::mask::UNMASKED},
      {'C'_dna4, bio::alphabet::mask::UNMASKED},
      {'G'_dna4,   bio::alphabet::mask::MASKED},
      {'T'_dna4, bio::alphabet::mask::UNMASKED}
    };
    EXPECT_EQ(cmpref, functor0);
}

TEST(view_get, tuple_pair)
{
    std::vector<std::pair<int, int>> pair_test{
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 4}
    };
    std::vector<std::tuple<int, int>> tuple_test{
      {0, 1},
      {1, 2},
      {2, 3},
      {3, 4}
    };

    // functor notation
    std::vector<int> cmp{0, 1, 2, 3};
    std::vector<int> pair_func  = bio::ranges::views::get<0>(pair_test) | bio::ranges::views::to<std::vector>();
    std::vector<int> tuple_func = bio::ranges::views::get<0>(tuple_test) | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp, pair_func);
    EXPECT_EQ(cmp, tuple_func);

    // reference test
    cmp[0]        = 4;
    pair_func[0]  = 4;
    tuple_func[0] = 4;
    EXPECT_EQ(cmp, pair_func);
    EXPECT_EQ(cmp, tuple_func);

    // pipe notation
    cmp[0]                      = 0;
    std::vector<int> pair_pipe  = pair_test | bio::ranges::views::get<0> | bio::ranges::views::to<std::vector>();
    std::vector<int> tuple_pipe = tuple_test | bio::ranges::views::get<0> | bio::ranges::views::to<std::vector>();
    EXPECT_EQ(cmp, pair_pipe);
    EXPECT_EQ(cmp, tuple_pipe);
}

TEST(view_get, concepts)
{
    std::vector<std::tuple<int, int>> vec{
      {0, 1},
      {0, 1},
      {0, 1},
      {0, 1},
      {0, 1}
    };
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), std::tuple<int, int>>));

    auto v1 = vec | bio::ranges::views::get<0>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), std::tuple<int, int>>));
    EXPECT_TRUE((std::ranges::output_range<decltype(v1), int>));
}

// https://github.com/seqan/seqan3/issues/745
TEST(view_get, nested_zip_view)
{
    std::vector vec1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto get_view = bio::ranges::views::zip(bio::ranges::views::zip(vec1, vec1), vec1) | bio::ranges::views::get<0>;

    for (auto && elem : get_view)
        std::get<0>(elem) = -1;

    EXPECT_EQ(vec1[0], -1);
}
