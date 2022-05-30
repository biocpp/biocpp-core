# The BioC++ Core Library

This is the core library of the [BioC++ project](https://github.com/biocpp/biocpp). It provides the following modules:

  * Alphabet: data structures and utilities for representing single biological compounds like DNA, RNA and amino acids.
  * Ranges: containers, views and utilities for storing and manipulating sequences of biological compounds.
  * Meta: meta-programming and auxilliary data structures used by many BioC++ modules.

The design of this module is heavily influenced by **C++ Ranges** and **C++ Concepts**.
A short introduction into the topic can be found [here](https://hannes.hauswedell.net/post/2019/11/30/range_intro/).

## Example

```cpp
std::vector seq = "GATTACA"_dna4;

auto translated = seq | bio::views::translate;

fmt::print("The six frames of \"{}\" are:\n{}\n", seq, translated);
/*
TODO
*/
fmt::print("The second amino acid in the third frame is: {}\n", translated[2][1]);
/*
TODO
*/
```

Here `translated` appears like a vector of protein sequences, however, it is actually a "view", i.e. a light-weight, lazy-evaluated data structure.
It's "elements" are computed on-the-fly and occupy no memory. It still supports random access like a vector.

## About

  * General information on the **BioC++ project** can be found in the [main repository](https://github.com/biocpp/biocpp).
  * The **full documention** (all BioC++ modules) can found here: (TODO).
  * The documentation for just the core module can be found here: (TODO).

## Easy to use

  * Header-only → just drop it in your source code or include it as a git submodule!
  * Zero dependencies → keep complexity low!
  * No build-system and no configure steps required.
  * Optional CMake support available.
  * Integrates well with the standard library and [fmt](https://github.com/fmtlib/fmt).

## Quick-Setup

  * Clone the repository with submodules: `git clone https://github.com/biocpp/biocpp-core.git`
  * Add the following to your compiler invocation:
    * the include directory of BioC++
    * C++20 mode
  * The command could look like this:
```sh
g++-10 -O3 -DNDEBUG -Wall -Wextra          \
    -std=c++20                             \
    -I       /path/to/biocpp-core/include  \
  your_file.cpp
```

**Currently, GCC ≥ 10 is the only supported compiler.** Clang support is planned.
