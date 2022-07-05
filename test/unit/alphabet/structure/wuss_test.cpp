// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/structure/concept.hpp>
#include <bio/alphabet/structure/wuss.hpp>
#include <bio/ranges/views/zip.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using bio::alphabet::operator""_wuss51;

INSTANTIATE_TYPED_TEST_SUITE_P(wuss51, alphabet, bio::alphabet::wuss51, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss51, semi_alphabet_test, bio::alphabet::wuss51, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss51, alphabet_constexpr, bio::alphabet::wuss51, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss51, semi_alphabet_constexpr, bio::alphabet::wuss51, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss15, alphabet, bio::alphabet::wuss<15>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss15, semi_alphabet_test, bio::alphabet::wuss<15>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss15, alphabet_constexpr, bio::alphabet::wuss<15>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss15, semi_alphabet_constexpr, bio::alphabet::wuss<15>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss67, alphabet, bio::alphabet::wuss<67>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss67, semi_alphabet_test, bio::alphabet::wuss<67>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss67, alphabet_constexpr, bio::alphabet::wuss<67>, );
INSTANTIATE_TYPED_TEST_SUITE_P(wuss67, semi_alphabet_constexpr, bio::alphabet::wuss<67>, );

// assign_char functions
TEST(wuss, assign_char)
{
    std::vector<char> input{'.', '(', ')', ':', ',', '-', '_', '~', ';', '<', '>',
                            '[', ']', '{', '}', 'H', 'B', 'E', 'G', 'I', 'T', 'S'};

    std::vector<bio::alphabet::wuss51> cmp{'.'_wuss51, '('_wuss51, ')'_wuss51, ':'_wuss51, ','_wuss51, '-'_wuss51,
                                           '_'_wuss51, '~'_wuss51, ';'_wuss51, '<'_wuss51, '>'_wuss51, '['_wuss51,
                                           ']'_wuss51, '{'_wuss51, '}'_wuss51, 'H'_wuss51, 'B'_wuss51, 'E'_wuss51,
                                           'G'_wuss51, 'I'_wuss51, 'T'_wuss51, 'S'_wuss51};

    for (auto [ch, cm] : bio::ranges::views::zip(input, cmp))
        EXPECT_EQ((bio::alphabet::assign_char_to(ch, bio::alphabet::wuss51{})), cm);
}

// to_char functions
TEST(wuss, to_char)
{
    EXPECT_EQ(bio::alphabet::to_char('.'_wuss51), '.');
    EXPECT_EQ(bio::alphabet::to_char(':'_wuss51), ':');
    EXPECT_EQ(bio::alphabet::to_char(','_wuss51), ',');
    EXPECT_EQ(bio::alphabet::to_char('-'_wuss51), '-');
    EXPECT_EQ(bio::alphabet::to_char('_'_wuss51), '_');
    EXPECT_EQ(bio::alphabet::to_char('~'_wuss51), '~');
    EXPECT_EQ(bio::alphabet::to_char(';'_wuss51), ';');
    EXPECT_EQ(bio::alphabet::to_char('<'_wuss51), '<');
    EXPECT_EQ(bio::alphabet::to_char('>'_wuss51), '>');
    EXPECT_EQ(bio::alphabet::to_char('('_wuss51), '(');
    EXPECT_EQ(bio::alphabet::to_char(')'_wuss51), ')');
    EXPECT_EQ(bio::alphabet::to_char('['_wuss51), '[');
    EXPECT_EQ(bio::alphabet::to_char(']'_wuss51), ']');
    EXPECT_EQ(bio::alphabet::to_char('{'_wuss51), '{');
    EXPECT_EQ(bio::alphabet::to_char('}'_wuss51), '}');
}

// concepts
TEST(wuss, concept_check)
{
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss51>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss51 &>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss51 const>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss51 const &>);
    EXPECT_NE(bio::alphabet::max_pseudoknot_depth<bio::alphabet::wuss51>, 0);

    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<>>); // same as wuss51
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<> &>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<> const>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<> const &>);
    EXPECT_NE(bio::alphabet::max_pseudoknot_depth<bio::alphabet::wuss<>>, 0);

    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<67>>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<67> &>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<67> const>);
    EXPECT_TRUE(bio::alphabet::rna_structure_alphabet<bio::alphabet::wuss<67> const &>);
    EXPECT_NE(bio::alphabet::max_pseudoknot_depth<bio::alphabet::wuss<67>>, 0);
}

TEST(wuss, literals)
{
    std::vector<bio::alphabet::wuss51> vec1;
    vec1.resize(5, '<'_wuss51);
    EXPECT_EQ(vec1, "<<<<<"_wuss51);

    std::vector<bio::alphabet::wuss51> vec2{'.'_wuss51, '<'_wuss51, '<'_wuss51, '>'_wuss51, '>'_wuss51, '.'_wuss51};
    EXPECT_EQ(vec2, ".<<>>."_wuss51);
}

TEST(wuss, rna_structure_properties)
{
    EXPECT_EQ(bio::alphabet::wuss51::max_pseudoknot_depth, 22);
    std::vector<bio::alphabet::wuss51> vec = ".:,-_~;<>()[]{}AaBbCcDd"_wuss51;
    for (unsigned idx = 0; idx <= 6; ++idx)
    {
        EXPECT_TRUE(vec[idx].is_unpaired());
        EXPECT_FALSE(vec[idx].is_pair_open());
        EXPECT_FALSE(vec[idx].is_pair_close());
        EXPECT_FALSE(vec[idx].pseudoknot_id());
    }
    for (unsigned idx = 7; idx <= 21; idx += 2)
    {
        EXPECT_TRUE(vec[idx].is_pair_open());
        EXPECT_FALSE(vec[idx].is_unpaired());
        EXPECT_FALSE(vec[idx].is_pair_close());
        EXPECT_TRUE(vec[idx].pseudoknot_id());
        EXPECT_EQ(vec[idx].pseudoknot_id(), std::optional<uint8_t>{(idx - 7) / 2});
    }
    for (unsigned idx = 8; idx <= 22; idx += 2)
    {
        EXPECT_TRUE(vec[idx].is_pair_close());
        EXPECT_FALSE(vec[idx].is_unpaired());
        EXPECT_FALSE(vec[idx].is_pair_open());
        EXPECT_TRUE(vec[idx].pseudoknot_id());
        EXPECT_EQ(vec[idx].pseudoknot_id(), std::optional<uint8_t>{(idx - 8) / 2});
    }
}
