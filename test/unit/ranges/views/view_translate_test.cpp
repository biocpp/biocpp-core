// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/nucleotide/all.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/ranges/views/complement.hpp>
#include <bio/ranges/views/translate.hpp>
#include <bio/ranges/views/translate_single.hpp>
#include <bio/test/expect_range_eq.hpp>
#include <ranges>

using namespace bio::alphabet::literals;

template <typename T>
class nucleotide : public ::testing::Test
{};

// add all alphabets here
using nucleotide_types = ::testing::Types<bio::alphabet::dna4,
                                          bio::alphabet::dna5,
                                          bio::alphabet::dna15,
                                          bio::alphabet::rna4,
                                          bio::alphabet::rna5,
                                          bio::alphabet::rna15>;

TYPED_TEST_SUITE(nucleotide, nucleotide_types, );

TYPED_TEST(nucleotide, view_translate_single)
{
    std::string const          in{"ACGTACGTACGTA"};
    std::vector<TypeParam>     vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    bio::alphabet::aa27_vector cmp1{"TYVR"_aa27};
    bio::alphabet::aa27_vector cmp2{"CMHA"_aa27};
    bio::alphabet::aa27_vector cmp3{"AHMC"_aa27};

    // default parameter translation_frames
    auto v1 = vec | bio::ranges::views::translate_single;
    // == [T,Y,V,R]
    EXPECT_RANGE_EQ(v1, cmp1);

    // default parameter translation_frames
    auto v2 = vec | bio::ranges::views::translate_single();
    // == [T,Y,V,R]
    EXPECT_RANGE_EQ(v2, cmp1);

    // single frame translation
    auto v3 = vec | bio::ranges::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [T,Y,V,R]
    EXPECT_RANGE_EQ(v3, cmp1);

    // function syntax
    auto v4 = bio::ranges::views::translate_single(vec, bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [T,Y,V,R]
    EXPECT_RANGE_EQ(v4, cmp1);

    // combinability
    auto v5 = vec | bio::ranges::views::complement |
              bio::ranges::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [C,M,H,A]
    EXPECT_RANGE_EQ(v5, cmp2);

    // combinability
    auto v6 = vec | bio::ranges::views::complement |
              bio::ranges::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_0) |
              std::views::reverse;
    // == [A,H,M,C]
    EXPECT_RANGE_EQ(v6, cmp3);
}

TYPED_TEST(nucleotide, view_translate)
{
    std::string const      in{"ACGTACGTACGTA"};
    std::vector<TypeParam> vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    std::vector<std::vector<bio::alphabet::aa27>> cmp1{{"TYVR"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp2{{"TYVR"_aa27}, {"YVRT"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp3{{"TYVR"_aa27}, {"RTYV"_aa27}, {"VRT"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp4{{"TYVR"_aa27},
                                                       {"RTYV"_aa27},
                                                       {"VRT"_aa27},
                                                       {"YVRT"_aa27},
                                                       {"TYVR"_aa27},
                                                       {"RTY"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp5{{"TYVR"_aa27}, {"VRT"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp6{{"CMHA"_aa27}, {"MHAC"_aa27}};
    std::vector<std::vector<bio::alphabet::aa27>> cmp7{{"CMHA"_aa27}};

    // default parameter translation_frames
    auto v1 = vec | bio::ranges::views::translate;
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]
    EXPECT_EQ(v1.size(), cmp4.size());
    for (unsigned i = 0; i < v1.size(); i++)
        EXPECT_RANGE_EQ(v1[i], cmp4[i]);

    // default parameter translation_frames
    auto v2 = vec | bio::ranges::views::translate();
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]
    EXPECT_EQ(v2.size(), cmp4.size());
    for (unsigned i = 0; i < v2.size(); i++)
        EXPECT_RANGE_EQ(v2[i], cmp4[i]);

    // single frame translation
    auto v3 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_FRAME_0);
    // == [[T,Y,V,R]]
    EXPECT_EQ(v3.size(), cmp1.size());
    for (unsigned i = 0; i < v3.size(); i++)
        EXPECT_RANGE_EQ(v3[i], cmp1[i]);

    // reverse translation
    auto v4 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_REV_0);
    // == [[T,Y,V,R],[Y,V,R,T]]
    EXPECT_EQ(v4.size(), cmp2.size());
    for (unsigned i = 0; i < v4.size(); i++)
        EXPECT_RANGE_EQ(v4[i], cmp2[i]);

    // forward frames translation
    auto v5 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::FWD);
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T]]
    EXPECT_EQ(v5.size(), cmp3.size());
    for (unsigned i = 0; i < v5.size(); i++)
        EXPECT_RANGE_EQ(v5[i], cmp3[i]);

    // six frame translation
    auto v6 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::SIX_FRAME);
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]
    EXPECT_EQ(v6.size(), cmp4.size());
    for (unsigned i = 0; i < v6.size(); i++)
        EXPECT_RANGE_EQ(v6[i], cmp4[i]);

    // user-defined frame combination
    auto v7 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_FRAME_0 |
                                                  bio::alphabet::translation_frames::FWD_FRAME_2);
    // == [[T,Y,V,R],[V,R,T]]
    EXPECT_EQ(v7.size(), cmp5.size());
    for (unsigned i = 0; i < v7.size(); i++)
        EXPECT_RANGE_EQ(v7[i], cmp5[i]);

    // function syntax
    auto v8 = bio::ranges::views::translate(vec, bio::alphabet::translation_frames::FWD_REV_0);
    // == [[T,Y,V,R],[Y,V,R,T]]
    EXPECT_EQ(v8.size(), cmp2.size());
    for (unsigned i = 0; i < v8.size(); i++)
        EXPECT_RANGE_EQ(v8[i], cmp2[i]);

    // combinability
    auto v9 = vec | bio::ranges::views::complement |
              bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_REV_0);
    // == [[C,M,H,A],[M,H,A,C]]
    EXPECT_EQ(v9.size(), cmp6.size());
    for (unsigned i = 0; i < v9.size(); i++)
        EXPECT_RANGE_EQ(v9[i], cmp6[i]);

    // combinability
    auto v10 = vec | bio::ranges::views::complement |
               bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_REV_0) | std::views::take(1);
    // == [[C,M,H,A]]
    EXPECT_EQ(v10.size(), cmp7.size());
    for (unsigned i = 0; i < v10.size(); i++)
        EXPECT_RANGE_EQ(v10[i], cmp7[i]);

    // combinability and function syntax
    auto v11 = bio::views::translate(bio::ranges::views::complement(vec), bio::alphabet::translation_frames::FWD_REV_0);
    // == [[C,M,H,A],[M,H,A,C]]
    EXPECT_EQ(v11.size(), cmp6.size());
    for (unsigned i = 0; i < v11.size(); i++)
        EXPECT_RANGE_EQ(v11[i], cmp6[i]);
}

