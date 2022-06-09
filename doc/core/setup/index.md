# Quick Setup (using CMake) {#core_setup}

<b>Learning Objective:</b><br>
In this short guide you will learn how to set up BioC++ and how to compile a small example to test whether everything
works.

\tutorial_head{Easy, 30 Minutes, No prerequisites, }

[TOC]

<br>

# Software
Requirements:
  - gcc >= 10
  - cmake >= 3.14
  - git

BioC++ requires a GCC >= 10. Current versions of LLVM/Clang and VisualStudio/MSVC are **not supported**.
You can check your GCC version by calling `g++ --version`. Depending on your operating system, multiple
versions might be available as `g++-10` or `g++10`. We recommend using the latest version of GCC you have access to.
For more information refer to your operating system's documentation.

# Directory Structure
For this project, we recommend following directory layout:

```
tutorial
├── source
├── build
└── biocpp-core
```

To set these directories up you can follow this script (note the <b>\--recurse-submodules</b> when cloning BioC++):
```
mkdir tutorial
cd tutorial
mkdir build
mkdir source
git clone https://github.com/biocpp/biocpp-core.git
```

The output of the command ``` tree -L 2 ``` should now look like this:
```
.
├── build
├── biocpp-core
│   ├── build_system
│   ├── CHANGELOG.md
│   ├── CMakeLists.txt
│   ├── CODE_OF_CONDUCT.md
│   ├── CONTRIBUTING.md
│   ├── doc
│   ├── include
│   ├── LICENSE.md
│   ├── README.md
│   ├── submodules
│   └── test
└── source

8 directories, 6 files
```

# Compiling and Running

To test whether everything works, we will now compile and run a small example.

First we create the file `hello_world.cpp` in the `source` directory with the following contents:

\include hello_world.cpp

To compile it we first create a `CMakeLists.txt` file in the `source` directory:

```cmake
cmake_minimum_required (VERSION 3.4)
project (biocpp_core_tutorial CXX)

find_package (BioCpp REQUIRED HINTS "${CMAKE_SOURCE_DIR}/../biocpp-core/build_system")

add_executable (hello_world hello_world.cpp)

target_link_libraries (hello_world biocpp::core)
```

The directories should now look like this:

```
tutorial
├── source
    ├── CMakeLists.txt
    └── hello_world.cpp
├── build
└── biocpp-core
    ├── CMakeLists.txt
    ├── LICENSE
    ...
```

Now we can switch to the directory `build` and run:

```bash
cmake ../source
make
./hello_world
```

The output should be `Hello world`.

\remark Depending on the standard C++ on your system, you may need to specify the compiler via `-DCMAKE_CXX_COMPILER=`, for example:
```bash
cmake ../source -DCMAKE_CXX_COMPILER=/path/to/executable/g++-10
```

\note In some situations it can happen that the correct assembler is not found.
You will see an error during the cmake configuration that says something like: `... could not understand flag m ...`.
In this case you can try to export the Path:
```
export PATH=/util/bin:$PATH
```
and try running cmake again.

# Adding a new source file to your project

If you create a new `cpp` file and want to compile it, you need to add another `add_executable` and
`target_link_libraries` directive to you `CMakeLists.txt`.
For example, after adding `another_program.cpp` your `CMakeLists.txt` may look like this:
```cmake
cmake_minimum_required (VERSION 3.4)
project (biocpp-core_tutorial CXX)

find_package (BioCpp REQUIRED HINTS "${CMAKE_SOURCE_DIR}/../biocpp-core/build_system")

add_executable (hello_world hello_world.cpp)
add_executable (another_program another_program.cpp)

target_link_libraries (hello_world biocpp::core)
target_link_libraries (another_program biocpp::core)
```
