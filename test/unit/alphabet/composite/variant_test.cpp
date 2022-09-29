// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <variant>

#include <gtest/gtest.h>

#include <bio/alphabet/adaptation/char.hpp>
#include <bio/alphabet/composite/variant.hpp>
#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/meta/type_list/function.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

using namespace bio::alphabet::literals;

using variant_types =
  ::testing::Types<bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::gap>,
                   bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>,
                   bio::alphabet::variant<char, bio::alphabet::gap>>;

INSTANTIATE_TYPED_TEST_SUITE_P(variant, alphabet, variant_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(variant, semi_alphabet_test, variant_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(variant, alphabet_constexpr, variant_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(variant, semi_alphabet_constexpr, variant_types, );

template <typename T>
using variant_test = ::testing::Test;

TYPED_TEST_SUITE(variant_test, variant_types, );

TEST(variant_test, initialise_from_component_alphabet)
{
    bio::alphabet::dna5 l('A'_rna5);

    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using variant_t  = std::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;

    constexpr variant_t  variant0{'A'_dna4};
    constexpr alphabet_t letter0{'A'_dna4};

    constexpr variant_t  variant1 = 'C'_dna4;
    constexpr alphabet_t letter1  = 'C'_dna4;

    constexpr variant_t  variant2 = {'G'_dna4};
    constexpr alphabet_t letter2  = {'G'_dna4};

    constexpr variant_t  variant3 = static_cast<variant_t>('T'_dna4);
    constexpr alphabet_t letter3  = static_cast<alphabet_t>('T'_dna4);

    constexpr variant_t  variant4 = {static_cast<variant_t>('A'_dna5)};
    constexpr alphabet_t letter4  = {static_cast<alphabet_t>('A'_dna5)};

    variant_t  variant5{'C'_dna5};
    alphabet_t letter5{'C'_dna5};

    variant_t  variant6 = 'G'_dna5;
    alphabet_t letter6  = 'G'_dna5;

    variant_t  variant7 = {'N'_dna5};
    alphabet_t letter7  = {'N'_dna5};

    variant_t  variant8 = static_cast<variant_t>('T'_dna5);
    alphabet_t letter8  = static_cast<alphabet_t>('T'_dna5);

    variant_t  variant9 = {static_cast<variant_t>(bio::alphabet::gap{})};
    alphabet_t letter9  = {static_cast<alphabet_t>(bio::alphabet::gap{})};

    EXPECT_EQ(letter0.to_rank(), 0);
    EXPECT_EQ(letter1.to_rank(), 1);
    EXPECT_EQ(letter2.to_rank(), 2);
    EXPECT_EQ(letter3.to_rank(), 3);
    EXPECT_EQ(letter4.to_rank(), 4);
    EXPECT_EQ(letter5.to_rank(), 5);
    EXPECT_EQ(letter6.to_rank(), 6);
    EXPECT_EQ(letter7.to_rank(), 7);
    EXPECT_EQ(letter8.to_rank(), 8);
    EXPECT_EQ(letter9.to_rank(), 9);
}

TEST(variant_test, initialise_from_component_alphabet_subtype)
{
    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using variant_t  = std::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;

    variant_t  variant0{'A'_rna4};
    alphabet_t letter0{'A'_rna4};

    variant_t  variant1 = 'C'_rna4;
    alphabet_t letter1  = 'C'_rna4;

    variant_t  variant2 = {'G'_rna4};
    alphabet_t letter2  = {'G'_rna4};

    variant_t  variant3 = static_cast<variant_t>('T'_rna4);
    alphabet_t letter3  = static_cast<alphabet_t>('T'_rna4);

    variant_t  variant4 = {static_cast<variant_t>('A'_rna5)};
    alphabet_t letter4  = {static_cast<alphabet_t>('A'_rna5)};

    variant_t  variant5{'C'_rna5};
    alphabet_t letter5{'C'_rna5};

    variant_t  variant6 = 'G'_rna5;
    alphabet_t letter6  = 'G'_rna5;

    variant_t  variant7 = {'N'_rna5};
    alphabet_t letter7  = {'N'_rna5};

    variant_t  variant8 = static_cast<variant_t>('T'_rna5);
    alphabet_t letter8  = static_cast<alphabet_t>('T'_rna5);

    EXPECT_EQ(letter0.to_rank(), 0);
    EXPECT_EQ(letter1.to_rank(), 1);
    EXPECT_EQ(letter2.to_rank(), 2);
    EXPECT_EQ(letter3.to_rank(), 3);
    EXPECT_EQ(letter4.to_rank(), 4);
    EXPECT_EQ(letter5.to_rank(), 5);
    EXPECT_EQ(letter6.to_rank(), 6);
    EXPECT_EQ(letter7.to_rank(), 7);
    EXPECT_EQ(letter8.to_rank(), 8);
}

TEST(variant_test, assign_from_component_alphabet)
{
    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using variant_t  = std::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    alphabet_t letter{};
    variant_t  variant{};

    variant = 'A'_dna4;
    letter  = 'A'_dna4;
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 0);

    variant = {'C'_dna4};
    letter  = {'C'_dna4};
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 1);

    variant = static_cast<variant_t>('G'_dna4);
    letter  = static_cast<alphabet_t>('G'_dna4);
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 2);

    variant = {static_cast<variant_t>('T'_dna4)};
    letter  = {static_cast<alphabet_t>('T'_dna4)};
    EXPECT_EQ(letter.to_rank(), 3);

    letter = 'A'_dna5;
    EXPECT_EQ(letter.to_rank(), 4);

    letter = 'C'_dna5;
    EXPECT_EQ(letter.to_rank(), 5);

    letter = 'G'_dna5;
    EXPECT_EQ(letter.to_rank(), 6);

    letter = 'N'_dna5;
    EXPECT_EQ(letter.to_rank(), 7);

    letter = 'T'_dna5;
    EXPECT_EQ(letter.to_rank(), 8);

    letter = bio::alphabet::gap{};
    EXPECT_EQ(letter.to_rank(), 9);
}

