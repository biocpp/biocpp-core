// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Adds seqan2 to the test environment.
 * \author Marcel Ehrhardt <marcel.ehrhardt AT fu-berlin.de>
 */

#pragma once

#include <bio/meta/platform.hpp>

// SeqAn 2 [optional]
/*!\def BIOCPP_HAS_SEQAN2
 * \brief Whether SeqAn2 library is available or not.
 * \ingroup test
 */
#if __has_include(<seqan/version.h>)

#include <seqan/version.h>

// Require minimum version of seqan2, see https://github.com/seqan/seqan3/issues/748
#if SEQAN_VERSION_MAJOR == 2 && SEQAN_VERSION_MINOR >= 4
#define BIOCPP_HAS_SEQAN2 1
#endif

#endif

#ifdef BIOCPP_HAS_SEQAN2

#include <ranges>

#include <seqan/basic.h>

namespace ranges
{

/*!\brief This makes seqan2 iterators ranges-aware, i.e. makes them fulfil ranges::indirectly_readable which is in turn
 *        needed for ranges::input_iterator and ranges::input_range.
 */
template <typename ...args_t>
struct indirectly_readable_traits<seqan::Iter<args_t...>>
{
    using value_type = typename seqan::Value<seqan::Iter<args_t...>>::Type;
};

} // namespace ranges

#if __cpp_lib_ranges // C++20 ranges available
namespace std
{

/*!\brief This makes seqan2 iterators ranges-aware, i.e. makes them fulfil std::indirectly_readable which is in turn
 *        needed for std::input_iterator and std::ranges::input_range.
 */
template <typename ...args_t>
struct indirectly_readable_traits<seqan::Iter<args_t...>>
{
    using value_type = typename seqan::Value<seqan::Iter<args_t...>>::Type;
};

} // namespace std
#endif //__cpp_lib_ranges
#endif // BIOCPP_HAS_SEQAN2
