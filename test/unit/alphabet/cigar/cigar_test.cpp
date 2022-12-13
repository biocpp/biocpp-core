// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/cigar/cigar.hpp>

#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

INSTANTIATE_TYPED_TEST_SUITE_P(cigar, semi_alphabet_test, bio::alphabet::cigar, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar, semi_alphabet_constexpr, bio::alphabet::cigar, );

TEST(cigar, brace_init)
{
    using namespace bio::alphabet::literals;

    bio::alphabet::cigar c1{223, 'M'_cigar_op};
    EXPECT_EQ(c1.to_string(), bio::ranges::small_string<10>{"223M"});
}

TEST(cigar, to_string)
{
    bio::alphabet::cigar c1{};

    bio::alphabet::assign_rank_to(uint32_t{223}, get<0>(c1));
    bio::alphabet::assign_char_to('M', get<1>(c1));
    EXPECT_EQ(c1.to_string(), bio::ranges::small_string<10>{"223M"});
}

TEST(cigar, to_string2)
{
    bio::alphabet::cigar c1{};

    bio::alphabet::assign_rank_to(uint32_t{223}, get<0>(c1));
    bio::alphabet::assign_char_to('M', get<1>(c1));

    std::string buffer;
    buffer.resize(10);
    EXPECT_EQ(c1.to_string(buffer), bio::ranges::small_string<10>{"223M"});
}

TEST(cigar, assign_string)
{
    bio::alphabet::cigar c1{};
    c1.assign_string("223M");
    EXPECT_EQ(uint32_t{223}, bio::alphabet::to_rank(get<0>(c1)));
    EXPECT_EQ('M', get<1>(c1).to_char());
}

TEST(cigar, assign_string_fail1)
{
    bio::alphabet::cigar c1{};
    EXPECT_THROW(c1.assign_string("223"), std::invalid_argument);
}

TEST(cigar, assign_string_fail2)
{
    bio::alphabet::cigar c1{};
    EXPECT_THROW(c1.assign_string("!223M"), std::invalid_argument);
}

TEST(cigar, assign_string_fail3)
{
    bio::alphabet::cigar c1{};
    EXPECT_THROW(c1.assign_string("223L"), bio::alphabet::invalid_char_assignment);
}

TEST(cigar, assign_string_fail4)
{
    bio::alphabet::cigar c1{};
    EXPECT_THROW(c1.assign_string("5000000000M"), std::invalid_argument);
}

TEST(cigar, assign_string_fail5)
{
    // lower-case is not supported
    bio::alphabet::cigar c1{};
    EXPECT_THROW(c1.assign_string("223m"), bio::alphabet::invalid_char_assignment);
}
