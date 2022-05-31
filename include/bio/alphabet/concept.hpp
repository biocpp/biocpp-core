// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT fu-berlin.de>
 * \brief Core alphabet concept and free function/type trait wrappers.
 */

#pragma once

#include <bio/alphabet/exception.hpp>
#include <bio/meta/concept/cereal.hpp>
#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/detail/customisation_point.hpp>
#include <bio/meta/detail/type_inspection.hpp>
#include <bio/meta/type_traits/basic.hpp>
#include <type_traits>

// ============================================================================
// forwards
// ============================================================================

namespace bio::custom
{

/*!\brief A type that can be specialised to provide customisation point implementations so that third party types
 *        model alphabet concepts.
 * \tparam t The type you wish to specialise for.
 * \ingroup alphabet
 *
 * \details
 *
 * For examples of when and how you can make use of this type, please see \link about_customisation the page on
 * customisation \endlink and the \link howto_write_an_alphabet_custom section on third party types \endlink in
 * the Alphabet HowTo.
 *
 * Please note that by default the `t const`, `t &` and `t const &` specialisations of this class inherit the
 * specialisation for `t` so you usually only need to provide a specialisation for `t`.
 *
 * \note Only use this, if you cannot provide respective functions in your namespace.
 */
template <typename t>
struct alphabet
{};

//!\cond
template <typename t>
struct alphabet<t const> : alphabet<t>
{};

template <typename t>
struct alphabet<t &> : alphabet<t>
{};

template <typename t>
struct alphabet<t const &> : alphabet<t>
{};
//!\endcond

} // namespace bio::custom

// ============================================================================
// to_rank()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void to_rank(args_t ...) = delete;

