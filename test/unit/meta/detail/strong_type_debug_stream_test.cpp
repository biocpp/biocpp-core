// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/meta/detail/strong_type.hpp>

struct my_type : bio::detail::strong_type<int, my_type>
{
    using bio::detail::strong_type<int, my_type>::strong_type;
};

TEST(strong_type, debug_stremable)
{
    my_type obj{10};

    std::ostringstream buffer{};
    bio::debug_stream_type stream{buffer};
    stream << obj;
    EXPECT_EQ(buffer.str(), "10");
}
