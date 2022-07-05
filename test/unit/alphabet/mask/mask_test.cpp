// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/mask/mask.hpp>

#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(mask, semi_alphabet_test, bio::alphabet::mask, );
INSTANTIATE_TYPED_TEST_SUITE_P(mask, semi_alphabet_constexpr, bio::alphabet::mask, );

TEST(mask, assign_rank)
{
    // l-value
    bio::alphabet::mask lmask;
    EXPECT_EQ(lmask.assign_rank(1), bio::alphabet::mask::MASKED);
    EXPECT_TRUE(lmask.to_rank());
    EXPECT_EQ(lmask.assign_rank(0), bio::alphabet::mask::UNMASKED);
    EXPECT_FALSE(lmask.to_rank());
    EXPECT_EQ(lmask.assign_rank(true), bio::alphabet::mask::MASKED);
    EXPECT_EQ(lmask.assign_rank(false), bio::alphabet::mask::UNMASKED);

    // const l-value
    lmask.assign_rank(1);
    bio::alphabet::mask const clmask{lmask};
    EXPECT_TRUE(clmask.to_rank());

    // r-value
    bio::alphabet::mask rmask{lmask};
    EXPECT_EQ(std::move(rmask).to_rank(), lmask.to_rank());
    EXPECT_TRUE((std::is_same_v<decltype(std::move(rmask)), bio::alphabet::mask &&>));
    EXPECT_EQ(std::move(rmask).assign_rank(1), bio::alphabet::mask::MASKED);
    EXPECT_TRUE(std::move(rmask).to_rank());
    EXPECT_EQ(std::move(rmask).assign_rank(0), bio::alphabet::mask::UNMASKED);
    EXPECT_FALSE(std::move(rmask).to_rank());
    EXPECT_EQ(std::move(rmask).assign_rank(true), bio::alphabet::mask::MASKED);
    EXPECT_EQ(std::move(rmask).assign_rank(false), bio::alphabet::mask::UNMASKED);

    // const r-value
    bio::alphabet::mask const crmask{lmask};
    EXPECT_EQ(std::move(crmask).to_rank(), lmask.to_rank());
    EXPECT_TRUE((std::is_same_v<decltype(std::move(crmask)), bio::alphabet::mask const &&>));
}
