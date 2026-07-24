// Copyright © 2002-2004 Ingo Ralf Blum.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: StdIOCallback.h 1090 2005-03-16 12:47:59Z robux4 $
*/
#ifndef LIBEBML_STDIOCALLBACK_H
#define LIBEBML_STDIOCALLBACK_H

#include "IOCallback.h"

#include <stdexcept>
#include <cerrno>

// ----- Added 10/15/2003 by jcsston from Zen -----
#if defined (__BORLANDC__) //Maybe other compilers?
  #include <errno.h>
  #include <stdio.h>
#endif //__BORLANDC__
// ------------------------------------------------

namespace libebml {

class EBML_DLL_API CRTError:public std::runtime_error
{
// Variablen...
private:
  int Error;

// Methoden...
public:
  CRTError(int Error,const std::string&Description);
  CRTError(const std::string&Description,int Error=errno);

  int getError() const noexcept { return Error; }
};

// This class is currently private to the library, so there's no MATROSKA_EXPORT.
class EBML_DLL_API StdIOCallback:public IOCallback
{
    private:
      FILE*File;
    uint64 mCurrentPosition;

    public:
//  StdIOCallback(const char*Path,const char*Mode);
  StdIOCallback(const char*Path, open_mode Mode);
  ~StdIOCallback() noexcept override;

  uint32 read(void*Buffer,size_t Size) override;

  // Seek to the specified position. The mode can have either SEEK_SET, SEEK_CUR
  // or SEEK_END. The callback should return true(1) if the seek operation succeeded
  // or false (0), when the seek fails.
  void setFilePointer(int64 Offset,seek_mode Mode=seek_beginning) override;

  // This callback just works like its read pendant. It returns the number of bytes written.
  size_t write(const void*Buffer,size_t Size) override;

  // Although the position is always positive, the return value of this callback is signed to
  // easily allow negative values for returning errors. When an error occurs, the implementor
  // should return -1 and the file pointer otherwise.
  //
  // If an error occurs, an exception should be thrown.
  uint64 getFilePointer() override;

  // The close callback flushes the file buffers to disk and closes the file. When using the stdio
  // library, this is equivalent to calling fclose. When the close is not successful, an exception
  // should be thrown.
  void close() override;
};

} // namespace libebml

#endif
