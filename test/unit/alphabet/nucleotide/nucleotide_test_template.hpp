// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <sstream>

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/nucleotide/concept.hpp>

template <typename t>
using nucleotide = ::testing::Test;

TYPED_TEST_SUITE_P(nucleotide);

TYPED_TEST_P(nucleotide, concept_check)
{
    EXPECT_TRUE(bio::nucleotide_alphabet<TypeParam>);
    EXPECT_TRUE(bio::nucleotide_alphabet<TypeParam &>);
    EXPECT_TRUE(bio::nucleotide_alphabet<TypeParam const>);
    EXPECT_TRUE(bio::nucleotide_alphabet<TypeParam const &>);
}

TYPED_TEST_P(nucleotide, complement)
{
    EXPECT_EQ(bio::complement(TypeParam{}.assign_char('A')), TypeParam{}.assign_char('T'));
    EXPECT_EQ(bio::complement(TypeParam{}.assign_char('C')), TypeParam{}.assign_char('G'));
    EXPECT_EQ(bio::complement(TypeParam{}.assign_char('G')), TypeParam{}.assign_char('C'));
    EXPECT_EQ(bio::complement(TypeParam{}.assign_char('T')), TypeParam{}.assign_char('A'));

    using vsize_t = std::decay_t<decltype(bio::alphabet_size<TypeParam>)>;

    for (vsize_t i = 0u; i < bio::alphabet_size<TypeParam>; ++i)
    {
        TypeParam c = bio::assign_rank_to(i, TypeParam{});

        EXPECT_EQ(bio::complement(bio::complement(c)), c);
    }
}

REGISTER_TYPED_TEST_SUITE_P(nucleotide, concept_check, complement);
