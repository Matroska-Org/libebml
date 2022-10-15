/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2004 Ingo Ralf Blum.  All rights reserved.
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
*/
#ifndef LIBEBML_STDIOCALLBACK_H
#define LIBEBML_STDIOCALLBACK_H

#include "IOCallback.h"

#include <stdexcept>
#include <cerrno>

namespace libebml {

class EBML_DLL_API CRTError:public std::runtime_error
{
// Variablen...
private:
  int Error;

// Methoden...
public:
  CRTError(int Error,const std::string&Description);
  explicit CRTError(const std::string&Description,int Error=errno);

  int getError() const noexcept { return Error; }
};

// This class is currently private to the library, so there's no MATROSKA_EXPORT.
class EBML_DLL_API StdIOCallback:public IOCallback
{
    private:
      FILE*File;
    std::uint64_t mCurrentPosition;

    public:
//  StdIOCallback(const char*Path,const char*Mode);
  StdIOCallback(const char*Path, open_mode Mode);
  ~StdIOCallback() noexcept override;
  StdIOCallback(const StdIOCallback&) = delete;
  StdIOCallback& operator=(const StdIOCallback&) = delete;

  std::size_t read(void*Buffer,std::size_t Size) override;

  // Seek to the specified position. The mode can have either SEEK_SET, SEEK_CUR
  // or SEEK_END. The callback should return true(1) if the seek operation succeeded
  // or false (0), when the seek fails.
  void setFilePointer(std::int64_t Offset,seek_mode Mode=seek_beginning) override;

  // This callback just works like its read pendant. It returns the number of bytes written.
  std::size_t write(const void*Buffer,std::size_t Size) override;

  // Although the position is always positive, the return value of this callback is signed to
  // easily allow negative values for returning errors. When an error occurs, the implementor
  // should return -1 and the file pointer otherwise.
  //
  // If an error occurs, an exception should be thrown.
  std::uint64_t getFilePointer() override;

  // The close callback flushes the file buffers to disk and closes the file. When using the stdio
  // library, this is equivalent to calling fclose. When the close is not successful, an exception
  // should be thrown.
  void close() override;
};

} // namespace libebml

#endif
