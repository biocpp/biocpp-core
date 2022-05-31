// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/sam_dna16.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using bio::operator""_sam_dna16;

// ------------------------------------------------------------------
// sam_dna16 alphabet
// ------------------------------------------------------------------

INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, alphabet, bio::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, semi_alphabet_test, bio::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, alphabet_constexpr, bio::sam_dna16, );
INSTANTIATE_TYPED_TEST_SUITE_P(sam_dna16, semi_alphabet_constexpr, bio::sam_dna16, );

// nucleotide test: (because the complement is not bijective for sam_dna16 we need to test it manually)
TEST(sam_dna16, nucleotide)
{
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::sam_dna16>);
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::sam_dna16 &>);

    EXPECT_EQ(bio::complement('='_sam_dna16), 'N'_sam_dna16);
    EXPECT_EQ(bio::complement('A'_sam_dna16), 'T'_sam_dna16);
    EXPECT_EQ(bio::complement('C'_sam_dna16), 'G'_sam_dna16);
    EXPECT_EQ(bio::complement('M'_sam_dna16), 'K'_sam_dna16);
    EXPECT_EQ(bio::complement('G'_sam_dna16), 'C'_sam_dna16);
    EXPECT_EQ(bio::complement('R'_sam_dna16), 'Y'_sam_dna16);
    EXPECT_EQ(bio::complement('S'_sam_dna16), 'S'_sam_dna16);
    EXPECT_EQ(bio::complement('V'_sam_dna16), 'B'_sam_dna16);
    EXPECT_EQ(bio::complement('T'_sam_dna16), 'A'_sam_dna16);
    EXPECT_EQ(bio::complement('W'_sam_dna16), 'W'_sam_dna16);
    EXPECT_EQ(bio::complement('Y'_sam_dna16), 'R'_sam_dna16);
    EXPECT_EQ(bio::complement('H'_sam_dna16), 'D'_sam_dna16);
    EXPECT_EQ(bio::complement('K'_sam_dna16), 'M'_sam_dna16);
    EXPECT_EQ(bio::complement('D'_sam_dna16), 'H'_sam_dna16);
    EXPECT_EQ(bio::complement('B'_sam_dna16), 'V'_sam_dna16);
    EXPECT_EQ(bio::complement('N'_sam_dna16), 'N'_sam_dna16);
}

TEST(sam_dna16, to_char_assign_char)
{
    using rank_t = bio::alphabet_rank_t<bio::sam_dna16>;
    for (rank_t rank = 0; rank < bio::alphabet_size<bio::sam_dna16>; ++rank)
    {
        char chr = bio::to_char(bio::assign_rank_to(rank, bio::sam_dna16{}));
        EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char(chr)), chr);
    }

    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('a')), 'A');
    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('c')), 'C');
    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('g')), 'G');
    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('t')), 'T');

    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::to_char(bio::sam_dna16{}.assign_char('!')), 'N');
}

TEST(sam_dna16, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_sam_dna16), 'A');
    EXPECT_EQ(bio::to_char('C'_sam_dna16), 'C');
    EXPECT_EQ(bio::to_char('G'_sam_dna16), 'G');

    EXPECT_EQ(bio::to_char('U'_sam_dna16), 'T');
    EXPECT_EQ(bio::to_char('T'_sam_dna16), 'T');

    EXPECT_EQ(bio::to_char('R'_sam_dna16), 'R');
    EXPECT_EQ(bio::to_char('Y'_sam_dna16), 'Y');
    EXPECT_EQ(bio::to_char('S'_sam_dna16), 'S');
    EXPECT_EQ(bio::to_char('W'_sam_dna16), 'W');
    EXPECT_EQ(bio::to_char('K'_sam_dna16), 'K');
    EXPECT_EQ(bio::to_char('M'_sam_dna16), 'M');
    EXPECT_EQ(bio::to_char('B'_sam_dna16), 'B');
    EXPECT_EQ(bio::to_char('D'_sam_dna16), 'D');
    EXPECT_EQ(bio::to_char('H'_sam_dna16), 'H');
    EXPECT_EQ(bio::to_char('V'_sam_dna16), 'V');

    EXPECT_EQ(bio::to_char('='_sam_dna16), '=');

    EXPECT_EQ(bio::to_char('N'_sam_dna16), 'N');
    EXPECT_EQ(bio::to_char('!'_sam_dna16), 'N');
}

TEST(sam_dna16, string_literal)
{
    bio::sam_dna16_vector v;
    v.resize(5, 'A'_sam_dna16);
    EXPECT_EQ(v, "AAAAA"_sam_dna16);

    std::vector<bio::sam_dna16> w{'A'_sam_dna16,
                                     '='_sam_dna16,
                                     'G'_sam_dna16,
                                     'T'_sam_dna16,
                                     'U'_sam_dna16,
                                     'N'_sam_dna16};
    EXPECT_EQ(w, "A=GTTN"_sam_dna16);
}

TEST(sam_dna16, char_is_valid)
{
    constexpr auto validator = bio::is_char<'A'> || bio::is_char<'C'> || bio::is_char<'G'> ||
                               bio::is_char<'T'> || bio::is_char<'U'> || bio::is_char<'a'> ||
                               bio::is_char<'c'> || bio::is_char<'g'> || bio::is_char<'t'> ||
                               bio::is_char<'u'> || bio::is_char<'N'> || bio::is_char<'n'> ||
                               bio::is_char<'R'> || bio::is_char<'Y'> || bio::is_char<'S'> ||
                               bio::is_char<'W'> || bio::is_char<'K'> || bio::is_char<'M'> ||
                               bio::is_char<'B'> || bio::is_char<'D'> || bio::is_char<'H'> ||
                               bio::is_char<'V'> || bio::is_char<'r'> || bio::is_char<'y'> ||
                               bio::is_char<'s'> || bio::is_char<'w'> || bio::is_char<'k'> ||
                               bio::is_char<'m'> || bio::is_char<'b'> || bio::is_char<'d'> ||
                               bio::is_char<'h'> || bio::is_char<'v'> || bio::is_char<'='>;

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::sam_dna16::char_is_valid(c), validator(c));
}
