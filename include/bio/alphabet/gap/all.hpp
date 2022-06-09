// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Marcel Ehrhardt <marcel.ehrhardt AT fu-berlin.de>
 * \brief Meta-header for the gap submodule; includes all headers from alphabet/gap/.
 */

#pragma once

#include <bio/alphabet/gap/gap.hpp>
#include <bio/alphabet/gap/gapped.hpp>

/*!\defgroup gap Gap
 * \brief Provides the gap alphabet and functionality to make an alphabet a gapped alphabet.
 * \ingroup alphabet
 *
 * \details
 *
 * ### Introduction
 *
 * The gap symbol (`-`) is used in alignments to represent an interruption in an alignment, usually the result of an
 * insertion or deletion. The bio::gap alphabet represents this (single) gap symbol and satisfies the
 * bio::alphabet.
 *
 * The main purpose of bio::gap is to be combined with other alphabets. This can easily be achieved by using the
 * bio::gapped<> template which transforms any other alphabet to be a composite of that alphabet + the gap
 * character.
 */
