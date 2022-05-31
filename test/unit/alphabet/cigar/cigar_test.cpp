// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/cigar/cigar.hpp>

#include "../semi_alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"

using bio::get;

INSTANTIATE_TYPED_TEST_SUITE_P(cigar, semi_alphabet_test, bio::cigar, );
INSTANTIATE_TYPED_TEST_SUITE_P(cigar, semi_alphabet_constexpr, bio::cigar, );

TEST(cigar, brace_init)
{
    using bio::operator""_cigar_op;

    bio::cigar c1{uint32_t{223}, 'M'_cigar_op};
    EXPECT_EQ(c1.to_string(), bio::small_string<11>{"223M"});
}

TEST(cigar, to_string)
{
    bio::cigar c1{};

    bio::assign_rank_to(uint32_t{223}, get<0>(c1));
    bio::assign_char_to('M', get<1>(c1));
    EXPECT_EQ(c1.to_string(), bio::small_string<11>{"223M"});
}

TEST(cigar, assign_string)
{
    bio::cigar c1{};
    c1.assign_string("223M");
    EXPECT_EQ(uint32_t{223}, bio::to_rank(get<0>(c1)));
    EXPECT_EQ('M',           get<1>(c1).to_char());
}
