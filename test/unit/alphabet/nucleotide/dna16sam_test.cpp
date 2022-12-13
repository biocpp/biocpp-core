// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna16sam.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using namespace bio::alphabet::literals;

// ------------------------------------------------------------------
// dna16sam alphabet
// ------------------------------------------------------------------

INSTANTIATE_TYPED_TEST_SUITE_P(dna16sam, alphabet, bio::alphabet::dna16sam, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna16sam, semi_alphabet_test, bio::alphabet::dna16sam, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna16sam, alphabet_constexpr, bio::alphabet::dna16sam, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna16sam, semi_alphabet_constexpr, bio::alphabet::dna16sam, );

// nucleotide test: (because the complement is not bijective for dna16sam we need to test it manually)
TEST(dna16sam, nucleotide)
{
    EXPECT_TRUE(bio::alphabet::nucleotide<bio::alphabet::dna16sam>);
    EXPECT_TRUE(bio::alphabet::nucleotide<bio::alphabet::dna16sam &>);

    EXPECT_EQ(bio::alphabet::complement('='_dna16sam), 'N'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('A'_dna16sam), 'T'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('C'_dna16sam), 'G'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('M'_dna16sam), 'K'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('G'_dna16sam), 'C'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('R'_dna16sam), 'Y'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('S'_dna16sam), 'S'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('V'_dna16sam), 'B'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('T'_dna16sam), 'A'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('W'_dna16sam), 'W'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('Y'_dna16sam), 'R'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('H'_dna16sam), 'D'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('K'_dna16sam), 'M'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('D'_dna16sam), 'H'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('B'_dna16sam), 'V'_dna16sam);
    EXPECT_EQ(bio::alphabet::complement('N'_dna16sam), 'N'_dna16sam);
}

TEST(dna16sam, to_char_assign_char)
{
    using rank_t = bio::alphabet::rank_t<bio::alphabet::dna16sam>;
    for (rank_t rank = 0; rank < bio::alphabet::size<bio::alphabet::dna16sam>; ++rank)
    {
        char chr = bio::alphabet::to_char(bio::alphabet::assign_rank_to(rank, bio::alphabet::dna16sam{}));
        EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char(chr)), chr);
    }

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('a')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('c')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('g')), 'G');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('t')), 'T');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::dna16sam{}.assign_char('!')), 'N');
}

TEST(dna16sam, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_dna16sam), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_dna16sam), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_dna16sam), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_dna16sam), 'T');
    EXPECT_EQ(bio::alphabet::to_char('T'_dna16sam), 'T');

    EXPECT_EQ(bio::alphabet::to_char('R'_dna16sam), 'R');
    EXPECT_EQ(bio::alphabet::to_char('Y'_dna16sam), 'Y');
    EXPECT_EQ(bio::alphabet::to_char('S'_dna16sam), 'S');
    EXPECT_EQ(bio::alphabet::to_char('W'_dna16sam), 'W');
    EXPECT_EQ(bio::alphabet::to_char('K'_dna16sam), 'K');
    EXPECT_EQ(bio::alphabet::to_char('M'_dna16sam), 'M');
    EXPECT_EQ(bio::alphabet::to_char('B'_dna16sam), 'B');
    EXPECT_EQ(bio::alphabet::to_char('D'_dna16sam), 'D');
    EXPECT_EQ(bio::alphabet::to_char('H'_dna16sam), 'H');
    EXPECT_EQ(bio::alphabet::to_char('V'_dna16sam), 'V');

    EXPECT_EQ(bio::alphabet::to_char('='_dna16sam), '=');

    EXPECT_EQ(bio::alphabet::to_char('N'_dna16sam), 'N');
}

TEST(dna16sam, string_literal)
{
    std::vector<bio::alphabet::dna16sam> v;
    v.resize(5, 'A'_dna16sam);
    EXPECT_EQ(v, "AAAAA"_dna16sam);

    std::vector<bio::alphabet::dna16sam> w{'A'_dna16sam,
                                           '='_dna16sam,
                                           'G'_dna16sam,
                                           'T'_dna16sam,
                                           'U'_dna16sam,
                                           'N'_dna16sam};
    EXPECT_EQ(w, "A=GTTN"_dna16sam);
}

TEST(dna16sam, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u' || c == 'N' || c == 'n' || c == 'R' || c == 'Y' || c == 'S' || c == 'W' ||
               c == 'K' || c == 'M' || c == 'B' || c == 'D' || c == 'H' || c == 'V' || c == 'r' || c == 'y' ||
               c == 's' || c == 'w' || c == 'k' || c == 'm' || c == 'b' || c == 'd' || c == 'h' || c == 'v' || c == '=';
    };

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::dna16sam::char_is_valid(c), validator(c));
}
