// Copyright © 2003-2004 Jory Stone.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: MemIOCallback.h 1298 2008-02-21 22:14:18Z mosu $
  \author Jory Stone <jcsston @ toughguy.net>
*/
#ifndef LIBEBML_MEMIOCALLBACK_H
#define LIBEBML_MEMIOCALLBACK_H

#include "IOCallback.h"
#include <string>
#ifndef __BEOS__
#include <sstream>
#else
#include <strstream>
#define stringstream strstream
#endif

namespace libebml {

class EBML_DLL_API MemIOCallback : public IOCallback
{
public:
  MemIOCallback(uint64 DefaultSize = 128);
  ~MemIOCallback() override;

  /*!
    Use this to copy some data to the Buffer from this classes data
  */
  uint32 read(void *Buffer, size_t Size) override;

  /*!
    Seek to the specified position. The mode can have either SEEK_SET, SEEK_CUR
    or SEEK_END. The callback should return true(1) if the seek operation succeeded
    or false (0), when the seek fails.
  */
  void setFilePointer(int64 Offset, seek_mode Mode=seek_beginning) override;

  /*!
    This callback just works like its read pendant. It returns the number of bytes written.
  */
  size_t write(const void *Buffer, size_t Size) override;

  /*!
    Although the position is always positive, the return value of this callback is signed to
    easily allow negative values for returning errors. When an error occurs, the implementor
    should return -1 and the file pointer otherwise.

    If an error occurs, an exception should be thrown.
  */
  uint64 getFilePointer() override {return dataBufferPos;}

  /*!
    The close callback flushes the file buffers to disk and closes the file. When using the stdio
    library, this is equivalent to calling fclose. When the close is not successful, an exception
    should be thrown.
  */
  void close() override {}

  binary *GetDataBuffer() const {return dataBuffer;}
  uint64 GetDataBufferSize() const {return dataBufferTotalSize;}
  void SetDataBufferSize(uint64 newDataBufferSize) {dataBufferTotalSize = newDataBufferSize;}
  /*!
    Use this to write some data from another IOCallback
  */
  uint32 write(IOCallback & IOToRead, size_t Size);

  bool IsOk() { return mOk; }
  const std::string &GetLastErrorStr() { return mLastErrorStr; }
protected:
  bool mOk;
  std::string mLastErrorStr;

  binary *dataBuffer;
  /*!
    Postion where we start 'writing' to the dataBuffer
  */
  uint64 dataBufferPos;
  /*!
    Size of the data in the dataBuffer
  */
  uint64 dataBufferTotalSize;
  /*!
    Size of the memory malloc()/realloc()
  */
  uint64 dataBufferMemorySize;
};

} // namespace libebml

#endif // LIBEBML_MEMIOCALLBACK_H
