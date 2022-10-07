// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/rna4.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "nucleotide_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(rna4, alphabet, bio::alphabet::rna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna4, semi_alphabet_test, bio::alphabet::rna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna4, alphabet_constexpr, bio::alphabet::rna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna4, semi_alphabet_constexpr, bio::alphabet::rna4, );
INSTANTIATE_TYPED_TEST_SUITE_P(rna4, nucleotide, bio::alphabet::rna4, );

TEST(rna4, to_char_assign_char)
{
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('A')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('C')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('G')), 'G');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('U')), 'U');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('T')), 'U');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('R')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('Y')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('S')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('W')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('K')), 'G');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('M')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('B')), 'C');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('D')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('H')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('V')), 'A');

    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('N')), 'A');
    EXPECT_EQ(bio::alphabet::to_char(bio::alphabet::rna4{}.assign_char('!')), 'A');
}

TEST(rna4, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_rna4), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_rna4), 'C');
    EXPECT_EQ(bio::alphabet::to_char('G'_rna4), 'G');

    EXPECT_EQ(bio::alphabet::to_char('U'_rna4), 'U');
    EXPECT_EQ(bio::alphabet::to_char('T'_rna4), 'U');
}

TEST(rna4, string_literal)
{
    bio::alphabet::rna4_vector v;
    v.resize(5, 'A'_rna4);
    EXPECT_EQ(v, "AAAAA"_rna4);

    std::vector<bio::alphabet::rna4> w{'A'_rna4, 'C'_rna4, 'G'_rna4, 'T'_rna4, 'U'_rna4};
    EXPECT_EQ(w, "ACGUU"_rna4);
}

TEST(rna4, char_is_valid)
{
    constexpr auto validator = [](char const c)
    {
        return c == 'A' || c == 'C' || c == 'G' || c == 'T' || c == 'U' || c == 'a' || c == 'c' || c == 'g' ||
               c == 't' || c == 'u';
    };
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::alphabet::rna4::char_is_valid(c), validator(c));
}
