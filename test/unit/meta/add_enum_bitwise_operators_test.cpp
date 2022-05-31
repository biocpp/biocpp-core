// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <type_traits>

#include <gtest/gtest.h>

#include <bio/meta/add_enum_bitwise_operators.hpp>

namespace bio
{
enum class my_enum
{
    ZERO = 0,
    VAL1 = 1,
    VAL2 = 2,
    COMB = 3
};
} // namespace bio

template <>
constexpr bool bio::add_enum_bitwise_operators<bio::my_enum> = true;


TEST(add_enum_bitwise_operators, AND)
{
    bio::my_enum e = bio::my_enum::VAL1;
    bio::my_enum e2 = e & bio::my_enum::VAL2;
    EXPECT_EQ(e2, bio::my_enum::ZERO);
}

TEST(add_enum_bitwise_operators, OR)
{
    bio::my_enum e = bio::my_enum::VAL1;
    bio::my_enum e2 = e | bio::my_enum::VAL2;
    EXPECT_EQ(e2, bio::my_enum::COMB);
}

TEST(add_enum_bitwise_operators, XOR)
{
    bio::my_enum e = bio::my_enum::VAL1;
    bio::my_enum e2 = e ^ bio::my_enum::VAL2;
    EXPECT_EQ(e2, bio::my_enum::COMB);
}

TEST(add_enum_bitwise_operators, NOT)
{
    bio::my_enum e = bio::my_enum::VAL1;
    bio::my_enum e2 = ~e;
    EXPECT_NE(e, e2);
    e2 = ~e2;
    EXPECT_EQ(e, e2);
}

TEST(add_enum_bitwise_operators, AND_ASSIGN)
{
    bio::my_enum e = bio::my_enum::VAL1;
    e &= bio::my_enum::VAL2;
    EXPECT_EQ(e, bio::my_enum::ZERO);
}

TEST(add_enum_bitwise_operators, OR_ASSIGN)
{
    bio::my_enum e = bio::my_enum::VAL1;
    e |= bio::my_enum::VAL2;
    EXPECT_EQ(e, bio::my_enum::COMB);
}

TEST(add_enum_bitwise_operators, XOR_ASSIGN)
{
    bio::my_enum e = bio::my_enum::VAL1;
    e ^= bio::my_enum::VAL2;
    EXPECT_EQ(e, bio::my_enum::COMB);
}
