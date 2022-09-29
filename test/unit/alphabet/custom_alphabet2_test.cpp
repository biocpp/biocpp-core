// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/core.hpp>

// Tests the ADL-capabilities of the alphabet customisation point objects; type that isn't default constructible

//![my_alph]
#include <cstddef>     // for size_t
#include <type_traits> // for std::type_identity

#include <bio/alphabet/concept.hpp> // for bio::alphabet::alphabet

namespace my_namespace
{

class my_alph
{
public:
    uint8_t rank;

    my_alph()                                      = delete;
    constexpr my_alph(my_alph const &)             = default;
    constexpr my_alph & operator=(my_alph const &) = default;

    constexpr my_alph(uint8_t rank) : rank{rank} {}

    constexpr friend auto operator<=>(my_alph lhs, my_alph rhs) = default;

    consteval friend size_t tag_invoke(bio::alphabet::cpo::size, std::type_identity<my_alph>) noexcept { return 2; }

    constexpr friend uint8_t tag_invoke(bio::alphabet::cpo::to_rank, my_alph const a) noexcept { return a.rank; }

    constexpr friend my_alph & tag_invoke(bio::alphabet::cpo::assign_rank_to, uint8_t const r, my_alph & a) noexcept
    {
        a.rank = r;
        return a;
    }

    constexpr friend char tag_invoke(bio::alphabet::cpo::to_char, my_alph const a) noexcept
    {
        if (a.rank)
            return '1';
        else
            return '0';
    }

    constexpr friend my_alph & tag_invoke(bio::alphabet::cpo::assign_char_to, char const c, my_alph & a) noexcept
    {
        switch (c)
        {
            case '0':
            case 'F':
            case 'f':
                a.rank = 0;
                return a;
            default:
                a.rank = 1;
                return a;
        }
    }

    constexpr friend bool tag_invoke(bio::alphabet::cpo::char_is_valid_for,
                                     char const c,
                                     std::type_identity<my_alph>) noexcept
    {
        switch (c)
        {
            case '0':
            case 'F':
            case 'f':
            case '1':
            case 'T':
            case 't':
                return true;
            default:
                return false;
        }
    }
};

} // namespace my_namespace

static_assert(bio::alphabet::size<my_namespace::my_alph> == 2);
static_assert(bio::alphabet::char_is_valid_for<my_namespace::my_alph>('T'));
static_assert(!bio::alphabet::char_is_valid_for<my_namespace::my_alph>('!'));
static_assert(bio::alphabet::semialphabet<my_namespace::my_alph>);
static_assert(bio::alphabet::alphabet<my_namespace::my_alph>);
//![my_alph]

// Not tested with rest of test-suite because the test-suite relies on default-constructibility
