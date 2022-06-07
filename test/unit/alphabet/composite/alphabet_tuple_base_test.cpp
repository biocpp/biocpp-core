// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/composite/alphabet_tuple_base.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>

#include "../semi_alphabet_test_template.hpp"
#include "alphabet_tuple_base_test_template.hpp"

using bio::operator""_dna4;
using bio::operator""_dna5;
using bio::operator""_rna4;
using bio::operator""_rna5;

template <typename type1, typename type2>
struct test_composite : public bio::alphabet_tuple_base<test_composite<type1, type2>, type1, type2>
{
    using base_t = bio::alphabet_tuple_base<test_composite<type1, type2>, type1, type2>;
    using base_t::base_t;
    using base_t::operator=;
};

template <>
class alphabet_tuple_base_test<test_composite<bio::dna4, bio::dna5>> : public ::testing::Test
{
public:
    using T = test_composite<bio::dna4, bio::dna5>;

    T instance = T{value_1(), value_2()};
    T zero_instance = T{decltype(value_1()){}, decltype(value_2()){}};
    size_t tup_size{2};

    // test_composite<bio::dna4, bio::dna5>
    // -------------------------------------------------------------------------
    bio::dna4 value_1()
    {
        return 'G'_dna4;
    }
    bio::rna4 assignable_to_value_1()
    {
        return 'G'_rna4;
    }
    bio::dna5 value_2()
    {
        return 'G'_dna5;
    }
    bio::rna5 assignable_to_value_2()
    {
        return 'G'_rna5;
    }
    auto values_to_cmp()
    {
        return std::make_tuple(/*low */'A'_dna4, 'A'_dna5,
                               /*mid */'C'_dna4, 'C'_dna5,
                               /*high*/'T'_dna4, 'T'_dna5);
    }
};

using test_composite_types = ::testing::Types<test_composite<bio::dna4, bio::dna5>>;

INSTANTIATE_TYPED_TEST_SUITE_P(test_composite, semi_alphabet_test, test_composite_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(test_composite, alphabet_tuple_base_test, test_composite_types, );
