// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include <bio/alphabet/cigar/cigar_op.hpp>

using bio::operator""_cigar_op;

INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, alphabet, bio::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, semi_alphabet_test, bio::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, alphabet_constexpr, bio::cigar_op, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar_op, semi_alphabet_constexpr, bio::cigar_op, );

using bio::operator""_cigar_op;

TEST(cigar_op, to_char_assign_char)
{
    for (char chr : std::string{"MDISHNPX="})
        EXPECT_EQ(bio::to_char(bio::cigar_op{}.assign_char(chr)), chr);
}

TEST(cigar_op, char_literal)
{
    EXPECT_EQ(bio::to_char(bio::cigar_op{'M'_cigar_op}), 'M');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'D'_cigar_op}), 'D');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'I'_cigar_op}), 'I');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'S'_cigar_op}), 'S');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'H'_cigar_op}), 'H');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'N'_cigar_op}), 'N');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'P'_cigar_op}), 'P');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'X'_cigar_op}), 'X');
    EXPECT_EQ(bio::to_char(bio::cigar_op{'='_cigar_op}), '=');
}

TEST(cigar_op, assign_char_strictly_to)
{
    EXPECT_THROW(bio::assign_char_strictly_to('A', bio::cigar_op{}), bio::invalid_char_assignment);
}
