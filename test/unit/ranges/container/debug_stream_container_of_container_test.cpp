// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/meta/detail/debug_stream_alphabet.hpp>
#include <bio/meta/detail/debug_stream_range.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>

using bio::operator""_dna4;

template <typename T>
class debug_stream_test : public ::testing::Test
{};

using container_of_container_types =
    ::testing::Types<std::vector<std::vector<bio::dna4>>,
                     bio::concatenated_sequences<std::vector<bio::dna4>>,
                     bio::concatenated_sequences<bio::bitcompressed_vector<bio::dna4>>>;

TYPED_TEST_SUITE(debug_stream_test, container_of_container_types, );

TYPED_TEST(debug_stream_test, container_of_container)
{
    TypeParam t1{"ACGT"_dna4, "ACGT"_dna4, "GAGGA"_dna4};

    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << TypeParam{};

    o.flush();
    EXPECT_EQ(o.str(), "[]");

    my_stream << ", " << t1;

    o.flush();
    EXPECT_EQ(o.str(), "[], [ACGT,ACGT,GAGGA]");
}
