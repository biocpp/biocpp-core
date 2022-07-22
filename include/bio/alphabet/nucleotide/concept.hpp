// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \author Jörg Winkler <j.winkler AT fu-berlin.de>
 * \brief Provides bio::alphabet::nucleotide_alphabet.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
#include <concepts>

// ============================================================================
// complement()
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::complement.
//!\ingroup nucleotide
struct complement
{};

//!\}

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

/*!\name Function objects (Nucleotide)
 * \{
 */

/*!\brief Return the complement of a nucleotide object.
 * \tparam alph_type Type of the argument.
 * \param  alph      The nucleotide object for which you want to receive the complement.
 * \returns The complement character of `alph`, e.g. 'C' for 'G'.
 * \ingroup nucleotide
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all nucleotide alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/nucleotide/complement_fn.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). **If you don't want to create your own alphabet,
 * everything below is irrelevant to you!**
 *
 * This object acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```c++
 * constexpr alph_type tag_invoke(bio::alphabet::cpo::complement, alph_type const alph) noexcept
 * {}
 * ```
 *
 * Functions are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended). The returned type must be (implicitly) convertible to `alph_type` (usually it
 * should be the same type).
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 *
 * \hideinitializer
 */
inline constexpr auto complement = []<typename alph_t>(alph_t const a)
  //!\cond
  requires(requires {
      {tag_invoke(cpo::complement{}, a)};
      // NOTE we are using the trait here and not the concept, because the concept
      // also checks explicit convertibility but we don't want to substitute into
      // explicit constructors/conversion operators to prevent loops
      requires std::is_convertible_v<alph_t, decltype(tag_invoke(cpo::complement{}, a))>;
      requires noexcept(tag_invoke(cpo::complement{}, a));
  })
//!\endcond
{
    return tag_invoke(cpo::complement{}, a);
};
//!\}

// ============================================================================
// nucleotide_alphabet concept
// ============================================================================

/*!\interface bio::alphabet::nucleotide_alphabet <>
 * \extends bio::alphabet::alphabet
 * \brief A concept that indicates whether an alphabet represents nucleotides.
 * \ingroup nucleotide
 *
 * \details
 *
 * In addition to the requirements for bio::alphabet::alphabet, the nucleotide_alphabet introduces
 * a requirement for a complement function: bio::alphabet::nucleotide_alphabet::complement.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::alphabet
 *   2. bio::alphabet::complement needs to be defined for objects of type `t`
 *
 * See the documentation pages for the respective requirements.
 *
 * ### Related types
 *
 * If a given type `t` models this concept, the following types typically do so, as well:
 *
 *   * `t &`
 *   * `t const`
 *   * `t const &`
 */
//!\cond
template <typename t>
concept nucleotide_alphabet = alphabet<t> && requires(t val)
{
    {bio::alphabet::complement(val)};
};
//!\endcond

} // namespace bio::alphabet
