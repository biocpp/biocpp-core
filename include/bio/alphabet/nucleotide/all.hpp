// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2022 deCODE Genetics
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/biocpp/biocpp-core/blob/main/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \author Hannes Hauswedell <hannes.hauswedell AT decode.is>
 * \brief Meta-header for the nucleotide submodule; includes all headers from alphabet/nucleotide/.
 */

#pragma once

#include <bio/alphabet/nucleotide/concept.hpp>
#include <bio/alphabet/nucleotide/dna15.hpp>
#include <bio/alphabet/nucleotide/dna16sam.hpp>
#include <bio/alphabet/nucleotide/dna4.hpp>
#include <bio/alphabet/nucleotide/dna5.hpp>
#include <bio/alphabet/nucleotide/rna15.hpp>
#include <bio/alphabet/nucleotide/rna4.hpp>
#include <bio/alphabet/nucleotide/rna5.hpp>

/*!\defgroup nucleotide Nucleotide
 * \brief Provides the different DNA and RNA alphabet types.
 * \ingroup alphabet
 *
 * \details
 *
 * ### Introduction
 *
 * Nucleotide sequences are at the core of most bioinformatic data processing and while it is possible
 * to represent them in a regular std::string, it makes sense to have specialised data structures in most cases.
 * This sub-module offers multiple nucleotide alphabets that can be used with regular containers and ranges.
 *
 * | Letter   | Description            |            bio::alphabet::dna15        |            bio::alphabet::dna5         |           bio::alphabet::dna4          |         bio::alphabet::rna15           |             bio::alphabet::rna5        |          bio::alphabet::rna4           |
 * |:--------:|------------------------|:--------------------------------------:|:--------------------------------------:|:--------------------------------------:|:--------------------------------------:|:--------------------------------------:|:--------------------------------------:|
 * |   A      | Adenine                |                              A         |                              A         |                              A         |                              A         |                              A         |                              A         |
 * |   C      | Cytosine               |                              C         |                              C         |                              C         |                              C         |                              C         |                              C         |
 * |   G      | Guanine                |                              G         |                              G         |                              G         |                              G         |                              G         |                              G         |
 * |   T      | Thymine (DNA)          |                              T         |                              T         |                              T         | <span style="color:LightGrey">U</span> | <span style="color:LightGrey">U</span> | <span style="color:LightGrey">U</span> |
 * |   U      | Uracil (RNA)           | <span style="color:LightGrey">T</span> | <span style="color:LightGrey">T</span> | <span style="color:LightGrey">T</span> |                              U         |                              U         |                              U         |
 * |   M      | A *or* C               |                              M         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              M         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   R      | A *or* G               |                              R         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              R         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   W      | A *or* T               |                              W         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              W         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   Y      | C *or* T               |                              Y         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |                              Y         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |
 * |   S      | C *or* G               |                              S         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |                              S         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |
 * |   K      | G *or* T               |                              K         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">G</span> |                              K         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">G</span> |
 * |   V      | A *or* C *or* G        |                              V         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              V         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   H      | A *or* C *or* T        |                              H         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              H         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   D      | A *or* G *or* T        |                              D         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |                              D         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">A</span> |
 * |   B      | C *or* G *or* T        |                              B         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |                              B         | <span style="color:LightGrey">N</span> | <span style="color:LightGrey">C</span> |
 * |   N      | A *or* C *or* G *or* T |                              N         |                              N         | <span style="color:LightGrey">A</span> |                              N         |                              N         | <span style="color:LightGrey">A</span> |
 * | **Size** |                        |     15                                 |      5                                 |      4                                 |     15                                 |      5                                 |      4                                 |
 *
 * Keep in mind, that while we think of "the nucleotide alphabet" as consisting of four bases, there are indeed
 * more characters defined with different levels of ambiguity. Depending on your application it will make sense
 * to preserve this ambiguity or to discard it to save space and/or optimise computations.
 * BioC++ offers six distinct nucleotide alphabet types to accommodate for this. There is a seventh alphabet
 * bio::alphabet::dna16sam which implements the alphabet used in SAM/BAM/CRAM.
 *
 * The specialised RNA alphabets are provided for convenience, however the DNA alphabets can handle being assigned a
 * `'U'` character, as well. See below for the details.
 *
 * Which alphabet to chose?
 *   1. in most cases, take bio::alphabet::dna15 (includes all IUPAC characters)
 *   2. if you are memory constrained and sequence data is actually the main memory consumer, use bio::alphabet::dna5
 *   3. if you use specialised algorithms that profit from a 2-bit representation, use bio::alphabet::dna4
 *   4. if you are doing only RNA input/output, use the respective bio::alphabet::rna* type
 *   5. to actually save space from using smaller alphabets, you need a compressed container (e.g.
 *      bio::ranges::bitcompressed_vector)
 *
 * ### Printing and conversion to char
 *
 * As with all alphabets in BioC++, none of the nucleotide alphabets can be directly converted to char or printed
 * with iostreams.
 * You need to explicitly call bio::alphabet::to_char to convert to char or use the {fmt}-library which automatically converts.
 *
 * `T` and `U` are represented by the same rank and you cannot differentiate between them. The only difference between
 * e.g. bio::alphabet::dna4 and bio::alphabet::rna4 is the output when calling to_char().
 *
 * ### Assignment and conversions between nucleotide types
 *
 *   * Nucleotide types defined here are **implicitly** convertible to each other if they have the same size
 *     (e.g. bio::alphabet::dna4 ↔ bio::alphabet::rna4).
 *   * Other nucleotide types are **explicitly** convertible to each other through their character representation.
 *   * All ranges of nucleotide alphabets are convertible to each other via bio::views::convert.
 *   * None of the nucleotide alphabets can be directly converted or assigned from `char`. You need to explicitly call
 *     `assign_char` or use a literal (see below).
 *
 * When assigning from `char` or converting from a larger nucleotide alphabet to a smaller one, *loss of information*
 * can occur since obviously some bases are not available. When converting to bio::alphabet::dna5 or bio::alphabet::rna5,
 * non-canonical bases
 * (letters other than A, C, G, T, U) are converted to `'N'` to preserve ambiguity at that position, while
 * for bio::alphabet::dna4 and bio::alphabet::rna4 they are converted to the first of the possibilities they represent (because
 * there is no letter `'N'` to represent ambiguity). See the greyed out values in the table at the top for
 * an overview of which conversions take place.
 *
 * `char` values that are none of the IUPAC symbols, e.g. 'P', are always converted to the equivalent of assigning 'N',
 * i.e. they result in 'A' for bio::alphabet::dna4 and bio::alphabet::rna4, and in 'N' for the other alphabets.
 *
 * ### Literals
 *
 * To avoid writing ``dna4{}.assign_char('C')`` every time, you may instead use the literal ``'C'_dna4``.
 * All nucleotide types defined here have character literals and also string literals which return a vector of the
 * respective type.
 *
 * ### Concept
 *
 * The nucleotide submodule defines bio::alphabet::nucleotide_alphabet which encompasses all the alphabets defined in the
 * submodule and refines bio::alphabet::alphabet. The only additional requirement is that their values can be
 * complemented, see below.
 *
 * ### Complement
 *
 * | Letter   | Description            | Complement |
 * |:--------:|------------------------|:----------:|
 * |   A      | Adenine                |     T      |
 * |   C      | Cytosine               |     G      |
 * |   G      | Guanine                |     C      |
 * |   T      | Thymine (DNA)          |     A      |
 * |   U      | Uracil (RNA)           |     A      |
 * |   M      | A *or* C               |     K      |
 * |   R      | A *or* G               |     Y      |
 * |   W      | A *or* T               |     W      |
 * |   Y      | C *or* T               |     R      |
 * |   S      | C *or* G               |     S      |
 * |   K      | G *or* T               |     M      |
 * |   V      | A *or* C *or* G        |     B      |
 * |   H      | A *or* C *or* T        |     D      |
 * |   D      | A *or* G *or* T        |     H      |
 * |   B      | C *or* G *or* T        |     V      |
 * |   N      | A *or* C *or* G *or* T |     N      |
 *
 * In the typical structure of DNA molecules (or double-stranded RNA), each nucleotide has a complement that it
 * pairs with. To generate the complement value of a nucleotide letter, you can call an implementation of
 * bio::alphabet::nucleotide_alphabet::complement() on it.
 *
 *
 * For the ambiguous letters, the complement is the (possibly also ambiguous) letter representing the variant of the
 * individual complements.
 *
 */
