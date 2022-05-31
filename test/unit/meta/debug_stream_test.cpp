// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <iostream>
#include <string>

#include <bio/alphabet/mask/mask.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/meta/debug_stream.hpp>
#include <bio/ranges/container/bitcompressed_vector.hpp>
#include <bio/ranges/container/concatenated_sequences.hpp>
#include <filesystem>

TEST(debug_stream_test, basic)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << 'a';
    o.flush();
    EXPECT_EQ(o.str(), "a");

    my_stream << "AGA";
    o.flush();
    EXPECT_EQ(o.str(), "aAGA");

    my_stream << 42;
    o.flush();
    EXPECT_EQ(o.str(), "aAGA42");

    int const i = 7;
    my_stream << i;
    o.flush();
    EXPECT_EQ(o.str(), "aAGA427");
}

TEST(debug_stream_test, capture_std_error)
{
    testing::internal::CaptureStderr();
    bio::debug_stream << 'a';
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "a");

    testing::internal::CaptureStderr();
    bio::debug_stream << "AGA";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "AGA");

    testing::internal::CaptureStderr();
    bio::debug_stream << 42;
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "42");

    testing::internal::CaptureStderr();
    int const i = 7;
    bio::debug_stream << i;
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "7");
}

TEST(debug_stream_test, range)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    std::vector<int> vec{1, 4, 5, 7, 32, 321};
    my_stream << vec;
    o.flush();
    EXPECT_EQ(o.str(), "[1,4,5,7,32,321]");

    std::vector<std::vector<int>> const vec2 = {{1, 2, 33}, {22,11}};
    my_stream << vec2;
    o.flush();
    EXPECT_EQ(o.str(), "[1,4,5,7,32,321][[1,2,33],[22,11]]");
}

TEST(debug_stream_test, alphabet)
{
    using bio::operator""_dna4;
    using bio::operator""_dna5;

    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << 'A'_dna4;
    o.flush();
    EXPECT_EQ(o.str(), "A");

    bio::dna5 d = 'N'_dna5;
    my_stream << d;
    o.flush();
    EXPECT_EQ(o.str(), "AN");

    bio::dna5 const d2 = 'N'_dna5;
    my_stream << d2;
    o.flush();
    EXPECT_EQ(o.str(), "ANN");
}

TEST(debug_stream_test, mask_semialphabet)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << bio::mask::MASKED;
    o.flush();
    EXPECT_EQ(o.str(), "MASKED");

    my_stream << bio::mask::UNMASKED;
    o.flush();
    EXPECT_EQ(o.str(), "MASKEDUNMASKED");
}

TEST(debug_stream_test, range_of_alphabet)
{
    using bio::operator""_dna4;
    using bio::operator""_dna5;

    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    // temporary
    my_stream << "AGGATAC"_dna5;
    o.flush();
    EXPECT_EQ(o.str(), "AGGATAC");

    // lvalue ref
    auto d = "AGGATAC"_dna5;
    my_stream << d;
    o.flush();
    EXPECT_EQ(o.str(), "AGGATACAGGATAC");

    // lvalue const ref
    auto const d2 = "AGGATAC"_dna5;
    my_stream << d2;
    o.flush();
    EXPECT_EQ(o.str(), "AGGATACAGGATACAGGATAC");

    bio::concatenated_sequences<bio::bitcompressed_vector<bio::dna5>> const vec2 = {"ACGT"_dna5, "GAGGA"_dna5};
    my_stream << vec2;
    o.flush();
    EXPECT_EQ(o.str(), "AGGATACAGGATACAGGATAC[ACGT,GAGGA]");
}

TEST(debug_stream_test, std_endl)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    my_stream << "foo" << std::endl << "bar";
    o.flush();
    EXPECT_EQ(o.str(), "foo\nbar");
}

TEST(debug_stream_test, path)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    std::filesystem::path p{"my/path/my_file.txt"};

    my_stream << p;
    o.flush();
    EXPECT_EQ(o.str(), "\"my/path/my_file.txt\"");
}

TEST(debug_stream_test, tuple)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    std::tuple<size_t, std::string> t0{32, "dummy"};
    my_stream << t0;
    o.flush();
    EXPECT_EQ(o.str(), "(32,dummy)");

    std::tuple<size_t> t1{32};
    my_stream << t1;
    o.flush();
    EXPECT_EQ(o.str(), "(32,dummy)(32)");

    std::tuple<size_t, std::pair<size_t, size_t>> t2{2, {3,2}};
    my_stream << t2;
    o.flush();
    EXPECT_EQ(o.str(), "(32,dummy)(32)(2,(3,2))");
}

TEST(debug_stream_test, variant)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    std::variant<double, std::string> v;

    v = double{3.3};
    my_stream << v;
    o.flush();
    EXPECT_EQ(o.str(), "3.3");

    v = std::string{"foobar"};
    my_stream << v;
    o.flush();
    EXPECT_EQ(o.str(), "3.3foobar");

    /*const*/
    std::variant<double, std::string> const v2{double{4.2}};
    my_stream << v2;
    o.flush();
    EXPECT_EQ(o.str(), "3.3foobar4.2");

    /*rvalue*/
    my_stream << std::variant<double, std::string>{std::string{"tmp"}};
    o.flush();
    EXPECT_EQ(o.str(), "3.3foobar4.2tmp");
}

TEST(debug_stream_test, optional)
{
    std::ostringstream o;
    bio::debug_stream_type my_stream{o};

    std::optional<size_t> op;

    my_stream << op;
    o.flush();
    EXPECT_EQ(o.str(), "<VALUELESS_OPTIONAL>");

    op = 3u;

    my_stream << op;
    o.flush();
    EXPECT_EQ(o.str(), "<VALUELESS_OPTIONAL>3");
}

