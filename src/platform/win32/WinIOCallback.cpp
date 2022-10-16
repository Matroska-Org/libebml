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
  \version \$Id: WinIOCallback.cpp 1092 2005-03-16 13:01:15Z robux4 $
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Jory Stone       <jcsston @ toughguy.net>
  \author Cyrius           <suiryc @ users.sf.net>
*/

#include <cassert>

#include "WinIOCallback.h"

#include "ebml/Debug.h"

#ifndef INVALID_SET_FILE_POINTER // found in newer platform SDKs
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif // INVALID_SET_FILE_POINTER

namespace libebml {

WinIOCallback::WinIOCallback(const char* Path, const open_mode aMode, DWORD dwFlags)
  :mFile(NULL), mOk(false)
{
  mOk = open(Path, aMode, dwFlags);
}

WinIOCallback::WinIOCallback(const wchar_t* Path, const open_mode aMode, DWORD dwFlags)
  :mFile(NULL), mOk(false)
{
  mOk = open(Path, aMode, dwFlags);
}

WinIOCallback::~WinIOCallback()
{
  close();
}

bool WinIOCallback::open(const char* Path, const open_mode aMode, DWORD dwFlags)
{
  assert(Path!=0);

  DWORD AccessMode, ShareMode, Disposition;

  switch (aMode) {
    case MODE_READ:
      AccessMode = GENERIC_READ;
      ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
      Disposition = OPEN_EXISTING;
      break;
    case MODE_WRITE:
      AccessMode = GENERIC_WRITE;
      ShareMode = 0;
      Disposition = OPEN_ALWAYS;
      break;
    case MODE_SAFE:
      AccessMode = GENERIC_WRITE|GENERIC_READ;
      ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
      Disposition = OPEN_ALWAYS;
      break;
    case MODE_CREATE:
      AccessMode = GENERIC_WRITE;
      ShareMode = 0;
      Disposition = CREATE_ALWAYS;
      break;
    default:
      assert(false);
  }

  mFile = CreateFileA(Path, AccessMode, ShareMode, NULL, Disposition, dwFlags, NULL);
  if (mFile == INVALID_HANDLE_VALUE) {
    //File was not opened
    char err_msg[256];
    DWORD error_code = GetLastError();
    // An error message about the file already existing is not really an error message :P
    if (error_code != ERROR_ALREADY_EXISTS) {
      FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, err_msg, 255, NULL);
      EBML_TRACE("Failed to open file \"%hs\" in mode %d.", Path, aMode);

      mLastErrorStr = err_msg;
      return mOk = false;
    }
  }
  mCurrentPosition = 0;

  EBML_TRACE("Successfully opened file \"%hs\" in mode %d. The handle is %p\n", Path, aMode, mFile);

  return mOk = true;
};

