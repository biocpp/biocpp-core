// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <forward_list>
#include <iostream>
#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/to.hpp>
#include <bio/ranges/views/char_to.hpp>
#include <bio/test/expect_range_eq.hpp>

#include "bio/ranges/views/single_pass_input.hpp"

using namespace bio::alphabet::literals;

TEST(view_char_to, basic)
{
    std::string                      vec{"ACTTTGATA"};
    std::vector<bio::alphabet::dna5> cmp{"ACTTTGATA"_dna5};

    // pipe notation
    std::vector<bio::alphabet::dna5> v =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp, v);

    // function notation
    std::vector<bio::alphabet::dna5> v2(bio::ranges::views::char_to<bio::alphabet::dna5>(vec) |
                                        bio::ranges::to<std::vector>());
    EXPECT_EQ(cmp, v2);

    // combinability
    std::vector<bio::alphabet::dna5> cmp2{"ATAGTTTCA"_dna5};
    std::vector<bio::alphabet::dna5> v3 =
      vec | bio::ranges::views::char_to<bio::alphabet::dna5> | std::views::reverse | bio::ranges::to<std::vector>();
    EXPECT_EQ(cmp2, v3);
}

TEST(view_char_to, deep_view)
{
    std::vector<std::string> foo{"ACGTA", "TGCAT"};

    std::vector<std::vector<bio::alphabet::dna5>> v = foo | bio::ranges::views::char_to<bio::alphabet::dna5> |
                                                      bio::ranges::to<std::vector<std::vector<bio::alphabet::dna5>>>();

    ASSERT_EQ(size(v), 2u);
    EXPECT_RANGE_EQ(v[0], "ACGTA"_dna5);
    EXPECT_RANGE_EQ(v[1], "TGCAT"_dna5);
}

TEST(view_char_to, preserve_string)
{
    std::string const s{"ACTTTGATA"};

    auto v = s | bio::ranges::views::char_to<char>;

    EXPECT_EQ(v, s);
    EXPECT_TRUE((std::same_as<decltype(v), std::string_view>));
}

TEST(view_char_to, concepts)
{
    std::string vec{"ACTTTGATA"};
    EXPECT_TRUE(std::ranges::input_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(vec)>);
    EXPECT_FALSE(std::ranges::view<decltype(vec)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(vec)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(vec)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(vec)>);
    EXPECT_TRUE((std::ranges::output_range<decltype(vec), char>));

    auto v1 = vec | bio::ranges::views::char_to<bio::alphabet::dna5>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v1)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::view<decltype(v1)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v1)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v1)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v1)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), bio::alphabet::dna5>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v1), char>));

    auto v2 = vec | bio::ranges::views::char_to<char>;
    EXPECT_TRUE(std::ranges::input_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::random_access_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::contiguous_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::view<decltype(v2)>);
    EXPECT_TRUE(std::ranges::sized_range<decltype(v2)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v2)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v2)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v2), bio::alphabet::dna5>));
    EXPECT_TRUE((std::ranges::output_range<decltype(v2), char>));
}

TEST(view_char_to, exception)
{
    std::string foo = "ACGPTA";

    auto v = foo | bio::ranges::views::char_to<bio::alphabet::dna5>;
    EXPECT_NO_THROW((std::ranges::equal(v, "ACGNTA"_dna5)));
}

// --------------------------------------------------
// cigar special case
// --------------------------------------------------

TEST(view_char_to_cigar, typical)
{
    std::string                       s{"42I43M44D"};
    std::vector<bio::alphabet::cigar> comp{
      {42, 'I'_cigar_op},
      {43, 'M'_cigar_op},
      {44, 'D'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::view<decltype(v)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));
}

TEST(view_char_to_cigar, typical_reverse)
{
    std::string                       s{"42I43M44D"};
    std::vector<bio::alphabet::cigar> comp{
      {44, 'D'_cigar_op},
      {43, 'M'_cigar_op},
      {42, 'I'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar> | std::views::reverse;

    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::view<decltype(v)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));
}

TEST(view_char_to_cigar, typical_const)
{
    std::string                       s{"42I43M44D"};
    std::vector<bio::alphabet::cigar> comp{
      {42, 'I'_cigar_op},
      {43, 'M'_cigar_op},
      {44, 'D'_cigar_op}
    };

    auto const v = s | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::view<decltype(v)>); // const views aren't views o_O
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));

    auto const it = v.begin();
    EXPECT_EQ(*it, (bio::alphabet::cigar{42, 'I'_cigar_op}));
}

TEST(view_char_to_cigar, empty)
{
    std::vector<bio::alphabet::cigar> comp{};

    auto v = std::string_view{} | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_RANGE_EQ(v, comp);

    auto v2 = v | std::views::reverse;
    EXPECT_RANGE_EQ(v2, comp);
}

TEST(view_char_to_cigar, non_common)
{
    auto s = std::string_view{"42I43M44D"} | std::views::take_while([](char) { return true; });
    EXPECT_FALSE(std::ranges::common_range<decltype(s)>);

    std::vector<bio::alphabet::cigar> comp{
      {42, 'I'_cigar_op},
      {43, 'M'_cigar_op},
      {44, 'D'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::view<decltype(v)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::common_range<decltype(v)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));
}

TEST(view_char_to_cigar, non_common_reverse)
{
    auto s = std::string_view{"42I43M44D"} | std::views::take_while([](char) { return true; });
    std::vector<bio::alphabet::cigar> comp{
      {44, 'D'_cigar_op},
      {43, 'M'_cigar_op},
      {42, 'I'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar> | std::views::reverse;

    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::view<decltype(v)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v)>);          // always common apparently
    EXPECT_FALSE(bio::ranges::const_iterable_range<decltype(v)>); // reverse has to cache begin() now
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));
}

TEST(view_char_to_cigar, forward_only)
{
    std::forward_list<char> s{'4', '2', 'I', '4', '3', 'M', '4', '4', 'D'};

    std::vector<bio::alphabet::cigar> comp{
      {42, 'I'_cigar_op},
      {43, 'M'_cigar_op},
      {44, 'D'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_RANGE_EQ(v, comp);

    EXPECT_TRUE(std::ranges::input_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::forward_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::bidirectional_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::random_access_range<decltype(v)>);
    EXPECT_FALSE(std::ranges::contiguous_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::view<decltype(v)>);
    EXPECT_FALSE(std::ranges::sized_range<decltype(v)>);
    EXPECT_TRUE(std::ranges::common_range<decltype(v)>);
    EXPECT_TRUE(bio::ranges::const_iterable_range<decltype(v)>);
    EXPECT_FALSE((std::ranges::output_range<decltype(v), bio::alphabet::cigar>));
    EXPECT_FALSE((std::ranges::output_range<decltype(v), char>));
}

TEST(view_char_to_cigar, error1)
{
    std::string                       s{"42i43m44d"};
    std::vector<bio::alphabet::cigar> comp{
      {42, 'I'_cigar_op},
      {43, 'M'_cigar_op},
      {44, 'D'_cigar_op}
    };

    auto v = s | bio::ranges::views::char_to<bio::alphabet::cigar>;
    EXPECT_NO_THROW(v.begin());
    EXPECT_THROW(*v.begin(), bio::alphabet::invalid_char_assignment);
}
