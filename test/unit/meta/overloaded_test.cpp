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

#include <bio/meta/overloaded.hpp>

TEST(type_list, basic)
{
    bio::meta::overloaded fn{[](std::string const &) { return 0; }, [](std::integral auto const) { return 1; }};

    EXPECT_EQ(fn("foobar"), 0);
    EXPECT_EQ(fn(1), 1);
}
