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
#include <bio/ranges/container/bitcompressed_vector.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>
#include <bio/ranges/container/dynamic_bitset.hpp>
#include <bio/ranges/container/small_string.hpp>
#include <bio/ranges/container/small_vector.hpp>

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
    std::string buf = fmt::format("{}", t1);
    EXPECT_EQ(buf, "ACCGT");
}

TEST(range_cereal_smallstring, short)
{
    bio::ranges::small_string<100> str = "Hello, World!";
    std::string                    buf = fmt::format("{}", str);
    EXPECT_EQ(buf, "Hello, World!");
}

TEST(range_cereal_dynamic_bitset, short)
{
    bio::ranges::dynamic_bitset t1{0b100101};
    std::string                 buf = fmt::format("{}", t1);
    EXPECT_EQ(buf, "0b10'0101");

    bio::ranges::dynamic_bitset t2{0b1100101101010111};
    buf = fmt::format("{}", t2);
    EXPECT_EQ(buf, "0b1100'1011'0101'0111");
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
    auto      val1 = "ACGT"_dna4 | bio::views::to<typename tmp_type<TypeParam>::type>();
    auto      val2 = "GAGGA"_dna4 | bio::views::to<typename tmp_type<TypeParam>::type>();
    TypeParam t1;
    t1.push_back(val1);
    t1.push_back(val1);
    t1.push_back(val2);
    std::string buf = fmt::format("{}", t1);
    EXPECT_EQ(buf, "[ACGT, ACGT, GAGGA]");
}
