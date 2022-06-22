// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "aminoacid_test_template.hpp"

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/ranges/views/zip.hpp>

using bio::operator""_aa27;

INSTANTIATE_TYPED_TEST_SUITE_P(aa27, alphabet, bio::aa27, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa27, semi_alphabet_test, bio::aa27, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa27, alphabet_constexpr, bio::aa27, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa27, semi_alphabet_constexpr, bio::aa27, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa27, aminoacid, bio::aa27, );

TEST(aa27, assign_char)
{
    std::vector<char> chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'a', 'b', 'c', 'd', 'e',
                            'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                            'X', 'Y', 'Z', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '*', '!'};

    std::vector<bio::aa27> alphabets{
      'A'_aa27, 'B'_aa27, 'C'_aa27, 'D'_aa27, 'E'_aa27, 'F'_aa27, 'G'_aa27, 'H'_aa27, 'I'_aa27, 'J'_aa27, 'K'_aa27,
      'L'_aa27, 'M'_aa27, 'A'_aa27, 'B'_aa27, 'C'_aa27, 'D'_aa27, 'E'_aa27, 'F'_aa27, 'G'_aa27, 'H'_aa27, 'I'_aa27,
      'J'_aa27, 'K'_aa27, 'L'_aa27, 'M'_aa27, 'N'_aa27, 'O'_aa27, 'P'_aa27, 'Q'_aa27, 'R'_aa27, 'S'_aa27, 'T'_aa27,
      'U'_aa27, 'V'_aa27, 'W'_aa27, 'X'_aa27, 'Y'_aa27, 'Z'_aa27, 'N'_aa27, 'O'_aa27, 'P'_aa27, 'Q'_aa27, 'R'_aa27,
      'S'_aa27, 'T'_aa27, 'U'_aa27, 'V'_aa27, 'W'_aa27, 'X'_aa27, 'Y'_aa27, 'Z'_aa27, '*'_aa27, 'X'_aa27};

    for (auto [chr, alp] : bio::views::zip(chars, alphabets))
        EXPECT_EQ((bio::assign_char_to(chr, bio::aa27{})), alp);
}

TEST(aa27, to_char)
{
    std::vector<char> chars{'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q',
                            'R', 'S', 'T', 'V', 'W', 'Y', 'B', 'J', 'O', 'U', 'X', 'Z', '*', 'X'};

    std::vector<bio::aa27> alphabets{'A'_aa27, 'C'_aa27, 'D'_aa27, 'E'_aa27, 'F'_aa27, 'G'_aa27, 'H'_aa27,
                                     'I'_aa27, 'K'_aa27, 'L'_aa27, 'M'_aa27, 'N'_aa27, 'P'_aa27, 'Q'_aa27,
                                     'R'_aa27, 'S'_aa27, 'T'_aa27, 'V'_aa27, 'W'_aa27, 'Y'_aa27, 'B'_aa27,
                                     'J'_aa27, 'O'_aa27, 'U'_aa27, 'X'_aa27, 'Z'_aa27, '*'_aa27, 'X'_aa27};

    for (auto [alp, chr] : bio::views::zip(alphabets, chars))
        EXPECT_EQ(bio::to_char(alp), chr);
}

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

TEST(literals, char_literal)
{
    EXPECT_EQ(bio::to_char('A'_aa27), 'A');
    EXPECT_EQ(bio::to_char('B'_aa27), 'B');
    EXPECT_EQ(bio::to_char('C'_aa27), 'C');
    EXPECT_EQ(bio::to_char('D'_aa27), 'D');
    EXPECT_EQ(bio::to_char('E'_aa27), 'E');
    EXPECT_EQ(bio::to_char('F'_aa27), 'F');
    EXPECT_EQ(bio::to_char('G'_aa27), 'G');
    EXPECT_EQ(bio::to_char('H'_aa27), 'H');
    EXPECT_EQ(bio::to_char('I'_aa27), 'I');
    EXPECT_EQ(bio::to_char('J'_aa27), 'J');
    EXPECT_EQ(bio::to_char('K'_aa27), 'K');
    EXPECT_EQ(bio::to_char('L'_aa27), 'L');
    EXPECT_EQ(bio::to_char('M'_aa27), 'M');
    EXPECT_EQ(bio::to_char('N'_aa27), 'N');
    EXPECT_EQ(bio::to_char('O'_aa27), 'O');
    EXPECT_EQ(bio::to_char('P'_aa27), 'P');
    EXPECT_EQ(bio::to_char('Q'_aa27), 'Q');
    EXPECT_EQ(bio::to_char('R'_aa27), 'R');
    EXPECT_EQ(bio::to_char('S'_aa27), 'S');
    EXPECT_EQ(bio::to_char('T'_aa27), 'T');
    EXPECT_EQ(bio::to_char('U'_aa27), 'U');
    EXPECT_EQ(bio::to_char('V'_aa27), 'V');
    EXPECT_EQ(bio::to_char('W'_aa27), 'W');
    EXPECT_EQ(bio::to_char('X'_aa27), 'X');
    EXPECT_EQ(bio::to_char('Y'_aa27), 'Y');
    EXPECT_EQ(bio::to_char('Z'_aa27), 'Z');
    EXPECT_EQ(bio::to_char('*'_aa27), '*');

    EXPECT_EQ(bio::to_char('!'_aa27), 'X');
}

TEST(literals, vector)
{
    bio::aa27_vector v27;
    v27.resize(5, 'A'_aa27);
    EXPECT_EQ(v27, "AAAAA"_aa27);

    std::vector<bio::aa27>
      w27{'A'_aa27, 'Y'_aa27, 'P'_aa27, 'T'_aa27, 'U'_aa27, 'N'_aa27, 'X'_aa27, '!'_aa27, '*'_aa27};
    EXPECT_EQ(w27, "AYPTUNXX*"_aa27);
}

TEST(aa27, char_is_valid)
{
    constexpr auto validator = [](char c) { return std::isalpha(c) || c == '*'; };
    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
        EXPECT_EQ(bio::aa27::char_is_valid(c), validator(c));
}