//!\brief Functor definition for bio::to_rank.
struct to_rank_fn
{
public:
    SEQAN3_CPO_IMPL(2, bio::custom::alphabet<decltype(v)>::to_rank(v))    // explicit customisation
    SEQAN3_CPO_IMPL(1, to_rank(v)                                       )    // ADL
    SEQAN3_CPO_IMPL(0, v.to_rank()                                      )    // member

public:
    //!\brief Operator definition.
    template <typename alph_t>
    //!\cond
        requires requires (alph_t const a)
        {
            { impl(priority_tag<2>{}, a) };
            requires noexcept(impl(priority_tag<2>{}, a));
            requires std::integral<decltype(impl(priority_tag<2>{}, a))>;
        }
    //!\endcond
    constexpr auto operator()(alph_t const a) const noexcept
    {
        return impl(priority_tag<2>{}, a);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Return the rank representation of a (semi-)alphabet object.
 * \tparam your_type Type of the argument.
 * \param  alph      The (semi-)alphabet object.
 * \returns The rank representation; an integral type.
 * \ingroup alphabet
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   1. A static member function `to_rank(your_type const a)` of the class `bio::custom::alphabet<your_type>`.
 *   2. A free function `to_rank(your_type const a)` in the namespace of your type (or as `friend`).
 *   3. A member function called `to_rank()`.
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type models std::integral.
 *
 * Every (semi-)alphabet type must provide one of the above.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/to_rank.cpp
 *
 * For an example of a full alphabet definition with free function implementations (solution 1. above),
 * see bio::alphabet.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
inline constexpr auto to_rank = detail::adl_only::to_rank_fn{};
//!\}

//!\brief The `rank_type` of the semi-alphabet; defined as the return type of bio::to_rank.
//!\ingroup alphabet
template <typename semi_alphabet_type>
//!\cond
    requires requires { { bio::to_rank(std::declval<semi_alphabet_type>()) }; }
//!\endcond
using alphabet_rank_t = decltype(bio::to_rank(std::declval<semi_alphabet_type>()));

} // namespace bio

// ============================================================================
// assign_rank_to()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void assign_rank_to(args_t ...) = delete;

//!\brief Functor definition for bio::assign_rank_to.
//!\ingroup alphabet
struct assign_rank_to_fn
{
public:
    SEQAN3_CPO_IMPL(2, (bio::custom::alphabet<decltype(v)>::assign_rank_to(args..., v))) // explicit customisation
    SEQAN3_CPO_IMPL(1, (assign_rank_to(args..., v)                                       )) // ADL
    SEQAN3_CPO_IMPL(0, (v.assign_rank(args...)                                           )) // member

public:
    //!\brief Operator definition.
    template <typename alph_t>
    //!\cond
        requires requires (bio::alphabet_rank_t<alph_t> const r, alph_t & a)
            {
                { impl(priority_tag<2>{}, a, r) };
                requires noexcept(impl(priority_tag<2>{}, a, r));
                requires std::same_as<alph_t &, decltype(impl(priority_tag<2>{}, a, r))>;
            }
    //!\endcond
    constexpr alph_t operator()(bio::alphabet_rank_t<alph_t> const r, alph_t && a) const noexcept
    {
        return impl(priority_tag<2>{}, a, r);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a rank to an alphabet object.
 * \tparam your_type Type of the target object.
 * \param chr  The rank being assigned; must be of the bio::alphabet_rank_t of the target object.
 * \param alph The target object.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   1. A static member function `assign_rank_to(rank_type const chr, your_type & a)` of the class
 *      `bio::custom::alphabet<your_type>`.
 *   2. A free function `assign_rank_to(rank_type const chr, your_type & a)` in the namespace of your
 *      type (or as `friend`).
 *   3. A member function called `assign_rank(rank_type const chr)` (not `assign_rank_to`).
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is `your_type &`.
 *
 * Every (semi-)alphabet type must provide one of the above. *Note* that temporaries of `your_type` are handled
 * by this function object and **do not** require an additional overload.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_rank_to.cpp
 *
 * For an example of a full alphabet definition with free function implementations (solution 1. above),
 * see bio::alphabet.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
inline constexpr auto assign_rank_to = detail::adl_only::assign_rank_to_fn{};
//!\}
} // namespace bio

// ============================================================================
// to_char()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void to_char(args_t ...) = delete;

//!\brief Functor definition for bio::to_char.
struct to_char_fn
{
public:
    SEQAN3_CPO_IMPL(2, bio::custom::alphabet<decltype(v)>::to_char(v))    // explicit customisation
    SEQAN3_CPO_IMPL(1, to_char(v)                                       )    // ADL
    SEQAN3_CPO_IMPL(0, v.to_char()                                      )    // member

public:
    //!\brief Operator definition.
    template <typename alph_t>
    //!\cond
        requires requires (alph_t const a)
        {
            { impl(priority_tag<2>{}, a) };
            requires noexcept(impl(priority_tag<2>{}, a));
            requires builtin_character<decltype(impl(priority_tag<2>{}, a))>;
        }
    //!\endcond
    constexpr decltype(auto) operator()(alph_t const a) const noexcept
    {
        return impl(priority_tag<2>{}, a);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Return the char representation of an alphabet object.
 * \tparam your_type Type of the argument.
 * \param  alph      The alphabet object.
 * \returns The char representation; usually `char`.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   2. A static member function `to_char(your_type const a)` of the class `bio::custom::alphabet<your_type>`.
 *   1. A free function `to_char(your_type const a)` in the namespace of your type (or as `friend`).
 *   3. A member function called `to_char()`.
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type models bio::builtin_character.
 *
 * Every alphabet type must provide one of the above.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/to_char.cpp
 *
 * For an example of a full alphabet definition with free function implementations (solution 1. above),
 * see bio::alphabet.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
inline constexpr auto to_char = detail::adl_only::to_char_fn{};
//!\}

//!\brief The `char_type` of the alphabet; defined as the return type of bio::to_char.
//!\ingroup alphabet
template <typename alphabet_type>
//!\cond
    requires requires (alphabet_type const a) { { bio::to_char(a) }; }
//!\endcond
using alphabet_char_t = decltype(bio::to_char(std::declval<alphabet_type const>()));

} // namespace bio

// ============================================================================
// assign_char_to()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void assign_char_to(args_t ...) = delete;

//!\brief Functor definition for bio::assign_char_to.
//!\ingroup alphabet
struct assign_char_to_fn
{
public:
    SEQAN3_CPO_IMPL(2, (bio::custom::alphabet<decltype(v)>::assign_char_to(args..., v))) // explicit customisation
    SEQAN3_CPO_IMPL(1, (assign_char_to(args..., v)                                       )) // ADL
    SEQAN3_CPO_IMPL(0, (v.assign_char(args...)                                           )) // member

public:
    //!\brief Operator definition.
    template <typename alph_t>
    //!\cond
        requires requires (bio::alphabet_char_t<alph_t> const r, alph_t & a)
            {
                { impl(priority_tag<2>{}, a, r) };
                requires noexcept(impl(priority_tag<2>{}, a, r));
                requires std::same_as<alph_t &, decltype(impl(priority_tag<2>{}, a, r))>;
            }
    //!\endcond
    constexpr alph_t operator()(bio::alphabet_char_t<alph_t> const r, alph_t && a) const noexcept
    {
        return impl(priority_tag<2>{}, a, r);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a character to an alphabet object.
 * \tparam your_type Type of the target object.
 * \param chr  The character being assigned; must be of the bio::alphabet_char_t of the target object.
 * \param alph The target object; its type must model bio::alphabet.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   1. A static member function `assign_char_to(char_type const chr, your_type & a)`
 *      of the class `bio::custom::alphabet<your_type>`.
 *   2. A free function `assign_char_to(char_type const chr, your_type & a)` in the namespace of your
 *      type (or as `friend`).
 *   3. A member function called `assign_char(char_type const chr)` (not `assign_char_to`).
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is `your_type &`.
 *
 * Every alphabet type must provide one of the above. *Note* that temporaries of `your_type` are handled
 * by this function object and **do not** require an additional overload.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_char_to.cpp
 *
 * For an example of a full alphabet definition with free function implementations (solution 1. above),
 * see bio::alphabet.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
inline constexpr auto assign_char_to = detail::adl_only::assign_char_to_fn{};
//!\}
} // namespace bio

// ============================================================================
// char_is_valid_for()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void char_is_valid_for(args_t ...) = delete;

/*!\brief Functor definition for bio::char_is_valid_for.
 * \tparam alph_t   The alphabet type being queried.
 * \ingroup alphabet
 */
template <typename alph_t>
struct char_is_valid_for_fn
{
public:
    //!\brief `alph_t` with cvref removed and possibly wrapped in std::type_identity.
    using s_alph_t = std::conditional_t<std::is_nothrow_default_constructible_v<std::remove_cvref_t<alph_t>>,
                                        std::remove_cvref_t<alph_t>,
                                        std::type_identity<alph_t>>;

