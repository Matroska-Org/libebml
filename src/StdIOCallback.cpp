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
  \version \$Id: StdIOCallback.cpp 1298 2008-02-21 22:14:18Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#include <cassert>
#include <limits>
#include <sstream>

#include "ebml/StdIOCallback.h"
#include "ebml/Debug.h"
#include "ebml/EbmlConfig.h"

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
    throw CRTError(Msg.str());
  }
  mCurrentPosition = 0;
}


StdIOCallback::~StdIOCallback() noexcept
{
  close();
}



uint32 StdIOCallback::read(void*Buffer,size_t Size)
{
  assert(File!=nullptr);

  const size_t result = fread(Buffer, 1, Size, File);
  mCurrentPosition += result;
  return result;
}

void StdIOCallback::setFilePointer(int64 Offset,seek_mode Mode)
{
  assert(File!=nullptr);

  assert(Offset <= (std::numeric_limits<long>::max)());
  assert(Offset >= (std::numeric_limits<long>::min)());

  assert(Mode==SEEK_CUR||Mode==SEEK_END||Mode==SEEK_SET);

  if(fseek(File,Offset,Mode)!=0) {
    ostringstream Msg;
    Msg<<"Failed to seek file "<<File<<" to offset "<<Offset<<" in mode "<<Mode;
    throw CRTError(Msg.str());
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

size_t StdIOCallback::write(const void*Buffer,size_t Size)
{
  assert(File!=nullptr);
  const uint32 Result = fwrite(Buffer,1,Size,File);
  mCurrentPosition += Result;
  return Result;
}

uint64 StdIOCallback::getFilePointer()
{
  assert(File!=nullptr);

#if 0
  long Result=ftell(File);
  if(Result<0) {
    stringstream Msg;
    Msg<<"Can't tell the current file pointer position for "<<File;
    throw CRTError(Msg.str());
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
    throw CRTError(Msg.str());
  }

  File=nullptr;
}

} // namespace libebml
