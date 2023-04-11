# Changelog {#core_changelog}

[TOC]

This changelog contains a top-level entry for each release with sections on new features, API changes and notable
bug-fixes (not all bug-fixes will be listed).

# 0.7.1

Summary: fix some ranges by using our own tuple.

## Added

* `bio::meta::tuple` which behaves like C++23's `std::tuple`.

## Fixed

* `bio::views::zip` uses `bio::meta::tuple` and now also models `std::ranges::output_range`.
* `bio::views::pairwise_combine` uses `bio::meta::tuple` and now also models `std::ranges::output_range`.
* `bio::ranges::dictionary` uses `bio::meta::tuple`. Now `bio::ranges::dictionary<> const` works correctly.

## API

* This release is not yet API-stable.
* This release is *mostly* backwards compatible with 0.7.0 (see changes above).


# 0.7.0

Summary: we now support LLVM/clang as a compiler; CIGARs have been much improved and there is a new container.

## Added

* 🐉 Full support for Clang-16 as a compiler. This is a big milestone for 1.0.0!
* A new associative container: `bio::ranges::dictionary`. It has features of `std::vector` and `std::unordered_map` at the cost of extra size.
* New implementation for `bio::alphabet::cigar`. This is now 32bits in size (instead of 64bit) and ABI-compatible to BAM/htslib.
* `bio::views::to_char`, `bio::views::char_to<>` and `bio::views::char_strictly_to<>` now work for `bio::alphabet::cigar` (even though CIGARs are not full alphabets).

## Fixed

* `bio::views::to_char`, `bio::views::char_to<>` and `bio::views::char_strictly_to<>` are now "no-op" views if no transformation needs to happen (e.g. when input *and* output alphabet would be `char`).
* When using `find_package(biocpp COMPONENTS core io)`, the version strings were not set correctly.

## Maintained

* `bio::ranges::small_string` now satisfies the std::ranges::view-concept for capacities <= 30.
* `bio::views::interleave` has been redefined in terms of `bio::views::transform_by_pos` (much less code); should also be a bit faster now.
* Clang-tidy is automatically run as part of the CI.
* Concepts are now properly supported and integrated into the documentation; doxygen-1.9.6 is recommended.

## Renamed

* `bio::alphabet::nucleotide_alphabet` → `bio::alphabet::nucleotide` (same for `aminoacid_alphabet`, `quality_alphabet` and `writable_quality_alphabet`).

## Removed

* `bio::views::as_const` has been removed, use C++23's `std::views::as_const` instead.
* `bio::views::move` has been removed, use C++23's `std::views::as_rvalue` instead.

## API

* This release is not yet API-stable.
* This release is *mostly* backwards compatible with 0.6.0 (see changes above).


# 0.6.0

Summary: The alphabet module is almost API-stable. Several things in meta and ranges were cleaned up.

## Added

* `bio::meta::one_of`, `bio::meta::decays_to` and `bio::meta::different_from` concepts.
* `bio::meta::vtag`, `bio::meta::ttag` and `bio::meta::priority_tag`.
* `bio::ranges::back_insertable` and `bio::ranges::back_insertable_with` as light-weight "container" concepts.
* `bio::views::char_strictly_to` and `bio::views::validate_char_for`; as well as `bio::views::char_conversion_view_t`.
* `bio::views::transform_by_pos`, a more flexible version of `std::views::transform`.
* `bio::views::add_reverse_complement` which turns a range of sequences into range of sequences with their reverse complements interleaved as every second element.

## Fixed

* Some edge-cases with composite alphabets were fixed.
* Many potential bugs and missed optimisations found by clang-tidy (and fixed).
* `bio::ranges::dynamic_bitset` was not an `output_range<bool>` because of incomplete proxy types.

## Maintained

* `bio::views::translate*` have been redefined in terms of `bio::views::transform_by_pos` (much less code); `bio::views::translate_single` is now in `include/bio/ranges/views/translate_single.hpp`.
* Cleaned up most of the concept mess in composite alphabets.
* All alphabet literals (e.g. 'C'_dna4, "GATTACA"_dna4) are now strict and validated at compile-time. Character literals are `consteval`. This prevents bugs early on.

## Renamed

* `bio::alphabet::alphabet_*` to `bio::alphabet::*`, e.g. `bio::alphabet::alphabet_variant` to `bio::alphabet::variant`.
* `bio::alphabet::cpo::` to `bio::alphabet::custom::`; rename `bio/alphabet/adaptation` to `bio/alphabet/custom` and move customisation tags to that folder.
* `bio::ranges::views::to()` has been renamed to `bio::ranges::to()` to be in line with C++23.

## Removed

* `bio::alphabet::*_vector` aliases were removed; just use `std::vector<alphabet>` instead.
* `bio::meta::*` several unused traits were removed.
* `bio::ranges::pseudo_random_access_iterator` and `bio::ranges::pseudo_random_access_range` have been removed (because they are not used).
* `bio::ranges::sequence` concept has been removed (because it is not used).
* `bio::views::get` has been removed. Use `std::views::elements` instead (same functionality).

## API

* This release is not yet API-stable.
* This release is *mostly* backwards compatible with 0.5.0 (see changes above).


# 0.5.0 – "less is more"

## Features

* This release is a fork of SeqAn-3.0.2.
* The feature set is stripped to contain the important "core" aspects: Alphabets and Ranges, as well as metaprogramming support utilities.
* It has no dependencies.

## Some notable changes for users coming from SeqAn3

* 🧹 Huge code cleanup and countless bug-fixes.
* ✂ Removal of all dependencies… and… most of the code :)
* 📦 Separate namespaces for each module.
* 🏷 Much simpler customisation points by using `tag_invoke()`.
* 🖨 Replaced the `debug_stream` with {fmt}.
* 🛸 More Modern C++ goodness: use of the spaceship operator, `consteval`, …
* 📉 Compile-time down.
* 📈 Performance up (for a few things).

## API stability

* This release is not yet API-stable.
* This release is not backwards compatible with any BioC++ or SeqAn version.
