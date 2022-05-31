// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using bio::operator""_dna5;

INSTANTIATE_TYPED_TEST_SUITE_P(dna5, alphabet, bio::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, semi_alphabet_test, bio::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, alphabet_constexpr, bio::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, semi_alphabet_constexpr, bio::dna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna5, nucleotide, bio::dna5, );

TEST(dna5, to_char_assign_char)
{
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('R')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('Y')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('S')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('W')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('K')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('M')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('B')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('D')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('H')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('V')), 'N');

    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna5{}.assign_char('!')), 'N');
}

TEST(dna5, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_dna5), 'A');
    EXPECT_EQ(bio::to_char('C'_dna5), 'C');
    EXPECT_EQ(bio::to_char('G'_dna5), 'G');

    EXPECT_EQ(bio::to_char('U'_dna5), 'T');
    EXPECT_EQ(bio::to_char('T'_dna5), 'T');

    EXPECT_EQ(bio::to_char('R'_dna5), 'N');
    EXPECT_EQ(bio::to_char('Y'_dna5), 'N');
    EXPECT_EQ(bio::to_char('S'_dna5), 'N');
    EXPECT_EQ(bio::to_char('W'_dna5), 'N');
    EXPECT_EQ(bio::to_char('K'_dna5), 'N');
    EXPECT_EQ(bio::to_char('M'_dna5), 'N');
    EXPECT_EQ(bio::to_char('B'_dna5), 'N');
    EXPECT_EQ(bio::to_char('D'_dna5), 'N');
    EXPECT_EQ(bio::to_char('H'_dna5), 'N');
    EXPECT_EQ(bio::to_char('V'_dna5), 'N');

    EXPECT_EQ(bio::to_char('N'_dna5), 'N');
    EXPECT_EQ(bio::to_char('!'_dna5), 'N');
}

TEST(dna5, string_literal)
{
    bio::dna5_vector v;
    v.resize(5, 'A'_dna5);
    EXPECT_EQ(v, "AAAAA"_dna5);

    std::vector<bio::dna5> w{'A'_dna5, 'C'_dna5, 'G'_dna5, 'T'_dna5, 'U'_dna5, 'N'_dna5};
    EXPECT_EQ(w, "ACGTTN"_dna5);
}

TEST(dna5, char_is_valid)
{
    constexpr auto validator = bio::is_char<'A'> || bio::is_char<'C'> || bio::is_char<'G'> ||
                               bio::is_char<'T'> || bio::is_char<'U'> || bio::is_char<'a'> ||
                               bio::is_char<'c'> || bio::is_char<'g'> || bio::is_char<'t'> ||
                               bio::is_char<'u'> || bio::is_char<'N'> || bio::is_char<'n'>;
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::dna5::char_is_valid(c), validator(c));
}
