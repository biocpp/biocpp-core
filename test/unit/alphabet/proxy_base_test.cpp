// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/proxy_base.hpp>

#include "alphabet_proxy_test_template.hpp"

class proxy_base_example : public bio::alphabet::proxy_base<proxy_base_example, bio::alphabet::dna4>
{
private:
    using alphabet_type = bio::alphabet::dna4;
    using base_t        = bio::alphabet::proxy_base<proxy_base_example, alphabet_type>;
    friend base_t;

    bio::alphabet::dna4 * ptr;

public:
    constexpr proxy_base_example() noexcept                  = delete;
    constexpr proxy_base_example(proxy_base_example const &) = default;
    constexpr proxy_base_example(proxy_base_example &&)      = default;
    ~proxy_base_example()                                    = default;

    constexpr proxy_base_example(bio::alphabet::dna4 & in) : ptr{&in} {}

    using base_t::operator=;

    constexpr proxy_base_example & operator=(proxy_base_example const & rhs) { return assign_rank(rhs.to_rank()); }

    constexpr proxy_base_example const & operator=(proxy_base_example const & rhs) const
    {
        return assign_rank(rhs.to_rank());
    }

    constexpr size_t to_rank() const noexcept
    {
        return ptr->to_rank(); // This would have actual implementation
    }

    constexpr proxy_base_example & assign_rank(size_t const r) noexcept
    {
        ptr->assign_rank(r);
        return *this;
    }

    constexpr proxy_base_example const & assign_rank(size_t const r) const noexcept
    {
        ptr->assign_rank(r);
        return *this;
    }
};

using namespace bio::alphabet::literals;

template <>
struct proxy_fixture<proxy_base_example> : public ::testing::Test
{
    bio::alphabet::dna4 def{};
    bio::alphabet::dna4 a0 = 'A'_dna4;
    bio::alphabet::dna4 a1 = 'C'_dna4;

    proxy_base_example default_init{def};
    proxy_base_example t0{a0};
    proxy_base_example t1{a1};
};

INSTANTIATE_TYPED_TEST_SUITE_P(proxy1_test, proxy_fixture, ::testing::Types<proxy_base_example>, );

// -----------------------------------------------------------------------------------------------------
// check handling of external types that do not provide members
// -----------------------------------------------------------------------------------------------------

namespace my_namespace
{

class my_alph
{
public:
    uint8_t rank{};

    constexpr my_alph() noexcept                   = default;
    constexpr my_alph(my_alph const &)             = default;
    constexpr my_alph & operator=(my_alph const &) = default;

    constexpr my_alph(uint8_t rank) : rank{rank} {}

    constexpr friend auto operator<=>(my_alph, my_alph) = default;
};

consteval size_t tag_invoke(bio::alphabet::custom::size, my_alph const &) noexcept
{
    return 2;
}

constexpr uint8_t tag_invoke(bio::alphabet::custom::to_rank, my_alph const a) noexcept
{
    return a.rank;
}

constexpr my_alph & tag_invoke(bio::alphabet::custom::assign_rank_to, uint8_t const r, my_alph & a) noexcept
{
    a.rank = r;
    return a;
}

constexpr char tag_invoke(bio::alphabet::custom::to_char, my_alph const a) noexcept
{
    if (a.rank)
        return '1';
    else
        return '0';
}

constexpr my_alph & tag_invoke(bio::alphabet::custom::assign_char_to, char const c, my_alph & a) noexcept
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

} // namespace my_namespace

static_assert(bio::alphabet::size<my_namespace::my_alph> == 2);
static_assert(bio::alphabet::semialphabet<my_namespace::my_alph>);
static_assert(bio::alphabet::alphabet<my_namespace::my_alph>);

class proxy_base_example2 : public bio::alphabet::proxy_base<proxy_base_example2, my_namespace::my_alph>
{
private:
    using alphabet_type = my_namespace::my_alph;
    using base_t        = bio::alphabet::proxy_base<proxy_base_example2, alphabet_type>;
    friend base_t;

    my_namespace::my_alph * ptr;

public:
    constexpr proxy_base_example2() noexcept                   = default;
    constexpr proxy_base_example2(proxy_base_example2 const &) = default;
    constexpr proxy_base_example2(proxy_base_example2 &&)      = default;
    ~proxy_base_example2()                                     = default;

    constexpr proxy_base_example2(my_namespace::my_alph & val) : ptr{&val} {}

    constexpr proxy_base_example2 & operator=(proxy_base_example2 const & rhs) { return assign_rank(rhs.to_rank()); }

    constexpr proxy_base_example2 const & operator=(proxy_base_example2 const & rhs) const
    {
        return assign_rank(rhs.to_rank());
    }

    constexpr size_t to_rank() const noexcept { return ptr->rank; }

    constexpr proxy_base_example2 & assign_rank(size_t const r) noexcept
    {
        ptr->rank = r;
        return *this;
    }

    constexpr proxy_base_example2 const & assign_rank(size_t const r) const noexcept
    {
        ptr->rank = r;
        return *this;
    }
};

template <>
struct proxy_fixture<proxy_base_example2> : public ::testing::Test
{
    my_namespace::my_alph def{};
    my_namespace::my_alph a0{0};
    my_namespace::my_alph a1{1};

    proxy_base_example2 default_init{def};
    proxy_base_example2 t0{a0};
    proxy_base_example2 t1{a1};
};

INSTANTIATE_TYPED_TEST_SUITE_P(proxy2_test, proxy_fixture, ::testing::Types<proxy_base_example2>, );
