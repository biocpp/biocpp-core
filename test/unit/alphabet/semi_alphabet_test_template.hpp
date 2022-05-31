// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/exception.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/test/pretty_printing.hpp>

template <typename T>
using semi_alphabet_test = ::testing::Test;

constexpr size_t maximum_iterations = 65536u;

TYPED_TEST_SUITE_P(semi_alphabet_test);

TYPED_TEST_P(semi_alphabet_test, concept_check)
{
    EXPECT_TRUE(bio::semialphabet<TypeParam>);
    EXPECT_TRUE(bio::semialphabet<TypeParam &>);
    EXPECT_TRUE(bio::semialphabet<TypeParam const>);
    EXPECT_TRUE(bio::semialphabet<TypeParam const &>);

    EXPECT_TRUE(bio::writable_semialphabet<TypeParam>);
    EXPECT_TRUE(bio::writable_semialphabet<TypeParam &>);
    EXPECT_FALSE(bio::writable_semialphabet<TypeParam const>);
    EXPECT_FALSE(bio::writable_semialphabet<TypeParam const &>);
}

TYPED_TEST_P(semi_alphabet_test, type_properties)
{
    // It is highly recommended that non-reference types that model this concept, also model:
    EXPECT_TRUE((std::regular<TypeParam>));
    EXPECT_TRUE((bio::trivially_copyable<TypeParam>));
    EXPECT_TRUE((bio::standard_layout<TypeParam>));
}

TYPED_TEST_P(semi_alphabet_test, alphabet_size)
{
    EXPECT_GT(bio::alphabet_size<TypeParam>, 0u);
}

TYPED_TEST_P(semi_alphabet_test, default_value_constructor)
{
    EXPECT_TRUE((std::is_nothrow_default_constructible_v<TypeParam>));
}

TYPED_TEST_P(semi_alphabet_test, assign_rank_to)
{
    // this double checks the value initialisation
    EXPECT_EQ((bio::assign_rank_to(0, TypeParam{})), TypeParam{});

    TypeParam t0;
    for (size_t i = 0u; i < bio::alphabet_size<TypeParam> && i < maximum_iterations; ++i)
        bio::assign_rank_to(i, t0);

    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_rank_to(0, t0)), TypeParam &>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::assign_rank_to(0, TypeParam{})), TypeParam>));
}

TYPED_TEST_P(semi_alphabet_test, to_rank)
{
    // this double checks the value initialisation
    EXPECT_EQ(bio::to_rank(TypeParam{}), 0u);

    TypeParam t0;
    for (size_t i = 0; i < bio::alphabet_size<TypeParam> && i < maximum_iterations; ++i)
        EXPECT_EQ((bio::to_rank(bio::assign_rank_to(i, t0))), i);

    EXPECT_TRUE((std::is_same_v<decltype(bio::to_rank(t0)), bio::alphabet_rank_t<TypeParam>>));
}

TYPED_TEST_P(semi_alphabet_test, copy_constructor)
{
    // the module operation ensures that the result is within the valid rank range;
    // it will be in the most cases 1 except for alphabets like bio::gap where it will be 0
    constexpr bio::alphabet_rank_t<TypeParam> rank = 1 % bio::alphabet_size<TypeParam>;
    TypeParam t1;
    bio::assign_rank_to(rank, t1);
    TypeParam t2{t1};
    TypeParam t3(t1);
    EXPECT_EQ(t1, t2);
    EXPECT_EQ(t2, t3);
}

TYPED_TEST_P(semi_alphabet_test, move_constructor)
{
    constexpr bio::alphabet_rank_t<TypeParam> rank = 1 % bio::alphabet_size<TypeParam>;
    TypeParam t0;
    bio::assign_rank_to(rank, t0);
    TypeParam t1{t0};

    TypeParam t2{std::move(t1)};
    EXPECT_EQ(t2, t0);
    TypeParam t3(std::move(t2));
    EXPECT_EQ(t3, t0);
}

TYPED_TEST_P(semi_alphabet_test, copy_assignment)
{
    constexpr bio::alphabet_rank_t<TypeParam> rank = 1 % bio::alphabet_size<TypeParam>;
    TypeParam t1;
    bio::assign_rank_to(rank, t1);
    TypeParam t2;
    t2 = t1;
    EXPECT_EQ(t1, t2);
}

TYPED_TEST_P(semi_alphabet_test, move_assignment)
{
    constexpr bio::alphabet_rank_t<TypeParam> rank = 1 % bio::alphabet_size<TypeParam>;
    TypeParam t0;
    bio::assign_rank_to(rank, t0);
    TypeParam t1{t0};
    TypeParam t2;
    TypeParam t3;
    t2 = std::move(t1);
    EXPECT_EQ(t2, t0);
    t3 = std::move(t2);
    EXPECT_EQ(t3, t0);
}

TYPED_TEST_P(semi_alphabet_test, swap)
{
    constexpr bio::alphabet_rank_t<TypeParam> rank = 1 % bio::alphabet_size<TypeParam>;
    TypeParam t0;
    bio::assign_rank_to(rank, t0);
    TypeParam t1{t0};
    TypeParam t2{};
    TypeParam t3{};

    std::swap(t1, t2);
    EXPECT_EQ(t2, t0);
    EXPECT_EQ(t1, t3);
}

TYPED_TEST_P(semi_alphabet_test, comparison_operators)
{
    TypeParam t0{};
    TypeParam t1{};

    bio::assign_rank_to(0, t0);
    bio::assign_rank_to(1 % bio::alphabet_size<TypeParam>, t1);

    EXPECT_EQ(t0, t0);
    EXPECT_LE(t0, t1);
    EXPECT_LE(t1, t1);
    EXPECT_EQ(t1, t1);
    EXPECT_GE(t1, t1);
    EXPECT_GE(t1, t0);

    if constexpr (bio::alphabet_size<TypeParam> == 1)
    {
        EXPECT_EQ(t0, t1);
    }
    else
    {
        EXPECT_LT(t0, t1);
        EXPECT_NE(t0, t1);
        EXPECT_GT(t1, t0);
    }
}

REGISTER_TYPED_TEST_SUITE_P(semi_alphabet_test,
                            concept_check,
                            type_properties,
                            alphabet_size,
                            default_value_constructor,
                            assign_rank_to,
                            to_rank,
                            copy_constructor,
                            move_constructor,
                            copy_assignment,
                            move_assignment,
                            swap,
                            comparison_operators);
