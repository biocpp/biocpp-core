# Changelog {#core_changelog}

[TOC]

This changelog contains a top-level entry for each release with sections on new features, API changes and notable
bug-fixes (not all bug-fixes will be listed).

# 0.6.0

## Features

* Added `bio::meta::one_of`, `bio::meta::decays_to` and `bio::meta::different_from` concepts.
* Added `bio::meta::vtag`, `bio::meta::ttag` and `bio::meta::priority_tag`.
* Added `bio::ranges::back_insertable` and `bio::ranges::back_insertable_with` as light-weight "container" concepts.
* Added `bio::views::char_strictly_to` and `bio::views::validate_char_for`; as well as `bio::views::char_conversion_view_t`.
* Added `bio::views::transform_by_pos`, a more flexible version of `std::views::transform`.
* Added `bio::views::add_reverse_complement` which turns a range of sequences into range of sequences with their reverse complements interleaved as every second element.

## Bug-fixes

* Some edge-cases with composite alphabets were fixed.

### Misc changes

* rename `bio::alphabet::alphabet_*` to `bio::alphabet::*`, e.g. `bio::alphabet::alphabet_variant` to `bio::alphabet::variant`.
* `bio::meta::*` several unused traits were removed.
* `bio::ranges::pseudo_random_access_iterator` and `bio::ranges::pseudo_random_access_range` have been removed (because they are not used).
* `bio::ranges::views::to()` has been renamed to `bio::ranges::to()` to be in line with C++23.
* `bio::ranges::sequence` concept has been removed (because it is not used.
* `bio::views::get` has been removed. Use `std::views::elements` instead (same functionality).
* `bio::views::translate*` have been redefined in terms of `bio::views::transform_by_pos` (much less code); `bio::views::translate_single` is now in `include/bio/ranges/views/translate_single.hpp`.
* Cleaned up most of the concept mess in composite alphabets.


## API

* This release is not yet API-stable.
* This release is *mostly* backwards compatible with 0.5.0 (see changes above).


# 0.5.0

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