TEST(variant_test, assign_from_component_alphabet_subtype)
{
    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using variant_t  = std::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    alphabet_t letter{};
    variant_t  variant{};

    variant = 'A'_rna4;
    letter  = 'A'_rna4;
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 0);

    variant = {'C'_rna4};
    letter  = {'C'_rna4};
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 1);

    variant = static_cast<variant_t>('G'_rna4);
    letter  = static_cast<alphabet_t>('G'_rna4);
    EXPECT_EQ(variant.index(), 0u);
    EXPECT_EQ(letter.to_rank(), 2);

    variant = {static_cast<variant_t>('T'_rna4)};
    letter  = {static_cast<alphabet_t>('T'_rna4)};
    EXPECT_EQ(letter.to_rank(), 3);

    letter = 'A'_rna5;
    EXPECT_EQ(letter.to_rank(), 4);

    letter = 'C'_rna5;
    EXPECT_EQ(letter.to_rank(), 5);

    letter = 'G'_rna5;
    EXPECT_EQ(letter.to_rank(), 6);

    letter = 'N'_rna5;
    EXPECT_EQ(letter.to_rank(), 7);

    letter = 'T'_rna5;
    EXPECT_EQ(letter.to_rank(), 8);
}

TEST(variant_test, compare_to_component_alphabet)
{
    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5>;

    constexpr alphabet_t letter0{'G'_dna4};

    EXPECT_EQ(letter0, 'G'_dna4);
    EXPECT_NE(letter0, 'A'_dna4);
    EXPECT_NE(letter0, 'A'_dna5);

    EXPECT_EQ('G'_dna4, letter0);
    EXPECT_NE('A'_dna4, letter0);
    EXPECT_NE('A'_dna5, letter0);
}

TEST(variant_test, compare_to_component_alphabet_subtype)
{
    using alphabet_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5>;

    constexpr alphabet_t letter0{'G'_dna4};

    EXPECT_EQ(letter0, 'G'_rna4);
    EXPECT_NE(letter0, 'A'_rna4);
    EXPECT_NE(letter0, 'A'_rna5);

    EXPECT_EQ('G'_rna4, letter0);
    EXPECT_NE('A'_rna4, letter0);
    EXPECT_NE('A'_rna5, letter0);
}

TEST(variant_test, rank_type)
{
    using alphabet1_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using alphabet2_t = bio::alphabet::variant<bio::alphabet::gap, bio::alphabet::dna5, bio::alphabet::dna4>;
    using alphabet3_t = bio::alphabet::variant<char, bio::alphabet::gap>;

    EXPECT_TRUE((std::is_same_v<bio::alphabet::rank_t<alphabet1_t>, uint8_t>));
    EXPECT_TRUE((std::is_same_v<bio::alphabet::rank_t<alphabet2_t>, uint8_t>));
    EXPECT_TRUE((std::is_same_v<bio::alphabet::rank_t<alphabet3_t>, uint16_t>));
}

