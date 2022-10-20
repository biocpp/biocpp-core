# Overview {#biocpp_overview}

## Introduction

BioC++ is a project that encompasses multiple C++ libraries that each provide one or more modules. These modules cover typical tasks in Bioinformatics like storing sequence data or reading/writing file formats. The goal of BioC++ is to provide interfaces that are easier to use than those of the C and C++ libraries currently available while still delivering an excellent performance. It leverages *Modern C++* to achieve this.


## Module descriptions

| Library | Module(s)  | Description                                           |
|---------|------------|-------------------------------------------------------|
| Core    | Alphabet   | Datastructures for storing DNA, RNA and amino acids.  |
|         | Ranges     | Containers and Views                                  |
|         | Meta       | Metaprogramming utilities, concepts, tuples           |
| I/O     | I/O        | Input/Output for TXT, CSV, FastA, FastQ, VCF, BCF, …  |

More modules are planned!

## Repositories / CMake / Packages

| Library | Github repo                                                 | CMake pkg   | CMake lib    |
|---------|-------------------------------------------------------------|-------------|--------------|
| Core    | [biocpp/biocpp-core](https://github.com/biocpp/biocpp-core) | biocpp_core | biocpp::core |
| I/O     | [biocpp/biocpp-io](https://github.com/biocpp/biocpp-io)     | biocpp_io   | biocpp::io   |

**CMake pkg:** Include the library(s) with multiple calls to `find_package(biocpp_core REQUIRED)` (changing the respective
CMake pkg) or with a single call to `find_package(biocpp REQUIRED COMPONENTS core io [...])`.

**CMake lib:** Add the respective library to your make target with `target_link_libraries(your_target INTERFACE biocpp::core biocpp::io [...])`.

## Headers and namespaces

| Library |Module(s)  | Include          |Namespaces                            | Code-Owner |
|---------|-----------|------------------|--------------------------------------|------------|
| Core    |Alphabet   | <bio/alphabet/*> |`bio::alphabet`                       | `@h-2`     |
|         |Ranges     | <bio/ranges/*>   |`bio::ranges`, `bio::{ranges::}views` | `@h-2`     |
|         |Meta       | <bio/meta/*>     |`bio::meta`                           | `@h-2`     |
| I/O     |I/O        | <bio/io/*>       |`bio::io`                             | `@h-2`     |


The primary namespace for BioC++ is `bio::`, and each Module is found in a sub-namespace (usually identical to the module name). Some namespaces are aliased into the primary namespace, so `bio::ranges::views` is identical to `bio::views`. Only top-level namespaces are displayed here.

Some modules provide literals in an `inline namespace` within their main namespace. This allows doing e.g.
`using namespace bio::alphabet::literals;` to get easy access to the alphabet literals without importing the rest
of the module into your namespace.

## Attribution

If no citable publication is listed for a Module, please cite [10.1007/978-3-030-90990-1](https://link.springer.com/book/10.1007/978-3-030-90990-1), which is a book by the main author of BioC++.
