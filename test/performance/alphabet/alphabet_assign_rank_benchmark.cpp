// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstring>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/test/seqan2.hpp>

#if BIOCPP_HAS_SEQAN2
#include <seqan/align.h>
#include <seqan/basic.h>
#include <seqan/modifier.h>
#endif

template <typename rank_t>
void fill_rank_array(std::array<rank_t, 256> & ranks, size_t const alphabet_size)
{
    uint8_t i = 0;
    for (rank_t & r : ranks)
        r = i++ % alphabet_size;
}

template <bio::alphabet::semialphabet alphabet_t>
void assign_rank(benchmark::State & state)
{
    using rank_t = bio::alphabet::rank_t<alphabet_t>;

    std::array<rank_t, 256> ranks{};
    fill_rank_array<rank_t>(ranks, bio::alphabet::size<alphabet_t>);

    alphabet_t a{};
    for (auto _ : state)
        for (rank_t r : ranks)
            benchmark::DoNotOptimize(bio::alphabet::assign_rank_to(r, a));
}

/* regular alphabets, sorted by size */
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::gap);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::rna4);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::dna5);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::rna5);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::rna15);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::aa20);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::phred42);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::phred63);
/* adaptations */
BENCHMARK_TEMPLATE(assign_rank, char);
BENCHMARK_TEMPLATE(assign_rank, char32_t);
/* alphabet variant */
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::alphabet_variant<bio::alphabet::gap, bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::dna15,
                                                         bio::alphabet::rna15, bio::alphabet::rna4, bio::alphabet::rna5>);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::alphabet_variant<bio::alphabet::dna4, char>);
/* alphabet tuple */
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::masked<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>);
BENCHMARK_TEMPLATE(assign_rank, bio::alphabet::qualified<bio::alphabet::dna5, bio::alphabet::phred63>);

#if BIOCPP_HAS_SEQAN2
template <typename alphabet_t>
void assign_rank_seqan2(benchmark::State & state)
{
    using rank_t = uint8_t;

    std::array<rank_t, 256> ranks{};
    fill_rank_array<rank_t>(ranks, seqan::ValueSize<alphabet_t>::VALUE);

    alphabet_t a{};
    for (auto _ : state)
        for (rank_t r : ranks)
            benchmark::DoNotOptimize(a = r);
}

BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Dna);
BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Rna);
BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Dna5);
BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Rna5);
BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Iupac);
BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::AminoAcid);

BENCHMARK_TEMPLATE(assign_rank_seqan2, seqan::Dna5Q);
BENCHMARK_TEMPLATE(assign_rank_seqan2, typename seqan::GappedValueType<seqan::Dna>::Type);
#endif

BENCHMARK_MAIN();
