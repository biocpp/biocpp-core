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

template <typename alphabet_t, bool is_seqan2>
std::array<alphabet_t, 256> create_alphabet_array()
{
    std::array<alphabet_t, 256> alphabet_array{};

    auto convert_to_alphabet = [] (auto const c, auto & a)
    {
        if constexpr (is_seqan2)
        {
             a = (char) c;
        }
        else
        {
            using char_t = bio::alphabet_char_t<alphabet_t>;
            bio::assign_char_to(char_t(c), a);
        }
    };

    uint8_t i = 0;
    for (alphabet_t & a : alphabet_array)
        convert_to_alphabet(i++, a);

    return alphabet_array;
}

template <bio::alphabet alphabet_t>
void to_char(benchmark::State & state)
{
    std::array<alphabet_t, 256> alphs = create_alphabet_array<alphabet_t, false>();

    for (auto _ : state)
        for (alphabet_t a : alphs)
            benchmark::DoNotOptimize(bio::to_char(a));
}

/* regular alphabets, sorted by size */
BENCHMARK_TEMPLATE(to_char, bio::gap);
BENCHMARK_TEMPLATE(to_char, bio::dna4);
BENCHMARK_TEMPLATE(to_char, bio::rna4);
BENCHMARK_TEMPLATE(to_char, bio::dna5);
BENCHMARK_TEMPLATE(to_char, bio::rna5);
BENCHMARK_TEMPLATE(to_char, bio::dna15);
BENCHMARK_TEMPLATE(to_char, bio::rna15);
BENCHMARK_TEMPLATE(to_char, bio::aa20);
BENCHMARK_TEMPLATE(to_char, bio::aa27);
BENCHMARK_TEMPLATE(to_char, bio::phred42);
BENCHMARK_TEMPLATE(to_char, bio::phred63);
/* adaptations */
BENCHMARK_TEMPLATE(to_char, char);
BENCHMARK_TEMPLATE(to_char, char32_t);
/* alphabet variant */
BENCHMARK_TEMPLATE(to_char, bio::gapped<bio::dna4>);
BENCHMARK_TEMPLATE(to_char, bio::alphabet_variant<bio::gap, bio::dna4, bio::dna5, bio::dna15,
                                                     bio::rna15, bio::rna4, bio::rna5>);
BENCHMARK_TEMPLATE(to_char, bio::alphabet_variant<bio::dna4, char>);
/* alphabet tuple */
BENCHMARK_TEMPLATE(to_char, bio::masked<bio::dna4>);
BENCHMARK_TEMPLATE(to_char, bio::qualified<bio::dna4, bio::phred42>);
BENCHMARK_TEMPLATE(to_char, bio::qualified<bio::dna5, bio::phred63>);

#if BIOCPP_HAS_SEQAN2
template <typename alphabet_t>
void to_char_seqan2(benchmark::State & state)
{
    std::array<alphabet_t, 256> alphs = create_alphabet_array<alphabet_t, true>();

    for (auto _ : state)
        for (alphabet_t a : alphs)
            benchmark::DoNotOptimize(static_cast<char>(a));
}

BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Dna);
BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Rna);
BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Dna5);
BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Rna5);
BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Iupac);
BENCHMARK_TEMPLATE(to_char_seqan2, seqan::AminoAcid);

BENCHMARK_TEMPLATE(to_char_seqan2, seqan::Dna5Q);
BENCHMARK_TEMPLATE(to_char_seqan2, typename seqan::GappedValueType<seqan::Dna>::Type);
#endif

BENCHMARK_MAIN();