    SEQAN3_CPO_IMPL(3, (deferred_type_t<bio::custom::alphabet<alph_t>, decltype(v)>::char_is_valid(v))) // expl. cst.
    SEQAN3_CPO_IMPL(2, (char_is_valid_for(v, s_alph_t{})                                                )) // ADL
    SEQAN3_CPO_IMPL(1, (deferred_type_t<std::remove_cvref_t<alph_t>, decltype(v)>::char_is_valid(v)          )) // member
    SEQAN3_CPO_IMPL(0, (bio::to_char(bio::assign_char_to(v, s_alph_t{})) == v                     )) // fallback

public:
    //!\brief Operator definition.
    template <typename dummy = int> // need to make this a template to enforce deferred instantiation
    //!\cond
        requires requires (alphabet_char_t<alph_t> const a)
        {
            { impl(priority_tag<3>{}, a, dummy{}) };
            requires noexcept(impl(priority_tag<3>{}, a, dummy{}));
            { impl(priority_tag<3>{}, a, dummy{}) } -> std::convertible_to<bool>;
        }
    //!\endcond
    constexpr bool operator()(alphabet_char_t<alph_t> const a) const noexcept
    {
        return impl(priority_tag<3>{}, a);
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Returns whether a character is in the valid set of a bio::alphabet (usually implies a bijective mapping
 *        to an alphabet value).
 * \tparam your_type The alphabet type being queried.
 * \param  chr       The character being checked; must be convertible to `bio::alphabet_char_t<your_type>`.
 * \param  alph      The target object; its type must model bio::alphabet.
 * \returns `true` or `false`.
 * \ingroup alphabet
 *
 * \details
 *
 * This is a function object. Invoke it with the parameter(s) specified above.
 *
 * It acts as a wrapper and looks for three possible implementations (in this order):
 *
 *   1. A static member function `char_is_valid(char_type const chr)` of the class `bio::custom::alphabet<your_type>`.
 *   2. A free function `char_is_valid_for(char_type const chr, your_type const &)` in the namespace of your
 *      type (or as `friend`).
 *   3. A `static` member function called `char_is_valid(char_type)` (not `char_is_valid_for`).
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` (`constexpr` is not required,
 * but recommended) and if the returned type is convertible to `bool`. For 2. the value of the second argument
 * to the function shall be ignored, it is only used to select the function via
 * [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl).
 *
 * An alphabet type *may* provide one of the above. If none is provided, this function will declare every character
 * `c` as valid for whom it holds that `bio::to_char(bio::assign_char_to(c, alph_t{})) == c`, i.e. converting
 * back and forth results in the same value.
 *
 * *Note* that if the alphabet type with cvref removed is not std::is_nothrow_default_constructible, this function
 * object will instead look for `char_is_valid_for(char_type const chr, std::type_identity<your_type> const &)`
 * in case 2. In that case the "fallback" above also does not work and you are required to provide
 * such an implementation.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/char_is_valid_for.cpp
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
template <typename alph_t>
//!\cond
    requires requires { { to_char(std::declval<alph_t>()) }; } // to_char() is required by some defs
//!\endcond
inline constexpr auto char_is_valid_for = detail::adl_only::char_is_valid_for_fn<alph_t>{};
//!\}
} // namespace bio

// ============================================================================
// assign_char_strictly_to()
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Functor definition for bio::assign_char_strictly_to.
//!\ingroup alphabet
struct assign_char_strictly_to_fn
{
    //!\brief Operator overload for lvalues.
    template <typename alph_t>
    //!\cond
        requires requires (alph_t a, bio::alphabet_char_t<alph_t> r)
        {
            { bio::assign_char_to(r, a) } -> std::convertible_to<alph_t>;
            { bio::char_is_valid_for<alph_t>(r) } -> std::same_as<bool>;
        }
    //!\endcond
    decltype(auto) operator()(bio::alphabet_char_t<alph_t> const r, alph_t & a) const
    {
        if (!bio::char_is_valid_for<alph_t>(r))
            throw bio::invalid_char_assignment{detail::type_name_as_string<alph_t>, r};

        return bio::assign_char_to(r, a);
    }

    //!\brief Operator overload for rvalues.
    template <typename alph_t>
    //!\cond
        requires requires (alph_t a, bio::alphabet_char_t<alph_t> r)
        {
            { bio::assign_char_to(r, a) } -> std::convertible_to<alph_t>;
            { bio::char_is_valid_for<alph_t>(r) } -> std::same_as<bool>;
        }
    //!\endcond
    auto operator()(bio::alphabet_char_t<alph_t> const r, alph_t && a) const
    {
        return operator()(r, a); // call above function but return by value
    }
};

} // namespace bio::detail::adl_only

namespace bio
{

/*!\name Function objects
 * \{
 */

/*!\brief Assign a character to an alphabet object, throw if the character is not valid.
 * \tparam your_type Type of the target object.
 * \param chr  The character being assigned; must be of the bio::alphabet_char_t of the target object.
 * \param alph The target object; its type must model bio::alphabet.
 * \returns Reference to `alph` if `alph` was given as lvalue, otherwise a copy.
 * \throws bio::invalid_char_assignment If `bio::char_is_valid_for<decltype(alph)>(chr) == false`.
 * \ingroup alphabet

 * \details
 *
 * This is a function object. Invoke it with the parameters specified above.
 *
 * Note that this is not a customisation point and it cannot be "overloaded".
 * It simply invokes bio::char_is_valid_for and bio::assign_char_to.
 *
 * ### Example
 *
 * \include test/snippet/alphabet/assign_char_strictly_to.cpp
 *
 */
inline constexpr auto assign_char_strictly_to = detail::adl_only::assign_char_strictly_to_fn{};
//!\}
} // namespace bio

// ============================================================================
// alphabet_size
// ============================================================================

namespace bio::detail::adl_only
{

//!\brief Poison-pill overload to prevent non-ADL forms of unqualified lookup.
template <typename ...args_t>
void alphabet_size(args_t ...) = delete;

/*!\brief Functor definition used indirectly by for bio::detail::alphabet_size.
 * \tparam alph_t   The type being queried.
 * \ingroup alphabet
 */
template <typename alph_t>
struct alphabet_size_fn
{
public:
    //!\brief `alph_t` with cvref removed and possibly wrapped in std::type_identity.
    using s_alph_t = std::conditional_t<std::is_nothrow_default_constructible_v<std::remove_cvref_t<alph_t>> &&
                                        bio::is_constexpr_default_constructible_v<std::remove_cvref_t<alph_t>>,
                                        std::remove_cvref_t<alph_t>,
                                        std::type_identity<alph_t>>;

    SEQAN3_CPO_IMPL(2, (deferred_type_t<bio::custom::alphabet<alph_t>, decltype(v)>::alphabet_size)) // expl. cst.
    SEQAN3_CPO_IMPL(1, (alphabet_size(v)                                                             )) // ADL
    SEQAN3_CPO_IMPL(0, (deferred_type_t<std::remove_cvref_t<alph_t>, decltype(v)>::alphabet_size          )) // member

public:
    //!\brief Operator definition.
    template <typename dummy = int> // need to make this a template to enforce deferred instantiation
    //!\cond
        requires requires
        {
            { impl(priority_tag<2>{}, s_alph_t{}, dummy{}) };
            requires noexcept(impl(priority_tag<2>{}, s_alph_t{}, dummy{}));
            requires std::integral<std::remove_cvref_t<decltype(impl(priority_tag<2>{}, s_alph_t{}, dummy{}))>>;
        }
    //!\endcond
    constexpr auto operator()() const noexcept
    {
        // The following cannot be added to the list of constraints, because it is not properly deferred
        // for incomplete types which leads to breakage.
        static_assert(SEQAN3_IS_CONSTEXPR(impl(priority_tag<2>{}, s_alph_t{})),
            "Only overloads that are marked constexpr are picked up by bio::alphabet_size.");
        return impl(priority_tag<2>{}, s_alph_t{});
    }
};

//!\cond
// required to prevent https://gcc.gnu.org/bugzilla/show_bug.cgi?id=89953
template <typename alph_t>
    requires requires { { alphabet_size_fn<alph_t>{} }; }
inline constexpr auto alphabet_size_obj = alphabet_size_fn<alph_t>{};
//!\endcond

} // namespace bio::detail::adl_only

namespace bio
{

/*!\brief A type trait that holds the size of a (semi-)alphabet.
 * \tparam your_type The (semi-)alphabet type being queried.
 * \ingroup alphabet
 *
 * \details
 *
 * This type trait is implemented as a global variable template.
 *
 * It is only defined for types that provide one of the following (checked in this order):
 *
 *   1. A `static constexpr` data member of `bio::custom::alphabet<your_type>` called `alphabet_size`.
 *   2. A free function `alphabet_size(your_type const &)` in the namespace of your type (or as `friend`) that
 *      returns the size.
 *   3. A `static constexpr` data member of `your_type` called `alphabet_size`.
 *
 * Functions are only considered for one of the above cases if they are marked `noexcept` **and** `constexpr` and
 * if the returned type models std::integral. For 2. the value of the argument to the function shall be
 * ignored, the argument is only used to select the function via
 * [argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl).
 *
 * Every (semi-)alphabet type must provide one of the above.
 *
 * *Note* that if the (semi-)alphabet type with cvref removed is not std::is_nothrow_default_constructible or not
 * bio::is_constexpr_default_constructible, this object will instead look for
 * `alphabet_size(std::type_identity<your_type> const &)` with the same semantics (in case 2.).
 *
 * ### Example
 *
 * \include test/snippet/alphabet/alphabet_size.cpp
 *
 * For an example of a full alphabet definition with free function implementations (solution 1. above),
 * see bio::alphabet.
 *
 * ### Customisation point
 *
 * This is a customisation point (see \ref about_customisation). To specify the behaviour for your own alphabet type,
 * simply provide one of the three functions specified above.
 */
template <typename alph_t>
//!\cond
    requires requires { { detail::adl_only::alphabet_size_fn<alph_t>{} }; } &&
             requires { { detail::adl_only::alphabet_size_obj<alph_t>() }; } // ICE workarounds
//!\endcond
inline constexpr auto alphabet_size = detail::adl_only::alphabet_size_obj<alph_t>();

// ============================================================================
// semialphabet
// ============================================================================

/*!\interface bio::semialphabet <>
 * \brief The basis for bio::alphabet, but requires only rank interface (not char).
 * \extends std::totally_ordered
 * \extends std::copy_constructible
 * \ingroup alphabet
 *
 * This concept represents the "rank part" of what is considered "an alphabet" in SeqAn. It requires no
 * `char` representation and corresponding interfaces. It is mostly used internally.
 *
 * ### Requirements
 *
 *   1. `t` shall model std::totally_ordered ("has all comparison operators")
 *   2. objects of type `t` shall be efficiently copyable:
 *     * `t` shall model std::copy_constructible and be std::is_nothrow_copy_constructible
 *     * move construction shall not be more efficient than copy construction; this implies no dynamic memory
 *       (de-)allocation [this is a semantic requirement that cannot be checked]
 *   3. bio::alphabet_size needs to be defined for `t`
 *   4. bio::to_rank needs to be defined for objects of type `t`
 *
 * See the documentation pages for the respective requirements.
 * The implications of 2. are that you can always take function arguments of types that model bio::semialphabet
 * by value.
 *
 * It is highly recommended that non-reference types that model this concept, also model:
 *
 *   * std::regular
 *   * std::is_trivially_copyable
 *   * bio::standard_layout
 *
 * All alphabets available in SeqAn (with very few exceptions) do so.
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
concept semialphabet =
    std::totally_ordered<t> &&
    std::copy_constructible<t> &&
    std::is_nothrow_copy_constructible_v<t> &&
    requires (t v)
{
    { bio::alphabet_size<t> };
    { bio::to_rank(v) };
};
//!\endcond

// ============================================================================
// writable_semialphabet
// ============================================================================

/*!\interface bio::writable_semialphabet <>
 * \brief A refinement of bio::semialphabet that adds assignability.
 * \extends bio::semialphabet
 * \ingroup alphabet
 *
 * This concept refines bio::semialphabet and adds the requirement to be able to change the value by
 * assigning a value of the rank representation.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::semialphabet
 *   2. bio::assign_rank_to needs to be defined for objects of type `t`
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
 * Types that model the concept (and all refinements) can be serialised via SeqAn
 * serialisation support.
 * The rank value is (de-)serialised, types need not provide any overloads themselves.
 */
//!\cond
template <typename t>
concept writable_semialphabet = semialphabet<t> && requires (t v, alphabet_rank_t<t> r)
{
    { bio::assign_rank_to(r, v) };
};
//!\endcond

// ============================================================================
// alphabet
// ============================================================================

/*!\interface bio::alphabet <>
 * \brief The generic alphabet concept that covers most data types used in ranges.
 * \extends bio::semialphabet
 * \ingroup alphabet
 *
 * This is the core alphabet concept that many other alphabet concepts refine.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::semialphabet ("has all rank representation")
 *   2. bio::to_char needs to be defined for objects of type `t`
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
concept alphabet = semialphabet<t> && requires (t v)
{
    { bio::to_char(v) };
};
//!\endcond

// ============================================================================
// writable_alphabet
// ============================================================================

/*!\interface bio::writable_alphabet <>
 * \brief Refines bio::alphabet and adds assignability.
 * \extends bio::alphabet
 * \extends bio::writable_semialphabet
 * \ingroup alphabet
 *
 * This concept refines bio::alphabet and bio::writable_semialphabet and adds the requirement to be able to change
 * the value by assigning a value of the character representation.
 *
 * For a detailed overview of how the different alphabet concepts are related, see
 * \ref alphabet module.
 *
 * ### Requirements
 *
 *   1. `t` shall model bio::alphabet
 *   2. `t` shall model bio::writable_semialphabet
 *   3. bio::assign_char_to needs to be defined for objects of type `t`
 *   4. bio::char_is_valid_for needs to be defined for type `t` and an argument of the character representation
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
 * Types that model the concept (and all refinements) can be serialised via SeqAn
 * serialisation support.
 * The rank value is (de-)serialised, types need not provide any overloads themselves.
 */
//!\cond
template <typename t>
concept writable_alphabet = alphabet<t> && writable_semialphabet<t> && requires (t v, alphabet_char_t<t> c)
{
    { bio::assign_char_to(c, v) };

    { bio::char_is_valid_for<t>(c) };
};
//!\endcond

// ============================================================================
//  serialisation
// ============================================================================

/*!\cond DEV
 * \name Generic serialisation functions for all bio::semialphabet
 * \brief All types that satisfy bio::semialphabet can be serialised via Cereal.
 *
 * \{
 */
/*!
 * \brief Save an alphabet letter to stream.
 * \tparam archive_t Must satisfy bio::cereal_output_archive.
 * \tparam alphabet_t Type of l; must satisfy bio::semialphabet.
 * \param l The alphabet letter.
 * \relates bio::semialphabet
 *
 * \details
 *
 * Delegates to bio::to_rank.
 *
 * \attention These functions are never called directly, see the \ref alphabet module on how to use serialisation.
 */
template <cereal_output_archive archive_t, semialphabet alphabet_t>
alphabet_rank_t<alphabet_t> CEREAL_SAVE_MINIMAL_FUNCTION_NAME(archive_t const &, alphabet_t const & l)
{
    return to_rank(l);
}

/*!\brief Restore an alphabet letter from a saved rank.
 * \tparam archive_t Must satisfy bio::cereal_input_archive.
 * \tparam wrapped_alphabet_t A bio::semialphabet after Cereal mangles it up.
 * \param l The alphabet letter (cereal wrapped).
 * \param r The assigned value.
 * \relates bio::semialphabet
 *
 * \details
 *
 * Delegates to bio::assign_rank.
 *
 * \attention These functions are never called directly, see the \ref alphabet module on how to use serialisation.
 */
template <cereal_input_archive archive_t, typename wrapped_alphabet_t>
void CEREAL_LOAD_MINIMAL_FUNCTION_NAME(archive_t const &,
                                       wrapped_alphabet_t && l,
                                       alphabet_rank_t<detail::strip_cereal_wrapper_t<wrapped_alphabet_t>> const & r)
    requires semialphabet<detail::strip_cereal_wrapper_t<wrapped_alphabet_t>>
{
    assign_rank_to(r, static_cast<detail::strip_cereal_wrapper_t<wrapped_alphabet_t> &>(l));
}
/*!\}
 * \endcond
 */

} // namespace bio

namespace bio::detail
{
// ============================================================================
// constexpr_semialphabet
// ============================================================================

/*!\interface bio::detail::constexpr_semialphabet <>
 * \brief A bio::semialphabet that has constexpr accessors.
 * \extends bio::semialphabet
 * \ingroup alphabet
 *
 * The same as bio::semialphabet, except that all required functions are also required to be callable
 * in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept constexpr_semialphabet = semialphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires SEQAN3_IS_CONSTEXPR(to_rank(std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// writable_constexpr_semialphabet
// ============================================================================

/*!\interface bio::detail::writable_constexpr_semialphabet <>
 * \brief A bio::writable_semialphabet that has a constexpr assignment.
 * \extends bio::detail::constexpr_semialphabet
 * \extends bio::writable_semialphabet
 * \ingroup alphabet
 *
 * Refines bio::detail::constexpr_semialphabet and bio::writable_semialphabet and requires that the call to
 * bio::assign_rank_to can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept writable_constexpr_semialphabet = constexpr_semialphabet<t> && writable_semialphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires SEQAN3_IS_CONSTEXPR(bio::assign_rank_to(alphabet_rank_t<t>{}, std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// constexpr_alphabet
// ============================================================================

/*!\interface bio::detail::constexpr_alphabet <>
 * \brief A bio::alphabet that has constexpr accessors.
 * \extends bio::detail::constexpr_semialphabet
 * \extends bio::alphabet
 * \ingroup alphabet
 *
 * Refines bio::detail::constexpr_semialphabet and bio::alphabet and requires that the call to
 * bio::to_char can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept constexpr_alphabet = constexpr_semialphabet<t> && alphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires SEQAN3_IS_CONSTEXPR(to_char(std::remove_reference_t<t>{}));
};
//!\endcond

// ============================================================================
// writable_constexpr_alphabet
// ============================================================================

/*!\interface bio::detail::writable_constexpr_alphabet <>
 * \brief A bio::writable_alphabet that has constexpr accessors.
 * \extends bio::detail::constexpr_alphabet
 * \extends bio::detail::writable_constexpr_semialphabet
 * \extends bio::writable_alphabet
 * \ingroup alphabet
 *
 * Refines bio::detail::constexpr_alphabet, bio::detail::writable_constexpr_semialphabet and
 * bio::writable_alphabet and requires that the calls to bio::assign_char_to and bio::char_is_valid_for
 * can happen in a `constexpr`-context.
 */
//!\cond
template <typename t>
concept writable_constexpr_alphabet =
    constexpr_alphabet<t> && writable_constexpr_semialphabet<t> && writable_alphabet<t> && requires
{
    // currently only tests rvalue interfaces, because we have no constexpr values in this scope to get references to
    requires SEQAN3_IS_CONSTEXPR(bio::assign_char_to(alphabet_char_t<t>{}, std::remove_reference_t<t>{}));
    requires SEQAN3_IS_CONSTEXPR(bio::char_is_valid_for<t>(alphabet_char_t<t>{}));
};
//!\endcond

} // namespace bio::detail
