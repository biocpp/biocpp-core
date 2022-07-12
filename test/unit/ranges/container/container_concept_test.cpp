// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>
#include <bio/ranges/container/concept.hpp>

TEST(range_concept, forward_range)
{
    EXPECT_TRUE((std::ranges::forward_range<std::array<char, 2>>));
    EXPECT_TRUE((std::ranges::forward_range<std::list<char>>));
    EXPECT_TRUE((std::ranges::forward_range<std::forward_list<char>>)); // `.size()` missing
    EXPECT_TRUE((std::ranges::forward_range<std::vector<char>>));
    EXPECT_TRUE((std::ranges::forward_range<std::deque<char>>));
    EXPECT_TRUE((std::ranges::forward_range<std::string>));

    EXPECT_TRUE((std::ranges::forward_range<bio::ranges::concatenated_sequences<std::string>>));
    EXPECT_TRUE((std::ranges::forward_range<bio::ranges::concatenated_sequences<std::vector<char>>>));
    EXPECT_TRUE((std::ranges::forward_range<bio::ranges::bitcompressed_vector<bio::alphabet::dna4>>));
    EXPECT_TRUE(
      (std::ranges::forward_range<
        bio::ranges::bitcompressed_vector<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>>>));
}

TEST(container, container)
{
    EXPECT_TRUE((bio::ranges::detail::container<std::array<char, 2>>));
    EXPECT_TRUE((bio::ranges::detail::container<std::list<char>>));
    EXPECT_FALSE((bio::ranges::detail::container<std::forward_list<char>>)); // `.size()` missing
    EXPECT_TRUE((bio::ranges::detail::container<std::vector<char>>));
    EXPECT_TRUE((bio::ranges::detail::container<std::deque<char>>));
    EXPECT_TRUE((bio::ranges::detail::container<std::string>));

    EXPECT_TRUE((bio::ranges::detail::container<bio::ranges::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::ranges::detail::container<bio::ranges::concatenated_sequences<std::vector<char>>>));
}

TEST(container, sequence_container)
{
    EXPECT_FALSE((bio::ranges::detail::sequence_container<std::array<char, 2>>));
    EXPECT_TRUE((bio::ranges::detail::sequence_container<std::list<char>>));
    EXPECT_FALSE((bio::ranges::detail::sequence_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::ranges::detail::sequence_container<std::vector<char>>));
    EXPECT_TRUE((bio::ranges::detail::sequence_container<std::deque<char>>));
    EXPECT_TRUE((bio::ranges::detail::sequence_container<std::string>));

    EXPECT_TRUE((bio::ranges::detail::sequence_container<bio::ranges::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::ranges::detail::sequence_container<bio::ranges::concatenated_sequences<std::vector<char>>>));
}

TEST(container, random_access_container)
{
    EXPECT_FALSE((bio::ranges::detail::random_access_container<std::array<char, 2>>));
    EXPECT_FALSE((bio::ranges::detail::random_access_container<std::list<char>>));
    EXPECT_FALSE((bio::ranges::detail::random_access_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::ranges::detail::random_access_container<std::vector<char>>));
    EXPECT_TRUE((bio::ranges::detail::random_access_container<std::deque<char>>));
    EXPECT_TRUE((bio::ranges::detail::random_access_container<std::string>));

    EXPECT_TRUE((bio::ranges::detail::random_access_container<bio::ranges::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::ranges::detail::random_access_container<bio::ranges::concatenated_sequences<std::vector<char>>>));
}

TEST(container, reservible_container)
{
    EXPECT_FALSE((bio::ranges::detail::reservible_container<std::array<char, 2>>));
    EXPECT_FALSE((bio::ranges::detail::reservible_container<std::list<char>>));
    EXPECT_FALSE((bio::ranges::detail::reservible_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::ranges::detail::reservible_container<std::vector<char>>));
    EXPECT_FALSE((bio::ranges::detail::reservible_container<std::deque<char>>));
    EXPECT_TRUE((bio::ranges::detail::reservible_container<std::string>));

    EXPECT_TRUE((bio::ranges::detail::reservible_container<bio::ranges::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::ranges::detail::reservible_container<bio::ranges::concatenated_sequences<std::vector<char>>>));

    EXPECT_TRUE((bio::ranges::detail::reservible_container<bio::ranges::bitcompressed_vector<bio::alphabet::dna4>>));
    EXPECT_TRUE(
      (bio::ranges::detail::reservible_container<
        bio::ranges::bitcompressed_vector<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>>>));
}
