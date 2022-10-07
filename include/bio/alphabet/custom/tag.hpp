// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides the customisation tags for the alphabet concepts.
 */

#pragma once

namespace bio::alphabet::custom
{

/*!\addtogroup alphabet_custom
 * \{
 */

//!\brief Customisation tag for bio::alphabet::to_rank.
struct to_rank
{};

//!\brief Customisation tag for bio::alphabet::assign_rank_to.#
struct assign_rank_to
{};

//!\brief Customisation tag for bio::alphabet::to_char.
struct to_char
{};

//!\brief Customisation tag for bio::alphabet::assign_char_to.
struct assign_char_to
{};

//!\brief Customisation tag for bio::alphabet::assign_char_to.
struct char_is_valid_for
{};

//!\brief CPO tag definition for bio::alphabet::size.
struct size
{};

//!\brief Customisation tag for bio::alphabet::complement.
struct complement
{};

//!\brief Customisation tag for bio::alphabet::assign_char_to.
struct to_phred
{};

//!\brief Customisation tag for bio::alphabet::assign_char_to.
struct assign_phred_to
{};

//!\}

} // namespace bio::alphabet::custom
