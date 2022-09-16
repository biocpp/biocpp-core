// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/gap/gapped.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/quality/aliases.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/phred63.hpp>
#include <bio/alphabet/quality/qualified.hpp>

#include "../alphabet_constexpr_test_template.hpp"
#include "../alphabet_proxy_test_template.hpp"
#include "../alphabet_test_template.hpp"
#include "../composite/tuple_base_test_template.hpp"
#include "../semi_alphabet_constexpr_test_template.hpp"
#include "../semi_alphabet_test_template.hpp"

template <typename alphabet_type, typename phred_type>
class tuple_base_test<bio::alphabet::qualified<alphabet_type, phred_type>> : public ::testing::Test
{
public:
    using T = bio::alphabet::qualified<alphabet_type, phred_type>;

    using other_type = std::conditional_t<
      std::is_same_v<alphabet_type, bio::alphabet::dna4>,
      bio::alphabet::rna4,
      std::conditional_t<std::is_same_v<alphabet_type, bio::alphabet::aa27>,
                         bio::alphabet::aa27,
                         std::conditional_t<std::is_same_v<alphabet_type, bio::alphabet::gapped<bio::alphabet::dna4>>,
                                            bio::alphabet::gapped<bio::alphabet::dna4>,
                                            alphabet_type>>>;

    T      instance      = T{value_1(), value_2()};
    T      zero_instance = T{decltype(value_1()){}, decltype(value_2()){}};
    size_t tup_size{2};

    // structured_rna<alphabet_type, phred_type>
    // -------------------------------------------------------------------------
    alphabet_type value_1() { return alphabet_type{}.assign_char('G'); }
    other_type    assignable_to_value_1() { return other_type{}.assign_char('G'); }
    phred_type    value_2() { return phred_type{6}; }
    phred_type    assignable_to_value_2()
    {
        return phred_type{6}; // replace if assignable subtype becomes available
    }
    auto values_to_cmp()
    {
        return std::make_tuple(/*low */ alphabet_type{}.assign_char('A'),
                               phred_type{1},
                               /*mid */ alphabet_type{}.assign_char('C'),
                               phred_type{4},
                               /*high*/ alphabet_type{}.assign_char('T'),
                               phred_type{9});
    }
};

using qualified_types =
  ::testing::Types<bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>,
                   bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred63>,
                   bio::alphabet::qualified<bio::alphabet::aa27, bio::alphabet::phred42>,
                   bio::alphabet::qualified<bio::alphabet::gapped<bio::alphabet::dna4>, bio::alphabet::phred42>,
                   bio::alphabet::dna4q>;

INSTANTIATE_TYPED_TEST_SUITE_P(qualified, alphabet, qualified_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(qualified, semi_alphabet_test, qualified_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(qualified, alphabet_constexpr, qualified_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(qualified, semi_alphabet_constexpr, qualified_types, );
INSTANTIATE_TYPED_TEST_SUITE_P(qualified, tuple_base_test, qualified_types, );

using namespace bio::alphabet::literals;

using tup   = bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>;
using ref_t = decltype(get<0>(std::declval<tup &>()));

template <>
struct proxy_fixture<ref_t> : public ::testing::Test
{
    tup data_def{'A'_dna4, '!'_phred42};
    tup data_t0{'A'_dna4, '!'_phred42};
    tup data_t1{'C'_dna4, '!'_phred42};

    ref_t default_init{get<0>(data_def)};
    ref_t t0{get<0>(data_t0)};
    ref_t t1{get<0>(data_t1)};
};

INSTANTIATE_TYPED_TEST_SUITE_P(proxy_test, proxy_fixture, ::testing::Types<ref_t>, );
