// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna15.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(dna15, alphabet, bio::alphabet::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, semi_alphabet_test, bio::alphabet::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, alphabet_constexpr, bio::alphabet::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, semi_alphabet_constexpr, bio::alphabet::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, nucleotide, bio::alphabet::dna15, );

TEST(dna15, to_char_assign_char)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('R')), 'R');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('Y')), 'Y');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('S')), 'S');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('W')), 'W');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('K')), 'K');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('M')), 'M');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('B')), 'B');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('D')), 'D');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('H')), 'H');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('V')), 'V');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna15{}.assign_char('!')), 'N');
}

TEST(dna15, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_dna15), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_dna15), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_dna15), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_dna15), 'T');
    EXPECT_EQ(bio::alphabet::to_char('T'_dna15), 'T');

    EXPECT_EQ(bio::alphabet::to_char('R'_dna15), 'R');
    EXPECT_EQ(bio::alphabet::to_char('Y'_dna15), 'Y');
    EXPECT_EQ(bio::alphabet::to_char('S'_dna15), 'S');
    EXPECT_EQ(bio::alphabet::to_char('W'_dna15), 'W');
    EXPECT_EQ(bio::alphabet::to_char('K'_dna15), 'K');
    EXPECT_EQ(bio::alphabet::to_char('M'_dna15), 'M');
    EXPECT_EQ(bio::alphabet::to_char('B'_dna15), 'B');
    EXPECT_EQ(bio::alphabet::to_char('D'_dna15), 'D');
    EXPECT_EQ(bio::alphabet::to_char('H'_dna15), 'H');
    EXPECT_EQ(bio::alphabet::to_char('V'_dna15), 'V');

    EXPECT_EQ(bio::alphabet::to_char('N'_dna15), 'N');
}

TEST(dna15, string_literal)
{
    std::vector<bio::alphabet::dna15> v;
    v.resize(5, 'A'_dna15);
    EXPECT_EQ(v, "AAAAA"_dna15);

    std::vector<bio::alphabet::dna15> w{'A'_dna15, 'C'_dna15, 'G'_dna15, 'T'_dna15, 'U'_dna15, 'N'_dna15};
    EXPECT_EQ(w, "ACGTTN"_dna15);
}

TEST(dna15, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u' || c == 'N' || c == 'n' || c == 'R' || c == 'Y' || c == 'S' || c == 'W' ||
               c == 'K' || c == 'M' || c == 'B' || c == 'D' || c == 'H' || c == 'V' || c == 'r' || c == 'y' ||
               c == 's' || c == 'w' || c == 'k' || c == 'm' || c == 'b' || c == 'd' || c == 'h' || c == 'v';
    };
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::dna15::char_is_valid(c), validator(c));
}
