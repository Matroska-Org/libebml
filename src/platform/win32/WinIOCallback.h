// Copyright © 2002-2002 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
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
