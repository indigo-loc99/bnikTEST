# The bnik library

## Introduction

> [!NOTE]
> This is an experimental work-in-progress done on spare time.

A collection of libraries for my personal niche use cases. *This library is not ready for use in production. Use at your own risk!*

### Current Libraries

| Name        | Description                                                                                            |
| ------------| -----------                                                                                            |
| **dstring** | A simple string library meant to have its contents cleared out after use. Uses explicit copy semantics |

## Building

Currently, the libraries require any of the MSVC, gcc, or Clang compilers that support C++23 and greater. *Testing is currently
not comprehensive.*

### Requirements

| Tool | Description
|----------|--------
| C++ compiler                 | Compile the libraries. Must support at least C++23 standard
| CMake                        |  **Version &ge;3.29 required**. Generate the respective platform buildsystems.
| Platform buildsystem tool    | E.g. *GNU make*, *ninja*, *MSBuild*, etc.
| Git                          | Source control. Recommended for tinkering with the libraries.

### Supported Compilers

| Compiler | Version
|----------|--------
| MSVC     | Visual Studio 2022 17.9 (MSVC 14.38) and greater
| GNU g++  | 13.1 and greater
| Clang    | LLVM 17.01 and greater

> [!NOTE]
> - Clang is also tested on Windows with MSVC toolset (with *clang-cl*)
> - Using the experimental C++20 modules generally require the latest versions for all compilers

Get a local copy of the source tree:

````Shell
~>$ cd ~/dev
~/dev>$ git clone https://github.com/inigo-loc99/bnik
````

(Or alternately download the zipped file from GitHub.)

Make a build root and go there:

````Shell
~/dev>$ cd bnik
~/dev/bnik>$ mkdir build && cd build
~/dev/bnik/build>$ cmake ..

````

Configure with CMake:
 
 ````Shell
~/dev/bnik/build>$ cmake ..
 ````

 Build the desired target. All libraries in this example:

 ````Shell
~/dev/bnik/build>$ make
 ````

 ## Example Usages

 *dstring* library:

 ````C++
 #include <bnik/dstring.hpp>
 #include <iostream>

 int main()
 {
     bnik::basic_dstring<char> str("Hello World from bnik!");

     std::cout << str.data() << std::endl;
 }
 ````
