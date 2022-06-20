// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using bio::operator""_dna4;

INSTANTIATE_TYPED_TEST_SUITE_P(dna4, alphabet, bio::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, semi_alphabet_test, bio::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, alphabet_constexpr, bio::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, semi_alphabet_constexpr, bio::dna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna4, nucleotide, bio::dna4, );

TEST(dna4, to_char_assign_char)
{
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('R')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('Y')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('S')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('W')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('K')), 'G');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('M')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('B')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('D')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('H')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('V')), 'A');

    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('N')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna4{}.assign_char('!')), 'A');
}

TEST(dna4, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_dna4), 'A');
    EXPECT_EQ(bio::to_char('C'_dna4), 'C');
    EXPECT_EQ(bio::to_char('G'_dna4), 'G');

    EXPECT_EQ(bio::to_char('U'_dna4), 'T');
    EXPECT_EQ(bio::to_char('T'_dna4), 'T');

    EXPECT_EQ(bio::to_char('R'_dna4), 'A');
    EXPECT_EQ(bio::to_char('Y'_dna4), 'C');
    EXPECT_EQ(bio::to_char('S'_dna4), 'C');
    EXPECT_EQ(bio::to_char('W'_dna4), 'A');
    EXPECT_EQ(bio::to_char('K'_dna4), 'G');
    EXPECT_EQ(bio::to_char('M'_dna4), 'A');
    EXPECT_EQ(bio::to_char('B'_dna4), 'C');
    EXPECT_EQ(bio::to_char('D'_dna4), 'A');
    EXPECT_EQ(bio::to_char('H'_dna4), 'A');
    EXPECT_EQ(bio::to_char('V'_dna4), 'A');

    EXPECT_EQ(bio::to_char('N'_dna4), 'A');
    EXPECT_EQ(bio::to_char('!'_dna4), 'A');
}

TEST(dna4, string_literal)
{
    bio::dna4_vector v;
    v.resize(5, 'A'_dna4);
    EXPECT_EQ(v, "AAAAA"_dna4);

    std::vector<bio::dna4> w{'A'_dna4, 'C'_dna4, 'G'_dna4, 'T'_dna4, 'U'_dna4, 'N'_dna4};
    EXPECT_EQ(w, "ACGTTA"_dna4);
}

TEST(dna4, char_is_valid)
{
    constexpr auto validator = bio::is_char<'A'> || bio::is_char<'C'> || bio::is_char<'G'> || bio::is_char<'T'> ||
                               bio::is_char<'U'> || bio::is_char<'a'> || bio::is_char<'c'> || bio::is_char<'g'> ||
                               bio::is_char<'t'> || bio::is_char<'u'>;
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::dna4::char_is_valid(c), validator(c));
}
