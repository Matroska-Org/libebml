# libebml
a C++ libary to parse EBML files

Specifications may be rendered at http://matroska-org.github.io/libebml/

# Building and installing

## Building a released version

libebml is based on autoconf and automake and requires only a C++
compiler. This means that the normal build process consists of the
usual three steps:

1. Configuration: `./configure`
2. Building: `make`
3. Installation (run this as root): `make install`

## Building from git

If you're building from git you must have the GNU autotools
(`autoconf`, `automake`, `libtoolize`) installed. Then run the
following two commands:

1. `libtoolize`
2. `autoreconf -vi`

Afterwards continue with the steps listed for building a released
version.
