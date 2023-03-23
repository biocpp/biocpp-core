// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Sara Hetzel <sara.hetzel AT fu-berlin.de>
 * \brief Provides bio::alphabet::semialphabet_any.
 */

#pragma once

#include <bio/alphabet/base.hpp>

namespace bio::alphabet
{

/*!\brief A semi-alphabet that type erases all other semi-alphabets of the same size.
 * \ingroup alphabet_composite
 * \implements bio::alphabet::semialphabet
 * \implements bio::meta::trivially_copyable
 * \implements bio::meta::standard_layout
 *
 * \details
 * This alphabet provides a generic representation for different alphabets of the same size by erasing
 * the type of the alphabet it is constructed from. This enables the usage of a single type although
 * assigning different alphabets. The semialphabet_any can also be converted to any other (semi-)alphabet
 * of the same size.
 * It is therefore possible to convert the semialphabet_any into an alphabet type that is not the original
 * alphabet type. However, this should either be avoided or used with care as no warnings are given when attempting
 * to convert the semialphabet_any into a type that is not comparable to the original alphabet type.
 * The main advantage of using this data structure is to reduce instantiation of templates when using multiple
 * alphabets of the same size and either their character representation is not important or they are reified at
 * a later point in the program.
 *
 * \see https://en.wikipedia.org/wiki/Type_erasure
 * \see https://en.wikipedia.org/wiki/Reification_(computer_science)
 *
 * ### Example
 * \include test/snippet/alphabet/composite/semialphabet_any.cpp
 */
template <size_t size_>
class semialphabet_any : public base<semialphabet_any<size_>, size_, void>
{
private:
    //!\brief Type of the base class.
    using base_t = base<semialphabet_any<size_>, size_, void>;

    //!\brief Befriend the base class.
    friend base_t;

public:
    using base_t::assign_rank;
    using base_t::to_rank;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr semialphabet_any() noexcept                                     = default; //!< Defaulted.
    constexpr semialphabet_any(semialphabet_any const &) noexcept             = default; //!< Defaulted.
    constexpr semialphabet_any(semialphabet_any &&) noexcept                  = default; //!< Defaulted.
    constexpr semialphabet_any & operator=(semialphabet_any const &) noexcept = default; //!< Defaulted.
    constexpr semialphabet_any & operator=(semialphabet_any &&) noexcept      = default; //!< Defaulted.
    ~semialphabet_any() noexcept                                              = default; //!< Defaulted.

    //!\brief Construct semialphabet_any from alphabet of the same size
    template <meta::different_from<semialphabet_any> other_alph_t>
        requires(semialphabet<other_alph_t> && size<other_alph_t> == size_)
    explicit semialphabet_any(other_alph_t const other)
    {
        assign_rank(bio::alphabet::to_rank(other));
    }
    //!\}

    //!\brief Enable conversion of semialphabet_any into other (semi-)alphabet of the same size
    template <meta::different_from<semialphabet_any> other_alph_t>
        requires(semialphabet<other_alph_t> && (size<other_alph_t> == size_) && std::regular<other_alph_t>)
    explicit operator other_alph_t() const
    {
        other_alph_t other;
        assign_rank_to(to_rank(), other);
        return other;
    }
};

} // namespace bio::alphabet
