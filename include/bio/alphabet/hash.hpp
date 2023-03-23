// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 * \brief Provides overloads for std::hash.
 */

#pragma once

#include <bio/alphabet/concept.hpp>

namespace std
{
/*!\brief Struct for hashing a character.
 * \ingroup alphabet
 * \tparam alphabet_t The type of character to hash; must model bio::alphabet::semialphabet.
 */
template <typename alphabet_t>
    requires bio::alphabet::semialphabet<alphabet_t>
struct hash<alphabet_t>
{
    /*!\brief Compute the hash for a character.
     * \param[in] character The character to process; must model bio::alphabet::semialphabet.
     *
     * \returns The rank of the character.
     * \sa bio::alphabet::to_rank.
     */
    size_t operator()(alphabet_t const character) const noexcept { return bio::alphabet::to_rank(character); }
};

} // namespace std
