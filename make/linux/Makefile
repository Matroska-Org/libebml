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

ifeq (Darwin,$(shell uname -s))
link=static
else
link=both
endif

targets_both = staticlib sharedlib
targets_shared = sharedlib
targets_static = staticlib

CWD=$(shell pwd)

SRC_DIR=$(CWD)/../../src/
INCLUDE_DIR=$(CWD)/../../ebml

# Libraries
INCLUDE=-I$(CWD)/../..
LIBS=

# Names
LIBRARY=libebml.a
LIBRARY_SO=libebml.so
LIBRARY_SO_VER=libebml.so.4

# source-files
sources:=$(wildcard ${SRC_DIR}*$(EXTENSION))

# header files; replace .cxx extension with .h
headers:=$(patsubst %$(EXTENSION),%.h,$(sources))

# object files; replace .cxx extension with .o
objects:=$(patsubst %$(EXTENSION),%.o,$(sources))
objects_so:=$(patsubst %$(EXTENSION),%.lo,$(sources))

WARNINGFLAGS=-Wall -Wextra -Wno-unknown-pragmas -Wshadow
COMPILEFLAGS=$(WARNINGFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(DEBUGFLAGS) $(INCLUDE)
DEPENDFLAGS  = $(CXXFLAGS) $(INCLUDE)

all: $(targets_$(link))

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

install: $(targets_$(link):%=install_%) install_headers

install_headers:
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(DESTDIR)$(includedir)
	for i in $(INCLUDE_DIR)/*.h; do \
		$(INSTALL) $(INSTALL_OPTS) $$i $(DESTDIR)$(includedir) ; \
	done
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(DESTDIR)$(includedir)/c
	for i in $(INCLUDE_DIR)/c/*.h; do \
		$(INSTALL) $(INSTALL_OPTS) $$i $(DESTDIR)$(includedir)/c ; \
	done

install_staticlib: $(LIBRARY)
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(DESTDIR)$(libdir)
	$(INSTALL) $(INSTALL_OPTS_LIB) $(LIBRARY) $(DESTDIR)$(libdir)

install_sharedlib: $(LIBRARY_SO)
	$(INSTALL) $(INSTALL_DIR_OPTS) -d $(DESTDIR)$(libdir)
	$(INSTALL) $(INSTALL_OPTS_LIB) $(LIBRARY_SO_VER) $(DESTDIR)$(libdir)
	ln -fs $(LIBRARY_SO_VER) $(DESTDIR)$(libdir)/$(LIBRARY_SO)


ifneq ($(wildcard .depend),)
include .depend
endif

# DO NOT DELETE

