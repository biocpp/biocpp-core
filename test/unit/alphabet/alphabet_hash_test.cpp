// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/ranges/hash.hpp>

template <typename T>
using alphabet_hashing = ::testing::Test;

using test_types = ::testing::Types<bio::alphabet::dna4,
                                    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>,
                                    bio::alphabet::gapped<bio::alphabet::dna4>>;

TYPED_TEST_SUITE(alphabet_hashing, test_types, );

TYPED_TEST(alphabet_hashing, hash)
{
    {
        TypeParam            t0{};
        std::hash<TypeParam> h{};
        if constexpr (std::same_as<TypeParam, char>)
        {
            for (size_t i = 0; i < bio::alphabet::size<TypeParam> / 2; ++i)
            {
                bio::alphabet::assign_rank_to(i, t0);
                ASSERT_EQ(h(t0), i);
            }
        }
        else
        {
            for (size_t i = 0; i < bio::alphabet::size<TypeParam>; ++i)
            {
                bio::alphabet::assign_rank_to(i, t0);
                ASSERT_EQ(h(t0), i);
            }
        }
    }
    {
        std::vector<TypeParam> text;
        text.reserve(4);
        for (size_t i = 0; i < 4; ++i)
        {
            text.push_back(bio::alphabet::assign_rank_to(0, TypeParam{}));
        }
        std::hash<decltype(text)> h{};
        ASSERT_EQ(h(text), 0u);
    }
    {
        std::hash<TypeParam const> h{};
        if constexpr (std::same_as<TypeParam, char>)
        {
            for (size_t i = 0; i < bio::alphabet::size<TypeParam> / 2; ++i)
            {
                TypeParam const t0 = bio::alphabet::assign_rank_to(i, TypeParam{});
                ASSERT_EQ(h(t0), i);
            }
        }
        else
        {
            for (size_t i = 0; i < bio::alphabet::size<TypeParam>; ++i)
            {
                TypeParam const t0 = bio::alphabet::assign_rank_to(i, TypeParam{});
                ASSERT_EQ(h(t0), i);
            }
        }
    }
    {
        std::vector<TypeParam> const text(4, bio::alphabet::assign_rank_to(0, TypeParam{}));
        std::hash<decltype(text)>    h{};
        ASSERT_EQ(h(text), 0u);
    }
}
