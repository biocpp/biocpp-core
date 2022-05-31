// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/adaptation/all.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using char_types = ::testing::Types<char, char16_t, char32_t, wchar_t>;

INSTANTIATE_TYPED_TEST_SUITE_P(char_adaptation, alphabet, char_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(char_adaptation, semi_alphabet_test, char_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(char_adaptation, alphabet_constexpr, char_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(char_adaptation, semi_alphabet_constexpr, char_types, );

template <typename T>
using char_adaptation = ::testing::Test;

TYPED_TEST_SUITE(char_adaptation, char_types, );

TYPED_TEST(char_adaptation, type_properties)
{
    EXPECT_TRUE((std::is_trivially_copyable_v<TypeParam>));
    EXPECT_TRUE((std::is_trivially_default_constructible_v<TypeParam>));
    EXPECT_TRUE((std::is_trivial_v<TypeParam>));
}

TYPED_TEST(char_adaptation, alphabet_char_t)
{
    EXPECT_TRUE((std::is_same_v<bio::alphabet_char_t<TypeParam>, TypeParam>));
    EXPECT_TRUE((std::is_same_v<bio::alphabet_char_t<TypeParam &>, TypeParam>));
    EXPECT_TRUE((std::is_same_v<bio::alphabet_char_t<TypeParam &&>, TypeParam>));
}

TYPED_TEST(char_adaptation, to_char)
{
    TypeParam l{'A'};
    EXPECT_TRUE((std::is_same_v<decltype(bio::to_char(l)),
                                bio::alphabet_char_t<TypeParam>>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::to_char(TypeParam{'A'})),
                                bio::alphabet_char_t<TypeParam>>));
    EXPECT_EQ(bio::to_char(TypeParam{'A'}), l);
}

TYPED_TEST(char_adaptation, assign_char_to)
{
    TypeParam l{'A'};
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_char_to('A', l)),
                                bio::alphabet_char_t<TypeParam> &>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_char_to('A', TypeParam{'A'})),
                                bio::alphabet_char_t<TypeParam>>));
    EXPECT_EQ((bio::assign_char_to('A', TypeParam{'C'})), l);
    EXPECT_EQ((bio::assign_char_to('C', l)), TypeParam{'C'});
}

TYPED_TEST(char_adaptation, assign_char_strictly_to)
{
    TypeParam l{'A'};
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_char_strictly_to('A', l)),
                                bio::alphabet_char_t<TypeParam> &>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_char_strictly_to('A', TypeParam{'A'})),
                                bio::alphabet_char_t<TypeParam>>));
    EXPECT_EQ((bio::assign_char_strictly_to('A', TypeParam{'C'})), l);
    EXPECT_EQ((bio::assign_char_strictly_to('C', l)), TypeParam{'C'});
}

TYPED_TEST(char_adaptation, alphabet_rank_t)
{
    EXPECT_TRUE((std::is_integral_v<bio::alphabet_rank_t<TypeParam>>));
    EXPECT_TRUE((std::is_unsigned_v<bio::alphabet_rank_t<TypeParam>>));
    EXPECT_GE(sizeof(bio::alphabet_rank_t<TypeParam>), sizeof(TypeParam));
}

TYPED_TEST(char_adaptation, to_rank)
{
    TypeParam l{'A'};
    EXPECT_TRUE((std::is_same_v<decltype(bio::to_rank(l)),
                                bio::alphabet_rank_t<TypeParam>>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::to_rank(TypeParam{'A'})),
                                bio::alphabet_rank_t<TypeParam>>));

    unsigned char cmp{'A'};
    EXPECT_EQ(bio::to_rank(TypeParam{65}), cmp);
}

TYPED_TEST(char_adaptation, assign_rank_to)
{
    TypeParam l{'A'};
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_rank_to(65, l)),
                                bio::alphabet_char_t<TypeParam> &>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_rank_to(65, TypeParam{'A'})),
                                bio::alphabet_char_t<TypeParam>>));
    EXPECT_EQ((bio::assign_rank_to(65, TypeParam{'C'})), l);
    EXPECT_EQ((bio::assign_rank_to(67, l)), TypeParam{'C'});
}

TYPED_TEST(char_adaptation, alphabet_size)
{
    EXPECT_EQ(bio::alphabet_size<TypeParam>,
        static_cast<size_t>(std::numeric_limits<TypeParam>::max()) + 1 - std::numeric_limits<TypeParam>::lowest());
}
