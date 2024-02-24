// Copyright © 2002-2004 Ingo Ralf Blum.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
*/
#ifndef MATROSKA_IOCALLBACK_H
#define MATROSKA_IOCALLBACK_H

#include "EbmlConfig.h"
#include "EbmlTypes.h"

#include <cstdio>


namespace libebml {

enum seek_mode
{
  seek_beginning=SEEK_SET
  ,seek_end=SEEK_END
  ,seek_current=SEEK_CUR
};

class EBML_DLL_API IOCallback
{
public:
  virtual ~IOCallback() = default;

  // The read callback works like most other read functions. You specify the
  // file, the buffer and the size and the function returns the bytes read.
  // If an error occurs or the file pointer points to the end of the file 0 is returned.
  // Users are encouraged to throw a descriptive exception, when an error occurs.
  virtual std::size_t read(void*Buffer,std::size_t Size)=0;

  // Seek to the specified position. The mode can have either SEEK_SET, SEEK_CUR
  // or SEEK_END. The callback should return true(1) if the seek operation succeeded
  // or false (0), when the seek fails.
  virtual void setFilePointer(std::int64_t Offset,seek_mode Mode=seek_beginning)=0;

  // This callback just works like its read pendant. It returns the number of bytes written.
  virtual std::size_t write(const void*Buffer,std::size_t Size)=0;

  // Although the position is always positive, the return value of this callback is signed to
  // easily allow negative values for returning errors. When an error occurs, the implementor
  // should return -1 and the file pointer otherwise.
  //
  // If an error occurs, an exception should be thrown.
  virtual std::uint64_t getFilePointer()=0;

  // The close callback flushes the file buffers to disk and closes the file. When using the stdio
  // library, this is equivalent to calling fclose. When the close is not successful, an exception
  // should be thrown.
  virtual void close()=0;


  // The readFully is made virtual to allow derived classes to use another
  // implementation for this method, which e.g. does not read any data
  // unlike this does
  void readFully(void*Buffer,std::size_t Size);

  template<class STRUCT> void readStruct(STRUCT&Struct){readFully(&Struct,sizeof(Struct));}

  void writeFully(const void*Buffer,std::size_t Size);

  template<class STRUCT> void writeStruct(const STRUCT&Struct){writeFully(&Struct,sizeof(Struct));}
};

/* cygwin incompatible
template<class TRAITS> std::basic_ostream<char,TRAITS>&operator<<(std::basic_ostream<char,TRAITS>&Stream,seek_mode Mode)
{
  switch(Mode)
  {
#define x(y) case seek_##y: Stream<<"seek_" #y; break
    x(beginning);
    x(current);
    x(end);
#undef x
  default:
    assert(false);
  }

  return Stream;
}
*/

} // namespace libebml

#endif // MATROSKA_IOCALLBACK_H
