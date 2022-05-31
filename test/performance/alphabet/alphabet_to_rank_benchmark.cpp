// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/test/seqan2.hpp>

#if SEQAN3_HAS_SEQAN2
#include <seqan/align.h>
#include <seqan/basic.h>
#include <seqan/modifier.h>
#endif

template <typename alphabet_t, bool is_seqan2>
std::array<alphabet_t, 256> create_alphabet_array(size_t const alphabet_size)
{
    std::array<alphabet_t, 256> alphabet_array{};

    auto convert_to_alphabet = [] (auto const rank, auto & a)
    {
        if constexpr (is_seqan2)
             a = static_cast<uint8_t>(rank);
        else
            bio::assign_rank_to(rank, a);
    };

    uint8_t i = 0;
    for (alphabet_t & a : alphabet_array)
        convert_to_alphabet(i++ % alphabet_size, a);

    return alphabet_array;
}

template <bio::semialphabet alphabet_t>
void to_rank(benchmark::State & state)
{
    std::array<alphabet_t, 256> alphs = create_alphabet_array<alphabet_t, false>(bio::alphabet_size<alphabet_t>);

    for (auto _ : state)
        for (alphabet_t a : alphs)
            benchmark::DoNotOptimize(bio::to_rank(a));
}

/* regular alphabets, sorted by size */
BENCHMARK_TEMPLATE(to_rank, bio::gap);
BENCHMARK_TEMPLATE(to_rank, bio::dna4);
BENCHMARK_TEMPLATE(to_rank, bio::rna4);
BENCHMARK_TEMPLATE(to_rank, bio::dna5);
BENCHMARK_TEMPLATE(to_rank, bio::rna5);
BENCHMARK_TEMPLATE(to_rank, bio::dna15);
BENCHMARK_TEMPLATE(to_rank, bio::rna15);
BENCHMARK_TEMPLATE(to_rank, bio::aa20);
BENCHMARK_TEMPLATE(to_rank, bio::aa27);
BENCHMARK_TEMPLATE(to_rank, bio::phred42);
BENCHMARK_TEMPLATE(to_rank, bio::phred63);
/* adaptations */
BENCHMARK_TEMPLATE(to_rank, char);
BENCHMARK_TEMPLATE(to_rank, char32_t);
/* alphabet variant */
BENCHMARK_TEMPLATE(to_rank, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet_variant<bio::gap, bio::dna4, bio::dna5, bio::dna15,
                                                     bio::rna15, bio::rna4, bio::rna5>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet_variant<bio::dna4, char>);
/* alphabet tuple */
BENCHMARK_TEMPLATE(to_rank, bio::masked<bio::dna4>);
BENCHMARK_TEMPLATE(to_rank, bio::qualified<bio::dna4, bio::phred42>);
BENCHMARK_TEMPLATE(to_rank, bio::qualified<bio::dna5, bio::phred63>);

#if SEQAN3_HAS_SEQAN2
template <typename alphabet_t>
void to_rank_seqan2(benchmark::State & state)
{
    std::array<alphabet_t, 256> alphs = create_alphabet_array<alphabet_t, true>(seqan::ValueSize<alphabet_t>::VALUE);

    for (auto _ : state)
        for (alphabet_t a : alphs)
            benchmark::DoNotOptimize(seqan::ordValue(a));
}

BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Dna);
BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Rna);
BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Dna5);
BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Rna5);
BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Iupac);
BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::AminoAcid);

BENCHMARK_TEMPLATE(to_rank_seqan2, seqan::Dna5Q);
BENCHMARK_TEMPLATE(to_rank_seqan2, typename seqan::GappedValueType<seqan::Dna>::Type);
#endif

BENCHMARK_MAIN();
