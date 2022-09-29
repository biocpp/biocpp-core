// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <iostream>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/transform_by_pos.hpp>
#include <bio/test/expect_same_type.hpp>

auto rev = [](auto && urange, size_t const pos) -> std::ranges::range_reference_t<decltype(urange)>
{ return urange[std::ranges::size(urange) - 1 - pos]; };

TEST(view_transform_by_pos, reverse)
{
    std::string vec = "ACGT";
    std::string cmp = "TGCA";

    auto v = vec | bio::views::transform_by_pos(rev);

    std::string s = v | bio::ranges::to<std::string>();
    EXPECT_EQ(cmp, s);
}

TEST(view_transform_by_pos, concepts)
{
    std::string vec = "ACGT";

    using t = decltype(vec | bio::views::transform_by_pos(rev));

    EXPECT_TRUE(std::ranges::input_range<t>);
    EXPECT_TRUE(std::ranges::forward_range<t>);
    EXPECT_TRUE(std::ranges::bidirectional_range<t>);
    EXPECT_TRUE(std::ranges::random_access_range<t>);
    EXPECT_FALSE(std::ranges::contiguous_range<t>);

    EXPECT_TRUE(std::ranges::viewable_range<t>);
    EXPECT_TRUE(std::ranges::view<t>);
    EXPECT_TRUE(std::ranges::sized_range<t>);
    EXPECT_TRUE(std::ranges::common_range<t>);
    EXPECT_TRUE((std::ranges::output_range<t, char>));
    EXPECT_TRUE(bio::ranges::const_iterable_range<t>);

    EXPECT_SAME_TYPE(std::ranges::range_reference_t<t>, char &);
    EXPECT_SAME_TYPE(std::ranges::range_value_t<t>, char);
}

TEST(view_transform_by_pos, concepts_const)
{
    std::string const vec = "ACGT";

    using t = decltype(vec | bio::views::transform_by_pos(rev));

    EXPECT_TRUE(std::ranges::input_range<t>);
    EXPECT_TRUE(std::ranges::forward_range<t>);
    EXPECT_TRUE(std::ranges::bidirectional_range<t>);
    EXPECT_TRUE(std::ranges::random_access_range<t>);
    EXPECT_FALSE(std::ranges::contiguous_range<t>);

    EXPECT_TRUE(std::ranges::viewable_range<t>);
    EXPECT_TRUE(std::ranges::view<t>);
    EXPECT_TRUE(std::ranges::sized_range<t>);
    EXPECT_TRUE(std::ranges::common_range<t>);
    EXPECT_FALSE((std::ranges::output_range<t, char>));
    EXPECT_TRUE(bio::ranges::const_iterable_range<t>);

    EXPECT_SAME_TYPE(std::ranges::range_reference_t<t>, char const &);
    EXPECT_SAME_TYPE(std::ranges::range_value_t<t>, char);
}

auto identity = [](auto && urange, size_t const pos) -> std::ranges::range_reference_t<decltype(urange)>
{ return urange[pos]; };

TEST(view_transform_by_pos, take)
{
    std::string vec = "ACGT";
    std::string cmp = "ACG";

    auto fixed_size = [](auto &&) -> size_t { return 3ull; };
    auto v          = vec | bio::views::transform_by_pos(identity, fixed_size);

    std::string s = v | bio::ranges::to<std::string>();
    EXPECT_EQ(cmp, s);
}
