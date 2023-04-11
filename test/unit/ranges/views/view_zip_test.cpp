// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <ranges>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/views/repeat.hpp>
#include <bio/ranges/views/zip.hpp>

#include "../../ranges/iterator_test_template.hpp"

using range_t        = std::vector<int>;
using other_range_t  = std::vector<std::string>;
using zip_view_t     = decltype(bio::ranges::views::zip(std::declval<range_t &>(), std::declval<other_range_t &>()));
using zip_iterator_t = std::ranges::iterator_t<zip_view_t>;

template <>
struct iterator_fixture<zip_iterator_t> : public ::testing::Test
{
    using iterator_tag = std::random_access_iterator_tag;

    static constexpr bool const_iterable = true;

    range_t range{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    other_range_t other_range{"AA", "BBB", "CC", "DDD"};

    std::vector<bio::meta::tuple<int, std::string>> expected_range{
      {0,  "AA"},
      {1, "BBB"},
      {2,  "CC"},
      {3, "DDD"}
    };

    zip_view_t test_range{bio::ranges::views::zip(range, other_range)};
};
INSTANTIATE_TYPED_TEST_SUITE_P(zip_iterator_test, iterator_fixture, zip_iterator_t, );

using namespace bio::alphabet::literals;

class zip_test : public ::testing::Test
{
protected:
    using range_t         = std::vector<int>;
    using const_range_t   = std::vector<int> const;
    using other_range_t   = std::vector<std::string>;
    using forward_range_t = std::forward_list<int>;
    using view_t          = decltype(bio::ranges::views::repeat('L'));

