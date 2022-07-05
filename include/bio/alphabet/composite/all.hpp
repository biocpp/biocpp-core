// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Marcel Ehrhardt <marcel.ehrhardt AT fu-berlin.de>
 * \brief Meta-header for the composite submodule; includes all headers from alphabet/composite/.
 */
#pragma once

#include <bio/alphabet/composite/alphabet_tuple_base.hpp>
#include <bio/alphabet/composite/alphabet_variant.hpp>
#include <bio/alphabet/composite/semialphabet_any.hpp>

/*!\defgroup alphabet_composite Composite
 * \brief Provides templates for combining existing alphabets into new alphabet types.
 * \ingroup alphabet
 *
 * \details
 *
 * ### Introduction
 *
 * This module provides various class templates that allow you to combine existing alphabets into new ones. For example,
 * you can add new characters to existing alphabets by using bio::alphabet::alphabet_variant or combine alphabets with quality
 * information by using bio::alphabet::alphabet_tuple_base.
 *
 * We have currently three major composite alphabets:
 * * bio::alphabet::alphabet_tuple_base which can be used to create a std::tuple like object that still models
 *   bio::alphabet::alphabet.
 * * bio::alphabet::alphabet_variant which roughly corresponds to the Union of the given types. It behaves similar to
 *   std::variant, but also models bio::alphabet::alphabet.
 * * bio::alphabet::semialphabet_any which type erases other alphabets of the same size and allows again transformation to
 *   alphabets of the same size by copying the rank.
 */
