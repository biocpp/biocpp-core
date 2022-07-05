// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/sam_dna16.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using bio::alphabet::operator""_sam_dna16;

// ------------------------------------------------------------------
// sam_dna16 alphabet
// ------------------------------------------------------------------

INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, alphabet, bio::alphabet::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, semi_alphabet_test, bio::alphabet::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, alphabet_constexpr, bio::alphabet::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, semi_alphabet_constexpr, bio::alphabet::sam_dna16, );

// nucleotide test: (because the complement is not bijective for sam_dna16 we need to test it manually)
TEST(sam_dna16, nucleotide)
{
    EXPECT_TRUE(bio::alphabet::nucleotide_alphabet<bio::alphabet::sam_dna16>);
    EXPECT_TRUE(bio::alphabet::nucleotide_alphabet<bio::alphabet::sam_dna16 &>);

    EXPECT_EQ(bio::alphabet::complement('='_sam_dna16), 'N'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('A'_sam_dna16), 'T'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('C'_sam_dna16), 'G'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('M'_sam_dna16), 'K'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('G'_sam_dna16), 'C'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('R'_sam_dna16), 'Y'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('S'_sam_dna16), 'S'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('V'_sam_dna16), 'B'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('T'_sam_dna16), 'A'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('W'_sam_dna16), 'W'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('Y'_sam_dna16), 'R'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('H'_sam_dna16), 'D'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('K'_sam_dna16), 'M'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('D'_sam_dna16), 'H'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('B'_sam_dna16), 'V'_sam_dna16);
    EXPECT_EQ(bio::alphabet::complement('N'_sam_dna16), 'N'_sam_dna16);
}

TEST(sam_dna16, to_char_assign_char)
{
    using rank_t = bio::alphabet::alphabet_rank_t<bio::alphabet::sam_dna16>;
    for (rank_t rank = 0; rank < bio::alphabet::alphabet_size<bio::alphabet::sam_dna16>; ++rank)
    {
        char chr = bio::alphabet::to_char(bio::alphabet::assign_rank_to(rank, bio::alphabet::sam_dna16{}));
        EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char(chr)), chr);
    }

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('a')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('c')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('g')), 'G');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('t')), 'T');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::sam_dna16{}.assign_char('!')), 'N');
}

TEST(sam_dna16, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_sam_dna16), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_sam_dna16), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_sam_dna16), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_sam_dna16), 'T');
    EXPECT_EQ(bio::alphabet::to_char('T'_sam_dna16), 'T');

    EXPECT_EQ(bio::alphabet::to_char('R'_sam_dna16), 'R');
    EXPECT_EQ(bio::alphabet::to_char('Y'_sam_dna16), 'Y');
    EXPECT_EQ(bio::alphabet::to_char('S'_sam_dna16), 'S');
    EXPECT_EQ(bio::alphabet::to_char('W'_sam_dna16), 'W');
    EXPECT_EQ(bio::alphabet::to_char('K'_sam_dna16), 'K');
    EXPECT_EQ(bio::alphabet::to_char('M'_sam_dna16), 'M');
    EXPECT_EQ(bio::alphabet::to_char('B'_sam_dna16), 'B');
    EXPECT_EQ(bio::alphabet::to_char('D'_sam_dna16), 'D');
    EXPECT_EQ(bio::alphabet::to_char('H'_sam_dna16), 'H');
    EXPECT_EQ(bio::alphabet::to_char('V'_sam_dna16), 'V');

    EXPECT_EQ(bio::alphabet::to_char('='_sam_dna16), '=');

    EXPECT_EQ(bio::alphabet::to_char('N'_sam_dna16), 'N');
    EXPECT_EQ(bio::alphabet::to_char('!'_sam_dna16), 'N');
}

TEST(sam_dna16, string_literal)
{
    bio::alphabet::sam_dna16_vector v;
    v.resize(5, 'A'_sam_dna16);
    EXPECT_EQ(v, "AAAAA"_sam_dna16);

    std::vector<bio::alphabet::sam_dna16> w{'A'_sam_dna16,
                                            '='_sam_dna16,
                                            'G'_sam_dna16,
                                            'T'_sam_dna16,
                                            'U'_sam_dna16,
                                            'N'_sam_dna16};
    EXPECT_EQ(w, "A=GTTN"_sam_dna16);
}

TEST(sam_dna16, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u' || c == 'N' || c == 'n' || c == 'R' || c == 'Y' || c == 'S' || c == 'W' ||
               c == 'K' || c == 'M' || c == 'B' || c == 'D' || c == 'H' || c == 'V' || c == 'r' || c == 'y' ||
               c == 's' || c == 'w' || c == 'k' || c == 'm' || c == 'b' || c == 'd' || c == 'h' || c == 'v' || c == '=';
    };

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::sam_dna16::char_is_valid(c), validator(c));
}
