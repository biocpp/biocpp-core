// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides functions for translating a triplet of nucleotides into an amino acid.
 */

#pragma once

#include <tuple>

#include <bio/alphabet/aminoacid/aa27.hpp>
#include <bio/alphabet/aminoacid/translation_details.hpp>
#include <bio/alphabet/aminoacid/translation_genetic_code.hpp>
#include <bio/meta/type_traits/range.hpp>

namespace bio
{

// forwards:
class dna4;
class dna5;
class dna15;
class rna4;
class rna5;
class rna15;

/*!\brief Translate one nucleotide triplet into single amino acid (single nucleotide interface).
 * \ingroup aminoacid
 * \tparam nucl_type The type of input nucleotides.
 * \param[in] n1 First nucleotide in triplet.
 * \param[in] n2 Second nucleotide in triplet.
 * \param[in] n3 Third nucleotide in triplet.
 *
 * \details
 *
 * Translates single nucleotides into amino acid according to given genetic code.
 *
 * ### Complexity
 *
 * Constant.
 *
 * ### Exceptions
 *
 * No-throw guarantee.
 */
template <genetic_code gc = genetic_code::CANONICAL, nucleotide_alphabet nucl_type>
constexpr aa27 translate_triplet(nucl_type const & n1, nucl_type const & n2, nucl_type const & n3) noexcept
{
    if constexpr (std::same_as<nucl_type, dna4> || std::same_as<nucl_type, dna5> || std::same_as<nucl_type, dna15>)
    {
        // table exists for dna15 and is generated for dna4 and dna5 (compile time ok, because small)
        return bio::detail::translation_table<nucl_type, gc>::VALUE[to_rank(n1)][to_rank(n2)][to_rank(n3)];
    }
    else if constexpr (std::same_as<nucl_type, rna4> || std::same_as<nucl_type, rna5> || std::same_as<nucl_type, rna15>)
    {
        using rna2dna_t =
          std::conditional_t<std::same_as<nucl_type, rna4>,
                             dna4,
                             std::conditional_t<std::same_as<nucl_type, rna5>,
                                                dna5,
                                                std::conditional_t<std::same_as<nucl_type, rna15>, dna15, void>>>;

        // we can use dna's tables, because ranks are identical
        return bio::detail::translation_table<rna2dna_t, gc>::VALUE[to_rank(n1)][to_rank(n2)][to_rank(n3)];
    }
    else // composites or user defined nucleotide
    {
        // we cast to dna15; slightly slower run-time, but lot's of compile time saved for large alphabets.
        // (nucleotide types can be converted to dna15 by definition)
        return bio::detail::translation_table<dna15, gc>::VALUE[to_rank(static_cast<dna15>(n1))][to_rank(
          static_cast<dna15>(n2))][to_rank(static_cast<dna15>(n3))];
    }
}

} // namespace bio
