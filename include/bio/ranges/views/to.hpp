// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2022, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2022, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <ranges>

#include <bio/ranges/views/detail.hpp>

namespace bio::detail
{

//!\brief Functor that creates the given container from a range.
//!\ingroup views
template <typename container_t>
struct to_fn
{
private:
    /*!\brief Copy some range into a container
     *
     * \tparam rng_t       type of the range
     * \tparam container_t type of the target container
     */
    auto impl(std::ranges::range auto && rng, container_t & container) const
    {
        std::ranges::copy(rng, std::back_inserter(container));
    }

    /*!\brief Overload for nested ranges.
     *
     * \tparam rng_t       type of the range
     * \tparam container_t type of the target container
     */
    auto impl(std::ranges::range auto && rng, container_t & container) const
        requires std::ranges::range<std::decay_t<decltype(*rng.begin())>>
    {
        auto adapter = to_fn<typename container_t::value_type>{};
        auto inner_rng = rng | std::views::transform(adapter);
        std::ranges::copy(inner_rng, std::back_inserter(container));
    }

public:
    /*!\brief Converts a template-template into a container.
     * \tparam rng_t  The type of the range being processed.
     * \tparam args_t The types of the arguments for the constructor.
     * \param  rng    The range being processed.
     * \param  args   Arguments to pass to the constructor of the container.
     */
    template <std::ranges::range rng_t, typename ...args_t>
    constexpr auto operator()(rng_t && rng, args_t &&... args) const
    {
        auto r = container_t(std::forward<args_t>(args)...);

        // reserve memory if functionality is available
        if constexpr (std::ranges::sized_range<rng_t>
                        && requires (container_t c) { c.reserve(std::size_t{}); }
                    )
        {
            r.reserve(std::ranges::size(rng));
        }

        impl(std::forward<rng_t>(rng), r);
        return r;
    }
};

/**
 * Similar to to_fn, but accepts a template-template as argument.
 * e.g.: to_fn<vector> instead of to_fn<vector<int>>;
 */
template <template<class> class container_t>
struct to_template_template_fn
{
    /*!\brief Converts a template-template into a container.
     * \tparam rng_t  The type of the range being processed.
     * \tparam args_t The types of the arguments for the constructor.
     * \param  rng    The range being processed.
     * \param  args   Arguments to pass to the constructor of the container.
     */
    template <std::ranges::range rng_t, typename... args_t>
    constexpr auto operator()(rng_t && rng, args_t &&... args) const
    {
        using value_t = std::decay_t<decltype(*rng.begin())>;
        auto adapter  = to_fn<container_t<value_t>>{};
        return adapter(std::forward<rng_t>(rng), std::forward<args_t>(args)...);
    }
};

} //namespace bio::detail

namespace bio::views
{

/*!\brief Converts a range to a container.
 * \ingroup views
 * \details
 * To convert a range to a container different call syntax can be used.
 * Either the "pipe syntax" or the "function call" syntax.
 * Both syntaxes support the explicit specification of the target container or
 * a specification with an deduced value type.
 *
 * \include snippet/range/views/to.cpp
 */
template <typename container_t, typename... args_t>
constexpr auto to(args_t &&... args)
{
    return detail::adaptor_from_functor{detail::to_fn<container_t>{}, std::forward<args_t>(args)...};
}

//!\overload
template <template<class...> typename container_t, typename... args_t>
constexpr auto to(args_t &&... args)
{
    return detail::adaptor_from_functor{detail::to_template_template_fn<container_t>{}, std::forward<args_t>(args)...};
}

//!\overload
template <typename container_t, std::ranges::range rng_t, typename... args_t>
constexpr auto to(rng_t && rng, args_t &&... args)
{
    return detail::adaptor_from_functor{detail::to_fn<container_t>{}, std::forward<args_t>(args)...}(std::forward<rng_t>(rng));
}

//!\overload
template <template<class...> typename container_t, std::ranges::range rng_t, typename... args_t>
constexpr auto to(rng_t && rng, args_t &&... args)
{
    return detail::adaptor_from_functor{detail::to_template_template_fn<container_t>{}, std::forward<args_t>(args)...}(std::forward<rng_t>(rng));
}

} // namespace bio::views
