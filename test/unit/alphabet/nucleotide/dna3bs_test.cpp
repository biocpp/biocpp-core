// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna3bs.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(dna3bs, alphabet, bio::dna3bs, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna3bs, semi_alphabet_test, bio::dna3bs, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna3bs, alphabet_constexpr, bio::dna3bs, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna3bs, semi_alphabet_constexpr, bio::dna3bs, );

using bio::operator""_dna3bs;

TEST(dna3bs, concept_check)
{
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::dna3bs>);
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::dna3bs &>);
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::dna3bs const>);
    EXPECT_TRUE(bio::nucleotide_alphabet<bio::dna3bs const &>);
}

TEST(dna3bs, complement)
{
    EXPECT_EQ(bio::complement(bio::dna3bs{}.assign_char('A')), bio::dna3bs{}.assign_char('T'));
    EXPECT_EQ(bio::complement(bio::dna3bs{}.assign_char('C')), bio::dna3bs{}.assign_char('A'));
    EXPECT_EQ(bio::complement(bio::dna3bs{}.assign_char('G')), bio::dna3bs{}.assign_char('T'));
    EXPECT_EQ(bio::complement(bio::dna3bs{}.assign_char('T')), bio::dna3bs{}.assign_char('A'));
}

TEST(dna3bs, to_char_assign_char)
{
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('C')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('R')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('Y')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('S')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('W')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('K')), 'G');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('M')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('B')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('D')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('H')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('V')), 'A');

    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('N')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna3bs{}.assign_char('!')), 'A');
}

TEST(dna3bs, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('C'_dna3bs), 'T');
    EXPECT_EQ(bio::to_char('G'_dna3bs), 'G');

    EXPECT_EQ(bio::to_char('U'_dna3bs), 'T');
    EXPECT_EQ(bio::to_char('T'_dna3bs), 'T');

    EXPECT_EQ(bio::to_char('R'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('Y'_dna3bs), 'T');
    EXPECT_EQ(bio::to_char('S'_dna3bs), 'T');
    EXPECT_EQ(bio::to_char('W'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('K'_dna3bs), 'G');
    EXPECT_EQ(bio::to_char('M'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('B'_dna3bs), 'T');
    EXPECT_EQ(bio::to_char('D'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('H'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('V'_dna3bs), 'A');

    EXPECT_EQ(bio::to_char('N'_dna3bs), 'A');
    EXPECT_EQ(bio::to_char('!'_dna3bs), 'A');
}

TEST(dna3bs, string_literal)
{
    bio::dna3bs_vector v;
    v.resize(5, 'A'_dna3bs);
    EXPECT_EQ(v, "AAAAA"_dna3bs);

    std::vector<bio::dna3bs> w{'A'_dna3bs, 'C'_dna3bs, 'G'_dna3bs, 'T'_dna3bs, 'U'_dna3bs, 'N'_dna3bs};
    EXPECT_EQ(w, "ATGTTA"_dna3bs);
}

TEST(dna3bs, char_is_valid)
{
    constexpr auto validator = bio::is_char<'A'> || bio::is_char<'G'> || bio::is_char<'T'>
                            || bio::is_char<'U'> || bio::is_char<'a'> || bio::is_char<'g'>
                            || bio::is_char<'t'> || bio::is_char<'u'>;
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::dna3bs::char_is_valid(c), validator(c));
}
