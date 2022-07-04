// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides bio::add_enum_bitwise_operators.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <type_traits>

#include <bio/meta/platform.hpp>

namespace bio
{
/*!\interface bio::enum_bitwise_operators
 * \brief You can expect these functions on all types that overload bio::add_enum_bitwise_operators.
 */
/*!\name Requirements for bio::enum_bitwise_operators
 * \relates bio::enum_bitwise_operators
 * \brief You can expect these member functions.
 * \{
 * \fn operator&(t lhs, t rhs)
 * \brief Returns the binary `&` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary conjunction of `lhs` and `rhs`.
 *
 * \fn operator|(t lhs, t rhs)
 * \brief Returns the binary `|` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary disjunction of `lhs` and `rhs`.
 *
 * \fn operator^(t lhs, t rhs)
 * \brief Returns the binary `^` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary XOR operation on `lhs` and `rhs`.
 *
 * \fn operator~(t lhs)
 * \brief Returns the binary `~` operator of lhs.
 * \param lhs First enum.
 *
 * \returns the binary NOT operation on `lhs`.
 *
 * \fn operator&=(t & lhs, t rhs)
 * \brief Returns the binary `&=` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary AND assigment on `lhs`.
 *
 * \fn operator|=(t & lhs, t rhs)
 * \brief Returns the binary `|=` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary OR assignment on `lhs`.
 *
 * \fn operator^=(t & lhs, t rhs)
 * \brief Returns the binary `^=` operator of lhs and rhs.
 * \param lhs First enum.
 * \param rhs Second enum.
 *
 * \returns the binary XOR assignment on `lhs`.
 * \}
 */

//!\cond DEV
/*!\brief Set to true for a scoped enum to have binary operators overloaded.
 * \ingroup meta
 *
 * \details
 *
 * If this type trait is specialised for an enum, the binary operators `&`, `|`, `^`, `~`, `&=`, `|=`, `^=` will be
 * added and behave just like for ints or unscoped enums.
 *
 * ### Example
 *
 * \include test/snippet/meta/add_enum_bitwise_operators.cpp
 */
template <typename t>
constexpr bool add_enum_bitwise_operators = false;

/*!\name Binary operators for scoped enums
 * \brief Perform binary operations like on ints or weak enums. These overloads are available if
 * bio::add_enum_bitwise_operators is defined for your type.
 * \ingroup meta
 *
 * \details
 *
 * \see bio::meta::add_enum_bitwise_operators
 * \{
 */
template <typename t>
constexpr t operator&(t lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    return static_cast<t>(static_cast<std::underlying_type_t<t>>(lhs) & static_cast<std::underlying_type_t<t>>(rhs));
}

template <typename t>
constexpr t operator|(t lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    return static_cast<t>(static_cast<std::underlying_type_t<t>>(lhs) | static_cast<std::underlying_type_t<t>>(rhs));
}

template <typename t>
constexpr t operator^(t lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    return static_cast<t>(static_cast<std::underlying_type_t<t>>(lhs) ^ static_cast<std::underlying_type_t<t>>(rhs));
}

template <typename t>
constexpr t operator~(t lhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    return static_cast<t>(~static_cast<std::underlying_type_t<t>>(lhs));
}

template <typename t>
constexpr t & operator&=(t & lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    lhs = lhs & rhs;
    return lhs;
}

template <typename t>
constexpr t & operator|=(t & lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    lhs = lhs | rhs;
    return lhs;
}

template <typename t>
constexpr t & operator^=(t & lhs, t rhs) noexcept requires(std::is_enum_v<t> && add_enum_bitwise_operators<t>)
{
    lhs = lhs ^ rhs;
    return lhs;
}
//!\}
//!\endcond

} // namespace bio
