// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/exception.hpp>
#include <bio/meta/concept/core_language.hpp>

template <typename T>
struct proxy_fixture : ::testing::Test
{
    /* needs to be specialised for ever type that uses this test!
     *
     * Must provide as public members:
     *
     * T default_init;
     * T t0;
     * T t1;
     *
     */
};

constexpr size_t maximum_iterations_ = 65536u;

TYPED_TEST_SUITE_P(proxy_fixture);

TYPED_TEST_P(proxy_fixture, concept_check)
{
    EXPECT_TRUE(bio::alphabet::semialphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::semialphabet<TypeParam &>);
    EXPECT_TRUE(bio::alphabet::semialphabet<TypeParam const>);
    EXPECT_TRUE(bio::alphabet::semialphabet<TypeParam const &>);

    EXPECT_TRUE(bio::alphabet::writable_semialphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::writable_semialphabet<TypeParam &>);
    // proxies ARE const-assignable
    EXPECT_TRUE(bio::alphabet::writable_semialphabet<TypeParam const>);
    EXPECT_TRUE(bio::alphabet::writable_semialphabet<TypeParam const &>);

    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam &>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam const>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam const &>);

    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam &>);
    // proxies ARE const-assignable
    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam const>);
    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam const &>);
}

TYPED_TEST_P(proxy_fixture, type_properties)
{
    // It is highly recommended that non-reference types that model this concept, also model:
    EXPECT_TRUE((bio::meta::standard_layout<TypeParam>));
    // not regular because not default-constructible
    //     EXPECT_TRUE((std::regular<TypeParam>));
    // usually not trivially copyable because of "assign-through"
    //     EXPECT_TRUE((bio::meta::trivially_copyable<TypeParam>));
}

TYPED_TEST_P(proxy_fixture, alphabet_size)
{
    EXPECT_GT(bio::alphabet::size<TypeParam>, 0u);
}

TYPED_TEST_P(proxy_fixture, assign_rank_to)
{
    EXPECT_EQ((bio::alphabet::assign_rank_to(0, this->t0)), this->default_init);

    for (size_t i = 0u; i < bio::alphabet::size<TypeParam> && i < maximum_iterations_; ++i)
        bio::alphabet::assign_rank_to(i, this->t0);

    EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_rank_to(0, this->t0)), TypeParam &>));
    EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_rank_to(0, std::move(this->t0))), TypeParam>));
}

TYPED_TEST_P(proxy_fixture, to_rank)
{
    EXPECT_EQ(bio::alphabet::to_rank(this->default_init), 0u);

    for (size_t i = 0; i < bio::alphabet::size<TypeParam> && i < maximum_iterations_; ++i)
        EXPECT_EQ((bio::alphabet::to_rank(bio::alphabet::assign_rank_to(i, this->t0))), i);

    EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::to_rank(this->t0)), bio::alphabet::rank_t<TypeParam>>));
}

TYPED_TEST_P(proxy_fixture, copy_constructor)
{
    // the module operation ensures that the result is within the valid rank range;
    // it will be in the most cases 1 except for alphabets like bio::alphabet::gap where it will be 0
    constexpr bio::alphabet::rank_t<TypeParam> rank = 1 % bio::alphabet::size<TypeParam>;
    bio::alphabet::assign_rank_to(rank, this->t1);
    TypeParam t2{this->t1};
    TypeParam t3(this->t1);
    EXPECT_EQ(this->t1, t2);
    EXPECT_EQ(t2, t3);
}

TYPED_TEST_P(proxy_fixture, move_constructor)
{
    constexpr bio::alphabet::rank_t<TypeParam> rank = 1 % bio::alphabet::size<TypeParam>;
    bio::alphabet::assign_rank_to(rank, this->t0);
    bio::alphabet::assign_rank_to(rank, this->t1);

    TypeParam t2{std::move(this->t1)};
    EXPECT_EQ(t2, this->t0);
    TypeParam t3(std::move(t2));
    EXPECT_EQ(t3, this->t0);
}

TYPED_TEST_P(proxy_fixture, copy_assignment)
{
    constexpr bio::alphabet::rank_t<TypeParam> rank = 1 % bio::alphabet::size<TypeParam>;
    bio::alphabet::assign_rank_to(rank, this->t1);
    TypeParam t2{this->default_init};
    EXPECT_NE(this->t1, t2);

    t2 = this->t1;
    EXPECT_EQ(this->t1, t2);
}

