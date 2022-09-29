// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::alphabet_proxy.
 */

#pragma once

#include <concepts>

#include <bio/alphabet/alphabet_base.hpp>
#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/alphabet/quality/concept.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <bio/meta/type_traits/template_inspection.hpp>

namespace bio::alphabet
{

/*!\brief A CRTP-base that eases the definition of proxy types returned in place of regular alphabets.
 * \tparam derived_type  The CRTP parameter type.
 * \tparam alphabet_type The type of the alphabet that this proxy emulates; must model at least
 *                       bio::alphabet::writable_semialphabet and std::regular.
 * \ingroup alphabet
 *
 * \details
 *
 * Certain containers and other data structure hold alphabet values in a non-standard way so they can convert
 * to that alphabet when being accessed, but cannot return a reference to the held value. These data structures
 * may instead return a *proxy* to the held value which still allows changing it (and updating the underlying data
 * structure to reflect this).
 *
 * This CRTP base facilitates the definition of such proxies. Most users of BioC++ will not need to understand the
 * details.
 *
 * This class ensures that the proxy itself also models bio::alphabet::semialphabet, bio::alphabet::alphabet,
 * bio::alphabet::quality_alphabet, bio::alphabet::nucleotide_alphabet and/or bio::alphabet::aminoacid_alphabet if the emulated type models
 * these. This makes sure that function templates which accept the original, also accept the proxy.
 *
 * ### Implementation notes
 *
 * The derived type needs to provide a `.to_rank()` member and an `.assign_rank()`
 * member with the actual implementations. Additionally, there needs to be a
 * `const` qualified overload for `.assign_rank()`.
 *
 * Most derived_types will also want to specify the default assignment operator
 * to implement "assign-through" and to also provide a `const`-qualified overload
 * of the same.
 *
 * See bio::ranges::bitcompressed_vector or bio::alphabet::tuple_base for examples of how this class is used.
 */
template <typename derived_type, writable_semialphabet alphabet_type>
    //!\cond
    requires std::regular<alphabet_type>
//!\endcond
class alphabet_proxy
{
private:
    //!\brief The type of the alphabet character.
    using char_type = meta::valid_template_spec_or_t<char, char_t, alphabet_type>;

    //!\brief The type of the phred score.
    using phred_type = meta::valid_template_spec_or_t<int8_t, alphabet_phred_t, alphabet_type>;

private:
    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr alphabet_proxy() noexcept                          = default; //!< Defaulted.
    constexpr alphabet_proxy(alphabet_proxy const &)             = default; //!< Defaulted.
    constexpr alphabet_proxy(alphabet_proxy &&)                  = default; //!< Defaulted.
    constexpr alphabet_proxy & operator=(alphabet_proxy const &) = default; //!< Defaulted.
    constexpr alphabet_proxy & operator=(alphabet_proxy &&)      = default; //!< Defaulted.
    ~alphabet_proxy()                                            = default; //!< Defaulted.

    //!\brief Assigment from the emulated type. This function triggers the specialisation in the derived_type.
    constexpr derived_type & operator=(alphabet_type const & c) noexcept
    {
        static_cast<derived_type &>(*this).assign_rank(bio::alphabet::to_rank(c)); // <- this invokes the actual proxy
        return static_cast<derived_type &>(*this);
    }

    //!\overload
    constexpr derived_type const & operator=(alphabet_type const & c) const noexcept
    {
        static_cast<derived_type const &>(*this).assign_rank(
          bio::alphabet::to_rank(c)); // <- this invokes the actual proxy
        return static_cast<derived_type const &>(*this);
    }

    //!\brief Assignment from any type that the emulated type is assignable from.
    template <typename indirect_assignable_type>
    constexpr derived_type & operator=(indirect_assignable_type const & c) const noexcept
      //!\cond
      requires meta::weakly_assignable_from<alphabet_type, indirect_assignable_type>
    //!\endcond
    {
        alphabet_type a;
        a = c;
        return operator=(a);
    }
    //!\}

    //!\brief Befriend the derived type so it can instantiate.
    friend derived_type;

public:
    //!\brief The alphabet size.
    static constexpr auto alphabet_size = bio::alphabet::size<alphabet_type>;

