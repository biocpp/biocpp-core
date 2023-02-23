// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2021, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2021, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Provides bio::alphabet::tuple_base.
 */

#pragma once

#include <cassert>
#include <concepts>
#include <utility>

#include <bio/alphabet/base.hpp>
#include <bio/alphabet/composite/detail.hpp>
#include <bio/alphabet/concept.hpp>
#include <bio/alphabet/proxy_base.hpp>
#include <bio/meta/type_list/function.hpp>
#include <bio/meta/type_list/traits.hpp>
#include <bio/meta/type_list/type_list.hpp>
#include <bio/meta/type_traits/template_inspection.hpp>

namespace bio::alphabet::detail
{

//!\brief Prevents wrong instantiations of bio::alphabet::tuple_base's equality comparison operators.
template <typename tuple_derived_t, typename rhs_t, typename... component_types>
concept tuple_concept_guard = !meta::one_of<rhs_t, tuple_derived_t, component_types...> &&
                              !meta::list_traits::contains<tuple_derived_t, recursive_required_types_t<rhs_t>>;

} // namespace bio::alphabet::detail

namespace bio::alphabet
{

// forwards
//!\cond
template <typename t>
decltype(auto) get();

template <size_t i>
decltype(auto) get();
//!\endcond

/*!\brief The CRTP base for a combined alphabet that contains multiple values of different alphabets at the same time.
 * \ingroup alphabet_composite
 * \implements bio::alphabet::writable_semialphabet
 * \if DEV
 * \implements bio::alphabet::detail::writable_constexpr_semialphabet
 * \tparam component_types Types of letters; must model bio::alphabet::detail::writable_constexpr_semialphabet.
 * \else
 * \tparam component_types Types of letters; must model std::regular and bio::alphabet::writable_semialphabet and all
 * required function calls need to be callable in `constexpr`-context.
 * \endif
 *
 *
 * This data structure is a CRTP base class for combined alphabets, where the different
 * alphabet letters exist independently as a components, similar to a tuple.
 *
 * Short description:
 *   * combines multiple alphabets as independent components, similar to a tuple;
 *   * models bio::meta::tuple_like, i.e. provides a get interface to its component_list;
 *   * is itself a bio::alphabet::writable_semialphabet, but most derived types implement the full bio::alphabet::writable_alphabet;
 *   * its alphabet size is the product of the individual sizes;
 *   * constructible, assignable and comparable with each component type and also all types that
 *     these are constructible/assignable/comparable with;
 *   * explicitly convertible to each of its component types
 *
 * \attention
 * This is a "pure base class", you cannot instantiate it, you can only inherit from it.
 * Most likely you are interested in using one of it's descendants like bio::alphabet::qualified or bio::alphabet::masked.
 * \cond DEV
 * To make a derived class "complete", you should add at least the following:
 *   * .to_char() member
 *   * .assign_char() member
 *   * a type deduction guide
 * \endcond
 *
 * \sa qualified
 * \sa masked
 *
 */
template <typename derived_type, typename... component_types>
//!\cond
    requires((detail::writable_constexpr_semialphabet<component_types> && ...) &&
             (std::regular<component_types> && ...))
//!\endcond
class tuple_base :
  public base<derived_type,
              (1 * ... * size<component_types>),
              void> // no char type, because this is only semi_alphabet
{
private:
    //!\brief The base type of this class.
    using base_t = base<derived_type,
                        (1 * ... * size<component_types>),
                        void>; // no char type, because this is only semi_alphabet

    //!\brief A bio::meta::type_list The types of each component in the composite
    using component_list = meta::type_list<component_types...>;

    //!\brief Is set to `true` if the type is contained in the type list.
    template <typename type>
    static constexpr bool is_component = meta::list_traits::contains<type, component_list>;

    //!\brief Is set to `true` if the type is uniquely contained in the type list.
    template <typename type>
    static constexpr bool is_unique_component = (meta::list_traits::count<type, component_list> == 1);

    // forward declaration: see implementation below
    template <typename alphabet_type, size_t index>
    class component_proxy;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    constexpr tuple_base() noexcept                          = default; //!< Defaulted.
    constexpr tuple_base(tuple_base const &)                 = default; //!< Defaulted.
    constexpr tuple_base(tuple_base &&) noexcept             = default; //!< Defaulted.
    constexpr tuple_base & operator=(tuple_base const &)     = default; //!< Defaulted.
    constexpr tuple_base & operator=(tuple_base &&) noexcept = default; //!< Defaulted.
    ~tuple_base()                                            = default; //!< Defaulted.

    using base_t::base_t;
    //!\}

    //!\brief Befriend the derived type so that it can instantiate.
    //!\sa https://isocpp.org/blog/2017/04/quick-q-prevent-user-from-derive-from-incorrect-crtp-base
    friend derived_type;

    // Import from base:
    using typename base_t::rank_type;

public:
    // Import from base:
    using base_t::alphabet_size;
    using base_t::assign_rank;
    using base_t::to_rank;

    //!\brief Export this type's components in a visible manner.
    //!\private
    using biocpp_required_types           = component_list;
    //!\brief Export this type's components and possibly the components' components in a visible manner.
    //!\private
    using biocpp_recursive_required_types = meta::list_traits::concat<
      component_list,
      meta::transformation_trait_or_t<detail::recursive_required_types<component_types>, meta::type_list<>>...>;
    //!\brief Make specialisations of this template identifiable in metapgrogramming contexts.
    //!\private
    static constexpr bool biocpp_alphabet_tuple_like = true;

    /*!\name Constructors, destructor and assignment
     * \{
     * \attention Please do not directly use the CRTP base class. The functions
     *            are only public for the usage in their derived classes (e.g.
     *            bio::alphabet::qualified, bio::alphabet::masked).
     */
    //!\brief Construction from initialiser-list.
    constexpr tuple_base(component_types... components) noexcept
    {
        assign_rank(rank_sum_helper(components..., std::make_index_sequence<sizeof...(component_types)>{}));
    }

    /*!\brief Construction via a value of one of the components.
     * \tparam component_type Must be one uniquely contained in the type list of the composite.
     * \param  alph           The value of a component that should be assigned.
     * \details
     *
     * Note: Since the tuple_base is a CRTP base class, we show the working examples
     * with one of its derived classes (bio::alphabet::qualified).
     *
     * \include test/snippet/alphabet/composite/tuple_base_value_construction.cpp
     *
     */
    template <typename component_type>
    //!\cond
        requires((!std::is_base_of_v<tuple_base, component_type>) && is_unique_component<component_type>)
    //!\endcond
    constexpr explicit tuple_base(component_type const alph) noexcept : tuple_base{}
    {
        get<component_type>(*this) = alph;
    }

    /*!\brief Construction via a value of a subtype that is assignable to one of the components.
     * \tparam indirect_component_type Type that models bio::alphabet::weakly_assignable_from for
     *                                 one of the component types.
     * \param  alph                    The value that should be assigned.
     * \details
     *
     * Note that the value will be assigned to the **FIRST** type T that fulfils
     * `assignable_from<T, indirect_component_type>`, regardless if other types are also
     * fit for assignment.
     *
     * Note: Since the tuple_base is a CRTP base class, we show the working examples
     * with one of its derived classes (bio::alphabet::qualified).
     *
     * \include test/snippet/alphabet/composite/tuple_base_subtype_construction.cpp
     *
     */
    template <typename indirect_component_type>
    //!\cond
        requires(detail::tuple_concept_guard<derived_type, indirect_component_type, component_types...> &&
                 (std::is_convertible_v<indirect_component_type, component_types> || ...))
    //!\endcond
    constexpr explicit tuple_base(indirect_component_type const alph) noexcept : tuple_base{}
    {
        using component_predicate = detail::implicitly_convertible_from<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        component_type tmp(alph); // delegate construction
        get<component_type>(*this) = tmp;
    }

    //!\cond
    template <typename indirect_component_type>
        requires(detail::tuple_concept_guard<derived_type, indirect_component_type, component_types...> &&
                 (!(std::is_convertible_v<indirect_component_type, component_types> || ...)) &&
                 (std::is_constructible_v<component_types, indirect_component_type> || ...))
    constexpr explicit tuple_base(indirect_component_type const alph) noexcept : tuple_base{}
    {
        using component_predicate = detail::constructible_from<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        component_type tmp(alph); // delegate construction
        get<component_type>(*this) = tmp;
    }
    //!\endcond

    /*!\brief Assignment via a value of one of the components.
     * \tparam component_type One of the component types. Must be uniquely contained in the type list of the composite.
     * \param  alph           The value of a component that should be assigned.
     * \details
     *
     * Note: Since the tuple_base is a CRTP base class, we show the working examples
     * with one of its derived classes (bio::alphabet::qualified).
     *
     * \include test/snippet/alphabet/composite/tuple_base_value_assignment.cpp
     *
     */
    template <typename component_type>
    //!\cond
        requires((!std::derived_from<component_type, tuple_base>) && is_unique_component<component_type>)
    //!\endcond
    constexpr derived_type & operator=(component_type const alph) noexcept
    {
        get<component_type>(*this) = alph;
        return static_cast<derived_type &>(*this);
    }

    /*!\brief Assignment via a value of a subtype that is assignable to one of the components.
     * \tparam indirect_component_type Type that models bio::alphabet::weakly_assignable_from for
     *                                 one of the component types.
     * \param  alph                    The value of a component that should be assigned.
     * \details
     *
     * Note: Since the tuple_base is a CRTP base class, we show the working examples
     * with one of its derived classes (bio::alphabet::qualified).
     *
     * \include test/snippet/alphabet/composite/tuple_base_subtype_assignment.cpp
     *
     */
    template <typename indirect_component_type>
    //!\cond
        requires((!std::derived_from<indirect_component_type, tuple_base>) &&
                 (!is_unique_component<indirect_component_type>) &&
                 (std::assignable_from<component_types, indirect_component_type> || ...))
    //!\endcond
    constexpr derived_type & operator=(indirect_component_type const alph) noexcept
    {
        using component_predicate = detail::assignable_from<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type       = meta::list_traits::at<component_position, component_list>;
        get<component_type>(*this) = alph; // delegate assignment
        return static_cast<derived_type &>(*this);
    }
    //!\cond
    // If not assignable but implicit convertible, convert first and assign afterwards
    template <typename indirect_component_type>
        requires((!std::derived_from<indirect_component_type, tuple_base>) &&
                 (!is_unique_component<indirect_component_type>) &&
                 (!(std::assignable_from<component_types, indirect_component_type> || ...)) &&
                 (std::convertible_to<indirect_component_type, component_types> || ...))
    constexpr derived_type & operator=(indirect_component_type const alph) noexcept
    {
        using component_predicate = detail::implicitly_convertible_from<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        component_type tmp(alph);
        get<component_type>(*this) = tmp;
        return static_cast<derived_type &>(*this);
    }

    template <typename indirect_component_type>
        requires((!std::derived_from<indirect_component_type, tuple_base>) &&
                 (!is_unique_component<indirect_component_type>) &&
                 (!(std::assignable_from<component_types, indirect_component_type> || ...)) &&
                 (!(std::convertible_to<indirect_component_type, component_types> || ...)) &&
                 (std::constructible_from<component_types, indirect_component_type> || ...))
    constexpr derived_type & operator=(indirect_component_type const alph) noexcept
    {
        using component_predicate = detail::constructible_from<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        component_type tmp(alph); // delegate construction
        get<component_type>(*this) = tmp;
        return static_cast<derived_type &>(*this);
    }
    //!\endcond
    //!\}

    /*!\name Read functions
     * \brief All read operations are constant complexity.
     * \{
     */
    /*!\brief Tuple-like access to the contained components.
     * \tparam index Return the i-th element.
     * \returns A proxy to the contained element that models the same alphabets concepts and supports assignment.
     *
     */
    template <size_t index>
    friend constexpr auto get(tuple_base & l) noexcept
    {
        static_assert(index < sizeof...(component_types), "Index out of range.");

        using t = meta::list_traits::at<index, component_list>;
        t val{};

        bio::alphabet::assign_rank_to(l.to_component_rank<index>(), val);

        return component_proxy<t, index>{l};
    }

    /*!\brief Tuple-like access to the contained components.
     * \tparam type Return the element of specified type; only available if the type is unique in the set of components.
     * \returns A proxy to the contained element that models the same alphabets concepts and supports assignment.
     *
     */
    template <typename type>
    friend constexpr auto get(tuple_base & l) noexcept
      //!\cond
        requires is_unique_component<type>
    //!\endcond
    {
        return get<meta::list_traits::find<type, component_list>>(l);
    }

    /*!\brief Tuple-like access to the contained components.
     * \tparam index Return the i-th element.
     * \returns A copy of the contained element.
     *
     */
    template <size_t index>
    friend constexpr auto get(tuple_base const & l) noexcept
    {
        static_assert(index < sizeof...(component_types), "Index out of range.");

        using t = meta::list_traits::at<index, component_list>;

        return bio::alphabet::assign_rank_to(l.to_component_rank<index>(), t{});
    }

    /*!\brief Tuple-like access to the contained components.
     * \tparam type Return the element of specified type; only available if the type is unique in the set of components.
     * \returns A copy of the contained element.
     *
     */
    template <typename type>
    friend constexpr type get(tuple_base const & l) noexcept
      //!\cond
        requires is_unique_component<type>
    //!\endcond
    {
        return get<meta::list_traits::find<type, component_list>>(l);
    }

    /*!\brief Implicit cast to a single letter. Works only if the type is unique in the type list.
     *
     */
    template <typename type>
    constexpr operator type() const noexcept
      //!\cond
        requires is_unique_component<type>
    //!\endcond
    {
        return get<type>(*this);
    }
    //!\}

    /*!\name Comparison operators (against indirect component_list)
     * \brief These overloads enable comparison for all types that a component type is comparable with.
     * \{
     */
    /*!\brief Comparison against types comparable with components.
     * \tparam indirect_component_type Must be comparable with a component's type.
     * \param lhs Left-hand-side of comparison.
     * \param rhs Right-hand-side of comparison.
     * \returns `true` or `false`.
     *
     * \details
     *
     * To determine (in-)equality/order, it is first deduced which component the argument is comparable with.
     * The tuple is then cast to that type and the resulting value compared with the argument.
     *
     */
    template <std::same_as<derived_type> derived_type_t, typename indirect_component_type>
    //!\cond
        requires(detail::tuple_concept_guard<derived_type, indirect_component_type, component_types...> &&
                 (meta::weakly_equality_comparable_with<indirect_component_type, component_types> || ...))
    //!\endcond
    friend constexpr bool operator==(derived_type_t const lhs, indirect_component_type const rhs) noexcept
    {
        using component_predicate = detail::weakly_equality_comparable_with_<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        return get<component_type>(lhs) == rhs;
    }

    //!\copydoc bio::alphabet::tuple_base::operator==(derived_type_t const lhs, indirect_component_type const rhs)
    template <std::same_as<derived_type> derived_type_t, typename indirect_component_type>
    //!\cond
        requires(detail::tuple_concept_guard<derived_type, indirect_component_type, component_types...> &&
                 (meta::weakly_ordered_with<indirect_component_type, component_types> || ...))
    //!\endcond
    friend constexpr auto operator<=>(derived_type_t const lhs, indirect_component_type const rhs) noexcept
    {
        using component_predicate = detail::weakly_ordered_with_<indirect_component_type>;
        constexpr auto component_position =
          meta::list_traits::find_if<component_predicate::template invoke, component_list>;
        using component_type = meta::list_traits::at<component_position, component_list>;
        return get<component_type>(lhs) <=> rhs;
    }
    //!\}

private:
    //!\brief Return the rank of the i-th component.
    template <size_t index>
    constexpr rank_type to_component_rank() const noexcept
    {
        if constexpr (alphabet_size < 1024) // computation is cached for small alphabets
        {
            return rank_to_component_rank[index][to_rank()];
        }
        else
        {
            return (to_rank() / cummulative_alph_sizes[index]) %
                   bio::alphabet::size<meta::detail::pack_traits::at<index, component_types...>>;
        }
    }

    //!\brief Assign via the rank of i-th component (does not update other components' state).
    template <size_t index>
    constexpr void assign_component_rank(ptrdiff_t const r) noexcept
    {
        assign_rank(static_cast<ptrdiff_t>(to_rank()) + ((r - static_cast<ptrdiff_t>(to_component_rank<index>())) *
                                                         static_cast<ptrdiff_t>(cummulative_alph_sizes[index])));
    }

    //!\brief The cumulative alphabet size products are cached.
    static constexpr std::array<rank_type, component_list::size()> cummulative_alph_sizes = []() constexpr
    {
        // create array (1, |sigma1|, |sigma1|*|sigma2|,  ... ,  |sigma1|*...*|sigmaN|)
        std::array<rank_type, component_list::size() + 1> ret{};
        ret[0]               = 1;
        size_t count         = 1;
        using reverse_list_t = decltype(meta::list_traits::detail::reverse(component_list{}));
        bio::meta::detail::for_each<reverse_list_t>(
          [&](auto alphabet_type_identity) constexpr
          {
              using alphabet_t = typename decltype(alphabet_type_identity)::type;
              ret[count]       = static_cast<rank_type>(bio::alphabet::size<alphabet_t> * ret[count - 1]);
              ++count;
          });

        // reverse and strip one: (|sigma1|*...*|sigmaN-1|, ... |sigma1|*|sigma2|, |sigma1|, 1)
        // reverse order guarantees that the first alphabet is the most significant rank contributer
        // resulting in element-wise alphabetical ordering on comparison
        std::array<rank_type, component_list::size()> ret2{};
        for (size_t i = 0; i < component_list::size(); ++i)
            ret2[i] = ret[component_list::size() - i - 1];

        return ret2;
    }();

    //!\brief For the given components, compute the combined rank.
    template <std::size_t... idx>
    static constexpr rank_type rank_sum_helper(component_types... components,
                                               std::index_sequence<idx...> const &) noexcept
    {
        return ((bio::alphabet::to_rank(components) * cummulative_alph_sizes[idx]) + ...);
    }

    //!\brief Conversion table from rank to the i-th component's rank.
    static constexpr std::array < std::array<rank_type,
                                             alphabet_size<1024 ? alphabet_size : 0>, // not for big alphs
                                             meta::list_traits::size<component_list>>
                                    rank_to_component_rank = []() constexpr
    {
        std::array < std::array<rank_type,
                                alphabet_size<1024 ? alphabet_size : 0>, // not for big alphs
                                meta::list_traits::size<component_list>>
                       ret{};

        if constexpr (alphabet_size < 1024)
        {
            std::array<size_t, alphabet_size> alph_sizes{bio::alphabet::size<component_types>...};

            for (size_t i = 0; i < meta::list_traits::size<component_list>; ++i)
                for (size_t j = 0; j < static_cast<size_t>(alphabet_size); ++j)
                    ret[i][j] = (j / cummulative_alph_sizes[i]) % alph_sizes[i];
        }

        return ret;
    }();
};

/*!\brief Specialisation of bio::alphabet::proxy_base that updates the rank of the tuple_base.
 * \tparam alphabet_type The type of the emulated component.
 * \tparam index         The index of the emulated component.
 *
 */
template <typename derived_type, typename... component_types>
//!\cond
    requires((detail::writable_constexpr_semialphabet<component_types> && ...) &&
             (std::regular<component_types> && ...))
//!\endcond
template <typename alphabet_type, size_t index>
class tuple_base<derived_type, component_types...>::component_proxy :
  public proxy_base<component_proxy<alphabet_type, index>, alphabet_type>
{
private:
    //!\brief The base type.
    using base_t = proxy_base<component_proxy<alphabet_type, index>, alphabet_type>;
    //!\brief Befriend the base type so it can call our bio::alphabet::tuple_base::component_proxy::on_update().
    friend base_t;

    //!\brief Store a pointer to the parent object so we can update it.
    tuple_base * parent;

public:
    //Import from base type:
    using base_t::operator=;

    /*!\name Constructors, destructor and assignment
     * \{
     */
    //!\brief Deleted, because using this proxy without parent would be undefined behaviour.
    component_proxy()                                           = delete;
    constexpr component_proxy(component_proxy const &) noexcept = default; //!< Defaulted.
    constexpr component_proxy(component_proxy &&) noexcept      = default; //!< Defaulted.
    ~component_proxy() noexcept                                 = default; //!< Defaulted.

    //!\brief Construct from a reference to the parent object.
    constexpr component_proxy(tuple_base & p) : parent{&p} {}

    //!\brief Assignment does not change `this`, instead it updates the referenced value.
    constexpr component_proxy & operator=(component_proxy const & rhs) // NOLINT(bugprone-unhandled-self-assignment)
    {
        return assign_rank(rhs.to_rank());
    }

    //!\brief Assignment does not change `this`, instead it updates the referenced value (also works on `const` objects).
    // NOLINTNEXTLINE(bugprone-unhandled-self-assignment)
    constexpr component_proxy const & operator=(component_proxy const & rhs) const
    {
        return assign_rank(rhs.to_rank());
    }
    //!\}

    //!\brief Retrieve the compressed representation.
    constexpr alphabet::rank_t<alphabet_type> to_rank() const noexcept { return parent->to_component_rank<index>(); }

    //!\brief Update the compressed representation.
    constexpr component_proxy & assign_rank(alphabet::rank_t<alphabet_type> const r) noexcept
    {
        parent->assign_component_rank<index>(r);
        return *this;
    }

    //!\brief Update the compressed representation (also works on `const` objects).
    constexpr component_proxy const & assign_rank(alphabet::rank_t<alphabet_type> const r) const noexcept
    {
        parent->assign_component_rank<index>(r);
        return *this;
    }

    /*!\name Comparison operators (proxy type against parent)
     * \brief Comparison against the bio::alphabet::tuple_base that this proxy originates from (necessary
     *        to prevent recursive template instantiation in the tuple).
     * \{
     */
    /*!\brief Comparison against the alphabet tuple by casting self and tuple to the emulated type.
     * \param lhs Left-hand-side of comparison.
     * \param rhs Right-hand-side of comparison.
     */
    friend constexpr bool operator==(derived_type const lhs, component_proxy const rhs) noexcept
    {
        return get<index>(lhs) == static_cast<alphabet_type>(rhs);
    }

    //!\copydoc bio::alphabet::tuple_base::component_proxy::operator==(derived_type const, component_proxy const)
    friend constexpr auto operator<=>(derived_type const lhs, component_proxy const rhs) noexcept
    {
        return get<index>(lhs) <=> static_cast<alphabet_type>(rhs);
    }
    //!\}
};

} // namespace bio::alphabet

namespace std
{

/*!\brief Obtains the type of the specified element.
 * \implements bio::meta::transformation_trait
 * \ingroup alphabet_composite
 * \see [std::tuple_element](https://en.cppreference.com/w/cpp/utility/tuple/tuple_element)
 *
 */
template <std::size_t i, bio::alphabet::detail::alphabet_tuple_like tuple_t>
struct tuple_element<i, tuple_t>
{
    //!\brief Element type.
    using type = bio::meta::list_traits::at<i, typename tuple_t::biocpp_required_types>;
};

/*!\brief Provides access to the number of elements in a tuple as a compile-time constant expression.
 * \implements bio::meta::unary_type_trait
 * \ingroup alphabet_composite
 * \see std::tuple_size_v
 *
 */
template <bio::alphabet::detail::alphabet_tuple_like tuple_t>
struct tuple_size<tuple_t> :
  public std::integral_constant<size_t, bio::meta::list_traits::size<typename tuple_t::biocpp_required_types>>
{};

} // namespace std
