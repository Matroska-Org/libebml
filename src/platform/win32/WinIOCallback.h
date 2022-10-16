/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2003 Steve Lhomme.  All rights reserved.
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
  \version \$Id: WinIOCallback.h 1090 2005-03-16 12:47:59Z robux4 $
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Jory Stone       <jcsston @ toughguy.net>
  \author Cyrius           <suiryc @ users.sf.net>
*/

#ifndef LIBEBML_WINIOCALLBACK_H
#define LIBEBML_WINIOCALLBACK_H

#include <windows.h>
#include <stdexcept>
#include <string>
#include "ebml/IOCallback.h"

namespace libebml {

class WinIOCallback: public IOCallback
{
public:
  WinIOCallback(const wchar_t* Path, const open_mode aMode, DWORD dwFlags=0);
  WinIOCallback(const char* Path, const open_mode aMode, DWORD dwFlags=0);
  virtual ~WinIOCallback();

  bool open(const wchar_t* Path, const open_mode Mode, DWORD dwFlags=0);
  bool open(const char* Path, const open_mode Mode, DWORD dwFlags=0);

  virtual std::size_t read(void*Buffer,std::size_t Size);
  virtual std::size_t write(const void*Buffer,std::size_t Size);
  virtual void setFilePointer(std::int64_t Offset,seek_mode Mode=seek_beginning);
  virtual std::uint64_t getFilePointer();
  virtual void close();

  bool IsOk() { return mOk; };
  const std::string &GetLastErrorStr() { return mLastErrorStr; };
  bool SetEOF();
protected:
  bool mOk;
  std::string mLastErrorStr;
  std::uint64_t mCurrentPosition;

  HANDLE mFile;
};

} // namespace libebml

#endif // LIBEBML_WINIOCALLBACK_H
