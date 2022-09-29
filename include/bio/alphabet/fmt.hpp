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

#include <bio/alphabet/concept.hpp>

#if __has_include(<fmt/format.h>)

#    include <fmt/format.h>
#    include <fmt/ranges.h>

#    include <bio/alphabet/composite/detail.hpp>
#    include <bio/ranges/views/to_char.hpp>

template <typename alph_t>
    requires((!std::integral<alph_t>)&&bio::alphabet::alphabet<alph_t>)
struct fmt::formatter<alph_t> : fmt::formatter<bio::alphabet::char_t<alph_t>, bio::alphabet::char_t<alph_t>>
{
    constexpr auto format(alph_t const a, auto & ctx) const
    {
        return fmt::formatter<bio::alphabet::char_t<alph_t>>::format(bio::alphabet::to_char(a), ctx);
    }
};

/* *** Tuple overload ***
 *
 * We need to remove our alphabet_tuples from the default overload first, because
 * there is no concept refinement (onyl enable_if).
 */

template <bio::alphabet::detail::alphabet_tuple_like alph_t>
struct fmt::is_tuple_like<alph_t> : std::false_type
{};

/* *** Ranges overload ***
 *
 * We need to remove our ranges from the default overload first, because
 * there is no concept refinement (onyl enable_if).
 */

//TODO: get rid of the following once formatting ranges as strings works in FMT via {:s}

template <typename rng_t>
concept bio_range = std::ranges::forward_range<rng_t> &&
  (!std::integral<std::ranges::range_value_t<rng_t>>)&&bio::alphabet::alphabet<std::ranges::range_reference_t<rng_t>>;

template <bio_range rng_t>
struct fmt::is_range<rng_t, bio::alphabet::char_t<std::ranges::range_reference_t<rng_t>>> : std::false_type
{};

template <bio_range rng_t, typename char_t>
struct fmt::formatter<rng_t, char_t> :
  fmt::formatter<
    fmt::join_view<std::ranges::iterator_t<decltype(std::declval<rng_t const &>() | bio::ranges::views::to_char)>,
                   std::ranges::sentinel_t<decltype(std::declval<rng_t const &>() | bio::ranges::views::to_char)>,
                   bio::alphabet::char_t<std::ranges::range_reference_t<rng_t const>>>,
    char_t>
{
    // TODO const & is not ideal here, but some fmt-bug breaks other solutions
    // all our formattable ranges are also const-formattable, so it should be OK
    auto format(rng_t const & r, auto & ctx) const
    {
        using trans_t = decltype(std::declval<rng_t const &>() | bio::ranges::views::to_char);
        using formatter_t =
          fmt::formatter<fmt::join_view<std::ranges::iterator_t<trans_t>,
                                        std::ranges::sentinel_t<trans_t>,
                                        bio::alphabet::char_t<std::ranges::range_reference_t<rng_t const>>>>;
        return formatter_t::format(fmt::join(r | bio::ranges::views::to_char, ""), ctx);
    }
};
#else
#    error "You included bio/alphabet/fmt.hpp which only works if the {fmt}-library is present."
#endif
