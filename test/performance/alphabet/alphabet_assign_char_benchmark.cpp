// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

#include <algorithm>
#include <cstring>
#include <numeric>

#include <benchmark/benchmark.h>

#include <bio/alphabet/all.hpp>
#include <bio/test/seqan2.hpp>

#if BIOCPP_HAS_SEQAN2
#include <seqan/align.h>
#include <seqan/basic.h>
#include <seqan/modifier.h>
#endif

template <bio::alphabet::alphabet alphabet_t>
void assign_char(benchmark::State & state)
{
    using char_t = bio::alphabet::char_t<alphabet_t>;

    std::array<char_t, 256> chars{};
    std::iota(chars.begin(), chars.end(), 0);

    alphabet_t a{};
    for (auto _ : state)
        for (char_t c : chars)
            benchmark::DoNotOptimize(bio::alphabet::assign_char_to(c, a));
}

/* regular alphabets, sorted by size */
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::gap);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::dna4);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::rna4);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::dna5);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::rna5);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::dna15);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::rna15);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::aa20);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::aa27);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::phred42);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::phred63);
/* adaptations */
BENCHMARK_TEMPLATE(assign_char, char);
BENCHMARK_TEMPLATE(assign_char, char32_t);
/* alphabet variant */
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::gapped<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::alphabet_variant<bio::alphabet::gap, bio::alphabet::dna4, bio::alphabet::dna5, bio::alphabet::dna15,
                                                         bio::alphabet::rna15, bio::alphabet::rna4, bio::alphabet::rna5>);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::alphabet_variant<bio::alphabet::dna4, char>);
/* alphabet tuple */
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::masked<bio::alphabet::dna4>);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::qualified<bio::alphabet::dna4, bio::alphabet::phred42>);
BENCHMARK_TEMPLATE(assign_char, bio::alphabet::qualified<bio::alphabet::dna5, bio::alphabet::phred63>);

#if BIOCPP_HAS_SEQAN2
template <typename alphabet_t>
void assign_char_seqan2(benchmark::State & state)
{
    std::array<char, 256> chars{};
    std::iota(chars.begin(), chars.end(), 0);

    alphabet_t a{};
    for (auto _ : state)
        for (char c : chars)
            benchmark::DoNotOptimize(a = c);
}

BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Dna);
BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Rna);
BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Dna5);
BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Rna5);
BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Iupac);
BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::AminoAcid);

BENCHMARK_TEMPLATE(assign_char_seqan2, seqan::Dna5Q);
BENCHMARK_TEMPLATE(assign_char_seqan2, typename seqan::GappedValueType<seqan::Dna>::Type);
#endif

BENCHMARK_MAIN();
