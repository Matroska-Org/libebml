// Copyright © 2002-2004 Ingo Ralf Blum.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#include <cassert>
#include <limits>
#include <sstream>
#include <ios>

#include "ebml/StdIOCallback.h"

using namespace std;

namespace libebml {

CRTError::CRTError(int nError, const std::string & Description)
  :std::runtime_error(Description+": "+strerror(nError))
  ,Error(nError)
{
}

CRTError::CRTError(const std::string & Description,int nError)
  :std::runtime_error(Description+": "+strerror(nError))
  ,Error(nError)
{
}


StdIOCallback::StdIOCallback(const char*Path, const open_mode aMode)
{
  assert(Path!=nullptr);

  const char *Mode;
  switch (aMode) {
    case MODE_READ:
      Mode = "rb";
      break;
    case MODE_SAFE:
      Mode = "rb+";
      break;
    case MODE_WRITE:
      Mode = "wb";
      break;
    case MODE_CREATE:
      Mode = "wb+";
      break;
    default:
      throw std::invalid_argument("Invalid file mode supplied.");
  }

  File=fopen(Path,Mode);
  if(File==nullptr) {
    stringstream Msg;
    Msg<<"Can't open stdio file \""<<Path<<"\" in mode \""<<Mode<<"\"";
    throw ios_base::failure(Msg.str(), error_code{errno, std::system_category()});
  }
  mCurrentPosition = 0;
}


StdIOCallback::~StdIOCallback() noexcept
{
  close();
}



std::size_t StdIOCallback::read(void*Buffer,std::size_t Size)
{
  assert(File!=nullptr);

  const std::size_t result = fread(Buffer, 1, Size, File);
  mCurrentPosition += result;
  return result;
}

void StdIOCallback::setFilePointer(std::int64_t Offset,seek_mode Mode)
{
  assert(File!=nullptr);

  assert(Offset <= (std::numeric_limits<long>::max)());
  assert(Offset >= (std::numeric_limits<long>::min)());

  assert(Mode==SEEK_CUR||Mode==SEEK_END||Mode==SEEK_SET);

  if(fseek(File,Offset,Mode)!=0) {
    ostringstream Msg;
    Msg<<"Failed to seek file "<<File<<" to offset "<<Offset<<" in mode "<<Mode;
    throw ios_base::failure(Msg.str(), error_code{errno, std::system_category()});
  }

  switch ( Mode ) {
    case SEEK_CUR:
      mCurrentPosition += Offset;
      break;
    case SEEK_END:
      mCurrentPosition = ftell(File);
      break;
    case SEEK_SET:
      mCurrentPosition = Offset;
      break;
  }
}

std::size_t StdIOCallback::write(const void*Buffer,std::size_t Size)
{
  assert(File!=nullptr);
  const std::uint32_t Result = fwrite(Buffer,1,Size,File);
  mCurrentPosition += Result;
  return Result;
}

std::uint64_t StdIOCallback::getFilePointer()
{
  assert(File!=nullptr);

#if 0
  long Result=ftell(File);
  if(Result<0) {
    stringstream Msg;
    Msg<<"Can't tell the current file pointer position for "<<File;
    throw ios_base::failure(Msg.str(), error_code{errno, std::system_category()});
  }
#endif

  return mCurrentPosition;
}

void StdIOCallback::close()
{
  if(File==nullptr)
    return;

  if(fclose(File)!=0) {
    stringstream Msg;
    Msg<<"Can't close file "<<File;
    throw ios_base::failure(Msg.str(), error_code{errno, std::system_category()});
  }

  File=nullptr;
}

} // namespace libebml
