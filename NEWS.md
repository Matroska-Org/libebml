# Version ? 2021-??-??

* A C++14 compliant C++ compiler is now required.
* Fix compilation with g++ 11. Fixes #80.
* Remove Coremake project files
* Updated the bundled utf8cpp to v3.2.1.
* Fixed several problems in `EbmlBinary:` potential segfault by
  writing to a `nullptr`, potential memory leak under certain error
  conditions.
* Fixed problems with the generated `pkg-config` files if any of the
  installation paths are absolute.

# Version 1.4.2 2021-02-18

* Fixed several heap overflow bugs in the `ReadData` functions of
  various data type classes. This fixes CVE-2021-3405.

# Version 1.4.1 2021-01-04

* Fixed a case `EbmlMaster::Read` where the element returned via
  `UpperEltFound` and `FountElt` points to a just-deleted element,
  causing callers to think the memory returned is valid, potentially
  leading to use-after-free/double-free errors. This can happen if the
  specific element's Read function throws an exception when
  encountering certain invalid data constellations.

# Version 1.4.0 2020-06-27

* Due to breaking ABI the soname version has been bumped to 5.0.0.
* Default symbol visibility is now "hidden", reducing binary size.
* Converted some things pointed out by `cppcheck` & `clang-tidy` to
  C++11.
* Fixed a lot of issues pointed out by `clang-tidy`.
* Added a function `ForceNoDefault` in the `EbmlElement` class.
* Added a function `OverwriteData` in the `EbmlElement` class to
  complement the existing `OverwriteHead` function.
* Fixed compilation on Windows wrt. `winapifamily.h`
* Fixed compilation on Haiku.
* A C++11 compliant compiler is now required. Several of its features
  are now used in the source.
* `EbmlElement::FindNextElement:` fixed a buffer overflow on the stack
  by one byte when the first byte read had its upper four bits unset.
* `EbmlCrc32`: added a explicit assignment operator with default
  implementation as an explicit copy constructor exists (implicit
  assignment operators are deprecated in such cases).

# Version 1.3.10 2019-12-07

* When reading binary elements with a size of 0, the element was
  skipped by libebml instead of returned to the calling function.

# Version 1.3.9 2019-05-27

* Build system: fixed symbol visibility issue causing linker failures
  since 1.3.8.

# Version 1.3.8 2019-05-25

* Build system: various fixes to the cmake rules regarding visibility,
  naming and building DLLs on Windows.

# Version 1.3.7 2019-03-12

* Fixed the handling of Unicode codepoints `> U+FFFF`.
* Fixed building on Windows with `cmake`.
* Fixed building with gcc >= 7.

# Version 1.3.6 2018-04-20

* Converted the build system from `autoconf`/`automake` to
  `cmake`. Patches by Github user "evpobr" with fixes by myself.
* Fixed undefined behavior when reading signed integers with negative
  values from files (though compilers implemented this the way we
  wanted them to already).
* Fixed a small memory leak when reading an element runs into an I/O
  exception (e.g. due to having reached the end of the file).
* Fixed the `EbmlMaster::GetDataStart()` function returning wrong
  values for elements with an infinite/unknown size.
* Fixed finding the next element ID when garbage data is encountered
  during the scan for the ID.
* Fixed several potential situations where reading child element data
  could exceed the parent element's size.
* Added a code of conduct to the project.

# Version 1.3.5 2017-08-19

* The function `EbmlMaster::CheckMandatory()` will now only return
  false if a mandatory element is missing for which there's no default
  value in the specifications. This means that callers such as
  `EbmlMaster::UpdateSize()` and by extension `EbmlMaster::Render()`
  will not insist on all mandatory elements being present anymore, but
  only those for which there's no default value.
* Added a template function `FindNextChild`. Patch by C.W. Betts.
* Fix reading and EBML element even though the ID was not found within
  the allowed reading limit.
* Fixed an instance of undefined behavior in
  `EbmlElement::GetSemantic()` due to binding a dereferenced null
  pointer to a reference.
* Replaced the outdated address of the Free Software Foundation with
  their current one. Fixes #15.

# Version 1.3.4 2016-07-02

* `EbmlVersion.cpp`: in order to enable deterministic builds the
  `EbmlCodeDate` variable has been set to "Unknown" instead of the
  date and time of compilation. Patch by Ed Schouten <ed@nuxi.nl>.
* libebml_t.h: use C99-style integer typedefs instead of BSD-style
  ones. Patch by Ed Schouten <ed@nuxi.nl>.
* `EbmlBinary.h`: add `#include <cstdlib>` for compilation with clang
  and libc++. Patch by Thomas Klausner <wiz@`NetBSD`.org>.

# Version 1.3.3 2015-10-20

* `EbmlMaster::Read():` When the parser encountered a deeply nested
  element with an infinite size then a following element of an upper
  level was not propagated correctly. Instead the element with the
  infinite size was added into the EBML element tree a second time
  resulting in memory access after freeing it and multiple attempts to
  free the same memory address during destruction. Fixes the issue
  reported as Cisco TALOS-CAN-0037.
