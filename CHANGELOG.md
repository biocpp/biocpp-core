# Changelog {#core_changelog}

[TOC]

This changelog contains a top-level entry for each release with sections on new features, API changes and notable
bug-fixes (not all bug-fixes will be listed).

# 0.6.0

## Features

## Bug-fixes

## API

* This release is not yet API-stable.
* This release is *mostly* backwards compatible with 0.5.0.

### Misc changes

* `bio::ranges::views::to()` has been renamed to `bio::ranges::to()` to be in line with C++23.

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

