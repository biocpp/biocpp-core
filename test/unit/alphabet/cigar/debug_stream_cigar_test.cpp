// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/cigar/cigar.hpp>

TEST(debug_stream_test, cigar)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    bio::cigar c1{};
    c1.assign_string("223M");

    my_stream << c1;

    o.flush();
    EXPECT_EQ(o.str().size(), 4u);
    EXPECT_EQ(o.str(), "223M");
}
