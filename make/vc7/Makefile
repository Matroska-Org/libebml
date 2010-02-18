# Project: libebml
# Makefile to use the Free Visual C++ 2003 compiler from Microsoft with GNU Make

# Compile with debug information?
#DEBUG = yes


#
# Don't change anything below this line.
#
CXX  = cl /Tp
CC   = cl /Tc
SRC  = $(wildcard ../../src/*.cpp)
OBJ  = $(patsubst %.cpp,%.obj,$(SRC))
INCS = /I../..
LDFLAGS = /NOLOGO /DLL /MAP:libebml.map /VERSION:0.7
OPTMIZ = /G6 /O2 /Oi /Wp64 /GL
CXXFLAGS = $(INCS) /DWIN32 /nologo /DEBML_DLL /DEBML_DLL_EXPORT


ifeq (yes,$(DEBUG))
CXXFLAGS += /Zi /DDEBUG /D_DEBUG /MTd /RTC1
LDFLAGS += /DEBUG
else
CXXFLAGS += /MT
LDFLAGS += /OPT:REF
endif

LIBS = libebml.dll

.PHONY: all all-before all-after clean clean-custom

%.obj : %.cpp
	$(CXX) $< /c $(CXXFLAGS) /Fo$@

all: lib

lib: $(LIBS)

clean:
	$(RM) $(OBJ) libebml.lib libebml.dll

libebml.lib: $(OBJ)
	lib /OUT:$@ /NODEFAULTLIB $(OBJ)

libebml.dll: $(OBJ)
	link $(LDFLAGS) /OUT:$@ $(OBJ) user32.lib

depend:
	$(CXX) $(CXXFLAGS) -MM $(SRC) > .depend

#
# include dependency files if they exist
#
ifneq ($(wildcard .depend),)
include .depend
endif
