// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/fmt.hpp>
#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/mask/masked.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>

using namespace bio::alphabet::literals;

TEST(alph_fmt, dna4)
{
    std::string buffer;
    buffer = fmt::format("{}", 'A'_dna4);
    EXPECT_EQ(buffer, "A");
}

TEST(alph_fmt, dna4_range)
{
    std::string buffer;
    buffer = fmt::format("{}", "ACGT"_dna4);

    EXPECT_EQ(buffer, "ACGT");
}

TEST(alph_fmt, gapped)
{
    std::string                                buffer;
    bio::alphabet::gapped<bio::alphabet::dna4> letter;

    letter = 'T'_dna4;
    buffer = fmt::format("{}", letter);
    EXPECT_EQ(buffer, "T");

    letter = bio::alphabet::gap{};
    buffer = fmt::format("{}", letter);
    EXPECT_EQ(buffer, "-");
}

TEST(alph_fmt, gapped_range)
{
    std::string                                             buffer;
    std::vector<bio::alphabet::gapped<bio::alphabet::dna4>> letters;
    letters.resize(4);
    letters[0] = 'A'_dna4;
    letters[1] = bio::alphabet::gap{};
    letters[2] = 'T'_dna4;
    letters[3] = bio::alphabet::gap{};

    buffer = fmt::format("{}", letters);
    EXPECT_EQ(buffer, "A-T-");
}

TEST(alph_fmt, qualified)
{
    std::string                                                           buffer;
    bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42> letter{'C'_dna4, '!'_phred42};

    buffer = fmt::format("{}", letter);
    EXPECT_EQ(buffer, "C");
}

TEST(alph_fmt, qualified_range)
{
    std::string                                                                        buffer;
    std::vector<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>> letters;
    letters.resize(4);
    letters[0] = 'A'_dna4;
    letters[1] = 'C'_dna4;
    letters[2] = 'G'_dna4;
    letters[3] = 'T'_dna4;

    buffer = fmt::format("{}", letters);
    EXPECT_EQ(buffer, "ACGT");
}

// Mask is broken, but this is unrelated
// TEST(alph_fmt, masked)
// {
//     std::string buffer;
//     bio::alphabet::masked<bio::alphabet::dna4> letter;
//
//     letter.assign_char('A');
//     buffer = fmt::format("{}", letter);
//     EXPECT_EQ(buffer, "A");
//
//     letter.assign_char('a');
//     buffer = fmt::format("{}", letter);
//     EXPECT_EQ(buffer, "a");
//
// }
//
// TEST(alph_fmt, masked_range)
// {
//     std::string buffer;
//     std::vector<bio::alphabet::masked<bio::alphabet::dna4>> letters;
//     letters.resize(4);
//     letters[0].assign_char('A');
//     letters[1].assign_char('c');
//     letters[2].assign_char('G');
//     letters[3].assign_char('t');
//
//     buffer = fmt::format("{}", letters);
//     EXPECT_EQ(buffer, "AcGt");
// }
