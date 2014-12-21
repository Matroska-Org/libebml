# -*- Makefile -*-

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I${top_srcdir}
AM_CXXFLAGS = -Wall -Wextra -Wno-unknown-pragmas -Wshadow
if ENABLE_DEBUG
AM_CPPFLAGS += -DDEBUG
AM_CXXFLAGS += -g
endif

lib_LTLIBRARIES = libebml.la
libebml_la_SOURCES = \
	src/Debug.cpp \
	src/EbmlBinary.cpp \
	src/EbmlContexts.cpp \
	src/EbmlCrc32.cpp \
	src/EbmlDate.cpp \
	src/EbmlDummy.cpp \
	src/EbmlElement.cpp \
	src/EbmlFloat.cpp \
	src/EbmlHead.cpp \
	src/EbmlMaster.cpp \
	src/EbmlSInteger.cpp \
	src/EbmlStream.cpp \
	src/EbmlString.cpp \
	src/EbmlSubHead.cpp \
	src/EbmlUInteger.cpp \
	src/EbmlUnicodeString.cpp \
	src/EbmlVersion.cpp \
	src/EbmlVoid.cpp \
	src/IOCallback.cpp \
	src/MemIOCallback.cpp \
	src/MemReadIOCallback.cpp \
	src/SafeReadIOCallback.cpp \
	src/StdIOCallback.cpp
libebml_la_LDFLAGS = -version-info 4:0:0 -no-undefined

nobase_include_HEADERS = \
	ebml/c/libebml_t.h \
	ebml/Debug.h \
	ebml/EbmlBinary.h \
	ebml/EbmlConfig.h \
	ebml/EbmlContexts.h \
	ebml/EbmlCrc32.h \
	ebml/EbmlDate.h \
	ebml/EbmlDummy.h \
	ebml/EbmlElement.h \
	ebml/EbmlEndian.h \
	ebml/EbmlFloat.h \
	ebml/EbmlHead.h \
	ebml/EbmlId.h \
	ebml/EbmlMaster.h \
	ebml/EbmlSInteger.h \
	ebml/EbmlStream.h \
	ebml/EbmlString.h \
	ebml/EbmlSubHead.h \
	ebml/EbmlTypes.h \
	ebml/EbmlUInteger.h \
	ebml/EbmlUnicodeString.h \
	ebml/EbmlVersion.h \
	ebml/EbmlVoid.h \
	ebml/IOCallback.h \
	ebml/MemIOCallback.h \
	ebml/MemReadIOCallback.h \
	ebml/SafeReadIOCallback.h \
	ebml/StdIOCallback.h

pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = libebml.pc

${pkgconfig_DATA}: config.status
