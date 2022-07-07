// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \author Joshua Kim <joshua.kim AT fu-berlin.de>
 * \brief Provides bio::alphabet::aminoacid_alphabet.
 */

#pragma once

#include <type_traits>

#include <bio/alphabet/concept.hpp>
#include <bio/meta/type_traits/template_inspection.hpp>

// ============================================================================
// aminoacid_empty_base
// ============================================================================

namespace bio::alphabet
{

/*!\brief This is an empty base class that can be inherited by types that shall model bio::alphabet::aminoacid_alphabet.
 * \ingroup aminoacid
 * \see bio::alphabet::enable_aminoacid
 */
struct aminoacid_empty_base
{};

} // namespace bio::alphabet

// ============================================================================
// enable_aminoacid
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\brief A trait that indicates whether a type shall model bio::alphabet::aminoacid_alphabet.
 * \tparam t Type of the argument.
 * \ingroup aminoacid
 * \details
 *
 * This is an auxiliary trait that is checked by bio::alphabet::aminoacid_alphabet to verify that a type is an amino acid.
 * This trait should never be read from, instead use bio::alphabet::aminoacid_alphabet.
 * However, user-defined alphabets that want to model bio::alphabet::aminoacid_alphabet need to specialise it.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). To change the default behaviour for your own alphabet,
 * do one of the following:
 *
 *   1. Inherit from bio::alphabet::aminoacid_empty_base; or
 *   2. Specialise this trait for your type.
 *
 * Note, that the concept check removes cvref-qualification from the type before evaluating this
 * trait, so you only need to specialise it for `t` and not for `t &` et cetera.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/aminoacid/enable_aminoacid.cpp
 */
template <typename t>
inline constexpr bool enable_aminoacid = std::derived_from<t, aminoacid_empty_base>;

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

// ============================================================================
// concept
// ============================================================================

/*!\interface bio::alphabet::aminoacid_alphabet <>
 * \extends bio::alphabet::alphabet
 * \brief A concept that indicates whether an alphabet represents amino acids.
 * \ingroup aminoacid
 *
 * Since an amino acid alphabet has no specific characteristics (like the complement
 * function for nucleotide alphabets), we distinguish an amino acid alphabet by
 * the bio::alphabet::is_aminoacid type trait.
 *
 * ###Concepts and doxygen
 * The requirements for this concept are given as related functions and type traits.
 * Types that satisfy this concept are shown as "implementing this interface".
 */
//!\cond
template <typename type>
concept aminoacid_alphabet = alphabet<type> && cpo::enable_aminoacid<std::remove_cvref_t<type>>;
//!\endcond

} // namespace bio::alphabet
