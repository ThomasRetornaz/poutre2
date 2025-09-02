Poutre2
=======

[![ci](https://github.com/ThomasRetornaz/poutre2/actions/workflows/ci.yml/badge.svg)](https://github.com/ThomasRetornaz/poutre2/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/ThomasRetornaz/poutre2/branch/main/graph/badge.svg)](https://codecov.io/gh/ThomasRetornaz/poutre2)
[![CodeQL](https://github.com/ThomasRetornaz/poutre2/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/ThomasRetornaz/poutre2/actions/workflows/codeql-analysis.yml)

## Rationale ##

__Poutre2__ is an open-source image processing framework, with a focus on Mathematical Morphology.  
It provides a modern c++ interface, genericity, with trying to not compromise speed.
It uses cpu intrinsics (SIMD) for low level code optimizations (thanks to [XSIMD](https://github.com/xtensor-stack/xsimd))

The core of __Poutre2__ is entirely written in C++, mainly using templatized code and metaprogramming.  

It provided generic code for 1d(signal),2d(Image) and higher level tensor.

Python bindings is provided (WIP)

## Features Targeted

* Container
    * Image in any dimension (2D, 3D ...)
    * Image with any value type (RGB-8, 32-bits, ...)

* Algorithms:
    * Morphology
        * (FAST) structuring element based filter
            * dilation, erosion, openings, gradients, ...
        * connected filters
            * attributes filters, grain filters
        * segmentation algorithms (watersheds and co)
      
## Licence ##

It is released under the very permissive **Boost license**.

## Prerequisites

__Poutre2__ is developed in modern C++ so you need a (very) recent C++ compiler.
The followings compilers are currently supported and tested:

| Compiler                | Version      |
|-------------------------|--------------|
| Microsoft Visual Studio | MSVC 2022    |
| g++                     | 14 and above |
| clang                   | 19 and above |

(Note the CI is broken for macOS, but it seems a libc++ mismatch)

## Dependencies ##

All the libraries dependencies are downloaded and build at configure time
Except optional OpenImageIO used to load store 2D images in various format.

## More Details

 * [Dependency Setup](README_dependencies.md)
 * [Building Details](README_building.md)
 * [Docker](README_docker.md)

## TODO HOT TOPICS

* Configure which intrinsics are used (and/or support multi-arch)
    * Default hard coded is AVX2 for x64 arch and NEON for arm.
* More CI configuration to relax constrains on compiler
* Make the library (almost) features complete

| Modules                            | Status      |
|------------------------------------|-------------|
| Fast arithmetic (Sup, Compare ...) | done        |
| Low level (aka erode,dilate, ...)  | almost done |
| Geodesy (h-extrema, leveling, ...) | done        |
| Distance (morpho, euclidian, ...)  | WIP         |
| Label                              | TBD         |
| Measures (perimeter, ...)          | TBD         |
| Segmentation (watershed, ...)      | TBD         |
| Morphology on graphs               | Dropped     |

## TODO

 * Introduce multi threading when it makes sens

## TODO far far away

* Try GPGPU approach using SYCL
 