TYPED_TEST_P(proxy_fixture, move_assignment)
{
    constexpr bio::alphabet::rank_t<TypeParam> rank = 1 % bio::alphabet::size<TypeParam>;
    bio::alphabet::assign_rank_to(rank, this->t1);
    TypeParam t2{this->default_init};
    EXPECT_NE(this->t1, t2);

    t2 = this->t1;
    EXPECT_EQ(this->t1, t2);
}

TYPED_TEST_P(proxy_fixture, swap_)
{
    constexpr bio::alphabet::rank_t<TypeParam> rank = 1 % bio::alphabet::size<TypeParam>;
    bio::alphabet::assign_rank_to(rank, this->t1);
    EXPECT_EQ(this->t0.to_rank(), 0ull);
    EXPECT_EQ(this->t1.to_rank(), 1ull);

    using std::swap;
    swap(this->t0, this->t1);
    EXPECT_EQ(this->t0.to_rank(), 1ull);
    EXPECT_EQ(this->t1.to_rank(), 0ull);
}

TYPED_TEST_P(proxy_fixture, comparison_operators)
{
    bio::alphabet::assign_rank_to(0, this->t0);
    bio::alphabet::assign_rank_to(1 % bio::alphabet::size<TypeParam>, this->t1);

    EXPECT_EQ(this->t0, this->t0);
    EXPECT_LE(this->t0, this->t1);
    EXPECT_LE(this->t1, this->t1);
    EXPECT_EQ(this->t1, this->t1);
    EXPECT_GE(this->t1, this->t1);
    EXPECT_GE(this->t1, this->t0);

    if constexpr (bio::alphabet::size<TypeParam> == 1)
    {
        EXPECT_EQ(this->t0, this->t1);
    }
    else
    {
        EXPECT_LT(this->t0, this->t1);
        EXPECT_NE(this->t0, this->t1);
        EXPECT_GT(this->t1, this->t0);
    }
}

TYPED_TEST_P(proxy_fixture, assign_char_to)
{
    using char_t = bio::alphabet::char_t<TypeParam>;
    if constexpr (std::integral<char_t>)
    {
        char_t i = std::numeric_limits<char_t>::min();
        char_t j = std::numeric_limits<char_t>::max();

        for (size_t k = 0; i < j && k < maximum_iterations_; ++i, ++k)
            bio::alphabet::assign_char_to(i, this->t0);

        EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_char_to(0, this->t0)), TypeParam &>));
        EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_char_to(0, std::declval<TypeParam>())), TypeParam>));
    }
}

TYPED_TEST_P(proxy_fixture, char_is_valid_for) // only test negative example for most; more inside specialised tests
{
    // includes most of our alphabets, but not the adaptations!
    if constexpr (bio::alphabet::size<TypeParam> < 255)
    {
        EXPECT_FALSE((bio::alphabet::char_is_valid_for<TypeParam>(0))); // for none of our alphabets char{0} is valid
    }
}

TYPED_TEST_P(proxy_fixture, assign_char_strictly_to)
{
    using char_t = bio::alphabet::char_t<TypeParam>;
    if constexpr (std::integral<char_t>)
    {
        char_t i = std::numeric_limits<char_t>::min();
        char_t j = std::numeric_limits<char_t>::max();

        for (size_t k = 0; i < j && k < maximum_iterations_; ++i, ++k)
        {
            if (bio::alphabet::char_is_valid_for<TypeParam>(i))
                EXPECT_NO_THROW(bio::alphabet::assign_char_strictly_to(i, this->t0));
            else
                EXPECT_THROW(bio::alphabet::assign_char_strictly_to(i, this->t0),
                             bio::alphabet::invalid_char_assignment);
        }
    }
}

TYPED_TEST_P(proxy_fixture, to_char)
{
    EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::to_char(this->t0)), bio::alphabet::char_t<TypeParam>>));

    // more elaborate tests are done in specific alphabets
}

REGISTER_TYPED_TEST_SUITE_P(proxy_fixture,
                            concept_check,
                            type_properties,
                            alphabet_size,
                            assign_rank_to,
                            to_rank,
                            copy_constructor,
                            move_constructor,
                            copy_assignment,
                            move_assignment,
                            swap_,
                            comparison_operators,
                            assign_char_to,
                            char_is_valid_for,
                            assign_char_strictly_to,
                            to_char);
