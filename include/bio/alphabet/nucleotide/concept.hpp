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
 * \brief Provides bio::nucleotide_alphabet.
 */

#pragma once

#include <bio/alphabet/concept.hpp>
#include <concepts>

// ============================================================================
// complement()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void complement(args_t ...) = delete;

//!\brief Functor definition for bio::complement.
struct complement_fn
{
public:
    BIOCPP_CPO_IMPL(2, bio::custom::alphabet<decltype(v)>::complement(v)) // explicit customisation
    BIOCPP_CPO_IMPL(1, complement(v)                                       ) // ADL
    BIOCPP_CPO_IMPL(0, v.complement()                                      ) // member

public:
    //!\brief Operator definition.
    template <typename nucleotide_t>
    //!\cond
        requires (requires (nucleotide_t const nucl)
        {
            { impl(priority_tag<2>{}, nucl) };
            requires noexcept(impl(priority_tag<2>{}, nucl));
            // requires std::common_with<decltype(impl(priority_tag<2>{}, nucl)), nucleotide_t>; // triggers an ICE
            requires alphabet<decltype(impl(priority_tag<2>{}, nucl))>;
            { impl(priority_tag<2>{}, impl(priority_tag<2>{}, nucl)) }; // you can take the complement again
        })
    //!\endcond
    constexpr auto operator()(nucleotide_t const nucl) const noexcept
    {
        return impl(priority_tag<2>{}, nucl);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects (Nucleotide)
 * \{
 */

/*!\brief Return the complement of a nucleotide object.
 * \tparam your_type Type of the argument.
 * \param  nucl      The nucleotide object for which you want to receive the complement.
 * \returns The complement character of `nucl`, e.g. 'C' for 'G'.
 * \ingroup nucleotide
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   1. A static member function `complement(your_type const a)` of the class `bio::custom::alphabet<your_type>`.
 *   2. A free function `complement(your_type const a)` in the namespace of your type (or as `friend`).
 *   3. A member function called `complement()`.
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is `your_type`.
 *
 * Every nucleotide alphabet type must provide one of the above.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/nucleotide/complement_fn.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
inline constexpr auto complement = detail::adl_only::complement_fn{};
//!\}

// ============================================================================
// nucleotide_alphabet concept
// ============================================================================

/*!\interface bio::nucleotide_alphabet <>
 * \extends bio::alphabet
 * \brief A concept that indicates whether an alphabet represents nucleotides.
 * \ingroup nucleotide
 *
 * \details
 *
 * In addition to the requirements for bio::alphabet, the nucleotide_alphabet introduces
 * a requirement for a complement function: bio::nucleotide_alphabet::complement.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet
 *   2. bio::complement needs to be defined for objects of type `t`
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
concept nucleotide_alphabet = alphabet<t> && requires (t val)
{
    { bio::complement(val) };
};
//!\endcond

} // namespace bio
