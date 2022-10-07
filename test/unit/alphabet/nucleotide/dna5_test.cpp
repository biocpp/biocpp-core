// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna5.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(dna5, alphabet, bio::alphabet::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, semi_alphabet_test, bio::alphabet::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, alphabet_constexpr, bio::alphabet::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, semi_alphabet_constexpr, bio::alphabet::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, nucleotide, bio::alphabet::dna5, );

TEST(dna5, to_char_assign_char)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('R')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('Y')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('S')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('W')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('K')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('M')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('B')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('D')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('H')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('V')), 'N');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna5{}.assign_char('!')), 'N');
}

TEST(dna5, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_dna5), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_dna5), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_dna5), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_dna5), 'T');
    EXPECT_EQ(bio::alphabet::to_char('T'_dna5), 'T');

    EXPECT_EQ(bio::alphabet::to_char('N'_dna5), 'N');
}

TEST(dna5, string_literal)
{
    std::vector<bio::alphabet::dna5> v;
    v.resize(5, 'A'_dna5);
    EXPECT_EQ(v, "AAAAA"_dna5);

    std::vector<bio::alphabet::dna5> w{'A'_dna5, 'C'_dna5, 'G'_dna5, 'T'_dna5, 'U'_dna5, 'N'_dna5};
    EXPECT_EQ(w, "ACGTTN"_dna5);
}

TEST(dna5, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u' || c == 'N' || c == 'n';
    };
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::dna5::char_is_valid(c), validator(c));
}
