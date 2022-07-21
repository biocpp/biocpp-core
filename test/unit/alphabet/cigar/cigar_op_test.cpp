// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/cigar/cigar_op.hpp>
#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, alphabet, bio::alphabet::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, semi_alphabet_test, bio::alphabet::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, alphabet_constexpr, bio::alphabet::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, semi_alphabet_constexpr, bio::alphabet::cigar_op, );

TEST(cigar_op, to_char_assign_char)
{
    for (char chr : std::string{"MDISHNPX="})
        EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{}.assign_char(chr)), chr);
}

TEST(cigar_op, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'M'_cigar_op}), 'M');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'D'_cigar_op}), 'D');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'I'_cigar_op}), 'I');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'S'_cigar_op}), 'S');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'H'_cigar_op}), 'H');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'N'_cigar_op}), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'P'_cigar_op}), 'P');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'X'_cigar_op}), 'X');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::cigar_op{'='_cigar_op}), '=');
}

TEST(cigar_op, assign_char_strictly_to)
{
    EXPECT_THROW(bio::alphabet::assign_char_strictly_to('A', bio::alphabet::cigar_op{}),
                 bio::alphabet::invalid_char_assignment);
}
