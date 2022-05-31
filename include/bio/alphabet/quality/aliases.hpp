// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 * \brief Provides aliases for qualified.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/quality/concept.hpp>
#include <bio/alphabet/quality/phred42.hpp>
#include <bio/alphabet/quality/qualified.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>
#include <bio/alphabet/nucleotide/rna15.hpp>

namespace bio
{

//!\brief An alphabet that stores a bio::dna4 letter and an bio::phred42 letter at each position.
using dna4q = qualified<dna4, phred42>;

//!\brief An alphabet that stores a bio::dna5 letter and an bio::phred42 letter at each position.
using dna5q = qualified<dna5, phred42>;

//!\brief An alphabet that stores a bio::rna4 letter and an bio::phred42 letter at each position.
using rna4q = qualified<rna4, phred42>;

//!\brief An alphabet that stores a bio::rna5 letter and an bio::phred42 letter at each position.
using rna5q = qualified<rna5, phred42>;

//!\brief An alphabet that stores a bio::dna15 letter and an bio::qualified letter at each position.
using dna15q = qualified<dna15, phred42>;

//!\brief An alphabet that stores a bio::rna15 letter and an bio::qualified letter at each position.
using rna15q = qualified<rna15, phred42>;

} // namespace bio
