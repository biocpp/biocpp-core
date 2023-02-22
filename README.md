<img align="left" src="test/documentation/biocpp_logo.svg">

# The BioC++ Core Library

This is the core library of the [BioC++ project](https://github.com/biocpp/biocpp). It provides the following modules:

  * Alphabet: data structures that represent single biological compounds like DNA, RNA and amino acids.
  * Ranges: containers, views and utilities for storing and manipulating sequences of biological compounds.
  * Meta: metaprogramming utilities and auxilliary data structures.

The design of this module is heavily influenced by **C++ Ranges** and **C++ Concepts**.
An introduction into the topic can be found [here](https://biocpp.github.io/core_ranges.html).

General information on the **BioC++ project** can be found in the [main repository](https://github.com/biocpp/biocpp).
The **full documention** (all BioC++ modules) can found [here](https://biocpp.github.io).

**Attention:** this library is currently a work-in-progress. Getting it into a stable state is of highest priority, but until then all kinds of changes are still possible.

## Example

```cpp
std::vector seq = "GATTACATATA"_dna4;
auto translated = seq | bio::views::translate;

fmt::print("The six protein frames of '{}' are:\n{}\n", seq, translated);
/*
The six protein frames of 'GATTACATATA' are:
[DYI, ITY, LHI, YM*, ICN, YVI]
 */

fmt::print("The third frame's second amino acid is: {}\n", translated[2][1]);
/*
The third frame's second amino acid is: H
 */
```

Here `translated` appears like a vector of protein sequences, however, it is actually a "view", i.e. a light-weight,
lazy-evaluated data structure. Its elements are computed on-the-fly and occupy no memory. It still supports
constant-time random access like a vector.

## Easy to use

  * Header-only → just drop it in your source code or include it as a git submodule!
  * Zero dependencies → keep complexity low!
  * No build-system and no configure steps required.
  * Optional CMake support available.
  * Integrates well with the standard library, [{fmt}](https://github.com/fmtlib/fmt) and [cereal](https://github.com/USCiLab/cereal).

## Quick-Setup

  * Clone the repository: `git clone https://github.com/biocpp/biocpp-core.git`
  * Add the include directory of BioC++, activate C++20 and you are good to go:
```sh
g++-10 -O3 -DNDEBUG -Wall -Wextra -std=c++20 -I /path/to/biocpp-core/include your_file.cpp
```
  * If you want to use {fmt}, add `-I /path/to/fmt/include -D FMT_HEADER_ONLY=1`.

**Currently, GCC ≥ 10 and Clang ≥ 16 are the only supported compiler.**
