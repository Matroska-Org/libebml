# libebml Makefile
# $Id: Makefile,v 1.8 2004/05/11 20:27:38 mosu Exp $
# Author: Steve Lhomme <robux4 @ users.sf.net>
# Author: Moritz Bunkus <moritz @ bunkus.org>

#
# The library is built without debug information. If you want
# debug information to be included then compile with
# 'make DEBUG=yes'.
#

# Paths
# BeOS wants the libs and headers in /boot/home/config
ifeq (BeOS,$(shell uname -s))
prefix=/boot/home/config
else
prefix=/usr/local
endif
libdir=$(prefix)/lib
includedir=$(prefix)/include/ebml

# Programs
CROSS =
CXX = $(CROSS)g++
LD = $(CXX)
AR = $(CROSS)ar
RANLIB = $(CROSS)ranlib
INSTALL = install
INSTALL_OPTS = -m 644
INSTALL_OPTS_LIB = -m 644
INSTALL_DIR_OPTS = -m 755

# Options
EXTENSION=.cpp

ifeq (yes,$(DEBUG))
DEBUGFLAGS=-g -DDEBUG
endif

CWD=$(shell pwd)

SRC_DIR=$(CWD)/../../src/
INCLUDE_DIR=$(CWD)/../../ebml

# Librarires
INCLUDE=-I$(CWD)/../..
LIBS=

# Names
LIBRARY=libebml.a
LIBRARY_SO=libebml.so
LIBRARY_SO_VER=libebml.so.3

# source-files
sources:=$(wildcard ${SRC_DIR}*$(EXTENSION))

# header files; replace .cxx extension with .h
headers:=$(patsubst %$(EXTENSION),%.h,$(sources))

# object files; replace .cxx extension with .o
objects:=$(patsubst %$(EXTENSION),%.o,$(sources))
objects_so:=$(patsubst %$(EXTENSION),%.lo,$(sources))

WARNINGFLAGS=-Wall -Wextra -Wno-unknown-pragmas -ansi -fno-gnu-keywords -Wshadow
COMPILEFLAGS=$(WARNINGFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(DEBUGFLAGS) $(INCLUDE)
DEPENDFLAGS  = $(CXXFLAGS) $(INCLUDE)

ifeq (Darwin,$(shell uname -s))
all: staticlib
else
all: staticlib sharedlib
endif

staticlib: $(LIBRARY)

sharedlib: $(LIBRARY_SO)

lib:
	@echo "Use the 'staticlib', 'sharedlib' or 'all' targets."
	@false

# Build rules
%.o: %$(EXTENSION)
	$(CXX) -c $(COMPILEFLAGS) -o $@ $<

%.lo: %$(EXTENSION)
	$(CXX) -c $(COMPILEFLAGS) -fPIC -o $@ $<

$(LIBRARY): $(objects)
	$(AR) rcvu $@ $(objects)
	$(RANLIB) $@

$(LIBRARY_SO): $(objects_so)
	$(CXX) -shared -Wl,-soname,$(LIBRARY_SO_VER) -o $(LIBRARY_SO_VER) $(objects_so)
	rm -f $(LIBRARY_SO)
	ln -s $(LIBRARY_SO_VER) $(LIBRARY_SO)

clean:
	rm -f $(objects) $(objects_so)
	rm -f $(LIBRARY)
	rm -f $(LIBRARY_SO)
	rm -f $(LIBRARY_SO_VER)
	rm -f CORE

distclean dist-clean: clean
	rm -f .depend

depend:
	@echo Calculating dependecies:
	@rm -f .depend
	@touch .depend
	@for i in $(sources); do \
		o="`echo $$i | sed -e 's/\.c$$/.o/' -e 's/\.cpp$$/.o/'`" ; \
		echo '  ' $$i: $$o ; \
		$(CXX) $(DEPENDFLAGS) -MM -MT $$o $$i >> .depend ; \
	done

ifeq (Darwin,$(shell uname -s))
install: install_staticlib install_headers
else
install: install_staticlib install_sharedlib install_headers
endif

install_headers:
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(includedir)
	for i in $(INCLUDE_DIR)/*.h; do \
		$(INSTALL) $(INSTALL_OPTS) $$i $(includedir) ; \
	done
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(includedir)/c
	for i in $(INCLUDE_DIR)/c/*.h; do \
		$(INSTALL) $(INSTALL_OPTS) $$i $(includedir)/c ; \
	done

install_staticlib: $(LIBRARY)
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(libdir)
	$(INSTALL) $(INSTALL_OPTS_LIB) $(LIBRARY) $(libdir)

install_sharedlib: $(LIBRARY_SO)
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(libdir)
	$(INSTALL) $(INSTALL_OPTS_LIB) $(LIBRARY_SO_VER) $(libdir)
	ln -fs $(LIBRARY_SO_VER) $(libdir)/$(LIBRARY_SO)


ifneq ($(wildcard .depend),)
include .depend
endif

# DO NOT DELETE

