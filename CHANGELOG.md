# Changelog {#core_changelog}

[TOC]

This changelog contains a top-level entry for each release with sections on new features, API changes and notable
bug-fixes (not all bug-fixes will be listed).

# 0.6.0

## Features

* Added `bio::meta::one_of` and `bio::meta::decays_to` concepts.
* Added `bio::meta::vtag`, `bio::meta::ttag` and `bio::meta::priority_tag`.
* Added `bio::views::char_strictly_to` and `bio::views::validate_char_for`.
* Added `bio::views::transform_by_pos`, a more flexible version of `std::views::transform`.

## Bug-fixes

* Some edge-cases with composite alphabets were fixed.

### Misc changes

* `bio::alphabet::alphabet_tuple_base` has been renamed to `bio::alphabet::tuple_base`.
* `bio::alphabet::alphabet_variant` has been renamed to `bio::alphabet::variant`.
* `bio::ranges::views::to()` has been renamed to `bio::ranges::to()` to be in line with C++23.
* `bio::views::get` has been removed. Use `std::views::elements` instead.
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

