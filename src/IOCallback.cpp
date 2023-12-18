// Copyright © 2002-2004 Ingo Ralf Blum.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Moritz Bunkus <moritz @ bunkus.org>
*/

#include <algorithm>
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
