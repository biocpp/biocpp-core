// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <ranges>

#include <gtest/gtest.h>

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/ranges/concept.hpp>
#include <bio/ranges/container/dictionary.hpp>
#include <bio/test/expect_range_eq.hpp>

using namespace bio::alphabet::literals;

using TypeParam = bio::ranges::dictionary<std::string, std::vector<bio::alphabet::dna4>>;
using value_t   = TypeParam::value_type;

inline std::vector<bio::alphabet::dna4> mapped  = "GATTACA"_dna4;
inline std::vector<bio::alphabet::dna4> mapped2 = "TATAGAG"_dna4;

TEST(dictionary_test, concepts)
{
    EXPECT_TRUE(std::ranges::random_access_range<TypeParam>);
    EXPECT_FALSE(std::ranges::contiguous_range<TypeParam>); // reference != value_type &
    EXPECT_TRUE(std::ranges::sized_range<TypeParam>);
    EXPECT_TRUE(std::ranges::common_range<TypeParam>);

    EXPECT_FALSE((std::ranges::output_range<TypeParam, value_t>)); // can't assign to reference
    EXPECT_TRUE((bio::ranges::back_insertable<TypeParam>));        // but can insert
    EXPECT_TRUE((bio::ranges::back_emplaceable_with<TypeParam, std::string, std::vector<bio::alphabet::dna4>>));
    EXPECT_TRUE(
      (bio::ranges::back_emplaceable_with<TypeParam, std::string const &, std::vector<bio::alphabet::dna4> &>));
}

TEST(dictionary_test, construction)
{
    TypeParam t1;
    TypeParam t2{};
    EXPECT_EQ(t1, t2);

    // from elements (pack)
    TypeParam t3{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    // the following does not work, because we don't offer initializer_lists construction
    // TypeParam t4 = {value_t{"A", mapped}, value_t{"B", mapped}, value_t{"C", mapped}, value_t{"D", mapped}};
    // EXPECT_EQ(t3, t4);

    // from another TypeParam's sub-range
    TypeParam t6{t3.begin() + 1, t3.begin() + 3};
    EXPECT_EQ(t6,
              (TypeParam{
                value_t{"B", mapped},
                value_t{"C", mapped},
    }));

    // direct from another container
    TypeParam t7{t3};
    EXPECT_EQ(t3, t7);
}

TEST(dictionary_test, swapping)
{
    TypeParam t0{};
    TypeParam t1{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    std::swap(t0, t1);
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped},
                value_t{"B", mapped},
                value_t{"C", mapped},
                value_t{"D", mapped}
    }));
    EXPECT_EQ(t1, (TypeParam{}));

    swap(t0, t1);
    EXPECT_EQ(t0, (TypeParam{}));
    EXPECT_EQ(t1,
              (TypeParam{
                value_t{"A", mapped},
                value_t{"B", mapped},
                value_t{"C", mapped},
                value_t{"D", mapped}
    }));
}

TEST(dictionary_test, assign)
{
    TypeParam t0{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };
    TypeParam t1{
      value_t{"E", mapped},
      value_t{"F", mapped}
    };

    // from another container's sub-range
    TypeParam t4;
    t4.assign(t1.cbegin(), t1.cend());
    EXPECT_EQ(t4, t1);

    // from elements
    TypeParam t5;
    t5.assign(value_t{"A", mapped}, value_t{"B", mapped}, value_t{"C", mapped}, value_t{"D", mapped});
    EXPECT_EQ(t5, t0);

    // direct from another container
    std::vector<value_t> vec{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };
    TypeParam t7;
    t7.assign(vec);
    EXPECT_EQ(t7, t0);

    // assign fail with duplicate keys
    EXPECT_THROW((t7.assign(value_t{"A", mapped}, value_t{"A", mapped})), std::runtime_error);
}