TEST(variant_test, alphabet_size)
{
    using alphabet1_t = bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap>;
    using alphabet2_t = bio::alphabet::variant<bio::alphabet::gap, bio::alphabet::dna5, bio::alphabet::dna4>;
    using alphabet3_t = bio::alphabet::variant<char, bio::alphabet::gap>;

    EXPECT_TRUE((std::is_same_v<decltype(alphabet1_t::alphabet_size), const size_t>));
    EXPECT_TRUE((std::is_same_v<decltype(alphabet2_t::alphabet_size), const size_t>));
    EXPECT_TRUE((std::is_same_v<decltype(alphabet3_t::alphabet_size), const size_t>));

    EXPECT_EQ(alphabet1_t::alphabet_size, 10ull);
    EXPECT_EQ(alphabet2_t::alphabet_size, 10ull);
    EXPECT_EQ(alphabet3_t::alphabet_size, 257ull);
}

TEST(variant_test, convert_by_index)
{
    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap> u;
    u = 'C'_dna5;

    EXPECT_FALSE(u.holds_alternative<0>());
    EXPECT_TRUE(u.holds_alternative<1>());
    EXPECT_FALSE(u.holds_alternative<2>());

    EXPECT_THROW(u.convert_to<0>(), std::bad_variant_access);
    EXPECT_NO_THROW(u.convert_to<1>());
    EXPECT_THROW(u.convert_to<2>(), std::bad_variant_access);

    bio::alphabet::dna5 out = u.convert_to<1>();
    EXPECT_EQ(out, 'C'_dna5);

    u = bio::alphabet::gap{};

    bio::alphabet::gap g = u.convert_unsafely_to<2>();
    EXPECT_EQ(g, bio::alphabet::gap{});
}

TEST(variant_test, convert_by_type)
{
    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::gap> u;
    u = 'C'_dna5;

    EXPECT_FALSE(u.holds_alternative<bio::alphabet::dna4>());
    EXPECT_TRUE(u.holds_alternative<bio::alphabet::dna5>());
    EXPECT_FALSE(u.holds_alternative<bio::alphabet::gap>());

    EXPECT_THROW(u.convert_to<bio::alphabet::dna4>(), std::bad_variant_access);
    EXPECT_NO_THROW(u.convert_to<bio::alphabet::dna5>());
    EXPECT_THROW(u.convert_to<bio::alphabet::gap>(), std::bad_variant_access);

    bio::alphabet::dna5 out = u.convert_to<bio::alphabet::dna5>();
    EXPECT_EQ(out, 'C'_dna5);

    u                    = bio::alphabet::gap{};
    bio::alphabet::gap g = u.convert_unsafely_to<bio::alphabet::gap>();
    EXPECT_EQ(g, bio::alphabet::gap{});
}

TEST(variant_test, two_different_variants)
{
    bio::alphabet::variant<bio::alphabet::dna4, bio::alphabet::gap> l{bio::alphabet::gap{}};
    bio::alphabet::variant<bio::alphabet::dna5, bio::alphabet::gap> r{bio::alphabet::gap{}};

    EXPECT_EQ(l, r);

    l = 'A'_dna4;
    r = 'C'_dna5;
    // r = 'A'_dna5 would also not be equal, because dna4 and dna5 are not comparable, only gap and both aren't gap

    EXPECT_NE(l, r);

    bio::alphabet::variant<bio::alphabet::rna4, bio::alphabet::gap> r2{'A'_rna4};
    EXPECT_EQ(l, r2); // this works because rna4 and dna4 are implicitly convertible to each other
}

TYPED_TEST(variant_test, char_is_valid_for)
{
    using gapped_alphabet_t       = TypeParam;
    using gapped_alphabet_bases_t = typename gapped_alphabet_t::biocpp_required_types;
    using char_t                  = bio::alphabet::char_t<gapped_alphabet_t>;

    char_t   i             = std::numeric_limits<char_t>::min();
    char_t   end           = std::numeric_limits<char_t>::max();
    uint64_t i_no_overflow = std::numeric_limits<char_t>::min();

    for (; i_no_overflow <= static_cast<uint64_t>(end); ++i, ++i_no_overflow)
    {
        bool is_valid{};
        bio::meta::detail::for_each<gapped_alphabet_bases_t>(
          [&is_valid, i](auto id)
          {
              using type = typename decltype(id)::type;
              is_valid   = is_valid || bio::alphabet::char_is_valid_for<type>(i);
          });
        EXPECT_EQ(bio::alphabet::char_is_valid_for<gapped_alphabet_t>(i), is_valid);
    }
}
