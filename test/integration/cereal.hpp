// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides cereal functionality for tests.
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 */

#pragma once

#include <gtest/gtest.h>

#include <fstream>

#include <bio/meta/platform.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <bio/test/tmp_filename.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

template <typename in_archive_t, typename out_archive_t, typename value_t>
void do_cerealisation(value_t && l)
{
    bio::test::tmp_filename filename{"cereal_test"};

    {
        std::ofstream os{filename.get_path(), std::ios::binary};
        out_archive_t oarchive{os};
        oarchive(l);
    }

    {
        std::remove_cvref_t<value_t> in_l{};
        std::ifstream                is{filename.get_path(), std::ios::binary};
        in_archive_t                 iarchive{is};
        iarchive(in_l);
        EXPECT_EQ(l, in_l);
    }
}

/*!\brief Tests if an object is serialise for all cereal archive types.
 * \tparam value_t The type to serialise.
 * \param l The object to serialise.
 */
template <typename value_t>
void do_serialisation(value_t && l)
{
    do_cerealisation<cereal::BinaryInputArchive, cereal::BinaryOutputArchive>(l);
    do_cerealisation<cereal::PortableBinaryInputArchive, cereal::PortableBinaryOutputArchive>(l);
    do_cerealisation<cereal::JSONInputArchive, cereal::JSONOutputArchive>(l);
    do_cerealisation<cereal::XMLInputArchive, cereal::XMLOutputArchive>(l);
}
