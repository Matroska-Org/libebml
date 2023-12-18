// Copyright © 2003 Jory Stone.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: MemIOCallback.cpp 693 2004-07-31 08:56:28Z robux4 $
  \author Jory Stone <jcsston @ toughguy.net>
*/

#include "ebml/MemIOCallback.h"
#include "ebml/Debug.h"
#include "ebml/EbmlConfig.h"

namespace libebml {

MemIOCallback::MemIOCallback(uint64 DefaultSize)
{
  //The default size of the buffer is 128 bytes
  dataBuffer = static_cast<binary *>(malloc(DefaultSize));
  if (dataBuffer == nullptr) {
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

MemIOCallback::~MemIOCallback()
{
  if (dataBuffer != nullptr)
    free(dataBuffer);
}

uint32 MemIOCallback::read(void *Buffer, size_t Size)
{
  if (Buffer == nullptr || Size < 1)
    return 0;
  //If the size is larger than than the amount left in the buffer
  if (Size + dataBufferPos < Size || // overflow, reading too much
      Size + dataBufferPos > dataBufferTotalSize) {
    //We will only return the remaining data
    memcpy(Buffer, dataBuffer + dataBufferPos, dataBufferTotalSize - dataBufferPos);
    uint64 oldDataPos = dataBufferPos;
    dataBufferPos = dataBufferTotalSize;
    return dataBufferTotalSize - oldDataPos;
  }

  //Well... We made it here, so do a quick and simple copy
  memcpy(Buffer, dataBuffer+dataBufferPos, Size);
  dataBufferPos += Size;

  return Size;
}

void MemIOCallback::setFilePointer(int64 Offset, seek_mode Mode)
{
  if (Mode == seek_beginning)
    dataBufferPos = Offset;
  else if (Mode == seek_current)
    dataBufferPos = dataBufferPos + Offset;
  else if (Mode == seek_end)
    dataBufferPos = dataBufferTotalSize + Offset;
}

size_t MemIOCallback::write(const void *Buffer, size_t Size)
{
  if (dataBufferPos + Size < Size) // overflow, we can't hold that much
    return 0;
  if (dataBufferMemorySize < dataBufferPos + Size) {
    //We need more memory!
    dataBuffer = static_cast<binary *>(realloc(static_cast<void *>(dataBuffer), dataBufferPos + Size));
  }
  memcpy(dataBuffer+dataBufferPos, Buffer, Size);
  dataBufferPos += Size;
  if (dataBufferPos > dataBufferTotalSize)
    dataBufferTotalSize = dataBufferPos;

  return Size;
}

uint32 MemIOCallback::write(IOCallback & IOToRead, size_t Size)
{
  if (dataBufferPos + Size < Size) // overflow, we can't hold that much
    return 0;
  if (dataBufferMemorySize < dataBufferPos + Size) {
    //We need more memory!
    dataBuffer = static_cast<binary *>(realloc(static_cast<void *>(dataBuffer), dataBufferPos + Size));
  }
  IOToRead.readFully(&dataBuffer[dataBufferPos], Size);
  dataBufferTotalSize = Size;
  return Size;
}

} // namespace libebml
