// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <sstream>

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/gap/gap.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(gap, alphabet, bio::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, semi_alphabet_test, bio::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, alphabet_constexpr, bio::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, semi_alphabet_constexpr, bio::gap, );

TEST(gap_test, default_initialization)
{
    bio::gap gap1;
    bio::gap gap2{};
    bio::gap gap3 = bio::gap{};

    EXPECT_EQ(gap1.to_rank(), 0);
    EXPECT_EQ(gap2.to_rank(), 0);
    EXPECT_EQ(gap3.to_rank(), 0);
    EXPECT_EQ(gap1.to_char(), '-');
    EXPECT_EQ(gap2.to_char(), '-');
    EXPECT_EQ(gap3.to_char(), '-');
}

TEST(gap_test, relations)
{
    EXPECT_EQ(bio::gap{}, bio::gap{});
    EXPECT_LE(bio::gap{}, bio::gap{});
    EXPECT_GE(bio::gap{}, bio::gap{});
}

TEST(gap_test, assign_char)
{
    EXPECT_EQ(bio::gap{}.assign_char('-'), bio::gap{});
    EXPECT_EQ(bio::gap{}.assign_char('x'), bio::gap{});
}

TEST(gap_test, to_rank)
{
    EXPECT_EQ(bio::gap{}.to_rank(), 0);
}

TEST(gap_test, assign_rank)
{
    EXPECT_EQ(bio::gap{}.assign_rank(0), bio::gap{});
    // EXPECT_EQ(bio::gap{}.assign_rank(13), bio::gap{});
}
