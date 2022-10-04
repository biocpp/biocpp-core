// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \author Marie Hoffmann <marie.hoffmann AT fu-berlin.de>
 * \author Jörg Winkler <j.winkler AT fu-berlin.de>
 * \brief Quality alphabet concept.
 */

#pragma once

#include <bio/alphabet/concept.hpp>

// ============================================================================
// to_phred()
// ============================================================================

namespace bio::alphabet::custom
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::assign_char_to.
//!\ingroup quality
struct to_phred
{};

//!\}

} // namespace bio::alphabet::custom

namespace bio::alphabet
{

/*!\name Function objects (Quality)
 * \{
 */

/*!\brief The public getter function for the phred representation of a quality score.
 * \tparam your_type The type of alphabet. Must model the bio::alphabet::quality_alphabet.
 * \param  chr       The quality value to convert into the phred score.
 * \returns the phred representation of a quality score.
 * \ingroup quality
 *
 * \details
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all quality alphabets in BioC++.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). **If you don't want to create your own alphabet,
 * everything below is irrelevant to you!**
 *
 * This object acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```c++
 * constexpr phred_type tag_invoke(bio::alphabet::custom::to_phred, alph_type const alph) noexcept
 * {}
 * ```
 *
 * Functions are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type models std::integral.
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 *
 * \hideinitializer
 */
inline constexpr auto to_phred = []<typename alph_t>(alph_t const a)
  //!\cond
  requires(requires {
      {
          tag_invoke(custom::to_phred{}, a)
          } -> std::integral;
      requires noexcept(tag_invoke(custom::to_phred{}, a));
  })
//!\endcond
{
    return tag_invoke(custom::to_phred{}, a);
};
//!\}

/*!\brief The `phred_type` of the alphabet; defined as the return type of bio::alphabet::to_phred.
 * \ingroup quality
 */
template <typename alphabet_type>
    //!\cond
    requires(requires { {bio::alphabet::to_phred(std::declval<alphabet_type>())}; })
//!\endcond
using phred_t = decltype(bio::alphabet::to_phred(std::declval<alphabet_type>()));

} // namespace bio::alphabet

// ============================================================================
// assign_phred_to()
// ============================================================================

namespace bio::alphabet::custom
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::assign_char_to.
//!\ingroup quality
struct assign_phred_to
{};

//!\}

} // namespace bio::alphabet::custom

namespace bio::alphabet
{

/*!\name Function objects (Quality)
 * \{
 */

/*!\brief Assign a phred score to a quality alphabet object.
 * \tparam alph_type The type of the target object. Must model the bio::alphabet::quality_alphabet.
 * \param  phr       The phred score being assigned; must be of the bio::alphabet::phred_t of the target object.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \ingroup quality
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all quality alphabets in BioC++.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). **If you don't want to create your own alphabet,
 * everything below is irrelevant to you!**
 *
 * This object acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```c++
 * constexpr alph_type & tag_invoke(bio::alphabet::assign_phred_to, phred_type const phr, alph_type & alph) noexcept
 * ```
 *
 * Functions are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is exactly `alph_type &`.
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 *
 * *Note* that temporaries of `alph_type` are handled
 * by this function object and **do not** require an additional overload.
 * \hideinitializer
 */
inline constexpr auto assign_phred_to = []<typename alph_t>(bio::alphabet::phred_t<alph_t> const p,
                                                            alph_t &&                            a) -> alph_t
  //!\cond
  requires(requires {
      {
          tag_invoke(custom::assign_phred_to{}, p, a)
          } -> std::same_as<alph_t &>;
      requires noexcept(tag_invoke(custom::assign_phred_to{}, p, a));
  })
//!\endcond
{
    return tag_invoke(custom::assign_phred_to{}, p, a);
};
//!\}

} // namespace bio::alphabet

// ============================================================================
// bio::alphabet::quality_alphabet
// ============================================================================

namespace bio::alphabet
{

/*!\interface bio::alphabet::quality_alphabet <>
 * \extends bio::alphabet::alphabet
 * \brief A concept that indicates whether an alphabet represents quality scores.
 * \ingroup quality
 *
 * \details
 *
 * In addition to the requirements for bio::alphabet::alphabet, the
 * quality_alphabet introduces a requirement for conversion functions from and to
 * a Phred score.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::alphabet
 *   2. bio::alphabet::to_phred needs to be defined for objects of type `t`
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
concept quality_alphabet = alphabet<t> && requires(t qual)
{
    {bio::alphabet::to_phred(qual)};
};
//!\endcond

// ============================================================================
// bio::alphabet::writable_quality_alphabet
// ============================================================================

/*!\interface bio::alphabet::writable_quality_alphabet <>
 * \extends bio::alphabet::alphabet
 * \extends bio::alphabet::quality_alphabet
 * \brief A concept that indicates whether a writable alphabet represents quality scores.
 * \ingroup quality
 *
 * \details
 *
 * In addition to the requirements for bio::alphabet::writable_alphabet, the bio::alphabet::writable_quality_alphabet
 * introduces the requirements of bio::alphabet::quality_alphabet.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::writable_alphabet
 *   2. `t` shall model bio::alphabet::quality_alphabet
 *   3. bio::alphabet::assign_phred_to needs to be defined for objects of type `t`
 *
 * See the documentation pages for the respective requirements.
 *
 * ### Related types
 *
 * If a given type `t` models this concept, the following types typically do so, as well:
 *
 *   * `t &`
 *
 * `const`-qualified types on the other hand are not assignable.
 */
//!\cond
template <typename t>
concept writable_quality_alphabet = writable_alphabet<t> && quality_alphabet<t> && requires(t v, phred_t<t> c)
{
    {bio::alphabet::assign_phred_to(c, v)};
};
//!\endcond

} // namespace bio::alphabet
