// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <sstream>

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/gap/gap.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(gap, alphabet, bio::alphabet::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, semi_alphabet_test, bio::alphabet::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, alphabet_constexpr, bio::alphabet::gap, );
INSTANTIATE_TYPED_TEST_SUITE_P(gap, semi_alphabet_constexpr, bio::alphabet::gap, );

TEST(gap_test, default_initialization)
{
    bio::alphabet::gap gap1;
    bio::alphabet::gap gap2{};
    bio::alphabet::gap gap3 = bio::alphabet::gap{};

    EXPECT_EQ(gap1.to_rank(), 0);
    EXPECT_EQ(gap2.to_rank(), 0);
    EXPECT_EQ(gap3.to_rank(), 0);
    EXPECT_EQ(gap1.to_char(), '-');
    EXPECT_EQ(gap2.to_char(), '-');
    EXPECT_EQ(gap3.to_char(), '-');
}

TEST(gap_test, relations)
{
    EXPECT_EQ(bio::alphabet::gap{}, bio::alphabet::gap{});
    EXPECT_LE(bio::alphabet::gap{}, bio::alphabet::gap{});
    EXPECT_GE(bio::alphabet::gap{}, bio::alphabet::gap{});
}

TEST(gap_test, assign_char)
{
    EXPECT_EQ(bio::alphabet::gap{}.assign_char('-'), bio::alphabet::gap{});
    EXPECT_EQ(bio::alphabet::gap{}.assign_char('x'), bio::alphabet::gap{});
}

TEST(gap_test, to_rank)
{
    EXPECT_EQ(bio::alphabet::gap{}.to_rank(), 0);
}

TEST(gap_test, assign_rank)
{
    EXPECT_EQ(bio::alphabet::gap{}.assign_rank(0), bio::alphabet::gap{});
    // EXPECT_EQ(bio::alphabet::gap{}.assign_rank(13), bio::alphabet::gap{});
}
