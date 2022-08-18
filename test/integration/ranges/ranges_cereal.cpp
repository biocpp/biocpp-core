// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>
#include <bio/ranges/container/small_string.hpp>
#include <bio/ranges/container/small_vector.hpp>
#include <bio/ranges/to.hpp>

#include "../cereal.hpp"

using namespace bio::alphabet::literals;

template <typename t>
using ranges_1d_generic = ::testing::Test;

using ranges_1d_types =
  ::testing::Types<std::vector<bio::alphabet::dna4>,
                   std::vector<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>>,
                   bio::ranges::bitcompressed_vector<bio::alphabet::dna4>,
                   bio::ranges::small_vector<bio::alphabet::dna4, 1000>>;

TYPED_TEST_SUITE(ranges_1d_generic, ranges_1d_types, );

TYPED_TEST(ranges_1d_generic, short)
{
    TypeParam t1;
    if constexpr (std::same_as<TypeParam,
                               std::vector<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>>>)
    {
        t1.emplace_back('A'_dna4);
        t1.emplace_back('C'_dna4);
        t1.emplace_back('C'_dna4);
        t1.emplace_back('G'_dna4);
        t1.emplace_back('T'_dna4);
    }
    else
    {
        t1.push_back('A'_dna4);
        t1.push_back('C'_dna4);
        t1.push_back('C'_dna4);
        t1.push_back('G'_dna4);
        t1.push_back('T'_dna4);
    }
    do_serialisation(t1);
}

TYPED_TEST(ranges_1d_generic, longer)
{
    TypeParam        range;
    constexpr size_t size = bio::alphabet::size<std::ranges::range_value_t<TypeParam>>;
    range.resize(1000);
    for (unsigned i = 0; i < 1000; ++i)
        bio::alphabet::assign_rank_to(i % size, range[i]);
    do_serialisation(range);
}

TEST(range_cereal_smallstring, simple)
{
    bio::ranges::small_string<100> str = "Hello, World!";
    do_serialisation(str);
}

TEST(range_cereal_dynamic_bitset, simple)
{
    bio::ranges::dynamic_bitset t1{0b100101};
    do_serialisation(t1);
}

template <typename t>
using ranges_2d_generic = ::testing::Test;

using ranges_2d_types =
  ::testing::Types<std::vector<std::vector<bio::alphabet::dna4>>,
                   std::vector<bio::ranges::bitcompressed_vector<bio::alphabet::dna4>>,
                   std::vector<bio::ranges::small_vector<bio::alphabet::dna4, 100>>,
                   bio::ranges::concatenated_sequences<std::vector<bio::alphabet::dna4>>,
                   bio::ranges::concatenated_sequences<bio::ranges::bitcompressed_vector<bio::alphabet::dna4>>>;

TYPED_TEST_SUITE(ranges_2d_generic, ranges_2d_types, );

template <typename t>
struct tmp_type
{
    using type = std::ranges::range_value_t<t>;
};

template <typename t>
struct tmp_type<bio::ranges::concatenated_sequences<t>>
{
    using type = t;
};

TYPED_TEST(ranges_2d_generic, short)
{
    auto val1 = "ACGT"_dna4 | bio::ranges::to<typename tmp_type<TypeParam>::type>();
    auto val2 = "GAGGA"_dna4 | bio::ranges::to<typename tmp_type<TypeParam>::type>();

    TypeParam t1{val1, val1, val2};
    do_serialisation(t1);
}

TYPED_TEST(ranges_2d_generic, longer)
{
    typename tmp_type<TypeParam>::type range;
    range.resize(4);
    range[0] = 'A'_dna4;
    range[1] = 'C'_dna4;
    range[2] = 'G'_dna4;
    range[3] = 'T'_dna4;

    TypeParam range_of_ranges;
    range_of_ranges.resize(100, range);
    do_serialisation(range_of_ranges);
}
