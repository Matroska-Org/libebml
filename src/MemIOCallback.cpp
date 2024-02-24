// Copyright © 2003 Jory Stone.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Jory Stone <jcsston @ toughguy.net>
*/

#include "ebml/MemIOCallback.h"

#include <cstring>

namespace libebml {

MemIOCallback::MemIOCallback(std::uint64_t DefaultSize)
{
  //The default size of the buffer is 128 bytes
  try {
    dataBuffer.resize(DefaultSize);
  } catch (...) {
    mOk = false;
    std::stringstream Msg;
    Msg << "Failed to alloc memory block of size ";
    // not working with VC6    Msg << DefaultSize;
    mLastErrorStr = Msg.str();
    return;
  }

  dataBufferMemorySize = DefaultSize;
  dataBufferPos = 0;
  dataBufferTotalSize = 0;
  mOk = true;
}

std::size_t MemIOCallback::read(void *Buffer, std::size_t Size)
{
  if (Buffer == nullptr || Size < 1)
    return 0;
  //If the size is larger than than the amount left in the buffer
  if (Size + dataBufferPos < Size || // overflow, reading too much
      Size + dataBufferPos > dataBufferTotalSize) {
    //We will only return the remaining data
    memcpy(Buffer, dataBuffer.data() + dataBufferPos, dataBufferTotalSize - dataBufferPos);
    const std::uint64_t oldDataPos = dataBufferPos;
    dataBufferPos = dataBufferTotalSize;
    return dataBufferTotalSize - oldDataPos;
  }

  //Well... We made it here, so do a quick and simple copy
  memcpy(Buffer, dataBuffer.data()+dataBufferPos, Size);
  dataBufferPos += Size;

  return Size;
}

void MemIOCallback::setFilePointer(std::int64_t Offset, seek_mode Mode)
{
  if (Mode == seek_beginning)
    dataBufferPos = Offset;
  else if (Mode == seek_current)
    dataBufferPos = dataBufferPos + Offset;
  else if (Mode == seek_end)
    dataBufferPos = dataBufferTotalSize + Offset;
}

std::size_t MemIOCallback::write(const void *Buffer, std::size_t Size)
{
  if (dataBufferPos + Size < Size) // overflow, we can't hold that much
    return 0;
  if (dataBufferMemorySize < dataBufferPos + Size) {
    //We need more memory!
    dataBuffer.resize(dataBufferPos + Size);
    dataBufferMemorySize = dataBufferPos + Size;
  }
  memcpy(dataBuffer.data()+dataBufferPos, Buffer, Size);
  dataBufferPos += Size;
  if (dataBufferPos > dataBufferTotalSize)
    dataBufferTotalSize = dataBufferPos;

  return Size;
}

std::size_t MemIOCallback::write(IOCallback & IOToRead, std::size_t Size)
{
  if (dataBufferPos + Size < Size) // overflow, we can't hold that much
    return 0;
  if (dataBufferMemorySize < dataBufferPos + Size) {
    //We need more memory!
    dataBuffer.resize(dataBufferPos + Size);
    dataBufferMemorySize = dataBufferPos + Size;
  }
  IOToRead.readFully(&dataBuffer[dataBufferPos], Size);
  dataBufferTotalSize = Size;
  return Size;
}

} // namespace libebml
