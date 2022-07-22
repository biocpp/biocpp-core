// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/aminoacid/aa20.hpp>
#include <bio/alphabet/mask/masked.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using masked_types =
  ::testing::Types<bio::alphabet::masked<bio::alphabet::dna4>, bio::alphabet::masked<bio::alphabet::dna5>>;

INSTANTIATE_TYPED_TEST_SUITE_P(masked, alphabet, masked_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(masked, semi_alphabet_test, masked_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(masked, alphabet_constexpr, masked_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(masked, semi_alphabet_constexpr, masked_types, );

using m_t = bio::alphabet::masked<bio::alphabet::dna4>;

TEST(masked_specific, size)
{
    EXPECT_EQ(bio::alphabet::size<m_t>, 8ull);
}

TEST(masked_specific, to_char)
{
    m_t         alph;
    std::string compare = "ACGTacgt";

    for (size_t i = 0; i < 8; ++i)
    {
        alph.assign_rank(i);
        EXPECT_EQ(alph.to_char(), compare[i]);
    }
}

TEST(masked_specific, assign_char)
{
    m_t         alph;
    std::string compare = "ACGTacgt";

    for (size_t i = 0; i < 8; ++i)
    {
        alph.assign_char(compare[i]);
        EXPECT_EQ(alph.to_rank(), i);
    }
}