TEST(dictionary_test, iterators)
{
    TypeParam t1{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };
    TypeParam const t2{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    // begin
    EXPECT_EQ(*t1.begin(), (value_t{"A", mapped}));
    EXPECT_EQ(*t1.cbegin(), (value_t{"A", mapped}));
    EXPECT_EQ(*t2.begin(), (value_t{"A", mapped}));
    EXPECT_EQ(*t2.cbegin(), (value_t{"A", mapped}));

    // end and arithmetic
    EXPECT_EQ(*(t1.end() - 1), (value_t{"D", mapped}));
    EXPECT_EQ(*(t1.cend() - 1), (value_t{"D", mapped}));
    EXPECT_EQ(*(t2.end() - 1), (value_t{"D", mapped}));
    EXPECT_EQ(*(t2.cend() - 1), (value_t{"D", mapped}));

    // convertibility between const and non-const
    EXPECT_TRUE(t1.cend() == t1.end());

    // only mapped value is mutable
    get<1>(*t1.begin()) = mapped2;
    EXPECT_RANGE_EQ(get<1>(*t1.begin()), mapped2);
}

TEST(dictionary_test, element_access)
{
    TypeParam t1{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };
    TypeParam const t2{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    // at
    EXPECT_EQ(t1.at(0), (value_t{"A", mapped}));
    EXPECT_EQ(t2.at(0), (value_t{"A", mapped}));
    EXPECT_THROW(t1.at(20), std::out_of_range);
    EXPECT_THROW(t2.at(20), std::out_of_range);

    // []
    EXPECT_EQ(t1[1], (value_t{"B", mapped}));
    EXPECT_EQ(t2[1], (value_t{"B", mapped}));

    // front
    EXPECT_EQ(t1.front(), (value_t{"A", mapped}));
    EXPECT_EQ(t2.front(), (value_t{"A", mapped}));

    // back
    EXPECT_EQ(t1.back(), (value_t{"D", mapped}));
    EXPECT_EQ(t2.back(), (value_t{"D", mapped}));

    // mutability
    get<1>(t1[0]) = mapped2;
    EXPECT_RANGE_EQ(get<1>(t1[0]), mapped2);

    get<1>(t1.front()) = mapped2;
    EXPECT_RANGE_EQ(get<1>(t1.front()), mapped2);

    get<1>(t1.back()) = mapped2;
    EXPECT_RANGE_EQ(get<1>(t1.back()), mapped2);

    get<1>(t1.begin()[2]) = mapped2;
    EXPECT_RANGE_EQ(get<1>(t1.begin()[2]), mapped2);

    get<1>(t1.begin()[1]) = mapped2;
    EXPECT_RANGE_EQ(get<1>(t1.begin()[1]), mapped2);

    EXPECT_RANGE_EQ(t1,
                    (TypeParam{
                      value_t{"A", mapped2},
                      value_t{"B", mapped2},
                      value_t{"C", mapped2},
                      value_t{"D", mapped2}
    }));
}

TEST(dictionary_test, element_access_by_key)
{
    TypeParam t1{
      value_t{"A",  mapped},
      value_t{"B", mapped2},
      value_t{"C",  mapped},
      value_t{"D", mapped2}
    };
    TypeParam const t2{
      value_t{"A", mapped2},
      value_t{"B",  mapped},
      value_t{"C", mapped2},
      value_t{"D",  mapped}
    };

    /* contains */
    EXPECT_TRUE(t1.contains("A"));
    EXPECT_TRUE(t2.contains("A"));
    EXPECT_FALSE(t1.contains("Z"));
    EXPECT_FALSE(t2.contains("Z"));

    /* count */
    EXPECT_EQ(t1.count("A"), 1ul);
    EXPECT_EQ(t2.count("A"), 1ul);
    EXPECT_EQ(t1.count("Z"), 0ul);
    EXPECT_EQ(t2.count("Z"), 0ul);

    /* find */
    EXPECT_EQ(t1.find("A"), t1.begin());
    EXPECT_EQ(t2.find("A"), t2.begin());
    EXPECT_EQ(t1.find("B"), t1.begin() + 1);
    EXPECT_EQ(t2.find("B"), t2.begin() + 1);
    EXPECT_EQ(t1.find("Z"), t1.end());
    EXPECT_EQ(t2.find("Z"), t2.end());

    /* at */
    EXPECT_EQ(t1.at("A"), mapped);
    EXPECT_EQ(t2.at("A"), mapped2);
    EXPECT_EQ(t1.at("B"), mapped2);
    EXPECT_EQ(t2.at("B"), mapped);
    EXPECT_THROW(t1.at("Z"), std::out_of_range);
    EXPECT_THROW(t2.at("Z"), std::out_of_range);

    /* operator[] */
    EXPECT_EQ(t1["A"], mapped);
    EXPECT_EQ(t2["A"], mapped2);
    EXPECT_EQ(t1["B"], mapped2);
    EXPECT_EQ(t2["B"], mapped);
    EXPECT_THROW(t1["Z"], std::out_of_range);
    EXPECT_THROW(t2["Z"], std::out_of_range);
}

TEST(dictionary_test, capacity)
{
    TypeParam t0{};
    TypeParam t1{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };
    TypeParam const t2{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    // empty
    EXPECT_TRUE(t0.empty());
    EXPECT_FALSE(t1.empty());
    EXPECT_FALSE(t2.empty());

    // size
    EXPECT_EQ(t0.size(), 0u);
    EXPECT_EQ(t1.size(), 4u);
    EXPECT_EQ(t2.size(), 4u);

    // capacity
    EXPECT_GE(t0.capacity(), t0.size());
    EXPECT_GE(t1.capacity(), t1.size());
    EXPECT_GE(t2.capacity(), t2.size());

    // max_size
    EXPECT_GT(t0.max_size(), 1'000'000'000'000u);
    EXPECT_GT(t1.max_size(), 1'000'000'000'000u);
    EXPECT_GT(t2.max_size(), 1'000'000'000'000u);

    // reserve
    EXPECT_LT(t0.capacity(), 1000u);
    t0.reserve(1000);
    EXPECT_GE(t0.capacity(), 1000u);

    // shrink_to_fit
    t1.reserve(1000);
    EXPECT_GT(t1.capacity(), t1.size() * 2);
    t1.shrink_to_fit();
    EXPECT_LE(t1.capacity(), std::max<size_t>(t1.size() * 2, 32ul));
}

TEST(dictionary_test, clear)
{
    TypeParam t0{};
    TypeParam t1{
      value_t{"A", mapped},
      value_t{"B", mapped},
      value_t{"C", mapped},
      value_t{"D", mapped}
    };

    t1.clear();
    EXPECT_EQ(t0, t1);
}

TEST(dictionary_test, push_pop)
{
    TypeParam t0{};

    /* push_back */
    t0.push_back(value_t{"A", mapped});
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped}
    }));
    t0.push_back(value_t{"C", mapped});
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped},
                value_t{"C", mapped}
    }));

    // duplicate ID
    EXPECT_THROW((t0.push_back(value_t{"A", mapped})), std::runtime_error);

    /* pop_back */
    t0.pop_back();
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped}
    }));
    t0.pop_back();
    EXPECT_EQ(t0, (TypeParam{}));

    /* emplace_back */
    t0.emplace_back("A", mapped);
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped}
    }));
    t0.emplace_back("C", mapped);
    EXPECT_EQ(t0,
              (TypeParam{
                value_t{"A", mapped},
                value_t{"C", mapped}
    }));

    // duplicate ID
    EXPECT_THROW((t0.emplace_back("A", mapped)), std::runtime_error);
}
