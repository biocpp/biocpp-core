// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <numeric>
#include <sstream>
#include <tuple>
#include <utility>

#include <gtest/gtest.h>

#include <bio/meta/type_pack/detail/type_pack_algorithm.hpp>
#include <bio/alphabet/quality/all.hpp>

template <typename T>
using quality_conversion = ::testing::Test;

// add all alphabets from the quality sub module here
using quality_conversion_types = bio::type_list<bio::phred42, bio::phred63, bio::phred68legacy>;
using quality_conversion_gtest_types = bio::detail::transfer_template_args_onto_t<quality_conversion_types,
                                                                                     ::testing::Types>;

TYPED_TEST_SUITE(quality_conversion, quality_conversion_gtest_types, );

TYPED_TEST(quality_conversion, explicit_conversion)
{
    bio::detail::for_each<quality_conversion_types>([&] (auto qual) constexpr
    {
        using out_type = std::decay_t<typename decltype(qual)::type>;
        EXPECT_EQ(static_cast<out_type>(TypeParam{ 0}), out_type{ 0});
        EXPECT_EQ(static_cast<out_type>(TypeParam{ 5}), out_type{ 5});
        EXPECT_EQ(static_cast<out_type>(TypeParam{15}), out_type{15});
        EXPECT_EQ(static_cast<out_type>(TypeParam{20}), out_type{20});
        EXPECT_EQ(static_cast<out_type>(TypeParam{40}), out_type{40});
    });
}
