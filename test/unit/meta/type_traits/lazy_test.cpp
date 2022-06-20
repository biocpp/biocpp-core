// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <list>
#include <vector>

#include <bio/meta/type_traits/lazy.hpp>
#include <concepts>

TEST(lazy, instantiate)
{
    EXPECT_TRUE((std::is_same_v<bio::detail::instantiate_t<std::vector<int>>, std::vector<int>>));

    EXPECT_TRUE((std::is_same_v<bio::detail::instantiate_t<bio::detail::lazy<std::vector, int>>, std::vector<int>>));
}

template <typename t>
    requires std::integral<t>
using integral_identity_t = t;

TEST(lazy, lazy_conditional)
{
    // regular conditional behaviour
    EXPECT_TRUE(
      (std::is_same_v<bio::detail::lazy_conditional_t<true, std::true_type, std::false_type>, std::true_type>));
    EXPECT_TRUE(
      (std::is_same_v<bio::detail::lazy_conditional_t<false, std::true_type, std::false_type>, std::false_type>));

    // lazy behaviour, safe
    EXPECT_TRUE(
      (std::is_same_v<
        bio::detail::lazy_conditional_t<true, bio::detail::lazy<std::vector, int>, bio::detail::lazy<std::list, int>>,
        std::vector<int>>));
    EXPECT_TRUE(
      (std::is_same_v<
        bio::detail::lazy_conditional_t<false, bio::detail::lazy<std::vector, int>, bio::detail::lazy<std::list, int>>,
        std::list<int>>));

    // lazy behaviour, important
    EXPECT_TRUE(
      (std::is_same_v<bio::detail::lazy_conditional_t<true, bio::detail::lazy<integral_identity_t, int>, void>, int>));
    EXPECT_TRUE(
      (std::is_same_v<bio::detail::lazy_conditional_t<false, void, bio::detail::lazy<integral_identity_t, int>>, int>));
}
