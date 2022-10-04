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
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#include <limits>
#include <sstream>
#include <stdexcept>

#include "ebml/IOCallback.h"

using namespace std;

namespace libebml {

void IOCallback::writeFully(const void*Buffer,std::size_t Size)
{
  if (Size == 0)
    return;

  if (Buffer == nullptr)
    throw;

  if(write(Buffer,Size) != Size) {
    stringstream Msg;
    Msg<<"EOF in writeFully("<<Buffer<<","<<Size<<")";
    throw runtime_error(Msg.str());
  }
}



void IOCallback::readFully(void*Buffer,std::size_t Size)
{
  if(Buffer == nullptr)
    throw;

  auto readBuf = static_cast<char *>(Buffer);
  auto readSize = static_cast<uint32_t>(std::min<std::size_t>(std::numeric_limits<std::uint32_t>::max(), Size));
  while (readSize != 0) {
    if(read(readBuf,readSize) != readSize) {
      stringstream Msg;
      Msg<<"EOF in readFully("<<Buffer<<","<<Size<<")";
      throw runtime_error(Msg.str());
    }
    Size -= readSize;
    readBuf += readSize;
    readSize = static_cast<uint32_t>(std::min<std::size_t>(std::numeric_limits<std::uint32_t>::max(), Size));
  }
}

} // namespace libebml
