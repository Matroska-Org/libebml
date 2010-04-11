# Project: libebml
# Makefile created by Dev-C++ 4.9.7.0

# Normally libebml is built as a static library.
# Uncomment this if you want a shared library instead.
# ATTENTION: If your app uses this DLL you have to define EBML_DLL !
SHARED = yes

# Compile with debug information?
#DEBUG = yes


#
# Don't change anything below this line.
#
ifeq (yes,$(DEBUG))
DEBUGFLAGS=-g -DDEBUG
endif
CROSS   =
CXX     = $(CROSS)g++
CC      = $(CROSS)gcc
WINDRES = $(CROSS)windres
RANLIB  = $(CROSS)ranlib
AR      = $(CROSS)ar
RES  =
SRC  = $(wildcard ../../src/*.cpp)
OBJ  = $(patsubst %.cpp,%.o,$(SRC))
ifeq (yes,$(SHARED))
LIBS = libebml.dll
DLLFLAGS = -DEBML_DLL -DEBML_DLL_EXPORT
else
LIBS = libebml.a
endif
INCS = -I$(shell pwd)/../..
COMPILEFLAGS = $(DEBUGFLAGS) $(INCS) $(DLLFLAGS) $(CXXFLAGS)

.PHONY: all all-before all-after clean clean-custom

.cpp.o:
	$(CXX) $(COMPILEFLAGS) -c -o $@ $<

all: lib

lib: $(LIBS)

clean:
	rm -f $(OBJ) libebml.a libebml.dll libebml.dll.a

distclean dist-clean: clean
	rm -f .depend

libebml.a: $(OBJ)
	$(AR) r $@ $(OBJ)
	$(RANLIB) $@

libebml.dll: $(OBJ)
	$(CXX) -shared -Wl,--export-all -Wl,--out-implib=$@.a -o $@ $(OBJ)

depend:
	@echo Calculating dependecies:
	@rm -f .depend
	@touch .depend
	@for i in $(SRC); do \
		o="`echo $$i | sed -e 's/\.c$$/.o/' -e 's/\.cpp$$/.o/'`" ; \
		echo '  ' $$i: $$o ; \
		$(CXX) $(CXXFLAGS) -MM -MT $$o $$i >> .depend ; \
	done

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
