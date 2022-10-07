// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/aminoacid/aa20.hpp>
#include <bio/ranges/views/zip.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "aminoacid_test_template.hpp"

using namespace bio::alphabet::literals;

INSTANTIATE_TYPED_TEST_SUITE_P(aa20, alphabet, bio::alphabet::aa20, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa20, semi_alphabet_test, bio::alphabet::aa20, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa20, alphabet_constexpr, bio::alphabet::aa20, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa20, semi_alphabet_constexpr, bio::alphabet::aa20, );
INSTANTIATE_TYPED_TEST_SUITE_P(aa20, aminoacid, bio::alphabet::aa20, );

TEST(aa20, assign_char)
{
    std::vector<char> chars{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'a', 'b', 'c', 'd', 'e',
                            'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',
                            'X', 'Y', 'Z', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '*', '!'};

    std::vector<bio::alphabet::aa20> alphabets{
      'A'_aa20, 'D'_aa20, 'C'_aa20, 'D'_aa20, 'E'_aa20, 'F'_aa20, 'G'_aa20, 'H'_aa20, 'I'_aa20, 'L'_aa20, 'K'_aa20,
      'L'_aa20, 'M'_aa20, 'A'_aa20, 'D'_aa20, 'C'_aa20, 'D'_aa20, 'E'_aa20, 'F'_aa20, 'G'_aa20, 'H'_aa20, 'I'_aa20,
      'L'_aa20, 'K'_aa20, 'L'_aa20, 'M'_aa20, 'N'_aa20, 'L'_aa20, 'P'_aa20, 'Q'_aa20, 'R'_aa20, 'S'_aa20, 'T'_aa20,
      'C'_aa20, 'V'_aa20, 'W'_aa20, 'S'_aa20, 'Y'_aa20, 'E'_aa20, 'N'_aa20, 'L'_aa20, 'P'_aa20, 'Q'_aa20, 'R'_aa20,
      'S'_aa20, 'T'_aa20, 'C'_aa20, 'V'_aa20, 'W'_aa20, 'S'_aa20, 'Y'_aa20, 'E'_aa20, 'W'_aa20, 'S'_aa20};

    for (auto [chr, alp] : bio::ranges::views::zip(chars, alphabets))
        EXPECT_EQ((bio::alphabet::assign_char_to(chr, bio::alphabet::aa20{})), alp);
}

TEST(aa20, to_char)
{
    std::vector<char> chars{'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q',
                            'R', 'S', 'T', 'V', 'W', 'Y', 'D', 'L', 'L', 'C', 'S', 'E', 'W', 'S'};

    std::vector<bio::alphabet::aa20> alphabets{'A'_aa20, 'C'_aa20, 'D'_aa20, 'E'_aa20, 'F'_aa20, 'G'_aa20, 'H'_aa20,
                                               'I'_aa20, 'K'_aa20, 'L'_aa20, 'M'_aa20, 'N'_aa20, 'P'_aa20, 'Q'_aa20,
                                               'R'_aa20, 'S'_aa20, 'T'_aa20, 'V'_aa20, 'W'_aa20, 'Y'_aa20, 'D'_aa20,
                                               'L'_aa20, 'L'_aa20, 'C'_aa20, 'S'_aa20, 'E'_aa20, 'W'_aa20, 'S'_aa20};

    for (auto [chr, alp] : bio::ranges::views::zip(chars, alphabets))
        EXPECT_EQ(bio::alphabet::to_char(alp), chr);
}

// ------------------------------------------------------------------
// literals
// ------------------------------------------------------------------

TEST(literals, char_literal)
{
    EXPECT_EQ(bio::alphabet::to_char('A'_aa20), 'A');
    EXPECT_EQ(bio::alphabet::to_char('C'_aa20), 'C');
    EXPECT_EQ(bio::alphabet::to_char('D'_aa20), 'D');
    EXPECT_EQ(bio::alphabet::to_char('E'_aa20), 'E');
    EXPECT_EQ(bio::alphabet::to_char('F'_aa20), 'F');
    EXPECT_EQ(bio::alphabet::to_char('G'_aa20), 'G');
    EXPECT_EQ(bio::alphabet::to_char('H'_aa20), 'H');
    EXPECT_EQ(bio::alphabet::to_char('I'_aa20), 'I');
    EXPECT_EQ(bio::alphabet::to_char('K'_aa20), 'K');
    EXPECT_EQ(bio::alphabet::to_char('L'_aa20), 'L');
    EXPECT_EQ(bio::alphabet::to_char('M'_aa20), 'M');
    EXPECT_EQ(bio::alphabet::to_char('N'_aa20), 'N');
    EXPECT_EQ(bio::alphabet::to_char('P'_aa20), 'P');
    EXPECT_EQ(bio::alphabet::to_char('Q'_aa20), 'Q');
    EXPECT_EQ(bio::alphabet::to_char('R'_aa20), 'R');
    EXPECT_EQ(bio::alphabet::to_char('S'_aa20), 'S');
    EXPECT_EQ(bio::alphabet::to_char('T'_aa20), 'T');
    EXPECT_EQ(bio::alphabet::to_char('V'_aa20), 'V');
    EXPECT_EQ(bio::alphabet::to_char('W'_aa20), 'W');
    EXPECT_EQ(bio::alphabet::to_char('Y'_aa20), 'Y');
}

TEST(literals, vector)
{
    bio::alphabet::aa20_vector v20;
    v20.resize(5, 'D'_aa20);
    EXPECT_EQ(v20, "DDDDD"_aa20);

    std::vector<bio::alphabet::aa20>
      w20{'A'_aa20, 'D'_aa20, 'L'_aa20, 'L'_aa20, 'C'_aa20, 'S'_aa20, 'E'_aa20, 'S'_aa20, 'W'_aa20, 'K'_aa20};
    EXPECT_EQ(w20, "ADLLCSESWK"_aa20);
}

TEST(aa20, char_is_valid)
{
    constexpr auto aa27_validator = [](char c) { return std::isalpha(c) || c == '*'; };

    for (char c : std::views::iota(std::numeric_limits<char>::min(), std::numeric_limits<char>::max()))
    {
        bool expect = false;
        switch (c)
        {
            case 'B':
            case 'J':
            case 'O':
            case 'U':
            case 'X':
            case 'Z':
            case 'b':
            case 'j':
            case 'o':
            case 'u':
            case 'x':
            case 'z':
            case '*':
                break;
            default:
                expect = aa27_validator(c);
                break;
        }

        EXPECT_EQ(bio::alphabet::aa20::char_is_valid(c), expect);
    }
}
