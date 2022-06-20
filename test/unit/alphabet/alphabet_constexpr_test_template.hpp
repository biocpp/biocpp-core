// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>

template <typename t>
using alphabet_constexpr = ::testing::Test;

TYPED_TEST_SUITE_P(alphabet_constexpr);

TYPED_TEST_P(alphabet_constexpr, concept_check)
{
    EXPECT_TRUE(bio::detail::constexpr_alphabet<TypeParam>);
    EXPECT_TRUE(bio::detail::constexpr_alphabet<TypeParam &>);

    EXPECT_TRUE(bio::detail::constexpr_alphabet<TypeParam const>);
    EXPECT_TRUE(bio::detail::constexpr_alphabet<TypeParam const &>);

    EXPECT_TRUE(bio::detail::writable_constexpr_alphabet<TypeParam>);
    EXPECT_TRUE(bio::detail::writable_constexpr_alphabet<TypeParam &>);

    EXPECT_FALSE(bio::detail::writable_constexpr_alphabet<TypeParam const>);
    EXPECT_FALSE(bio::detail::writable_constexpr_alphabet<TypeParam const &>);
}

TYPED_TEST_P(alphabet_constexpr, assign_char)
{
    [[maybe_unused]] constexpr TypeParam t0{bio::assign_char_to('A', TypeParam{})};
}

TYPED_TEST_P(alphabet_constexpr, to_char)
{
    constexpr TypeParam                                        t0{TypeParam{}};
    [[maybe_unused]] constexpr bio::alphabet_char_t<TypeParam> c = bio::to_char(t0);
}

REGISTER_TYPED_TEST_SUITE_P(alphabet_constexpr, concept_check, assign_char, to_char);
