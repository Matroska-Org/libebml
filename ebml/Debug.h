/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2004 Steve Lhomme.  All rights reserved.
**
** This file is part of libebml.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** See http://www.gnu.org/licenses/lgpl-2.1.html for LGPL licensing information.
**
** Contact license@matroska.org if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

/*!
  \file
  \version \$Id: Debug.h 639 2004-07-09 20:59:14Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/
#ifndef LIBEBML_DEBUG_H
#define LIBEBML_DEBUG_H

#include <cstdarg> // va_list
#include <cstring>

#ifdef WIN32
#include <windows.h>
#else
#include <cstdio>
#endif // WIN32

#include "EbmlConfig.h"

namespace libebml {

static const int MAX_PREFIX_LENGTH = 128;

#if defined(LIBEBML_DEBUG)
// define the working debugging class

class EBML_DLL_API ADbg
{
public:
  ADbg(int level = 0);
  virtual ~ADbg();

  /// \todo make an inline function to test the level first and the process
  int OutPut(int level, const char * format,...) const;

  int OutPut(const char * format,...) const;

  inline int setLevel(const int level) {
    return my_level = level;
  }

  inline bool setIncludeTime(const bool included = true) {
    return my_time_included = included;
  }

  bool setDebugFile(const char * NewFilename);
  bool unsetDebugFile();

  inline bool setUseFile(const bool usefile = true) {
    return my_use_file = usefile;
  }

  inline const char * setPrefix(const char * string) {
    return strncpy(prefix, string, MAX_PREFIX_LENGTH);
  }

private:
  int my_level;
  bool my_time_included;
  bool my_use_file;
  bool my_debug_output;

  int _OutPut(const char * format,va_list params) const;

  char prefix[MAX_PREFIX_LENGTH];

#ifdef WIN32
  HANDLE hFile;
#else
  FILE *hFile;
#endif // WIN32
};

#else // defined(LIBEBML_DEBUG)

// define a class that does nothing (no output)

class EBML_DLL_API ADbg
{
public:
  ADbg(int /* level */ = 0){}
  virtual ~ADbg() = default;

  static inline int OutPut(int /* level */, const char * /* format */,...) {
    return 0;
  }

  static inline int OutPut(const char * /* format */,...) {
    return 0;
  }

  static inline int setLevel(const int level) {
    return level;
  }

  static inline bool setIncludeTime(const bool /* included */ = true) {
    return true;
  }

  static inline bool setDebugFile(const char * /* NewFilename */) {
    return true;
  }

  static inline bool unsetDebugFile() {
    return true;
  }

  static inline bool setUseFile(const bool /* usefile */ = true) {
    return true;
  }

  static inline const char * setPrefix(const char * string) {
    return string;
  }
};

#endif // defined(LIBEBML_DEBUG)

extern ADbg globalDebug;


#ifdef LIBEBML_DEBUG
#define EBML_TRACE globalDebug.OutPut
#else
#define EBML_TRACE
#endif

// Unfortunately the Visual C++ new operator doesn't throw a std::bad_alloc. One solution is to
// define out own new operator. But we can't do this globally, since we allow static linking.
// The other is to check every new allocation with an MATROSKA_ASSERT_NEW.
#ifdef _MSC_VER
#define EBML_ASSERT_NEW(p) if(p==0)throw std::bad_alloc()
#else
#define EBML_ASSERT_NEW(p) assert((p)!=0)
#endif

} // namespace libebml

#endif
