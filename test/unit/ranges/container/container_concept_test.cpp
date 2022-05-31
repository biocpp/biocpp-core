// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

#include <sdsl/int_vector.hpp>

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

    EXPECT_TRUE((std::ranges::forward_range<bio::concatenated_sequences<std::string>>));
    EXPECT_TRUE((std::ranges::forward_range<bio::concatenated_sequences<std::vector<char>>>));
    EXPECT_TRUE((std::ranges::forward_range<bio::bitcompressed_vector<bio::dna4>>));
    EXPECT_TRUE((std::ranges::forward_range<bio::bitcompressed_vector<bio::qualified<bio::dna4,
                                                                                           bio::phred42>>>));
}

TEST(container, container)
{
    EXPECT_TRUE((bio::container<std::array<char, 2>>));
    EXPECT_TRUE((bio::container<std::list<char>>));
    EXPECT_FALSE((bio::container<std::forward_list<char>>)); // `.size()` missing
    EXPECT_TRUE((bio::container<std::vector<char>>));
    EXPECT_TRUE((bio::container<std::deque<char>>));
    EXPECT_TRUE((bio::container<std::string>));

    EXPECT_TRUE((bio::container<bio::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::container<bio::concatenated_sequences<std::vector<char>>>));
}

TEST(container, sequence_container_former_travis_bug)
{
    // This tests a bug with const iterators on strings which was there in a
    // special gcc 7.2 build (ppa) for ubuntu 14.04 and 16.04. We leave it as
    // a regression test.
    // see https://github.com/seqan/seqan3/pull/113/
    using namespace std::string_literals;

    // example code from https://en.cppreference.com/w/cpp/string/basic_string/insert
    std::string s = "xmplr";

    // insert(size_type index, size_type count, char ch)
    s.insert(0, 1, 'E');
    EXPECT_EQ("Exmplr", s);

    // insert(size_type index, const char* s)
    s.insert(2, "e");
    EXPECT_EQ("Exemplr", s);

    // insert(size_type index, string const& str)
    s.insert(6, "a"s);
    EXPECT_EQ("Exemplar", s);

    // insert(size_type index, string const& str, size_type index_str, size_type count)
    s.insert(8, " is an example string."s, 0, 14);
    EXPECT_EQ("Exemplar is an example", s);

    // insert(const_iterator pos, char ch)
    s.insert(s.cbegin() + s.find_first_of('n') + 1, ':');
    EXPECT_EQ("Exemplar is an: example", s);

    // insert(const_iterator pos, size_type count, char ch)
    s.insert(s.cbegin() + s.find_first_of(':') + 1, 2, '=');
    EXPECT_EQ("Exemplar is an:== example", s);

    // insert(const_iterator pos, InputIt first, InputIt last)
    {
        std::string seq = " string";
        s.insert(s.cbegin() + s.find_last_of('e') + 1,
            std::begin(seq), std::end(seq));
        EXPECT_EQ("Exemplar is an:== example string", s);
    }

    // insert(const_iterator pos, std::initializer_list<char>)
    s.insert(s.cbegin() + s.find_first_of('g') + 1, { '.' });
    EXPECT_EQ("Exemplar is an:== example string.", s);
}

TEST(container, sequence_container)
{
    EXPECT_FALSE((bio::sequence_container<std::array<char, 2>>));
    EXPECT_TRUE((bio::sequence_container<std::list<char>>));
    EXPECT_FALSE((bio::sequence_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::sequence_container<std::vector<char>>));
    EXPECT_TRUE((bio::sequence_container<std::deque<char>>));
    EXPECT_TRUE((bio::sequence_container<std::string>));

    EXPECT_TRUE((bio::sequence_container<bio::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::sequence_container<bio::concatenated_sequences<std::vector<char>>>));
}

TEST(container, random_access_container)
{
    EXPECT_FALSE((bio::random_access_container<std::array<char, 2>>));
    EXPECT_FALSE((bio::random_access_container<std::list<char>>));
    EXPECT_FALSE((bio::random_access_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::random_access_container<std::vector<char>>));
    EXPECT_TRUE((bio::random_access_container<std::deque<char>>));
    EXPECT_TRUE((bio::random_access_container<std::string>));

    EXPECT_TRUE((bio::random_access_container<bio::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::random_access_container<bio::concatenated_sequences<std::vector<char>>>));
}

TEST(container, reservible_container)
{
    EXPECT_FALSE((bio::reservible_container<std::array<char, 2>>));
    EXPECT_FALSE((bio::reservible_container<std::list<char>>));
    EXPECT_FALSE((bio::reservible_container<std::forward_list<char>>));
    EXPECT_TRUE((bio::reservible_container<std::vector<char>>));
    EXPECT_FALSE((bio::reservible_container<std::deque<char>>));
    EXPECT_TRUE((bio::reservible_container<std::string>));

    EXPECT_TRUE((bio::reservible_container<bio::concatenated_sequences<std::string>>));
    EXPECT_TRUE((bio::reservible_container<bio::concatenated_sequences<std::vector<char>>>));

    EXPECT_TRUE((bio::reservible_container<sdsl::bit_vector>));
    EXPECT_TRUE((bio::reservible_container<sdsl::int_vector<>>));
    EXPECT_TRUE((bio::reservible_container<sdsl::int_vector<13>>));
    EXPECT_TRUE((bio::reservible_container<sdsl::int_vector<64>>));
    EXPECT_TRUE((bio::reservible_container<bio::bitcompressed_vector<bio::dna4>>));
    EXPECT_TRUE((bio::reservible_container<bio::bitcompressed_vector<bio::qualified<bio::dna4,
                                                                                             bio::phred42>>>));
}

/* Check the SDSL containers */
//TODO

/* Check our containers */
//TODO
