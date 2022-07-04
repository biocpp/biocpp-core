// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Meta-header for the \link range ranges module \endlink.
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 */

#pragma once

#include <bio/ranges/container/all.hpp>
#include <bio/ranges/views/all.hpp>

/*!\defgroup range Ranges
 * \brief The ranges module provides general purpose containers, decorators and views.
 *
 * ### Introduction
 *
 * *Ranges* are an abstraction of "a collection of items", or "something iterable". The most basic definition
 * requires only the existence of `begin()` and `end()` on the range.
 *
 * There are different ways to classify ranges, one way is through the capabilities of its default iterators.
 * This is resembled by the range concepts defined in this module. Another way to classify ranges is by their storage
 * behaviour, i.e. whether they own the data that is accessible through them. See below for more details.
 *
 * Ranges are found throughout the BioC++ library, this module provides general-purpose ranges that are not specific
 * to another module or biological function.
 *
 * ### Iterator capabilities
 *
 * All ranges in BioC++ are either \link std::ranges::input_range input ranges \endlink (they can be read from) or
 * \link std::ranges::output_range output ranges \endlink (they can be written to) or both. E.g. an
 * `std::vector<int>` is both, but a `std::vector<int> const` would only be an input range.
 *
 * \link std::ranges::input_range Input ranges \endlink have different *strengths* that are realised through more
 * refined concepts:
 *
 * std::ranges::input_range < std::ranges::forward_range < std::ranges::bidirectional_range <
 * std::ranges::random_access_range < std::ranges::contiguous_range
 *
 * (Click on the respective concepts to learn the exact definitions)
 *
 * Independent of input or output, a range can also be \link std::ranges::sized_range sized \endlink and/or
 * \link std::ranges::common_range  common \endlink.
 *
 * ### Storage behaviour
 *
 * **Containers** are the ranges most well known, they own their elements. BioC++ makes use of standard STL containers
 * like `std::vector`, but also implements some custom containers. See the \link container container submodule \endlink
 * for more details.
 *
 * **Views** are ranges that are usually defined on another range and transform the underlying range
 * via some algorithm or operation, however, some views are stand-alone, i.e. they are just an algorithm that
 * produces elements. Views do not own any data beyond their algorithm and possible parameters to it and they
 * can always be copied in constant time. The algorithm is required to be lazy-evaluated so it is feasible to
 * combine multiple views. See the \link views views submodule \endlink for more details.
 *
 * The storage behaviour is orthogonal to the range concepts defined by the iterators mentioned above, i.e. you
 * can have a container that satisfies std::ranges::random_access_range (e.g. `std::vector` does, but `std::list`
 * does not) and you can have views or decorators that do so or don't. For some combinations of iterator capabilities
 * and storage behaviour there are extra concept definitions, e.g. bio::ranges::detail::random_access_container.
 *
 * \attention
 *
 * There are ranges in BioC++ that fit neither of these storage categories, e.g. all the files are
 * \link std::ranges::input_range input ranges \endlink (if they are input files) and
 * \link std::ranges::output_range output ranges \endlink (if they are output files), but they are neither
 * containers, decorators nor views.
 *
 * \sa range.hpp
 * \sa https://ericniebler.github.io/range-v3/index.html
 */

/*!\namespace bio::ranges
 * \brief The Ranges module's namespace.
 */

/*!\if DEV
 * \namespace bio::ranges::detail
 * \brief The internal BioC++ namespace.
 * \details
 * The contents of this namespace are not visible to consumers of the library and the documentation is
 * only generated for developers.
 * \endif
 */
