// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/exception.hpp>

template <typename T>
using alphabet = ::testing::Test;

constexpr size_t max_iterations = 65536u;

TYPED_TEST_SUITE_P(alphabet);

TYPED_TEST_P(alphabet, concept_check)
{
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam &>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam const>);
    EXPECT_TRUE(bio::alphabet::alphabet<TypeParam const &>);

    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam>);
    EXPECT_TRUE(bio::alphabet::writable_alphabet<TypeParam &>);
    EXPECT_FALSE(bio::alphabet::writable_alphabet<TypeParam const>);
    EXPECT_FALSE(bio::alphabet::writable_alphabet<TypeParam const &>);
}

TYPED_TEST_P(alphabet, assign_char_to)
{
    using char_t = bio::alphabet::char_t<TypeParam>;
    if constexpr (std::integral<char_t>)
    {
        char_t i = std::numeric_limits<char_t>::min();
        char_t j = std::numeric_limits<char_t>::max();

        TypeParam t0;
        for (size_t k = 0; i < j && k < max_iterations; ++i, ++k)
            bio::alphabet::assign_char_to(i, t0);

        EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_char_to(0, t0)), TypeParam &>));
        EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::assign_char_to(0, TypeParam{})), TypeParam>));
    }
}

TYPED_TEST_P(alphabet, char_is_valid_for) // only test negative example for most; more inside specialised tests
{
    if constexpr (bio::alphabet::size<TypeParam> < 255) // includes most of our alphabets, but not the adaptations!
    {
        EXPECT_FALSE((bio::alphabet::char_is_valid_for<TypeParam>(0))); // for none of our alphabets char{0} is valid
    }
}

TYPED_TEST_P(alphabet, assign_char_strictly_to)
{
    using char_t = bio::alphabet::char_t<TypeParam>;
    if constexpr (std::integral<char_t>)
    {
        char_t i = std::numeric_limits<char_t>::min();
        char_t j = std::numeric_limits<char_t>::max();

        for (size_t k = 0; i < j && k < max_iterations; ++i, ++k)
        {
            if (bio::alphabet::char_is_valid_for<TypeParam>(i))
                EXPECT_NO_THROW(bio::alphabet::assign_char_strictly_to(i, TypeParam{}));
            else
                EXPECT_THROW(bio::alphabet::assign_char_strictly_to(i, TypeParam{}),
                             bio::alphabet::invalid_char_assignment);
        }
    }
}

TYPED_TEST_P(alphabet, to_char)
{
    TypeParam t0;
    EXPECT_TRUE((std::is_same_v<decltype(bio::alphabet::to_char(t0)), bio::alphabet::char_t<TypeParam>>));

    // more elaborate tests are done in specific alphabets
}

REGISTER_TYPED_TEST_SUITE_P(alphabet,
                            concept_check,
                            assign_char_to,
                            char_is_valid_for,
                            assign_char_strictly_to,
                            to_char);
