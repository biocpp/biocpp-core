// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Core alphabet concept and free function/type trait wrappers.
 */

#pragma once

#include <type_traits>

#include <bio/alphabet/biocpp.hpp>
#include <bio/alphabet/exception.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/detail/priority_tag.hpp>
#include <bio/meta/detail/type_inspection.hpp>
#include <bio/meta/type_traits/basic.hpp>

// ============================================================================
// to_rank()
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::to_rank.
//!\ingroup alphabet
struct to_rank
{};
//!\}
} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

/*!\name Function objects
 * \{
 */

/*!\brief Return the rank representation of a (semi-)alphabet object.
 * \tparam alph_type Type of the argument.
 * \param  alph      The (semi-)alphabet object.
 * \returns The rank representation; an integral type.
 * \ingroup alphabet
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all (semi-)alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/to_rank.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation).
 *
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * constexpr rank_type tag_invoke(bio::alphabet::cpo::to_rank, alph_type const alph) noexcept
 * ```
 *
 * Implementations are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type models std::integral.
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 * \hideinitializer
 */
inline constexpr auto to_rank = []<typename alph_t>(alph_t const a) noexcept
  //!\cond
  requires(requires {
      {
          tag_invoke(cpo::to_rank{}, a)
          } -> std::integral;
      requires noexcept(tag_invoke(cpo::to_rank{}, a));
  })
//!\endcond
{
    return tag_invoke(cpo::to_rank{}, a);
};
//!\}

//!\brief The `rank_type` of the semi-alphabet; defined as the return type of bio::alphabet::to_rank.
//!\ingroup alphabet
template <typename semi_alphabet_type>
    //!\cond
    requires(requires { {bio::alphabet::to_rank(std::declval<semi_alphabet_type>())}; })
//!\endcond
using rank_t = decltype(bio::alphabet::to_rank(std::declval<semi_alphabet_type>()));

} // namespace bio::alphabet

// ============================================================================
// assign_rank_to()
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::assign_rank_to.#
//!\ingroup alphabet
struct assign_rank_to
{};

//!\}

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a rank to an alphabet object.
 * \tparam alph_type Type of the target object.
 * \param rank  The rank being assigned; must be of the bio::alphabet::rank_t of the target object.
 * \param alph The target object.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all (semi-)alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_rank_to.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation).
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * cosntexpr alph_type & tag_invoke(bio::alphabet::assign_rank_to_cpo, rank_type const rank, alph_type & alph) noexcept
 * ```
 *
 * Implementations are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is exactly `alph_type &`.
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 *
 * *Note* that temporaries of `alph_type` are handled
 * by this function object and **do not** require an additional overload.
 * \hideinitializer
 */
inline constexpr auto assign_rank_to = []<typename alph_t>(bio::alphabet::rank_t<alph_t> const r,
                                                           alph_t &&                           a) noexcept -> alph_t
  //!\cond
  requires(requires {
      {
          tag_invoke(cpo::assign_rank_to{}, r, a)
          } -> std::same_as<alph_t &>;
      requires noexcept(tag_invoke(cpo::assign_rank_to{}, r, a));
  })
//!\endcond
{
    return tag_invoke(cpo::assign_rank_to{}, r, a);
};
//!\}
} // namespace bio::alphabet

// ============================================================================
// to_char()
// ============================================================================

namespace bio::alphabet::cpo
{
/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::to_char.
//!\ingroup alphabet
struct to_char
{};
//!\}

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{
/*!\name Function objects
 * \{
 */

/*!\brief Return the char representation of an alphabet object.
 * \tparam alph_type Type of the argument.
 * \param  alph      The alphabet object.
 * \returns The char representation.
 * \ingroup alphabet
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/to_char.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation).
 *
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * constexpr char_type tag_invoke(bio::alphabet::cpo::to_char, alph_type const alph) noexcept
 * ```
 *
 * Implementations are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type models bio::meta::builtin_character.
 *
 * To specify the behaviour for your own alphabet type,
 * simply provide the above function as a `friend` or free function.
 * \hideinitializer
 */
inline constexpr auto to_char = []<typename alph_t>(alph_t const a) noexcept
  //!\cond
  requires(requires {
      {
          tag_invoke(cpo::to_char{}, a)
          } -> std::integral;
      requires noexcept(tag_invoke(cpo::to_char{}, a));
  })
