// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022-2023 deCODE Genetics
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::meta::tuple.
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 */

#pragma once

#include <tuple>

#include <bio/meta/concept/core_language.hpp>
#include <bio/meta/type_list/traits_detail.hpp>

//!\brief Workaround various GCC problems.
#if defined(__GNUC__) && __GNUC__ == 10 // undocumented ICE
#    define BIOCPP_TUPLE_REQ_EQ requires(sizeof...(Types) == sizeof...(UTypes))
#elif defined(__GNUC__) && (__GNUC__ == 11 || __GNUC__ == 12) // gcc-bug: 109425
#    define BIOCPP_TUPLE_REQ_EQ                                                                                        \
        requires((sizeof...(Types) == sizeof...(UTypes)) &&                                                            \
                 requires { requires(std::equality_comparable_with<Types, UTypes> && ...); })
#else
#    define BIOCPP_TUPLE_REQ_EQ                                                                                        \
        requires((sizeof...(Types) == sizeof...(UTypes)) && (std::equality_comparable_with<Types, UTypes> && ...))
#endif

//!\brief Workaround various GCC problems.
#if defined(__GNUC__) && __GNUC__ == 10 // undocumented ICE
#    define BIOCPP_TUPLE_REQ_THREEWAY requires(sizeof...(Types) == sizeof...(UTypes))
#elif defined(__GNUC__) && (__GNUC__ == 11 || __GNUC__ == 12) // gcc-bug: 109425
#    define BIOCPP_TUPLE_REQ_THREEWAY                                                                                  \
        requires((sizeof...(Types) == sizeof...(UTypes)) &&                                                            \
                 requires { requires(std::three_way_comparable_with<Types, UTypes> && ...); })
#else
#    define BIOCPP_TUPLE_REQ_THREEWAY                                                                                  \
        requires((sizeof...(Types) == sizeof...(UTypes)) && (std::three_way_comparable_with<Types, UTypes> && ...))
#endif

namespace bio::meta
{

/*!\brief A [std::tuple](https://en.cppreference.com/w/cpp/utility/tuple) implementation that incorporates most changes
 *        from C++23's standard library.
 * \ingroup meta
 */
template <class... Types>
class tuple : public std::tuple<Types...>
{
private:
    //!\brief The underlying std::tuple type.
    using base_t = std::tuple<Types...>;

    //!\brief Constructs by unfolding another tuple-like object.
    template <typename tuple_like_t, std::size_t... N>
    tuple(tuple_like_t && other, std::integer_sequence<size_t, N...>) :
      base_t(std::get<N>(std::forward<tuple_like_t>(other))...)
    {}

public:
    /*!\name Default constructors and assignments.
     * \{
     */
    tuple()                          = default; //!< Defaulted.
    tuple(tuple const &)             = default; //!< Defaulted.
    tuple & operator=(tuple const &) = default; //!< Defaulted.
    ~tuple()                         = default; //!< Defaulted.
    //!\}

    /*!\brief Returns the tuple as the underlying std::tuple type.
     * \private
     */
    base_t & as_base() noexcept { return *this; }

    /*!\brief Returns the tuple as the underlying std::tuple type.
     * \private
     */
    base_t const & as_base() const noexcept { return *this; }

    using base_t::base_t;

    /*!\name Construct from arguments.
     * \{
     */
    //!@{ Constructs from arguments.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes &> && ...)
    constexpr tuple(UTypes &... other) : base_t(other...)
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(UTypes const &... other) : base_t(other...)
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes> && ...)
    constexpr tuple(UTypes &&... other) : base_t(std::forward<UTypes>(other)...)
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(UTypes const &&... other) : base_t(std::forward<UTypes const>(other)...)
    {}
    //!@}
    //!\}

