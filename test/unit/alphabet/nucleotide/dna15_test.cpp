// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using bio::operator""_dna15;

INSTANTIATE_TYPED_TEST_SUITE_P(dna15, alphabet, bio::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, semi_alphabet_test, bio::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, alphabet_constexpr, bio::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, semi_alphabet_constexpr, bio::dna15, );
INSTANTIATE_TYPED_TEST_SUITE_P(dna15, nucleotide, bio::dna15, );

TEST(dna15, to_char_assign_char)
{
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('U')), 'T');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('T')), 'T');

    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('R')), 'R');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('Y')), 'Y');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('S')), 'S');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('W')), 'W');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('K')), 'K');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('M')), 'M');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('B')), 'B');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('D')), 'D');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('H')), 'H');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('V')), 'V');

    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::to_char(bio::dna15{}.assign_char('!')), 'N');
}

TEST(dna15, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_dna15), 'A');
    EXPECT_EQ(bio::to_char('C'_dna15), 'C');
    EXPECT_EQ(bio::to_char('G'_dna15), 'G');

    EXPECT_EQ(bio::to_char('U'_dna15), 'T');
    EXPECT_EQ(bio::to_char('T'_dna15), 'T');

    EXPECT_EQ(bio::to_char('R'_dna15), 'R');
    EXPECT_EQ(bio::to_char('Y'_dna15), 'Y');
    EXPECT_EQ(bio::to_char('S'_dna15), 'S');
    EXPECT_EQ(bio::to_char('W'_dna15), 'W');
    EXPECT_EQ(bio::to_char('K'_dna15), 'K');
    EXPECT_EQ(bio::to_char('M'_dna15), 'M');
    EXPECT_EQ(bio::to_char('B'_dna15), 'B');
    EXPECT_EQ(bio::to_char('D'_dna15), 'D');
    EXPECT_EQ(bio::to_char('H'_dna15), 'H');
    EXPECT_EQ(bio::to_char('V'_dna15), 'V');

    EXPECT_EQ(bio::to_char('N'_dna15), 'N');
    EXPECT_EQ(bio::to_char('!'_dna15), 'N');
}

TEST(dna15, string_literal)
{
    bio::dna15_vector v;
    v.resize(5, 'A'_dna15);
    EXPECT_EQ(v, "AAAAA"_dna15);

    std::vector<bio::dna15> w{'A'_dna15, 'C'_dna15, 'G'_dna15, 'T'_dna15, 'U'_dna15, 'N'_dna15};
    EXPECT_EQ(w, "ACGTTN"_dna15);
}

TEST(dna15, char_is_valid)
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
                               bio::is_char<'h'> || bio::is_char<'v'>;
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::dna15::char_is_valid(c), validator(c));
}
