// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Genetic codes used for translating a triplet of nucleotides into an amino acid.
 */

#pragma once

#include <bio/meta/platform.hpp>

namespace bio
{
/*!\brief Genetic codes used for translation of nucleotides into amino acids.
 *
 * \details
 * The numeric values of the enums correspond to the genbank transl_table values
 * (see https://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi).
 */
enum struct genetic_code : uint8_t
{
    CANONICAL=1,
//     VERT_MITOCHONDRIAL,
//     YEAST_MITOCHONDRIAL,
//     MOLD_MITOCHONDRIAL,
//     INVERT_MITOCHONDRIAL,
//     CILIATE,
//     FLATWORM_MITOCHONDRIAL = 9,
//     EUPLOTID,
//     PROKARYOTE,
//     ALT_YEAST,
//     ASCIDIAN_MITOCHONDRIAL,
//     ALT_FLATWORM_MITOCHONDRIAL,
//     BLEPHARISMA,
//     CHLOROPHYCEAN_MITOCHONDRIAL,
//     TREMATODE_MITOCHONDRIAL = 21,
//     SCENEDESMUS_MITOCHONDRIAL,
//     THRAUSTOCHYTRIUM_MITOCHONDRIAL,
//     PTEROBRANCHIA_MITOCHONDRIAL,
//     GRACILIBACTERIA
};
}
