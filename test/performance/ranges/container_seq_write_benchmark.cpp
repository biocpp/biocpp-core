// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <deque>
#include <list>
#include <vector>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/ranges/container/all.hpp>
#include <bio/test/performance/sequence_generator.hpp>

template <typename t>
using sdsl_int_vec = sdsl::int_vector<sizeof(t) * 8>;

template <typename t>
using small_vec = bio::small_vector<t, 10'000>;

// ============================================================================
//  sequential_write
// ============================================================================

template <template <typename> typename container_t, typename alphabet_t>
void sequential_write(benchmark::State & state)
{
    auto cont_rando = bio::test::generate_sequence<alphabet_t>(10000, 0, 0);
    container_t<alphabet_t> source(cont_rando.begin(), cont_rando.end());

    alphabet_t a{};
    for (auto _ : state)
    {
        for (auto && c : source)
            c = a;
    }

    state.counters["sizeof"] = sizeof(alphabet_t);
    if constexpr (bio::alphabet<alphabet_t>)
        state.counters["alph_size"] = bio::alphabet_size<alphabet_t>;
}

BENCHMARK_TEMPLATE(sequential_write, std::vector, char);
BENCHMARK_TEMPLATE(sequential_write, std::vector, uint8_t);
BENCHMARK_TEMPLATE(sequential_write, std::vector, uint16_t);
BENCHMARK_TEMPLATE(sequential_write, std::vector, uint32_t);
BENCHMARK_TEMPLATE(sequential_write, std::vector, uint64_t);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::gap);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::dna4);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::dna15);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::aa27);
BENCHMARK_TEMPLATE(sequential_write, std::vector, bio::alphabet_variant<char, bio::dna4>);

BENCHMARK_TEMPLATE(sequential_write, std::deque, char);
BENCHMARK_TEMPLATE(sequential_write, std::deque, uint8_t);
BENCHMARK_TEMPLATE(sequential_write, std::deque, uint16_t);
BENCHMARK_TEMPLATE(sequential_write, std::deque, uint32_t);
BENCHMARK_TEMPLATE(sequential_write, std::deque, uint64_t);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::gap);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::dna4);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::dna15);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::aa27);
BENCHMARK_TEMPLATE(sequential_write, std::deque, bio::alphabet_variant<char, bio::dna4>);

BENCHMARK_TEMPLATE(sequential_write, std::list, char);
BENCHMARK_TEMPLATE(sequential_write, std::list, uint8_t);
BENCHMARK_TEMPLATE(sequential_write, std::list, uint16_t);
BENCHMARK_TEMPLATE(sequential_write, std::list, uint32_t);
BENCHMARK_TEMPLATE(sequential_write, std::list, uint64_t);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::gap);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::dna4);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::dna15);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::aa27);
BENCHMARK_TEMPLATE(sequential_write, std::list, bio::alphabet_variant<char, bio::dna4>);

BENCHMARK_TEMPLATE(sequential_write, sdsl_int_vec, uint8_t);
BENCHMARK_TEMPLATE(sequential_write, sdsl_int_vec, uint16_t);
BENCHMARK_TEMPLATE(sequential_write, sdsl_int_vec, uint32_t);
BENCHMARK_TEMPLATE(sequential_write, sdsl_int_vec, uint64_t);

BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, char);
BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::gap);
BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::dna4);
//TODO(bio): works on gcc10, fails on gc11
// BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::dna15);
BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::aa27);
//TODO(bio): works on gcc10, fails on gc11
// BENCHMARK_TEMPLATE(sequential_write, bio::bitcompressed_vector, bio::alphabet_variant<char, bio::dna4>);

BENCHMARK_TEMPLATE(sequential_write, small_vec, char);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::gap);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::dna4);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::dna15);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::aa27);
BENCHMARK_TEMPLATE(sequential_write, small_vec, bio::alphabet_variant<char, bio::dna4>);

// ============================================================================
//  run
// ============================================================================

BENCHMARK_MAIN();
