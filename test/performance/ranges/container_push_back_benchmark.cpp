// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <deque>
#include <list>
#include <vector>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/ranges/container/all.hpp>

template <typename t>
using small_vec = bio::ranges::small_vector<t, 10'000>;

// ============================================================================
//  push_back
// ============================================================================

template <template <typename> typename container_t, typename alphabet_t>
void push_back(benchmark::State & state)
{
    alphabet_t a{};

    for (auto _ : state)
    {
        container_t<alphabet_t> c;
        for (size_t i = 0; i < 10'000; ++i)
            c.push_back(a);
        a = c.back();
    }

    state.counters["sizeof"] = sizeof(alphabet_t);
    if constexpr (bio::alphabet::alphabet<alphabet_t>)
        state.counters["alph_size"] = bio::alphabet::size<alphabet_t>;
}

BENCHMARK_TEMPLATE(push_back, std::vector, char);
BENCHMARK_TEMPLATE(push_back, std::vector, uint8_t);
BENCHMARK_TEMPLATE(push_back, std::vector, uint16_t);
BENCHMARK_TEMPLATE(push_back, std::vector, uint32_t);
BENCHMARK_TEMPLATE(push_back, std::vector, uint64_t);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::gap);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(push_back, std::vector, bio::alphabet::variant<char, bio::alphabet::dna4>);

BENCHMARK_TEMPLATE(push_back, std::deque, char);
BENCHMARK_TEMPLATE(push_back, std::deque, uint8_t);
BENCHMARK_TEMPLATE(push_back, std::deque, uint16_t);
BENCHMARK_TEMPLATE(push_back, std::deque, uint32_t);
BENCHMARK_TEMPLATE(push_back, std::deque, uint64_t);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::gap);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(push_back, std::deque, bio::alphabet::variant<char, bio::alphabet::dna4>);

BENCHMARK_TEMPLATE(push_back, std::list, char);
BENCHMARK_TEMPLATE(push_back, std::list, uint8_t);
BENCHMARK_TEMPLATE(push_back, std::list, uint16_t);
BENCHMARK_TEMPLATE(push_back, std::list, uint32_t);
BENCHMARK_TEMPLATE(push_back, std::list, uint64_t);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::gap);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(push_back, std::list, bio::alphabet::variant<char, bio::alphabet::dna4>);

#if __has_include(<sdsl/int_vector.hpp>)
#include <sdsl/int_vector.hpp>
template <typename t>
using sdsl_int_vec = sdsl::int_vector<sizeof(t) * 8>;

BENCHMARK_TEMPLATE(push_back, sdsl_int_vec, uint8_t);
BENCHMARK_TEMPLATE(push_back, sdsl_int_vec, uint16_t);
BENCHMARK_TEMPLATE(push_back, sdsl_int_vec, uint32_t);
BENCHMARK_TEMPLATE(push_back, sdsl_int_vec, uint64_t);
#endif

BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, char);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::gap);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(push_back, bio::ranges::bitcompressed_vector, bio::alphabet::variant<char, bio::alphabet::dna4>);

BENCHMARK_TEMPLATE(push_back, small_vec, char);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::gap);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(push_back, small_vec, bio::alphabet::variant<char, bio::alphabet::dna4>);

// ============================================================================
//  run
// ============================================================================

BENCHMARK_MAIN();
