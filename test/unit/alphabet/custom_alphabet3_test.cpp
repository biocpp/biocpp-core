// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/core.hpp>

#include "alphabet_constexpr_test_template.hpp"
#include "alphabet_test_template.hpp"
#include "semi_alphabet_constexpr_test_template.hpp"
#include "semi_alphabet_test_template.hpp"

// Tests the capabilities of the explicit alphabet customisation

//![third_party_type]
#include <bio/alphabet/concept.hpp> // for bio::alphabet::alphabet
#include <cstddef>                  // for size_t

// this is from some other library:
namespace third_party_ns
{

enum class third_party_type
{
    ZERO,
    ONE,
    TWO
};

} // namespace third_party_ns

// ------------------------------------------------------------------------------------

// in this case, overloads for tag_invoke are defined within bio::*::cpo
namespace bio::alphabet::cpo
{

consteval size_t tag_invoke(size, third_party_ns::third_party_type) noexcept
{
    return 3;
}

constexpr size_t tag_invoke(to_rank, third_party_ns::third_party_type const a) noexcept
{
    return static_cast<size_t>(a);
}

constexpr third_party_ns::third_party_type & tag_invoke(assign_rank_to,
                                                        size_t const                       r,
                                                        third_party_ns::third_party_type & a) noexcept
{
    switch (r)
    {
        case 0:
            a = third_party_ns::third_party_type::ZERO;
            return a;
        case 1:
            a = third_party_ns::third_party_type::ONE;
            return a;
        default:
            a = third_party_ns::third_party_type::TWO;
            return a;
    }
}

constexpr char tag_invoke(to_char, third_party_ns::third_party_type const a) noexcept
{
    switch (a)
    {
        case third_party_ns::third_party_type::ZERO:
            return '0';
        case third_party_ns::third_party_type::ONE:
            return '1';
        default:
            return '2';
    }
}

constexpr third_party_ns::third_party_type & tag_invoke(assign_char_to,
                                                        char const                         c,
                                                        third_party_ns::third_party_type & a) noexcept
{
    switch (c)
    {
        case '0':
            a = third_party_ns::third_party_type::ZERO;
            return a;
        case '1':
            a = third_party_ns::third_party_type::ONE;
            return a;
        default:
            a = third_party_ns::third_party_type::TWO;
            return a;
    }
}

} // namespace bio::alphabet::cpo

static_assert(bio::alphabet::alphabet<third_party_ns::third_party_type>);
//![third_party_type]

INSTANTIATE_TYPED_TEST_SUITE_P(third_party_type, alphabet, third_party_ns::third_party_type, );
INSTANTIATE_TYPED_TEST_SUITE_P(third_party_type, semi_alphabet_test, third_party_ns::third_party_type, );
INSTANTIATE_TYPED_TEST_SUITE_P(third_party_type, alphabet_constexpr, third_party_ns::third_party_type, );
INSTANTIATE_TYPED_TEST_SUITE_P(third_party_type, semi_alphabet_constexpr, third_party_ns::third_party_type, );
