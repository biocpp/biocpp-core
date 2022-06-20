// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <array>
#include <vector>

#include <bio/meta/concept/cereal.hpp>

#if BIOCPP_WITH_CEREAL
#    include <cereal/archives/binary.hpp>
#    include <cereal/archives/json.hpp>
#    include <cereal/archives/portable_binary.hpp>
#    include <cereal/archives/xml.hpp>

#    include <cereal/types/array.hpp>
#endif

#if BIOCPP_WITH_CEREAL

TEST(cereal, cereal_output_archive)
{
    EXPECT_TRUE((bio::cereal_output_archive<cereal::XMLOutputArchive>));
    EXPECT_TRUE((bio::cereal_output_archive<cereal::JSONOutputArchive>));
    EXPECT_TRUE((bio::cereal_output_archive<cereal::BinaryOutputArchive>));
    EXPECT_TRUE((bio::cereal_output_archive<cereal::PortableBinaryOutputArchive>));
    EXPECT_FALSE((bio::cereal_output_archive<cereal::XMLInputArchive>));
    EXPECT_FALSE((bio::cereal_output_archive<cereal::JSONInputArchive>));
    EXPECT_FALSE((bio::cereal_output_archive<cereal::BinaryInputArchive>));
    EXPECT_FALSE((bio::cereal_output_archive<cereal::PortableBinaryInputArchive>));
}

TEST(cereal, cereal_input_archive)
{
    EXPECT_FALSE((bio::cereal_input_archive<cereal::XMLOutputArchive>));
    EXPECT_FALSE((bio::cereal_input_archive<cereal::JSONOutputArchive>));
    EXPECT_FALSE((bio::cereal_input_archive<cereal::BinaryOutputArchive>));
    EXPECT_FALSE((bio::cereal_input_archive<cereal::PortableBinaryOutputArchive>));
    EXPECT_TRUE((bio::cereal_input_archive<cereal::XMLInputArchive>));
    EXPECT_TRUE((bio::cereal_input_archive<cereal::JSONInputArchive>));
    EXPECT_TRUE((bio::cereal_input_archive<cereal::BinaryInputArchive>));
    EXPECT_TRUE((bio::cereal_input_archive<cereal::PortableBinaryInputArchive>));
}

TEST(cereal, cereal_archive)
{
    EXPECT_TRUE((bio::cereal_archive<cereal::XMLOutputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::JSONOutputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::BinaryOutputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::PortableBinaryOutputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::XMLInputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::JSONInputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::BinaryInputArchive>));
    EXPECT_TRUE((bio::cereal_archive<cereal::PortableBinaryInputArchive>));
}

TEST(cereal, cereal_text_archive)
{
    EXPECT_TRUE((bio::cereal_text_archive<cereal::XMLOutputArchive>));
    EXPECT_TRUE((bio::cereal_text_archive<cereal::JSONOutputArchive>));
    EXPECT_FALSE((bio::cereal_text_archive<cereal::BinaryOutputArchive>));
    EXPECT_FALSE((bio::cereal_text_archive<cereal::PortableBinaryOutputArchive>));
    EXPECT_TRUE((bio::cereal_text_archive<cereal::XMLInputArchive>));
    EXPECT_TRUE((bio::cereal_text_archive<cereal::JSONInputArchive>));
    EXPECT_FALSE((bio::cereal_text_archive<cereal::BinaryInputArchive>));
    EXPECT_FALSE((bio::cereal_text_archive<cereal::PortableBinaryInputArchive>));
}

struct my_struct
{};

TEST(cereal, cerealisable)
{
    EXPECT_TRUE((bio::cerealisable<int>));
    EXPECT_TRUE((bio::cerealisable<float>));

    // my_struct does not define any serialise functions
    EXPECT_FALSE((bio::cerealisable<my_struct>));

    // will be true, since <cereal/types/array.hpp> is included
    EXPECT_TRUE((bio::cerealisable<std::array<int, 10>>));
    // is false, because <cereal/types/vector.hpp> is not included
    EXPECT_FALSE((bio::cerealisable<std::vector<int>>));

    // recursive containers of cerealisable value types work
    EXPECT_TRUE((bio::cerealisable<std::array<std::array<int, 10>, 10>>));
}

#endif
