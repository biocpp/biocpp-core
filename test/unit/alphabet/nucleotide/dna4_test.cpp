// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna4.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(dna4, alphabet, bio::alphabet::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, semi_alphabet_test, bio::alphabet::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, alphabet_constexpr, bio::alphabet::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, semi_alphabet_constexpr, bio::alphabet::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, nucleotide, bio::alphabet::dna4, );

TEST(dna4, to_char_assign_char)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('R')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('Y')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('S')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('W')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('K')), 'G');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('M')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('B')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('D')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('H')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('V')), 'A');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('N')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna4{}.assign_char('!')), 'A');
}

TEST(dna4, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_dna4), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_dna4), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_dna4), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_dna4), 'T');
    EXPECT_EQ(bio::alphabet::to_char('T'_dna4), 'T');
}

TEST(dna4, string_literal)
{
    bio::alphabet::dna4_vector v;
    v.resize(5, 'A'_dna4);
    EXPECT_EQ(v, "AAAAA"_dna4);

    std::vector<bio::alphabet::dna4> w{'A'_dna4, 'C'_dna4, 'G'_dna4, 'T'_dna4, 'U'_dna4};
    EXPECT_EQ(w, "ACGTT"_dna4);
}

TEST(dna4, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u';
    };

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::dna4::char_is_valid(c), validator(c));
}
