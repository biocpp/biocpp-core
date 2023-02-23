// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::aminoacid_base.
 */

#pragma once

#include <bio/alphabet/aminoacid/concept.hpp>
#include <bio/alphabet/base.hpp>
#include <bio/alphabet/detail/convert.hpp>
#include <bio/alphabet/detail/to_lower.hpp>

namespace bio::alphabet
{

/*!\brief A CRTP-base that refines bio::alphabet::base and is used by the amino acids.
 * \ingroup aminoacid
 * \tparam derived_type The CRTP parameter type.
 * \tparam size         The size of the alphabet.
 */
template <typename derived_type, auto size>
class aminoacid_base : public aminoacid_empty_base, public base<derived_type, size, char>
{
private:
    //!\brief Type of the base class.
    using base_t = base<derived_type, size, char>;

    //!\brief Befriend the base class.
    friend base_t;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr aminoacid_base() noexcept                                   = default; //!< Defaulted.
    constexpr aminoacid_base(aminoacid_base const &) noexcept             = default; //!< Defaulted.
    constexpr aminoacid_base(aminoacid_base &&) noexcept                  = default; //!< Defaulted.
    constexpr aminoacid_base & operator=(aminoacid_base const &) noexcept = default; //!< Defaulted.
    constexpr aminoacid_base & operator=(aminoacid_base &&) noexcept      = default; //!< Defaulted.
    ~aminoacid_base() noexcept                                            = default; //!< Defaulted.
    //!\}

    //!\brief Befriend the derived class so it can instantiate.
    friend derived_type;

protected:
    // Import from base:
    using typename base_t::char_type;
    using typename base_t::rank_type;

public:
    using base_t::alphabet_size;
    using base_t::to_rank;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    // This constructor needs to be public, because constructor templates are not inherited otherwise
    //!\brief Allow explicit construction from any other aminoacid type and convert via the character representation.
    template <meta::different_from<derived_type> other_aa_type>
    //!\cond
        requires aminoacid<other_aa_type>
    //!\endcond
    explicit constexpr aminoacid_base(other_aa_type const other) noexcept
    {
        if constexpr (meta::constexpr_default_initializable<other_aa_type> &&
                      detail::writable_constexpr_alphabet<other_aa_type>)
        {
            static_cast<derived_type &>(*this) =
              detail::convert_through_char_representation<derived_type, other_aa_type>[bio::alphabet::to_rank(other)];
        }
        else
        {
            bio::alphabet::assign_char_to(bio::alphabet::to_char(other), static_cast<derived_type &>(*this));
        }
    }
    //!\}

    /*!\brief Validate whether a character value has a one-to-one mapping to an alphabet value.
     *
     * \details
     *
     * Models the bio::alphabet::semialphabet::char_is_valid_for() requirement via the bio::alphabet::char_is_valid_for()
     * wrapper.
     *
     * Behaviour specific to amino acids: True also for lower case letters that silently convert to their upper case.
     *
     * ### Complexity
     *
     * Constant.
     *
     * ### Exceptions
     *
     * Guaranteed not to throw.
     */
    static constexpr bool char_is_valid(char_type const c) noexcept
    {
        return valid_char_table[static_cast<uint8_t>(c)];
    }

private:
    //!\brief Implementation of char_is_valid().
    static constexpr std::array<bool, 256> valid_char_table = []() constexpr
    {
        static_assert(sizeof(char_type) == 1, "This table is unusable for char types larger than 1 byte.");

        // init with false
        std::array<bool, 256> ret{};

        // the original valid chars and their lower cases
        for (char_type c : derived_type::rank_to_char)
        {
            ret[c]                   = true;
            ret[detail::to_lower(c)] = true;
        }

        return ret;
    }();
};

} // namespace bio::alphabet
