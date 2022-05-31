// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/quality/phred42.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "phred_test_template.hpp"

using bio::operator""_phred42;

INSTANTIATE_TYPED_TEST_SUITE_P(phred42, alphabet, bio::phred42, );
INSTANTIATE_TYPED_TEST_SUITE_P(phred42, semi_alphabet_test, bio::phred42, );
INSTANTIATE_TYPED_TEST_SUITE_P(phred42, alphabet_constexpr, bio::phred42, );
INSTANTIATE_TYPED_TEST_SUITE_P(phred42, semi_alphabet_constexpr, bio::phred42, );
INSTANTIATE_TYPED_TEST_SUITE_P(phred42, phred, bio::phred42, );

TEST(phred42, char_literal)
{
    EXPECT_EQ(bio::to_char('!'_phred42), '!');
    EXPECT_EQ(bio::to_char('"'_phred42), '"');
    EXPECT_EQ(bio::to_char('#'_phred42), '#');
    EXPECT_EQ(bio::to_char('$'_phred42), '$');
    EXPECT_EQ(bio::to_char('%'_phred42), '%');
    EXPECT_EQ(bio::to_char('&'_phred42), '&');
    EXPECT_EQ(bio::to_char('\''_phred42), '\'');
    EXPECT_EQ(bio::to_char('('_phred42), '(');
    EXPECT_EQ(bio::to_char(')'_phred42), ')');
    EXPECT_EQ(bio::to_char('*'_phred42), '*');
    EXPECT_EQ(bio::to_char('+'_phred42), '+');
    EXPECT_EQ(bio::to_char(','_phred42), ',');
    EXPECT_EQ(bio::to_char('-'_phred42), '-');
    EXPECT_EQ(bio::to_char('.'_phred42), '.');
    EXPECT_EQ(bio::to_char('/'_phred42), '/');
    EXPECT_EQ(bio::to_char('0'_phred42), '0');
    EXPECT_EQ(bio::to_char('1'_phred42), '1');
    EXPECT_EQ(bio::to_char('2'_phred42), '2');
    EXPECT_EQ(bio::to_char('3'_phred42), '3');
    EXPECT_EQ(bio::to_char('4'_phred42), '4');
    EXPECT_EQ(bio::to_char('5'_phred42), '5');
    EXPECT_EQ(bio::to_char('6'_phred42), '6');
    EXPECT_EQ(bio::to_char('7'_phred42), '7');
    EXPECT_EQ(bio::to_char('8'_phred42), '8');
    EXPECT_EQ(bio::to_char('9'_phred42), '9');
    EXPECT_EQ(bio::to_char(':'_phred42), ':');
    EXPECT_EQ(bio::to_char(';'_phred42), ';');
    EXPECT_EQ(bio::to_char('<'_phred42), '<');
    EXPECT_EQ(bio::to_char('='_phred42), '=');
    EXPECT_EQ(bio::to_char('>'_phred42), '>');
    EXPECT_EQ(bio::to_char('?'_phred42), '?');
    EXPECT_EQ(bio::to_char('@'_phred42), '@');
    EXPECT_EQ(bio::to_char('A'_phred42), 'A');
    EXPECT_EQ(bio::to_char('B'_phred42), 'B');
    EXPECT_EQ(bio::to_char('C'_phred42), 'C');
    EXPECT_EQ(bio::to_char('D'_phred42), 'D');
    EXPECT_EQ(bio::to_char('E'_phred42), 'E');
    EXPECT_EQ(bio::to_char('F'_phred42), 'F');
    EXPECT_EQ(bio::to_char('G'_phred42), 'G');
    EXPECT_EQ(bio::to_char('H'_phred42), 'H');
    EXPECT_EQ(bio::to_char('I'_phred42), 'I');
    EXPECT_EQ(bio::to_char('J'_phred42), 'J');
}

TEST(phred42, string_literal)
{
    std::vector<bio::phred42> v;
    v.resize(5, '#'_phred42);
    EXPECT_EQ(v, "#####"_phred42);

    std::vector<bio::phred42> w{'#'_phred42, '#'_phred42, '!'_phred42, '!'_phred42, '!'_phred42, '#'_phred42};
    EXPECT_EQ(w, "##!!!#"_phred42);
}