//!\endcond
{
    return tag_invoke(cpo::to_char{}, a);
};
//!\}

//!\brief The `char_type` of the alphabet; defined as the return type of bio::alphabet::to_char.
//!\ingroup alphabet
template <typename alphabet_type>
    //!\cond
    requires(requires(alphabet_type const a) { {bio::alphabet::to_char(a)}; })
//!\endcond
using char_t = decltype(bio::alphabet::to_char(std::declval<alphabet_type const>()));

} // namespace bio::alphabet

// ============================================================================
// assign_char_to()
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::assign_char_to.
//!\ingroup alphabet
struct assign_char_to
{};

//!\}

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a char to an alphabet object.
 * \tparam alph_type Type of the target object.
 * \param chr  The char being assigned; must be of the bio::alphabet::char_t of the target object.
 * \param alph The target object.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all (semi-)alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_char_to.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation).
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * constexpr alph_type & tag_invoke(bio::alphabet::assign_char_to_cpo, char_type const char, alph_type & alph) noexcept
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
inline constexpr auto assign_char_to = []<typename alph_t>(bio::alphabet::char_t<alph_t> const c,
                                                           alph_t &&                           a) noexcept -> alph_t
  //!\cond
  requires(requires {
      {
          tag_invoke(cpo::assign_char_to{}, c, a)
          } -> std::same_as<alph_t &>;
      requires noexcept(tag_invoke(cpo::assign_char_to{}, c, a));
  })
//!\endcond
{
    return tag_invoke(cpo::assign_char_to{}, c, a);
};
//!\}
} // namespace bio::alphabet

// ============================================================================
// char_is_valid_for()
// ============================================================================

namespace bio::alphabet::cpo
{

/*!\name Customisation tag types
 * \{
 */

//!\brief Customisation tag for bio::alphabet::assign_char_to.
//!\ingroup alphabet
struct char_is_valid_for
{};

} // namespace bio::alphabet::cpo

namespace bio::alphabet::detail
{

//!\brief Functor definition for bio::alphabet::char_is_valid_for
template <typename alph_t>
struct char_is_valid_for_fn
{
    //!\brief Default implementation that checks bijectivity.
    template <typename alph2_t>
    static constexpr bool impl(char_t<alph2_t> const chr, meta::detail::priority_tag<0>) noexcept requires
      std::is_nothrow_default_constructible_v<alph2_t>
    {
        return to_char(assign_char_to(chr, alph2_t{})) == chr;
    }

    //!\brief A user-defined implementation picked up via tag_invoke().
    template <typename alph2_t, typename wrap_t = meta::default_initialisable_wrap_t<alph2_t>>
        requires(requires(char_t<alph2_t> const c) {
            {
                tag_invoke(std::declval<cpo::char_is_valid_for>(), c, wrap_t{})
                } -> std::same_as<bool>;
            requires noexcept(tag_invoke(std::declval<cpo::char_is_valid_for>(), c, wrap_t{}));
        })
    static constexpr bool impl(char_t<alph2_t> const chr, meta::detail::priority_tag<1>) noexcept
    {
        return tag_invoke(cpo::char_is_valid_for{}, chr, wrap_t{});
    }

    //!\brief Operator definition.
    constexpr auto operator()(char_t<alph_t> const chr) const noexcept
      -> decltype(impl<std::remove_cvref_t<alph_t>>(chr, meta::detail::priority_tag<1>{}))
    {
        return impl<std::remove_cvref_t<alph_t>>(chr, meta::detail::priority_tag<1>{});
    }
};

//!\}

} // namespace bio::alphabet::detail

