# Changelog {#core_changelog}

[TOC]

This changelog contains a top-level entry for each release with sections on new features, API changes and notable
bug-fixes (not all bug-fixes will be listed).

# 0.1.0

## Features

* This release is a fork of BioC++-3.0.2.
* The feature set is stripped to contain the important "core" aspects: Alphabets and Ranges, as well as metaprogramming support utilities.
* It has no dependencies.

## Some notable changes for users coming from SeqAn3

* 🧹 Huge code cleanup, many bug-fixes and removal of most of the code :)
* 📦 Separate namespaces for each module.
* 🏷 Much simpler customisation points by using `tag_invoke()`.
* 🖨 Replaced the `debug_stream` with {fmt}.
* 🛸 Lots of Modern C++ goodness: use of the spaceship operator, `consteval`, …

## API stability

* This release is not yet API-stable.
* This release is not backwards compatible with any BioC++ or SeqAn version.

