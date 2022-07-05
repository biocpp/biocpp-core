# Overview {#biocpp_overview}

## Introduction

BioC++ is a project that encompasses multiple C++ libraries that each provide one or more modules. These modules cover typical tasks in Bioinformatics like storing sequence data or reading/writing file formats. The goal of BioC++ is to provide interfaces that are easier to use than those of the C and C++ libraries currently available while still delivering an excellent performance. It leverages *Modern C++* to achieve this.


## Module descriptions

| Library                                       | Module(s)  | Description                                           |
|-----------------------------------------------|------------|-------------------------------------------------------|
| [Core](https://github.com/biocpp/biocpp-core) | Alphabet   | Datastructures for storing DNA, RNA and amino acids.  |
|                                               | Ranges     | Containers and Views                                  |
|                                               | Meta       | Metaprogramming utilities, concepts, tuples           |
| [I/O](https://github.com/biocpp/biocpp-io)    | I/O        | Input/Output for TXT, CSV, FastA, FastQ, VCF, BCF, …  |


More modules are planned!

## Module details

| Library                                       | Module(s)  | Namespaces        | Code-Owner | Cite               |
|-----------------------------------------------|------------|-------------------|------------|--------------------|
| [Core](https://github.com/biocpp/biocpp-core) | Alphabet   | `bio::alphabet::a{lphabet}` | `@h-2` | [10.1007/978-3-030-90990-1](https://link.springer.com/book/10.1007/978-3-030-90990-1) |
|                                               | Ranges     | `bio::alphabet::r{anges}`, `bio::{r::}views` | `@h-2` | [10.1007/978-3-030-90990-1](https://link.springer.com/book/10.1007/978-3-030-90990-1) |
|                                               | Meta       | `bio::alphabet::m{eta}`     | `@h-2` | [10.1007/978-3-030-90990-1](https://link.springer.com/book/10.1007/978-3-030-90990-1) |
| [I/O](https://github.com/biocpp/biocpp-io)    | I/O        | `bio::alphabet::io`         | `@h-2` |  |


The primary namespace for BioC++ is `bio::`, and each Module is found in a sub-namespace (usually identical to the module name). Namespaces may have short forms, e.g. `bio::ranges::` is identical to `bio::alphabet::r::`. Some namespaces are aliased into the primary namespace, so `bio::ranges::views` is identical to `bio::alphabet::r::views` and `bio::views`. Only top-level namespaces are displayed here.

If no citable publication is listed for a Module, please cite [10.1007/978-3-030-90990-1](https://link.springer.com/book/10.1007/978-3-030-90990-1), which is a book by the main author of BioC++.