namespace bio::alphabet
{

/*!\name Function objects
 * \{
 */

/*!\brief Returns whether a character is in the valid set of a bio::alphabet::alphabet (usually implies a bijective mapping
 *        to an alphabet value).
 * \tparam alph_type The alphabet type being queried.
 * \param  chr       The character being checked; must be convertible to `bio::alphabet::char_t<alph_type>`.
 * \param  alph      The target object; its type must model bio::alphabet::alphabet.
 * \returns `true` or `false`.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all (semi-)alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/char_is_valid_for.cpp
 *
 * ### Default Behaviour
 *
 *  In contrast to the other alphabet related customisation points, it is **optional** to provide an implementation
 * of this one for most¹ alphabets, because a default implementation exists.
 *
 * The default behaviour is that all characters that are "preserved" when assigning to an object are valid,
 * i.e. `to_char(assign_char_to(chr, alph_t{})) == chr`.
 *
 * This means that e.g. assigning 'A' to bio::alphabet::dna4 would be valid, but 'a' would not be, because
 * bio::alphabet::to_char() always produces upper-case for bio::alphabet::dna4. For this reason, many
 * alphabets have a specialised validity-check that also accepts defines lower-case letters as valid.
 *
 * ¹ All alphabets where the type is std::is_nothrow_default_constructible.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation).
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * constexpr bool tag_invoke(bio::alphabet::cpo::char_is_valid_for, char_type const char, alph_type) noexcept
 * ```
 *
 * If no implementation is found, it behaves as specified above.
 *
 * Implementations are found via ADL and considered only if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is exactly `bool`.
 *
 * To specify the behaviour for your own alphabet type, simply provide the above function as a `friend` or
 * free function.
 *
 * *Note* that the value of the alph_type argument is irrelevant, only the type is needed.
 *
 * *Note* that if the alphabet type with cvref removed is not std::is_nothrow_default_constructible, this function
 * object will instead look for:
 *
 * ```cpp
 * constexpr bool tag_invoke(bio::alphabet::cpo::char_is_valid_for, char_type const char, std::type_identity<alph_type>) noexcept
 * ```
 *
 * i.e. the type will be wrapped in std::type_identity so it can still be passed as a tag.
 * In that case the *default behaviour* defined above **does not work**, and you are required to provide
 * such an implementation.
 * \hideinitializer
 */
template <typename alph_t>
    //!\cond
    requires(requires(char_t<alph_t> a) {
        {
            detail::char_is_valid_for_fn<alph_t>{}(a)
            } -> std::same_as<bool>;
    })
//!\endcond
inline constexpr auto char_is_valid_for = detail::char_is_valid_for_fn<alph_t>{};

//!\}

} // namespace bio::alphabet

// ============================================================================
// assign_char_strictly_to()
// ============================================================================

namespace bio::alphabet::detail
{

//!\brief Functor definition for bio::alphabet::assign_char_strictly_to.
//!\ingroup alphabet
struct assign_char_strictly_to_fn
{
    //!\brief Operator overload for lvalues.
    template <typename alph_t>
        //!\cond
        requires(requires(alph_t a, bio::alphabet::char_t<alph_t> r) {
            {
                bio::alphabet::assign_char_to(r, a)
                } -> std::convertible_to<alph_t>;
            {
                bio::alphabet::char_is_valid_for<alph_t>(r)
                } -> std::same_as<bool>;
        })
    //!\endcond
    decltype(auto) operator()(bio::alphabet::char_t<alph_t> const r, alph_t & a) const
    {
        if (!bio::alphabet::char_is_valid_for<alph_t>(r))
            throw bio::alphabet::invalid_char_assignment{meta::detail::type_name_as_string<alph_t>, r};

        return bio::alphabet::assign_char_to(r, a);
    }

    //!\brief Operator overload for rvalues.
    template <typename alph_t>
        //!\cond
        requires(requires(alph_t a, bio::alphabet::char_t<alph_t> r) {
            {
                bio::alphabet::assign_char_to(r, a)
                } -> std::convertible_to<alph_t>;
            {
                bio::alphabet::char_is_valid_for<alph_t>(r)
                } -> std::same_as<bool>;
        })
    //!\endcond
    auto operator()(bio::alphabet::char_t<alph_t> const r, alph_t && a) const
    {
        return operator()(r, a); // call above function but return by value
    }
};

} // namespace bio::alphabet::detail

namespace bio::alphabet
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a character to an alphabet object, throw if the character is not valid.
 * \tparam alph_type Type of the target object.
 * \param chr  The character being assigned; must be of the bio::alphabet::char_t of the target object.
 * \param alph The target object; its type must model bio::alphabet::alphabet.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \throws bio::alphabet::invalid_char_assignment If `bio::alphabet::char_is_valid_for<decltype(alph)>(chr) == false`.
 * \ingroup alphabet

 * \details
 *
 * This is a function object. Invoke it with the parameters specified above.
 *
 * Note that this is not a customisation point and it cannot be "overloaded".
 * It simply invokes bio::alphabet::char_is_valid_for and bio::alphabet::assign_char_to.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_char_strictly_to.cpp
 *
 * \hideinitializer
 */
