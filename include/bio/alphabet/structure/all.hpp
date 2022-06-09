// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Joerg Winkler <j.winkler AT fu-berlin.de>
 * \brief Meta-header for the structure module. It includes all headers from alphabet/structure/.
 *
 * \defgroup structure Structure
 * \ingroup alphabet
 * \brief Provides types to represent single elements of RNA and protein structures.
 *
 * \details
 *
 * The following alphabets are currently supported in BioC++. Please see the format's page for more details.
 *
 * Name                                     | Chars               | Description
 * ---------------------------------------- | ------------------------ | -----------
 * [Dot Bracket](@ref bio::dot_bracket3) | `().`                    | Simple annotation that defines base pairs. No pseudoknots allowed.
 * [WUSS](@ref bio::wuss)                | `.<>:,-_~;()[]{}AaBb...` | Annotation that provides further markups and pseudoknots.
 * [DSSP](@ref bio::dssp9)               | `HBEGITSCX`              | Structure encoding for proteins.
 */

#pragma once

#include <bio/alphabet/structure/concept.hpp>
#include <bio/alphabet/structure/dot_bracket3.hpp>
#include <bio/alphabet/structure/dssp9.hpp>
#include <bio/alphabet/structure/structured_aa.hpp>
#include <bio/alphabet/structure/structured_rna.hpp>
#include <bio/alphabet/structure/wuss.hpp>