    /*!\name Write functions
     * \brief All of these call the emulated type's write functions and then delegate to
     *        the assignment operator which invokes derived behaviour.
     * \{
     */
    //!\copydoc bio::alphabet::alphabet_base::assign_char
    constexpr derived_type & assign_char(char_type const c) noexcept
      //!\cond
      requires writable_alphabet<alphabet_type>
    //!\endcond
    {
        alphabet_type tmp;
        assign_char_to(c, tmp);
        return operator=(tmp);
    }

    //!\copydoc bio::alphabet::alphabet_base::assign_char
    constexpr derived_type const & assign_char(char_type const c) const noexcept
      //!\cond
      requires writable_alphabet<alphabet_type>
    //!\endcond
    {
        alphabet_type tmp;
        assign_char_to(c, tmp);
        return operator=(tmp);
    }

    //!\copydoc bio::alphabet::quality_base::assign_phred
    constexpr derived_type & assign_phred(phred_type const c) noexcept
      //!\cond
      requires writable_quality_alphabet<alphabet_type>
    //!\endcond
    {
        alphabet_type tmp;
        assign_phred_to(c, tmp);
        return operator=(tmp);
    }

    //!\copydoc bio::alphabet::quality_base::assign_phred
    constexpr derived_type const & assign_phred(phred_type const c) const noexcept
      //!\cond
      requires writable_quality_alphabet<alphabet_type>
    //!\endcond
    {
        alphabet_type tmp;
        assign_phred_to(c, tmp);
        return operator=(tmp);
    }
    //!\}

    /*!\name Read functions
     * \brief All of these call the emulated type's read functions.
     * \{
     */
    //!\brief Implicit conversion to the emulated type.
    constexpr operator alphabet_type() const noexcept
    {
        return alphabet::assign_rank_to(static_cast<derived_type const &>(*this).to_rank(), alphabet_type{});

        /* Instead of static_cast'ing to the alphabet_type which also considers the constructors of the alphabet_type,
         * we explicitly invoke this operator in various places.
         * This prevents errors associated with using alphabet_type's constructors.
         *
         * This is one of error cases:
         * The tuple composite bio::alphabet::qualified returns a component_proxy which inherits from alphabet_proxy_base.
         * The qualified alphabet itself inherits from quality_base.
         * Now when accessing get<1>(seq_qual_alph) we want to call to_phred at some point because we want the quality,
         * therefore the to_phred function from alphabet_proxy is called, but this function did a static_cast to the
         * derived type which is calling the constructor from quality_base. Unfortunately now, the generic quality_base
         * constructor uses `assign_phred_to(to_phred(other), static_cast<derived_type &>(*this))`; (here) which again
         * tries to call to_phred of the alphabet_proxy => infinite loop :boom:
         */
    }

    //!\brief Implicit conversion to types that the emulated type is convertible to.
    template <typename other_t>
        //!\cond
        requires(std::convertible_to<alphabet_type, other_t>)
    //!\endcond
    constexpr operator other_t() const noexcept { return operator alphabet_type(); }

    //!\copydoc bio::alphabet::alphabet_base::to_char
    constexpr auto to_char() const noexcept
      //!\cond
      requires alphabet<alphabet_type>
    //!\endcond
    {
        return bio::alphabet::to_char(operator alphabet_type());
    }

    //!\copydoc bio::alphabet::quality_base::to_phred
    constexpr auto to_phred() const noexcept
      //!\cond
      requires quality_alphabet<alphabet_type>
    //!\endcond
    {
        return bio::alphabet::to_phred(operator alphabet_type());
    }

    //!\copydoc bio::alphabet::nucleotide_base::complement
    constexpr alphabet_type complement() const noexcept
      //!\cond
      requires nucleotide_alphabet<alphabet_type>
    //!\endcond
    {
        return bio::alphabet::complement(operator alphabet_type());
    }

    //!\brief Delegate to the emulated type's validator.
    static constexpr bool char_is_valid(char_type const c) noexcept
      //!\cond
      requires writable_alphabet<alphabet_type>
    //!\endcond
    {
        return char_is_valid_for<alphabet_type>(c);
    }
    //!\}

    //!\brief Checks order of `lhs` and `rhs`.
    friend constexpr void swap(derived_type const & lhs, derived_type const & rhs)
    {
        alphabet_type lhs_value = lhs.operator alphabet_type();
        alphabet_type rhs_value = rhs.operator alphabet_type();
        lhs                     = rhs_value;
        rhs                     = lhs_value;
    }