inline constexpr auto assign_char_strictly_to = detail::assign_char_strictly_to_fn{};
//!\}
} // namespace bio::alphabet

// ============================================================================
// size
// ============================================================================

namespace bio::alphabet::cpo
{

//!\brief CPO tag definition for bio::alphabet::size.
//!\ingroup alphabet
struct size
{};

} // namespace bio::alphabet::cpo

namespace bio::alphabet
{

/*!\brief A type trait that holds the size of a (semi-)alphabet.
 * \tparam alph_type The (semi-)alphabet type being queried.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It is defined for all (semi-)alphabets in BioC++.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/alphabet_size.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref biocpp_customisation). *
 * It acts as a wrapper and looks for an implementation with the following signature:
 *
 * ```cpp
 * consteval size_t tag_invoke(bio::alphabet::cpo::size, alph_type) noexcept
 * ```
 *
 * If no implementation is found, it behaves as specified above.
 *
 * Implementations are found via ADL and considered only if they are marked `noexcept`, if they return
 * a std::integral type and if they can be evaluated at compile-time (`consteval` is recommended,
 * but`constexpr` is possible, too).
 *
 * To specify the behaviour for your own alphabet type, simply provide the above function as a `friend` or
 * free function.

 * *Note* that if the alphabet type with cvref removed is not std::is_nothrow_default_constructible
 * **at compile-time**, this function object will instead look for:
 *
 * ```cpp
 * consteval size_t tag_invoke(bio::alphabet::cpo::size, std::type_identity<alph_type>) noexcept
 * ```
 *
 * i.e. the type will be wrapped in std::type_identity so it can still be passed as a tag.
 * \hideinitializer
 */
template <typename alph_t, typename wrap_t = meta::default_initialisable_wrap_t<alph_t>>
    //!\cond
    requires(requires {
        {
            tag_invoke(cpo::size{}, wrap_t{})
            } -> std::integral;
        requires BIOCPP_IS_CONSTEXPR(tag_invoke(cpo::size{}, wrap_t{}));
    })
//!\endcond
inline constexpr auto size = tag_invoke(cpo::size{}, wrap_t{});

// ============================================================================
// semialphabet
// ============================================================================

/*!\interface bio::alphabet::semialphabet <>
 * \brief The basis for bio::alphabet::alphabet, but requires only rank interface (not char).
 * \extends std::totally_ordered
 * \extends std::copy_constructible
 * \ingroup alphabet
 *
 * This concept represents the "rank part" of what is considered "an alphabet" in BioC++. It requires no
 * `char` representation and corresponding interfaces. It is mostly used internally.
 *
 * ### Requirements
 *
 *   1. `t` shall model std::totally_ordered ("has all comparison operators")
 *   2. objects of type `t` shall be efficiently copyable:
 *     * `t` shall model std::copy_constructible and be std::is_nothrow_copy_constructible
 *     * move construction shall not be more efficient than copy construction; this implies no dynamic memory
 *       (de-)allocation [this is a semantic requirement that cannot be checked]
 *   3. bio::alphabet::size needs to be defined for `t`
 *   4. bio::alphabet::to_rank needs to be defined for objects of type `t`
 *
 * See the documentation pages for the respective requirements.
 * The implications of 2. are that you can always take function arguments of types that model bio::alphabet::semialphabet
 * by value.
 *
 * It is highly recommended that non-reference types that model this concept, also model:
 *
 *   * std::regular
 *   * bio::meta::trivial
 *   * bio::meta::standard_layout
 *
 * Almost all alphabets available in BioC++ do so.
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
concept semialphabet = std::totally_ordered<t> && std::copy_constructible<t> &&
  std::is_nothrow_copy_constructible_v<t> && requires(t v)
{
    {bio::alphabet::size<t>};
    {bio::alphabet::to_rank(v)};
};
//!\endcond

// ============================================================================
// writable_semialphabet
// ============================================================================

/*!\interface bio::alphabet::writable_semialphabet <>
 * \brief A refinement of bio::alphabet::semialphabet that adds assignability.
 * \extends bio::alphabet::semialphabet
 * \ingroup alphabet
 *
 * This concept refines bio::alphabet::semialphabet and adds the requirement to be able to change the value by
 * assigning a value of the rank representation.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::semialphabet
 *   2. bio::alphabet::assign_rank_to needs to be defined for objects of type `t`
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
 *
 * ### Serialisation
 *
 * Types that model the concept (and all refinements) can be serialised via BioC++
 * serialisation support.
 * The rank value is (de-)serialised, types need not provide any overloads themselves.
 */
//!\cond
template <typename t>
concept writable_semialphabet = semialphabet<t> && requires(t v, rank_t<t> r)
{
    {bio::alphabet::assign_rank_to(r, v)};
};
//!\endcond

// ============================================================================
// alphabet
// ============================================================================

/*!\interface bio::alphabet::alphabet <>
 * \brief The generic alphabet concept that covers most data types used in ranges.
 * \extends bio::alphabet::semialphabet
 * \ingroup alphabet
 *
 * This is the core alphabet concept that many other alphabet concepts refine.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::semialphabet ("has all rank representation")
 *   2. bio::alphabet::to_char needs to be defined for objects of type `t`
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
concept alphabet = semialphabet<t> && requires(t v)
{
    {bio::alphabet::to_char(v)};
};
//!\endcond

// ============================================================================
// writable_alphabet
// ============================================================================

/*!\interface bio::alphabet::writable_alphabet <>
 * \brief Refines bio::alphabet::alphabet and adds assignability.
 * \extends bio::alphabet::alphabet
 * \extends bio::alphabet::writable_semialphabet
 * \ingroup alphabet
 *
 * This concept refines bio::alphabet::alphabet and bio::alphabet::writable_semialphabet and adds the requirement to be able to change
 * the value by assigning a value of the character representation.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet::alphabet
 *   2. `t` shall model bio::alphabet::writable_semialphabet
 *   3. bio::alphabet::assign_char_to needs to be defined for objects of type `t`
 *   4. bio::alphabet::char_is_valid_for needs to be defined for type `t` and an argument of the character representation
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
 *
 * ### Serialisation
 *
 * Types that model the concept (and all refinements) can be serialised via BioC++
 * serialisation support.
 * The rank value is (de-)serialised, types need not provide any overloads themselves.
 */
//!\cond
template <typename t>
concept writable_alphabet = alphabet<t> && writable_semialphabet<t> && requires(t v, char_t<t> c)
{
    {bio::alphabet::assign_char_to(c, v)};

    {bio::alphabet::char_is_valid_for<t>(c)};
};
//!\endcond

} // namespace bio::alphabet
// ============================================================================
//  serialisation
// ============================================================================

