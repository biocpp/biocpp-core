// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/aminoacid/aa10murphy.hpp>
#include <bio/ranges/views/zip.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "aminoacid_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(aa10murphy, alphabet, bio::alphabet::aa10murphy, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa10murphy, semi_alphabet_test, bio::alphabet::aa10murphy, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa10murphy, alphabet_constexpr, bio::alphabet::aa10murphy, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa10murphy, semi_alphabet_constexpr, bio::alphabet::aa10murphy, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa10murphy, aminoacid, bio::alphabet::aa10murphy, );

TEST(aa10murphy, assign_char)
{
    std::vector<char> chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'a', 'b', 'c', 'd', 'e',
                            'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                            'X', 'Y', 'Z', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '*', '!'};

    std::vector<bio::alphabet::aa10murphy> alphabets{
      'A'_aa10murphy, 'B'_aa10murphy, 'C'_aa10murphy, 'B'_aa10murphy, 'B'_aa10murphy, 'F'_aa10murphy, 'G'_aa10murphy,
      'H'_aa10murphy, 'I'_aa10murphy, 'I'_aa10murphy, 'K'_aa10murphy, 'I'_aa10murphy, 'I'_aa10murphy, 'A'_aa10murphy,
      'B'_aa10murphy, 'C'_aa10murphy, 'B'_aa10murphy, 'B'_aa10murphy, 'F'_aa10murphy, 'G'_aa10murphy, 'H'_aa10murphy,
      'I'_aa10murphy, 'I'_aa10murphy, 'K'_aa10murphy, 'I'_aa10murphy, 'I'_aa10murphy, 'B'_aa10murphy, 'K'_aa10murphy,
      'P'_aa10murphy, 'B'_aa10murphy, 'K'_aa10murphy, 'S'_aa10murphy, 'S'_aa10murphy, 'C'_aa10murphy, 'I'_aa10murphy,
      'F'_aa10murphy, 'S'_aa10murphy, 'F'_aa10murphy, 'B'_aa10murphy, 'B'_aa10murphy, 'K'_aa10murphy, 'P'_aa10murphy,
      'B'_aa10murphy, 'K'_aa10murphy, 'S'_aa10murphy, 'S'_aa10murphy, 'C'_aa10murphy, 'I'_aa10murphy, 'F'_aa10murphy,
      'S'_aa10murphy, 'F'_aa10murphy, 'B'_aa10murphy, 'F'_aa10murphy, 'S'_aa10murphy};

    for (auto [chr, alp] : bio::ranges::views::zip(chars, alphabets))
        EXPECT_EQ((bio::alphabet::assign_char_to(chr, bio::alphabet::aa10murphy{})), alp);
}

TEST(aa10murphy, to_char)
{
    std::vector<char> chars{'A', 'C', 'B', 'B', 'F', 'G', 'H', 'I', 'K', 'I', 'I', 'B', 'P',
                            'B', 'K', 'S', 'S', 'I', 'F', 'F', 'B', 'I', 'K', 'C', 'S', 'B'};

    std::vector<bio::alphabet::aa10murphy> alphabets{
      'A'_aa10murphy, 'C'_aa10murphy, 'B'_aa10murphy, 'B'_aa10murphy, 'F'_aa10murphy, 'G'_aa10murphy, 'H'_aa10murphy,
      'I'_aa10murphy, 'K'_aa10murphy, 'I'_aa10murphy, 'I'_aa10murphy, 'B'_aa10murphy, 'P'_aa10murphy, 'B'_aa10murphy,
      'K'_aa10murphy, 'S'_aa10murphy, 'S'_aa10murphy, 'I'_aa10murphy, 'F'_aa10murphy, 'F'_aa10murphy, 'B'_aa10murphy,
      'I'_aa10murphy, 'K'_aa10murphy, 'C'_aa10murphy, 'S'_aa10murphy, 'B'_aa10murphy};

    for (auto [chr, alp] : bio::ranges::views::zip(chars, alphabets))
        EXPECT_EQ(bio::alphabet::to_char(alp), chr);
}

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

using namespace bio::alphabet::literals;

TEST(literals, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_aa10murphy), 'A');
    EXPECT_EQ(bio::alphabet::to_char('B'_aa10murphy), 'B');
    EXPECT_EQ(bio::alphabet::to_char('C'_aa10murphy), 'C');
    EXPECT_EQ(bio::alphabet::to_char('F'_aa10murphy), 'F');
    EXPECT_EQ(bio::alphabet::to_char('G'_aa10murphy), 'G');
    EXPECT_EQ(bio::alphabet::to_char('H'_aa10murphy), 'H');
    EXPECT_EQ(bio::alphabet::to_char('I'_aa10murphy), 'I');
    EXPECT_EQ(bio::alphabet::to_char('K'_aa10murphy), 'K');
    EXPECT_EQ(bio::alphabet::to_char('P'_aa10murphy), 'P');
    EXPECT_EQ(bio::alphabet::to_char('S'_aa10murphy), 'S');
}

TEST(literals, vector)
{
    bio::alphabet::aa10murphy_vector v20;
    v20.resize(5, 'B'_aa10murphy);
    EXPECT_EQ(v20, "BBBBB"_aa10murphy);

    std::vector<bio::alphabet::aa10murphy> w20{'A'_aa10murphy,
                                               'B'_aa10murphy,
                                               'C'_aa10murphy,
                                               'F'_aa10murphy,
                                               'G'_aa10murphy,
                                               'H'_aa10murphy,
                                               'I'_aa10murphy,
                                               'K'_aa10murphy,
                                               'P'_aa10murphy,
                                               'S'_aa10murphy};
    EXPECT_EQ(w20, "ABCFGHIKPS"_aa10murphy);
}

TEST(aa10murphy, char_is_valid)
{
    constexpr auto aa27_validator = [](char c) { return std::isalpha(c) || c == '*'; };

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
    {
        bool expect = false;
        switch (c)
        {
            case 'D':
            case 'E':
            case 'J':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'Q':
            case 'R':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case 'd':
            case 'e':
            case 'j':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'q':
            case 'r':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case '*':
                break;
            default:
                expect = aa27_validator(c);
                break;
        }

        EXPECT_EQ(bio::alphabet::aa10murphy::char_is_valid(c), expect);
    }
}