    //!\brief Checks order of `lhs` and `rhs`.
    friend constexpr void swap(derived_type & lhs, derived_type & rhs)
    {
        alphabet_type lhs_value = lhs.operator alphabet_type();
        alphabet_type rhs_value = rhs.operator alphabet_type();
        lhs                     = rhs_value;
        rhs                     = lhs_value;
    }

    /*!\name Comparison operators
     * \brief These are only required if the emulated type allows comparison with types it is not convertible to,
     *        e.g. bio::alphabet::variant.
     * \{
     */
    //!\brief Checks whether the letters `lhs` and `rhs` are equal.
    friend constexpr bool operator==(derived_type const lhs, derived_type const rhs)
    {
        return lhs.to_rank() == rhs.to_rank();
    }

    //!\brief Checks order of `lhs` and `rhs`.
    friend constexpr auto operator<=>(derived_type const lhs, derived_type const rhs)
    {
        return lhs.to_rank() <=> rhs.to_rank();
    }

public:
    //!\brief Allow (in-)equality comparison with types that the emulated type is comparable with.
    template <meta::different_from<derived_type> t>
        requires(meta::weakly_equality_comparable_with<alphabet_type, t>)
    friend constexpr auto operator==(derived_type const lhs, t const rhs) noexcept
    {
        return (lhs.operator alphabet_type() == rhs);
    }

    //!\brief Allow (in-)equality comparison with types that the emulated type is comparable with.
    template <meta::different_from<derived_type> t>
        requires(meta::weakly_equality_comparable_with<alphabet_type, t>)
    friend constexpr auto operator==(t const lhs, derived_type const rhs) noexcept { return (rhs == lhs); }
    //!\}

private:
    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::to_rank, derived_type const a) noexcept { return a.to_rank(); }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_rank_to, auto const r, derived_type & a) noexcept
      requires(requires { {a.assign_rank(r)}; })
    {
        return a.assign_rank(r);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type const & tag_invoke(cpo::assign_rank_to, auto const r, derived_type const & a) noexcept
      requires(requires { {a.assign_rank(r)}; })
    {
        return a.assign_rank(r);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::to_char, derived_type const a) noexcept requires(requires { {a.to_char()}; })
    {
        return a.to_char();
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_char_to, char_type const c, derived_type & a) noexcept
      requires(requires { {a.assign_char(c)}; })
    {
        return a.assign_char(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type const & tag_invoke(cpo::assign_char_to,
                                                     char_type const      c,
                                                     derived_type const & a) noexcept
      requires(requires { {a.assign_char(c)}; })
    {
        return a.assign_char(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for, char_type const c, derived_type) noexcept
      requires(requires { {derived_type::char_is_valid(c)}; })
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr bool tag_invoke(cpo::char_is_valid_for,
                                     char_type const c,
                                     std::type_identity<derived_type>) noexcept
      //!\cond REQ
      requires(requires { {derived_type::char_is_valid(c)}; } && !meta::constexpr_default_initializable<derived_type>)
    //!\endcond
    {
        return derived_type::char_is_valid(c);
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size,
                                     derived_type) noexcept requires meta::constexpr_default_initializable<derived_type>
    {
        return alphabet_size;
    }

    //!\brief tag_invoke() wrapper around member.
    friend consteval auto tag_invoke(cpo::size, std::type_identity<derived_type>) noexcept
      requires(!meta::constexpr_default_initializable<derived_type>)
    {
        return alphabet_size;
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr auto tag_invoke(cpo::complement, derived_type const a) noexcept
      requires(requires { {a.complement()}; })
    {
        return a.complement();
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr phred_type tag_invoke(cpo::to_phred, derived_type const a) noexcept
      requires(requires { {a.to_phred()}; })
    {
        return a.to_phred();
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type & tag_invoke(cpo::assign_phred_to, phred_type const p, derived_type & a) noexcept
      requires(requires { {a.assign_phred(p)}; })
    {
        return a.assign_phred(p);
    }

    //!\brief tag_invoke() wrapper around member.
    friend constexpr derived_type const & tag_invoke(cpo::assign_phred_to,
                                                     phred_type const     p,
                                                     derived_type const & a) noexcept
      requires(requires { {a.assign_phred(p)}; })
    {
        return a.assign_phred(p);
    }
};

} // namespace bio::alphabet