    /*!\name Construct from tuple.
     * \{
     */
    //!@{ Constructs from tuple.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes &> && ...)
    constexpr tuple(tuple<UTypes...> & other) : tuple(other, std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(tuple<UTypes...> const & other) : tuple(other, std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes> && ...)
    constexpr tuple(tuple<UTypes...> && other) : tuple(std::move(other), std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(tuple<UTypes...> const && other) : tuple(std::move(other), std::index_sequence_for<Types...>{})
    {}
    //!@}
    //!\}

    /*!\name Construct from std::tuple.
     * \{
     */
    //!@{ Constructs from std::tuple.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes &> && ...)
    constexpr tuple(std::tuple<UTypes...> & other) : tuple(other, std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(std::tuple<UTypes...> const & other) : tuple(other, std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes> && ...)
    constexpr tuple(std::tuple<UTypes...> && other) : tuple(std::move(other), std::index_sequence_for<Types...>{})
    {}

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<Types, UTypes const> && ...)
    constexpr tuple(std::tuple<UTypes...> const && other) : tuple(std::move(other), std::index_sequence_for<Types...>{})
    {}

    /*!\name Assign from tuple.
     * \{
     */
    //!@{ Assigns from tuple.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes &> && ...)
    constexpr tuple & operator=(tuple<UTypes...> & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(tuple<UTypes...> const & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes> && ...)
    constexpr tuple & operator=(tuple<UTypes...> && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(tuple<UTypes...> const && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes &> && ...)
    constexpr tuple const & operator=(tuple<UTypes...> & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(tuple<UTypes...> const & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes> && ...)
    constexpr tuple const & operator=(tuple<UTypes...> && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(tuple<UTypes...> const && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }
    //!@}
    //!\}

    /*!\name Assign from std::tuple.
     * \{
     */
    //!@{ Assigns from std::tuple.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes &> && ...)
    constexpr tuple & operator=(std::tuple<UTypes...> & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(std::tuple<UTypes...> const & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes> && ...)
    constexpr tuple & operator=(std::tuple<UTypes...> && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(std::tuple<UTypes...> const && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes &> && ...)
    constexpr tuple const & operator=(std::tuple<UTypes...> & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(std::tuple<UTypes...> const & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes> && ...)
    constexpr tuple const & operator=(std::tuple<UTypes...> && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(std::tuple<UTypes...> const && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }
    //!@}
    //!\}

    /*!\name Assign from std::pair.
     * \{
     */
    //!@{ Assigns from std::pair.
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes &> && ...)
    constexpr tuple & operator=(std::pair<UTypes...> & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(std::pair<UTypes...> const & other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes> && ...)
    constexpr tuple & operator=(std::pair<UTypes...> && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types, UTypes const> && ...)
    constexpr tuple & operator=(std::pair<UTypes...> const && other)
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes &> && ...)
    constexpr tuple const & operator=(std::pair<UTypes...> & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(std::pair<UTypes...> const & other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes> && ...)
    constexpr tuple const & operator=(std::pair<UTypes...> && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_assignable_v<Types const, UTypes const> && ...)
    constexpr tuple const & operator=(std::pair<UTypes...> const && other) const
    {
        [&]<size_t... N>(std::integer_sequence<size_t, N...>)
        { ((std::get<N>(*this) = std::get<N>(other)), ...); }(std::index_sequence_for<Types...>{});

        return *this;
    }
    //!@}
    //!\}

    /*!\name Conversion to std::tuple.
     * \{
     */
    //!@{ Converts to std::tuple
    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<UTypes, Types &> && ...)
    operator std::tuple<UTypes...>() &
    {
        return std::make_from_tuple<std::tuple<UTypes...>>(*this);
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<UTypes, Types const> && ...)
    operator std::tuple<UTypes...>() const &
    {
        return std::make_from_tuple<std::tuple<UTypes...>>(*this);
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<UTypes, Types> && ...)
    operator std::tuple<UTypes...>() &&
    {
        return std::make_from_tuple<std::tuple<UTypes...>>(std::move(*this));
    }

    template <class... UTypes>
        requires(sizeof...(Types) == sizeof...(UTypes)) && (std::is_constructible_v<UTypes, Types const> && ...)
    operator std::tuple<UTypes...>() const &&
    {
        return std::make_from_tuple<std::tuple<UTypes...>>(std::move(*this));
    }
    //!@}
    //!\}

    /*!\name Comparison operators (tuple)
     * \{
     */
    /*!\brief Checks whether `lhs` and `rhs` are equal.
     * \tparam UTypes The types of the elements of `rhs`. Automatically deduced.
     * \param lhs A tuple.
     * \param rhs A tuple with possibly different element types.
     * \returns A bool indicating the result of the comparison.
     */
    template <class... UTypes>
    BIOCPP_TUPLE_REQ_EQ constexpr friend bool operator==(tuple const & lhs, tuple<UTypes...> const & rhs)
    {
        static_assert((std::equality_comparable_with<Types, UTypes> && ...));
        return lhs.as_base() == rhs.as_base();
    }

    /*!\brief Performs a three-way comparison between `lhs` and `rhs`
     * \tparam UTypes The types of the elements of `rhs`. Automatically deduced.
     * \param lhs A tuple.
     * \param rhs A tuple with possibly different element types.
     * \returns An [ordering](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison)
     *          indicating the result of the comparison.
     */
    template <class... UTypes>
    BIOCPP_TUPLE_REQ_THREEWAY constexpr friend auto operator<=>(tuple const & lhs, tuple<UTypes...> const & rhs)
    {
        return lhs.as_base() <=> rhs.as_base();
    }
    //!\}

    /*!\name Comparison operators (std::tuple)
     * \{
     */
    /*!\brief Checks whether `lhs` and `rhs` are equal.
     * \tparam UTypes The types of the elements of `rhs`. Automatically deduced.
     * \param lhs A tuple.
     * \param rhs A std::tuple with possibly different element types.
     * \returns A bool indicating the result of the comparison.
     */
    template <class... UTypes>
    BIOCPP_TUPLE_REQ_EQ constexpr friend bool operator==(tuple const & lhs, std::tuple<UTypes...> const & rhs)
    {
        return lhs.as_base() == rhs;
    }

    /*!\brief Performs a three-way comparison between `lhs` and `rhs`
     * \tparam UTypes The types of the elements of `rhs`. Automatically deduced.
     * \param lhs A tuple.
     * \param rhs A std::tuple with possibly different element types.
     * \returns An [ordering](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison)
     *          indicating the result of the comparison.
     */
    template <class... UTypes>
    BIOCPP_TUPLE_REQ_THREEWAY constexpr friend auto operator<=>(tuple const & lhs, std::tuple<UTypes...> const & rhs)
    {
        return lhs.as_base() <=> rhs;
    }
    //!\}
};

//!\brief Class template argument deduction guide.
template <class... UTypes>
tuple(UTypes...) -> tuple<UTypes...>;

} // namespace bio::meta

#undef BIOCPP_TUPLE_REQ_EQ
#undef BIOCPP_TUPLE_REQ_THREEWAY

//!\cond
namespace std
{

template <typename... args>
struct tuple_size<bio::meta::tuple<args...>> : public tuple_size<std::tuple<args...>>
{};

template <size_t index, typename... args>
struct tuple_element<index, bio::meta::tuple<args...>> : public tuple_element<index, std::tuple<args...>>
{};

template <class... Ts, class... Us>
    requires requires { typename bio::meta::tuple<std::common_type_t<Ts, Us>...>; }
struct common_type<bio::meta::tuple<Ts...>, bio::meta::tuple<Us...>>
{
    using type = bio::meta::tuple<std::common_type_t<Ts, Us>...>;
};

template <class... Ts, class... Us>
    requires requires { typename bio::meta::tuple<std::common_type_t<Ts, Us>...>; }
struct common_type<std::tuple<Ts...>, bio::meta::tuple<Us...>>
{
    using type = bio::meta::tuple<std::common_type_t<Ts, Us>...>;
};

template <class... Ts, class... Us>
    requires requires { typename bio::meta::tuple<std::common_type_t<Ts, Us>...>; }
struct common_type<bio::meta::tuple<Ts...>, std::tuple<Us...>>
{
    using type = bio::meta::tuple<std::common_type_t<Ts, Us>...>;
};

template <class... Ts, class... Us, template <class> class TQual, template <class> class UQual>
    requires requires { typename bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>; }
struct basic_common_reference<bio::meta::tuple<Ts...>, bio::meta::tuple<Us...>, TQual, UQual>
{
    using type = bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>;
};

template <class... Ts, class... Us, template <class> class TQual, template <class> class UQual>
    requires requires { typename bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>; }
struct basic_common_reference<bio::meta::tuple<Ts...>, std::tuple<Us...>, TQual, UQual>
{
    using type = bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>;
};

template <class... Ts, class... Us, template <class> class TQual, template <class> class UQual>
    requires requires { typename bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>; }
struct basic_common_reference<std::tuple<Ts...>, bio::meta::tuple<Us...>, TQual, UQual>
{
    using type = bio::meta::tuple<std::common_reference_t<TQual<Ts>, UQual<Us>>...>;
};

template <std::size_t i, typename... types>
constexpr std::tuple_element_t<i, bio::meta::tuple<types...>> & get(bio::meta::tuple<types...> & t) noexcept
    requires(i < sizeof...(types))
{
    return std::get<i>(static_cast<std::tuple<types...> &>(t));
}

template <std::size_t i, typename... types>
constexpr std::tuple_element_t<i, bio::meta::tuple<types...>> const & get(bio::meta::tuple<types...> const & t) noexcept
    requires(i < sizeof...(types))
{
    return std::get<i>(static_cast<std::tuple<types...> const &>(t));
}

template <std::size_t i, typename... types>
constexpr std::tuple_element_t<i, bio::meta::tuple<types...>> && get(bio::meta::tuple<types...> && t) noexcept
    requires(i < sizeof...(types))
{
    return std::get<i>(static_cast<std::tuple<types...> &&>(std::move(t)));
}

template <std::size_t i, typename... types>
constexpr std::tuple_element_t<i, bio::meta::tuple<types...>> const && get(
  bio::meta::tuple<types...> const && t) noexcept
    requires(i < sizeof...(types))
{
    return std::get<i>(static_cast<std::tuple<types...> const &&>(std::move(t)));
}

template <typename type, typename... types>
constexpr type & get(bio::meta::tuple<types...> & t) noexcept
    requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return std::get<type>(static_cast<std::tuple<types...> &>(t));
}

template <typename type, typename... types>
constexpr type const & get(bio::meta::tuple<types...> const & t) noexcept
    requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return std::get<type>(static_cast<std::tuple<types...> const &>(t));
}

template <typename type, typename... types>
constexpr type && get(bio::meta::tuple<types...> && t) noexcept
    requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return std::get<type>(static_cast<std::tuple<types...> &&>(std::move(t)));
}

template <typename type, typename... types>
constexpr type const && get(bio::meta::tuple<types...> const && t) noexcept
    requires(bio::meta::detail::pack_traits::count<type, types...> == 1)
{
    return std::get<type>(static_cast<std::tuple<types...> const &&>(std::move(t)));
}

} // namespace std
//!\endcond