TYPED_TEST(nucleotide, view_translate_single_container_conversion)
{
    std::string const          in{"ACGTACGTACGTA"};
    std::vector<TypeParam>     vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    bio::alphabet::aa27_vector cmp1{"TYVR"_aa27};

    // default parameter translation_frames
    auto v1 = vec | bio::ranges::views::translate_single | bio::ranges::to<std::vector>();
    // == [T,Y,V,R]
    EXPECT_EQ(std::vector<bio::alphabet::aa27>(v1), cmp1);
}

TYPED_TEST(nucleotide, view_translate_container_conversion)
{
    std::string const      in{"ACGTACGTACGTA"};
    std::vector<TypeParam> vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    std::vector<std::vector<bio::alphabet::aa27>> cmp1{{"TYVR"_aa27},
                                                       {"RTYV"_aa27},
                                                       {"VRT"_aa27},
                                                       {"YVRT"_aa27},
                                                       {"TYVR"_aa27},
                                                       {"RTY"_aa27}};

    // six frame translation
    auto v1 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::SIX_FRAME);
    // == [[T,Y,V,R],[R,T,Y,V],[V,R,T],[Y,V,R,T],[T,Y,V,R],[R,T,Y]]
    EXPECT_EQ(v1.size(), cmp1.size());
    for (unsigned i = 0; i < v1.size(); i++)
        EXPECT_EQ(v1[i] | bio::ranges::to<std::vector>(), cmp1[i]);

    EXPECT_TRUE(bio::ranges::concatenated_sequences<std::vector<bio::alphabet::aa27>>(v1) == cmp1);
}

TYPED_TEST(nucleotide, view_translate_single_concepts)
{
    std::string const      in{"ACGTACGTACGTA"};
    std::vector<TypeParam> vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);

    auto v1 = vec | bio::ranges::views::translate_single(bio::alphabet::translation_frames::FWD_FRAME_0);
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE((std::is_same_v<bio::alphabet::aa27, std::ranges::range_value_t<decltype(v1)>>));
    EXPECT_TRUE((std::is_same_v<bio::alphabet::aa27, std::ranges::range_reference_t<decltype(v1)>>));
}

TYPED_TEST(nucleotide, view_translate_concepts)
{
    std::string const      in{"ACGTACGTACGTA"};
    std::vector<TypeParam> vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);

    auto v1 = vec | bio::ranges::views::translate(bio::alphabet::translation_frames::FWD_REV_0);
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::sized_range<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::view<std::ranges::range_value_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::random_access_range<std::ranges::range_reference_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::sized_range<std::ranges::range_reference_t<decltype(v1)>>);
    EXPECT_TRUE(std::ranges::view<std::ranges::range_reference_t<decltype(v1)>>);
}

TYPED_TEST(nucleotide, issue1339)
{
    // empty input
    std::string            in{};
    std::vector<TypeParam> vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();

    auto v = vec | bio::ranges::views::translate;

    auto out_vecvec = v | bio::ranges::to<std::vector<std::vector<bio::alphabet::aa27>>>();

    EXPECT_EQ(out_vecvec.size(), 6u);
    for (auto & out_vec : out_vecvec)
        EXPECT_TRUE(out_vec.empty());

    // input of size 1
    in  = "A";
    vec = in | bio::ranges::views::char_to<TypeParam> | bio::ranges::to<std::vector>();

    v = vec | bio::ranges::views::translate;

    out_vecvec = v | bio::ranges::to<std::vector<std::vector<bio::alphabet::aa27>>>();

    EXPECT_EQ(out_vecvec.size(), 6u);
    for (auto & out_vec : out_vecvec)
        EXPECT_TRUE(out_vec.empty());
}
