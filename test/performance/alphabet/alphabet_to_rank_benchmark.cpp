// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/test/seqan2.hpp>

#if BIOCPP_HAS_SEQAN2
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
            bio::alphabet::assign_rank_to(rank, a);
    };

    uint8_t i = 0;
    for (alphabet_t & a : alphabet_array)
        convert_to_alphabet(i++ % alphabet_size, a);

    return alphabet_array;
}

template <bio::alphabet::semialphabet alphabet_t>
void to_rank(benchmark::State & state)
{
    std::array<alphabet_t, 256> alphs = create_alphabet_array<alphabet_t, false>(bio::alphabet::size<alphabet_t>);

    for (auto _ : state)
        for (alphabet_t a : alphs)
            benchmark::DoNotOptimize(bio::alphabet::to_rank(a));
}

/* regular alphabets, sorted by size */
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::gap);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::rna4);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::dna5);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::rna5);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::rna15);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::aa20);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::phred42);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::phred63);
/* adaptations */
BENCHMARK_TEMPLATE(to_rank, char);
BENCHMARK_TEMPLATE(to_rank, char32_t);
/* alphabet variant */
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::variant<bio::alphabet::gap, bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::dna15,
                                                     bio::alphabet::rna15, bio::alphabet::rna4, bio::alphabet::rna5>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::variant<bio::alphabet::dna4, char>);
/* alphabet tuple */
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::masked<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>);
BENCHMARK_TEMPLATE(to_rank, bio::alphabet::qualified<bio::alphabet::dna5, bio::alphabet::phred63>);

#if BIOCPP_HAS_SEQAN2
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
