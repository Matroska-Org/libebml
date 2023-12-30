// Copyright © 2003-2004 Jory Stone.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
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
  explicit MemIOCallback(std::uint64_t DefaultSize = 128);
  ~MemIOCallback() override;

  MemIOCallback(const MemIOCallback&) = delete;
  MemIOCallback& operator=(const MemIOCallback&) = delete;

  /*!
    Use this to copy some data to the Buffer from this classes data
  */
  std::size_t read(void *Buffer, std::size_t Size) override;

  /*!
    Seek to the specified position. The mode can have either SEEK_SET, SEEK_CUR
    or SEEK_END. The callback should return true(1) if the seek operation succeeded
    or false (0), when the seek fails.
  */
  void setFilePointer(std::int64_t Offset, seek_mode Mode=seek_beginning) override;

  /*!
    This callback just works like its read pendant. It returns the number of bytes written.
  */
  std::size_t write(const void *Buffer, std::size_t Size) override;

  /*!
    Although the position is always positive, the return value of this callback is signed to
    easily allow negative values for returning errors. When an error occurs, the implementor
    should return -1 and the file pointer otherwise.

    If an error occurs, an exception should be thrown.
  */
  std::uint64_t getFilePointer() override {return dataBufferPos;}

  /*!
    The close callback flushes the file buffers to disk and closes the file. When using the stdio
    library, this is equivalent to calling fclose. When the close is not successful, an exception
    should be thrown.
  */
  void close() override {}

  const binary *GetDataBuffer() const {return dataBuffer;}
  std::uint64_t GetDataBufferSize() const {return dataBufferTotalSize;}
  void SetDataBufferSize(std::uint64_t newDataBufferSize) {dataBufferTotalSize = newDataBufferSize;}
  /*!
    Use this to write some data from another IOCallback
  */
  std::uint32_t write(IOCallback & IOToRead, std::size_t Size);

  bool IsOk() const { return mOk; }
  const std::string &GetLastErrorStr() { return mLastErrorStr; }
protected:
  bool mOk;
  std::string mLastErrorStr;

  binary *dataBuffer;
  /*!
    Postion where we start 'writing' to the dataBuffer
  */
  std::uint64_t dataBufferPos;
  /*!
    Size of the data in the dataBuffer
  */
  std::uint64_t dataBufferTotalSize;
  /*!
    Size of the memory malloc()/realloc()
  */
  std::uint64_t dataBufferMemorySize;
};

} // namespace libebml

#endif // LIBEBML_MEMIOCALLBACK_H