    range_t                 range{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const_range_t           const_range{range};
    other_range_t           other_range{"AA", "BBB", "CC", "DDD"};
    forward_range_t         forward_range{range.begin(), range.end()};
    static constexpr view_t view{};

    using zip_common_range_t     = decltype(bio::ranges::views::zip(range, other_range));
    using zip_not_common_range_t = decltype(bio::ranges::views::zip(range, other_range, view));
    using zip_const_range_t      = decltype(bio::ranges::views::zip(range, const_range));
    using zip_forward_range_t    = decltype(bio::ranges::views::zip(range, other_range, forward_range));
    using const_zip_t            = decltype(bio::ranges::views::zip(range, other_range)) const;

    zip_common_range_t zip_common_range() { return bio::ranges::views::zip(range, other_range); }

    zip_not_common_range_t zip_not_common_range() { return bio::ranges::views::zip(range, other_range, view); }

    zip_const_range_t zip_const_range() { return bio::ranges::views::zip(range, const_range); }

    zip_forward_range_t zip_forward_range() { return bio::ranges::views::zip(range, other_range, forward_range); }

    const_zip_t const_zip() { return bio::ranges::views::zip(range, other_range); }
};

TEST_F(zip_test, concepts)
{
    EXPECT_TRUE(std::ranges::forward_range<zip_forward_range_t>);
    EXPECT_FALSE(std::ranges::bidirectional_range<zip_forward_range_t>);

    EXPECT_TRUE(std::ranges::random_access_range<zip_common_range_t>);
    EXPECT_TRUE(std::ranges::random_access_range<zip_not_common_range_t>);
    EXPECT_TRUE(std::ranges::random_access_range<zip_const_range_t>);
    EXPECT_TRUE(std::ranges::random_access_range<const_zip_t>);

    EXPECT_TRUE(std::ranges::view<zip_common_range_t>);
    EXPECT_TRUE(std::ranges::view<zip_not_common_range_t>);
    EXPECT_TRUE(std::ranges::view<zip_const_range_t>);
    EXPECT_TRUE(std::ranges::view<zip_forward_range_t>);
    EXPECT_FALSE(std::ranges::view<const_zip_t>); // const lvalue is not movable, because it cannot be assigned to

    EXPECT_TRUE(std::ranges::sized_range<zip_common_range_t>);
    EXPECT_FALSE(std::ranges::sized_range<zip_not_common_range_t>); // bio::ranges::views::repeat has no size (infinite)
    EXPECT_TRUE(std::ranges::sized_range<zip_const_range_t>);
    EXPECT_FALSE(std::ranges::sized_range<zip_forward_range_t>);    // std::forward_list is not sized
    EXPECT_TRUE(std::ranges::sized_range<const_zip_t>);

    EXPECT_TRUE(std::ranges::common_range<zip_common_range_t>);
    EXPECT_FALSE(std::ranges::common_range<zip_not_common_range_t>);
    EXPECT_TRUE(std::ranges::common_range<zip_const_range_t>);
    EXPECT_TRUE(std::ranges::common_range<zip_forward_range_t>);
    EXPECT_TRUE(std::ranges::common_range<const_zip_t>);

    EXPECT_TRUE(std::ranges::common_range<zip_common_range_t>);
    EXPECT_FALSE(std::ranges::common_range<zip_not_common_range_t>);
    EXPECT_TRUE(std::ranges::common_range<zip_const_range_t>);
    EXPECT_TRUE(std::ranges::common_range<zip_forward_range_t>);
    EXPECT_TRUE(std::ranges::common_range<const_zip_t>);

    EXPECT_TRUE((std::ranges::output_range<zip_common_range_t, bio::meta::tuple<int &, std::string &>>));
    EXPECT_TRUE((std::ranges::output_range<zip_not_common_range_t, bio::meta::tuple<int &, std::string &, char &>>));
    EXPECT_FALSE((std::ranges::output_range<zip_const_range_t, bio::meta::tuple<int &, int &>>));
    EXPECT_FALSE((std::ranges::output_range<zip_const_range_t, bio::meta::tuple<int &, int const &>>));
    EXPECT_TRUE((std::ranges::output_range<zip_forward_range_t, bio::meta::tuple<int &, std::string &, int &>>));
    EXPECT_TRUE((std::ranges::output_range<const_zip_t, bio::meta::tuple<int &, std::string &>>));
}

TEST_F(zip_test, basic)
{
    {
        auto   zip_view = zip_common_range();
        size_t i{};
        for (auto && [elem_1, elem_2] : zip_view)
        {
            EXPECT_EQ(elem_1, range[i]);
            EXPECT_EQ(elem_2, other_range[i]);
            ++i;
        }
        EXPECT_EQ(i, 4u);
        EXPECT_EQ(zip_view.size(), 4u);
    }
    {
        auto   zip_view = bio::ranges::views::zip(other_range);
        size_t i{};
        for (auto && [elem_1] : zip_view)
        {
            EXPECT_EQ(elem_1, other_range[i]);
            ++i;
        }
        EXPECT_EQ(i, 4u);
        EXPECT_EQ(zip_view.size(), 4u);
    }
}

TEST_F(zip_test, combine)
{
    auto   zip_view = zip_common_range() | std::views::take(2);
    size_t i{};
    for (auto && [elem_1, elem_2] : zip_view)
    {
        EXPECT_EQ(elem_1, range[i]);
        EXPECT_EQ(elem_2, other_range[i]);
        ++i;
    }
    EXPECT_EQ(i, 2u);
    EXPECT_EQ(zip_view.size(), 2u);
}

TEST_F(zip_test, use_as_output_range)
{
    auto zip_view     = zip_common_range();
    *zip_view.begin() = bio::meta::tuple(23, "FF");
    EXPECT_EQ(std::get<0>(*zip_view.begin()), 23);
    EXPECT_EQ(std::get<1>(*zip_view.begin()), "FF");

    size_t i{1u};
    for (auto && [elem_1, elem_2] : zip_view | std::views::drop(1))
    {
        EXPECT_EQ(elem_1, range[i]);
        EXPECT_EQ(elem_2, other_range[i]);
        ++i;
    }
    EXPECT_EQ(i, 4u);
    EXPECT_EQ(zip_view.size(), 4u);
}

// // https://github.com/ericniebler/range-v3/issues/1480
TEST_F(zip_test, gcc10bug_rangev3_1480)
{
    // This regression test only checks if the respective code compiles.
    std::vector<char> const first_sequence{};
    std::vector<char> const second_sequence{};

    auto zip_view = bio::ranges::views::zip(first_sequence, second_sequence);
    std::ranges::for_each(zip_view, [&]([[maybe_unused]] auto && value) {});
}