bool WinIOCallback::open(const wchar_t* Path, const open_mode aMode, DWORD dwFlags)
{
  assert(Path!=0);

  DWORD AccessMode, ShareMode, Disposition;

  switch (aMode) {
    case MODE_READ:
      AccessMode = GENERIC_READ;
      ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
      Disposition = OPEN_EXISTING;
      break;
    case MODE_WRITE:
      AccessMode = GENERIC_WRITE;
      ShareMode = 0;
      Disposition = OPEN_ALWAYS;
      break;
    case MODE_SAFE:
      AccessMode = GENERIC_WRITE|GENERIC_READ;
      ShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
      Disposition = OPEN_ALWAYS;
      break;
    case MODE_CREATE:
      AccessMode = GENERIC_WRITE;
      ShareMode = 0;
      Disposition = CREATE_ALWAYS;
      break;
    default:
      assert(false);
  }

  if ((LONG)GetVersion() >= 0) {
    mFile = CreateFileW(Path, AccessMode, ShareMode, NULL, Disposition, dwFlags, NULL);
  } else {
    int errCode;
    int pathSize = wcslen(Path);
    unsigned int bufferSize = pathSize + sizeof(wchar_t) * 2;
    std::string PathA;
    PathA.resize(bufferSize);
    errCode = WideCharToMultiByte(CP_ACP, 0, Path, pathSize, (char *)PathA.c_str(), bufferSize, NULL, NULL);
    if (errCode == 0)
      errCode = GetLastError();
#ifdef _DEBUG
    if (errCode == ERROR_INSUFFICIENT_BUFFER) OutputDebugString(TEXT("WinIOCallback::WideCharToMultiByte::ERROR_INSUFFICIENT_BUFFER"));
    if (errCode == ERROR_INVALID_FLAGS) OutputDebugString(TEXT("WinIOCallback::WideCharToMultiByte::ERROR_INVALID_FLAGS"));
    if (errCode == ERROR_INVALID_PARAMETER) OutputDebugString(TEXT("WinIOCallback::WideCharToMultiByte::ERROR_INVALID_PARAMETER"));
#endif
    while (errCode == ERROR_INSUFFICIENT_BUFFER) {
      // Increase the buffer size
      bufferSize += MAX_PATH;
      PathA.resize(bufferSize);
      errCode = WideCharToMultiByte(CP_ACP, WC_SEPCHARS, Path, pathSize, (char *)PathA.c_str(), bufferSize, NULL, NULL);
      if (errCode == 0)
        errCode = GetLastError();
    }
    if (errCode != 0) {
      mFile = CreateFileA(PathA.c_str(), AccessMode, ShareMode, NULL, Disposition, dwFlags, NULL);
    } else {
      mLastErrorStr = "Couldn't convert Unicode filename to ANSI.";
      return mOk = false;
    }
  }
  if (mFile == INVALID_HANDLE_VALUE) {
    //File was not opened
    char err_msg[256];
    DWORD error_code = GetLastError();
    // An error message about the file already existing is not really an error message :P
    if (error_code != ERROR_ALREADY_EXISTS) {
      FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, error_code, 0, err_msg, 255, NULL);
      EBML_TRACE("Failed to open file \"%S\" in mode %d.", Path, aMode);
      mLastErrorStr = err_msg;
      return mOk = false;
    }
  }
  mCurrentPosition = 0;

  EBML_TRACE("Successfully opened file \"%S\" in mode %d. The handle is %p\n", Path, aMode, mFile);
  return mOk = true;
}

void WinIOCallback::close()
{
  if (mFile) {
    CloseHandle(mFile);
    mFile = NULL;
  }
}

std::uint64_t WinIOCallback::getFilePointer()
{
  if (!mFile) {
    return 0;
  }

  return mCurrentPosition;
#if 0
  LONG High = 0;
  DWORD Low = SetFilePointer(mFile, 0, &High, FILE_CURRENT);
  if ( (Low==INVALID_SET_FILE_POINTER) && (GetLastError()!=NO_ERROR) )
    return static_cast<std::uint64_t>(-1);
  return ((std::uint64_t(High)<<32) | Low);
#endif
}

void WinIOCallback::setFilePointer(std::int64_t Offset, seek_mode Mode)
{
  DWORD Method;
  switch(Mode) {
    case seek_beginning:
      Method=FILE_BEGIN;
      break;
    case seek_current:
      Method=FILE_CURRENT;
      break;
    case seek_end:
      Method=FILE_END;
      break;
    default:
      assert(false);
      break;
  }

  LONG High = LONG(Offset>>32);
  mCurrentPosition = SetFilePointer(mFile, LONG(Offset & 0xffffffff), &High, Method);
  if ( mCurrentPosition == INVALID_SET_FILE_POINTER ) {
    High = 0;
    DWORD Low = SetFilePointer(mFile, 0, &High, FILE_CURRENT);
    mCurrentPosition = ((std::uint64_t(High)<<32) | Low);
  } else {
    mCurrentPosition |= std::uint64_t(High)<<32;
  }
}

std::size_t WinIOCallback::read(void*Buffer,std::size_t Size)
{
  DWORD BytesRead;
  if (!ReadFile(mFile, Buffer, Size, &BytesRead, NULL)) {
    return 0;
  }
  mCurrentPosition += BytesRead;
  return BytesRead;
}

std::size_t WinIOCallback::write(const void*Buffer,std::size_t Size)
{
  DWORD BytesWriten;
  if (!WriteFile(mFile, Buffer, Size, &BytesWriten, NULL)) {
    return 0;
  }
  mCurrentPosition += BytesWriten;
  return BytesWriten;
}

bool WinIOCallback::SetEOF()
{
  return SetEndOfFile(mFile) != 0;
}

} // namespace libebml
