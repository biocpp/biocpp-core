// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <list>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include <bio/meta/tag/ttag.hpp>

TEST(ttag, simple)
{
    auto v = bio::meta::ttag<int>;
    EXPECT_TRUE((std::same_as<decltype(v), bio::meta::type_list<int>>));

    auto v2 = bio::meta::ttag<int, float>;
    EXPECT_TRUE((std::same_as<decltype(v2), bio::meta::type_list<int, float>>));
}