#if __has_include(<cereal/details/traits.hpp>)
#    include <cereal/details/traits.hpp>
#endif

namespace bio::alphabet::detail
{

/*!\brief Removes type-mangling that Cereal does with certain types on loading.
 * \details Helpful when defining templatised save/load/serialize functions.
 * \ingroup meta
 */
#if __has_include(<cereal/details/traits.hpp>)
template <typename type>
using strip_cereal_wrapper_t = typename cereal::traits::strip_minimal<std::decay_t<type>>::type;
#else
template <typename type>
using strip_cereal_wrapper_t = type;
#endif

} // namespace bio::alphabet::detail

namespace bio::alphabet
{

/*!\cond DEV
 * \name Generic serialisation functions for all bio::alphabet::semialphabet
 * \brief All types that satisfy bio::alphabet::semialphabet can be serialised via Cereal.
 *
 * \{
 */
/*!
 * \brief Save an alphabet letter to stream.
 * \tparam archive_t Must satisfy bio::cereal_output_archive.
 * \tparam alphabet_t Type of l; must satisfy bio::alphabet::semialphabet.
 * \param l The alphabet letter.
 * \relates bio::alphabet::semialphabet
 *
 * \details
 *
 * Delegates to bio::alphabet::to_rank.
 *
 * \attention These functions are never called directly, see the \ref alphabet module on how to use serialisation.
 */
template <typename archive_t, semialphabet alphabet_t>
rank_t<alphabet_t> save_minimal(archive_t const &, alphabet_t const & l)
{
    return to_rank(l);
}

/*!\brief Restore an alphabet letter from a saved rank.
 * \tparam archive_t Must satisfy bio::cereal_input_archive.
 * \tparam wrapped_alphabet_t A bio::alphabet::semialphabet after Cereal mangles it up.
 * \param l The alphabet letter (cereal wrapped).
 * \param r The assigned value.
 * \relates bio::alphabet::semialphabet
 *
 * \details
 *
 * Delegates to bio::alphabet::assign_rank.
 *
 * \attention These functions are never called directly, see the \ref alphabet module on how to use serialisation.
 */
template <typename archive_t, typename wrapped_alphabet_t>
void load_minimal(archive_t const &,
                  wrapped_alphabet_t &&                                              l,
                  rank_t<detail::strip_cereal_wrapper_t<wrapped_alphabet_t>> const & r) requires
  semialphabet<detail::strip_cereal_wrapper_t<wrapped_alphabet_t>>
{
    assign_rank_to(r, static_cast<detail::strip_cereal_wrapper_t<wrapped_alphabet_t> &>(l));
}
/*!\}
 * \endcond
 */

} // namespace bio::alphabet

