// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/alphabet/structure/all.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"
#include "../composite/alphabet_tuple_base_test_template.hpp"

using bio::operator""_aa27;
using bio::operator""_dssp9;

template <>
class alphabet_tuple_base_test<bio::structured_aa<bio::aa27, bio::dssp9>>: public ::testing::Test
{
public:
    using T = bio::structured_aa<bio::aa27, bio::dssp9>;

    T instance = T{value_1(), value_2()};
    T zero_instance = T{decltype(value_1()){}, decltype(value_2()){}};
    size_t tup_size{2};

    // bio::structured_aa<bio::aa27, bio::dssp9>
    // -------------------------------------------------------------------------
    bio::aa27 value_1()
    {
        return 'K'_aa27;
    }
    bio::aa27 assignable_to_value_1()
    {
        return 'K'_aa27; // replace if assignable subtype becomes available
    }
    bio::dssp9 value_2()
    {
        return 'I'_dssp9;
    }
    bio::dssp9 assignable_to_value_2()
    {
        return 'I'_dssp9; // replace if assignable subtype becomes available
    }
    auto values_to_cmp()
    {
        return std::make_tuple(/*low */'A'_aa27, 'H'_dssp9,
                               /*mid */'P'_aa27, 'I'_dssp9,
                               /*high*/'Z'_aa27, 'X'_dssp9);
    }
};

using structured_aa_types = ::testing::Types<bio::structured_aa<bio::aa27, bio::dssp9>>;

INSTANTIATE_TYPED_TEST_SUITE_P(structured_aa, alphabet, structured_aa_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(structured_aa, semi_alphabet_test, structured_aa_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(structured_aa, alphabet_constexpr, structured_aa_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(structured_aa, semi_alphabet_constexpr, structured_aa_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(structured_aa, alphabet_tuple_base_test, structured_aa_types, );