* `EbmlElement::ReadCodedSizeValue():` Fixed an invalid memory
  access. When reading a EBML variable length integer value a read
  access beyond the end of the available buffer was possible if fewer
  bytes were available than indicated by the first byte resulting in a
  heap information leak.
* `EbmlUnicodeString::UpdateFromUTF`8(): Fixed an invalid memory
  access. When reading from a UTF-8 string in which the length
  indicated by a UTF-8 character's first byte exceeds the string's
  actual number of bytes the parser would access beyond the end of the
  string resulting in a heap information leak. Fixes the issue
  reported as Cisco TALOS-CAN-0036.

# Version 1.3.2 2015-10-17

* `EbmlElement::FindNextElement():` Handle EOF when reading the
  element size properly.
* Disable debug code for builds for the Windows App Store.
* Update the license information: use latest official text for the
  LGPL.
* `EbmlString::ValidateSize():` only allow the same maximum size as
  `EbmlBinary`.

# Version 1.3.1 2015-01-04

* `EbmlElement::Render():` doesn't catch exceptions anymore. Instead
  exceptions generated from the `IOCallback` class (e.g. if a write
  failed) are propagated to the caller.
* build system: switched the build system from hand-crafted Makefiles
  to an `autoconf`/`automake`-based system. A `pkg-config` file will
  be installed as well; its name is `libebml`. Patch by Jan Engelhardt
  <jengelh@inai.de>.
* `EbmlMaster::Read():` when reading with `SCOPE_ALL_DATA` only those
  elements that could successfully be read will be kept
  (e.g. defective block groups will be dropped).
* `EbmlMemoryStream:` add a new class for safe memory access that
  throws exception on failures.
* `EbmlMaster:` Fixed read() trying to calculate the end position of
  elements with an unknown size. This avoids endless loops and
  assertions in certain cases. See https://trac.bunkus.org/ticket/1089

# Version 1.3.0 2013-03-19

* Note that this version is not binary compatible to its predecessors.
* Compilation warning fixes
* `GetValue()` and `SetValue()` functions for
  `EbmlFloat`/`EbmlSInteger`/`EbmlUInteger`
* `GetValue()`, `SetValue()`, `GetValueUTF`8(), `SetValueUTF`8()
  functions for `EbmlString`/`EbmlUnicodeString`
* Date values use 64bit integers
* Integer conversion operators were made `const`
* `GetElementList()` function for `EbmlMaster` providing a reference
  to the underlying `std::vector<>` for better interaction with the
  C++ standard library
* New constructor for `UTFstring` taking a single `std::wstring`
* Increased library .so name to .4 due to binary incompatible changes.
* Removed the Debian build files from our package.

# Version 1.2.2 2011-09-23

* fix usage of the `DEBUG` `#define` (use `LIBEBML_DEBUG` instead)
* The `EbmlCodeVersion` variable now resides in the library instead of
  being declared static in the header file.
* only use the test element to read once in the loop

# Version 1.2.1 2011-06-26

* fix the constructor of `CRTError` in `StdIOCallback` (patch from
  Graham 'gbooker' Booker)
* fix the size returned by `MemIOCallback::read()` when reaching the
  end of the buffer
* add a macro to define signed integer elements that have a default
  value
* fix compilation of debug builds on win32
* fix for Sun Studio C compiler

# Version 1.2.0 2011-01-30

* no need to put the internal code of `EbmlCrc32` in the public header
* more safety on the Binary buffer handling in case we run out of
  memory
* fix CRC32 detection in master elements
* fix compilation under gcc 4.5
* Fixed handling of UTF-8 strings with characters encoded in more than
  three bytes.
* Increased library `.so` name to `.3` due to binary incompatible
  changes.

# Version 1.1.0 2010-07-xx

* `EbmlElement::VoidMe()` now returns a uint64
* move some operator code in the `.cpp` files (MSVC has issues in
  DLLs)

# Version 1.0.0 2010-06-04

* rename the library .so name as it's backward incompatible
* fixes when reading Master elements with unknown/infinite size
* fix the infinite/unknown size reading in
  `EbmlElement::FindNextElement()`
* fix the `EbmlDummy` ID reading and displaying

# Version 0.8.0 2010-04-30

* new macros to simplify and unify the way to declare and define EBML
  elements
* new macros to access variables of a Class/Object so that switching
  to libebml v2 is transparent
* add an `EBML_STRICT_API` compilation flag (off by default) to avoid
  direct access to internal class variables
* fix some building issues on Linux and `MinGW`
* add accessor calls to the base classes to avoid direct access to
  internal class variables
* add iterators to navigate throught the `EbmlMaster` children
* use a new `filepos_t` type for everything that relates to an element
  position/data size with `INVALID_FILEPOS_T` in case of errors
* remove the const on integer values