namespace bio::alphabet::detail
{
// ============================================================================
// constexpr_semialphabet
// ============================================================================

/*!\interface bio::alphabet::detail::constexpr_semialphabet <>
 * \brief A bio::alphabet::semialphabet that has constexpr accessors.
 * \extends bio::alphabet::semialphabet
 * \ingroup alphabet
 *
 * The same as bio::alphabet::semialphabet, except that all required functions are also required to be callable
 * in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept constexpr_semialphabet = semialphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires BIOCPP_IS_CONSTEXPR(to_rank(std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// writable_constexpr_semialphabet
// ============================================================================

/*!\interface bio::alphabet::detail::writable_constexpr_semialphabet <>
 * \brief A bio::alphabet::writable_semialphabet that has a constexpr assignment.
 * \extends bio::alphabet::detail::constexpr_semialphabet
 * \extends bio::alphabet::writable_semialphabet
 * \ingroup alphabet
 *
 * Refines bio::alphabet::detail::constexpr_semialphabet and bio::alphabet::writable_semialphabet and requires that the call to
 * bio::alphabet::assign_rank_to can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept writable_constexpr_semialphabet = constexpr_semialphabet<t> && writable_semialphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires BIOCPP_IS_CONSTEXPR(bio::alphabet::assign_rank_to(rank_t<t>{}, std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// constexpr_alphabet
// ============================================================================

/*!\interface bio::alphabet::detail::constexpr_alphabet <>
 * \brief A bio::alphabet::alphabet that has constexpr accessors.
 * \extends bio::alphabet::detail::constexpr_semialphabet
 * \extends bio::alphabet::alphabet
 * \ingroup alphabet
 *
 * Refines bio::alphabet::detail::constexpr_semialphabet and bio::alphabet::alphabet and requires that the call to
 * bio::alphabet::to_char can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept constexpr_alphabet = constexpr_semialphabet<t> && alphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires BIOCPP_IS_CONSTEXPR(to_char(std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// writable_constexpr_alphabet
// ============================================================================

/*!\interface bio::alphabet::detail::writable_constexpr_alphabet <>
 * \brief A bio::alphabet::writable_alphabet that has constexpr accessors.
 * \extends bio::alphabet::detail::constexpr_alphabet
 * \extends bio::alphabet::detail::writable_constexpr_semialphabet
 * \extends bio::alphabet::writable_alphabet
 * \ingroup alphabet
 *
 * Refines bio::alphabet::detail::constexpr_alphabet, bio::alphabet::detail::writable_constexpr_semialphabet and
 * bio::alphabet::writable_alphabet and requires that the calls to bio::alphabet::assign_char_to and bio::alphabet::char_is_valid_for
 * can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept writable_constexpr_alphabet =
  constexpr_alphabet<t> && writable_constexpr_semialphabet<t> && writable_alphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires BIOCPP_IS_CONSTEXPR(bio::alphabet::assign_char_to(char_t<t>{}, std::remove_reference_t<t>{}));
    requires BIOCPP_IS_CONSTEXPR(bio::alphabet::char_is_valid_for<t>(char_t<t>{}));
};
//!\endcond

} // namespace bio::alphabet::detail
