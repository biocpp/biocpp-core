// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides test utilities for bio::alphabet::simd::simd_type types.
 * \author Marcel Ehrhardt <marcel.ehrhardt AT fu-berlin.de>
 */

#pragma once

#include <algorithm>
#include <random>

#include <bio/alphabet/concept.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/test/seqan2.hpp>

#ifdef BIOCPP_HAS_SEQAN2
#include <seqan/basic.h>
#include <seqan/sequence.h>
#endif

namespace bio::test
{

template <typename alphabet_t>
auto generate_sequence(size_t const len = 500,
                       size_t const variance = 0,
                       size_t const seed = 0)
{
    std::mt19937 gen(seed);
    size_t max_val{};

    if constexpr (std::unsigned_integral<alphabet_t>)
        max_val = std::numeric_limits<size_t>::max();
    else
        max_val = alphabet::size<alphabet_t> - 1ull;

    std::uniform_int_distribution<size_t> dis_alpha(0ull, max_val);
    std::uniform_int_distribution<size_t> dis_length(len - variance, len + variance);

    std::vector<alphabet_t> sequence;

    size_t length = dis_length(gen);
    for (size_t l = 0; l < length; ++l)
    {
        if constexpr (std::unsigned_integral<alphabet_t>)
            sequence.push_back(dis_alpha(gen));
        else
            sequence.push_back(alphabet::assign_rank_to(dis_alpha(gen), alphabet_t{}));
    }

    return sequence;
}

template <meta::arithmetic number_type>
auto generate_numeric_sequence(size_t const len = 500,
                               number_type const min = std::numeric_limits<number_type>::lowest(),
                               number_type const max = std::numeric_limits<number_type>::max(),
                               size_t const seed = 0)
{
    std::mt19937_64 engine(seed);
    std::uniform_int_distribution<size_t> dist{min, max};

    auto gen = [&dist, &engine]() { return dist(engine); };
    std::vector<number_type> sequence(len);
    std::ranges::generate(sequence, gen);

    return sequence;
}

template <typename alphabet_t>
auto generate_sequence_pairs(size_t const sequence_length,
                             size_t const set_size,
                             size_t const sequence_variance = 0)
{
    using sequence_t = decltype(generate_sequence<alphabet_t>());

    std::vector<std::pair<sequence_t, sequence_t>> vec;

    for (unsigned i = 0; i < set_size; ++i)
    {
        sequence_t seq1 = generate_sequence<alphabet_t>(sequence_length, sequence_variance, i);
        sequence_t seq2 = generate_sequence<alphabet_t>(sequence_length, sequence_variance, i + set_size);
        vec.push_back(std::pair{seq1, seq2});
    }

    return vec;
}

#ifdef BIOCPP_HAS_SEQAN2
template <typename alphabet_t>
auto generate_sequence_seqan2(size_t const len = 500,
                              size_t const variance = 0,
                              size_t const seed = 0)
{
    std::mt19937 gen(seed);
    std::uniform_int_distribution<size_t> dis_alpha(0, seqan::ValueSize<alphabet_t>::VALUE - 1);
    std::uniform_int_distribution<size_t> dis_length(len - variance, len + variance);

    seqan::String<alphabet_t> sequence;
    size_t length = dis_length(gen);

    for (size_t l = 0; l < length; ++l)
        appendValue(sequence, static_cast<alphabet_t>(dis_alpha(gen)));

    return sequence;
}

template <typename alphabet_t>
auto generate_sequence_pairs_seqan2(size_t const sequence_length,
                                    size_t const set_size,
                                    size_t const sequence_variance = 0)
{
    using sequence_t = decltype(generate_sequence_seqan2<alphabet_t>());

    seqan::StringSet<sequence_t> vec1;
    seqan::StringSet<sequence_t> vec2;

    for (unsigned i = 0; i < set_size; ++i)
    {
        sequence_t seq1 = generate_sequence_seqan2<alphabet_t>(sequence_length, sequence_variance, i);
        sequence_t seq2 = generate_sequence_seqan2<alphabet_t>(sequence_length, sequence_variance, i + set_size);
        appendValue(vec1, seq1);
        appendValue(vec2, seq2);
    }

    return std::tuple{vec1, vec2};
}
#endif // generate seqan2 data.

} // namespace bio::test
