// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/meta/detail/debug_stream_alphabet.hpp>

using bio::operator""_dna4;

template <typename T>
using debug_stream_test = ::testing::Test;

using alphabet_types = ::testing::Types<bio::dna4,
                                        bio::qualified<bio::dna4,
                                        bio::phred42>,
                                        bio::gapped<bio::dna4>>;

TYPED_TEST_SUITE(debug_stream_test, alphabet_types, );

TYPED_TEST(debug_stream_test, alphabet)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    TypeParam val{'C'_dna4};
    my_stream << val;

    o.flush();
    EXPECT_EQ(o.str().size(), 1u);
    EXPECT_EQ(o.str()[0], 'C');
}
