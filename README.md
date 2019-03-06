# libebml
a C++ libary to parse EBML files

Specifications may be rendered at http://matroska-org.github.io/libebml/

# Building and installing

## Building and installing the library

libebml is based on `cmake` and requires only a C++ compiler. This
means that the normal build process consists of the usual steps:

1. Create a build directory: `mkdir build ; cd build`
2. Generate the make file: `cmake ..`
3. Compilation: `make`
4. Installation (run this as root): `make install`

## Customizing what's build with options

By default only a static library is built.

This library supports the usual `cmake` options for specifying
installation paths (e.g. `-DCMAKE_INSTALL_PREFIX=/opt/libebml`). The
following additional `cmake` options are supported:

* `-DDISABLE_PKGCONFIG=YES` — don't generate and install the
  `libebml.pc` package configuration module for `pkg-config`
* `-DDISABLE_CMAKE_CONFIG=YES` — don't generate and install the
  package configuration module for `cmake`. Note that building
  `libmatroska` requires that the `cmake` configuration is available.
* `-DBUILD_SHARED_LIBS=YES` — build the shared library instead of the
  static one (default: no)

# Code of conduct

Please note that this project is released with a [Contributor Code of Conduct](CODE_OF_CONDUCT.md). By participating in this project you agree to abide by its terms.

# Included third-party components and their licenses

## utf8-cpp

UTF-8 with C++ in a Portable Way

  * Copyright: 2006 Nemanja Trifunovic
  * License: custom (see `src/lib/utf8-cpp/source/utf8.h`)
  * URL: http://utfcpp.sourceforge.net/
  * Corresponding files: `src/lib/utf8-cpp/*`
