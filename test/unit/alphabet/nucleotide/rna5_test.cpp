// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/meta/char_operations/predicate.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using bio::operator""_rna5;

INSTANTIATE_TYPED_TEST_SUITE_P(rna5, alphabet, bio::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, semi_alphabet_test, bio::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, alphabet_constexpr, bio::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, semi_alphabet_constexpr, bio::rna5, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna5, nucleotide, bio::rna5, );

TEST(rna5, to_char_assign_char)
{
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('U')), 'U');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('T')), 'U');

    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('R')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('Y')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('S')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('W')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('K')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('M')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('B')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('D')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('H')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('V')), 'N');

    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('N')), 'N');
    EXPECT_EQ(bio::to_char(bio::rna5{}.assign_char('!')), 'N');
}

TEST(rna5, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_rna5), 'A');
    EXPECT_EQ(bio::to_char('C'_rna5), 'C');
    EXPECT_EQ(bio::to_char('G'_rna5), 'G');

    EXPECT_EQ(bio::to_char('U'_rna5), 'U');
    EXPECT_EQ(bio::to_char('T'_rna5), 'U');

    EXPECT_EQ(bio::to_char('R'_rna5), 'N');
    EXPECT_EQ(bio::to_char('Y'_rna5), 'N');
    EXPECT_EQ(bio::to_char('S'_rna5), 'N');
    EXPECT_EQ(bio::to_char('W'_rna5), 'N');
    EXPECT_EQ(bio::to_char('K'_rna5), 'N');
    EXPECT_EQ(bio::to_char('M'_rna5), 'N');
    EXPECT_EQ(bio::to_char('B'_rna5), 'N');
    EXPECT_EQ(bio::to_char('D'_rna5), 'N');
    EXPECT_EQ(bio::to_char('H'_rna5), 'N');
    EXPECT_EQ(bio::to_char('V'_rna5), 'N');

    EXPECT_EQ(bio::to_char('N'_rna5), 'N');
    EXPECT_EQ(bio::to_char('!'_rna5), 'N');
}

TEST(rna5, string_literal)
{
    bio::rna5_vector v;
    v.resize(5, 'A'_rna5);
    EXPECT_EQ(v, "AAAAA"_rna5);

    std::vector<bio::rna5> w{'A'_rna5, 'C'_rna5, 'G'_rna5, 'T'_rna5, 'U'_rna5, 'N'_rna5};
    EXPECT_EQ(w, "ACGUUN"_rna5);
}

TEST(rna5, char_is_valid)
{
    constexpr auto validator = bio::is_char<'A'> || bio::is_char<'C'> || bio::is_char<'G'> || bio::is_char<'T'> ||
                               bio::is_char<'U'> || bio::is_char<'a'> || bio::is_char<'c'> || bio::is_char<'g'> ||
                               bio::is_char<'t'> || bio::is_char<'u'> || bio::is_char<'N'> || bio::is_char<'n'>;
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::rna5::char_is_valid(c), validator(c));
}
