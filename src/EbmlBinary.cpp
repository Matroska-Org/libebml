// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Julien Coloos  <suiryc @ users.sf.net>
*/
#include <string>
#include <stdexcept>

#include "ebml/EbmlBinary.h"

namespace libebml {

EbmlBinary::EbmlBinary(const EbmlCallbacks & classInfo)
  :EbmlElement(classInfo, 0, false)
{}

EbmlBinary::EbmlBinary(const EbmlBinary & ElementToClone)
  :EbmlElement(ElementToClone)
{
  if (ElementToClone.Data) {
    Data = static_cast<binary *>(malloc(GetSize()));
    if(Data)
      memcpy(Data, ElementToClone.Data, GetSize());
  }
}

EbmlBinary &
EbmlBinary::operator=(const EbmlBinary & ElementToClone)
{
  if (this == &ElementToClone)  // check for self-assigment
    return *this;

  free(Data);
  if (ElementToClone.Data == nullptr)
    Data = nullptr;
  else {
    Data = static_cast<binary *>(malloc(GetSize()));
    if(Data != nullptr)
      memcpy(Data, ElementToClone.Data, GetSize());
  }
  return *this;
}

EbmlBinary::~EbmlBinary() {
  if(Data)
    free(Data);
}

EbmlBinary::operator const binary &() const {return *Data;}


filepos_t EbmlBinary::RenderData(IOCallback & output, bool /* bForceRender */, const ShouldWrite & /* writeFilter */)
{
  output.writeFully(Data,GetSize());

  return GetSize();
}

/*!
  \note no Default binary value handled
*/
filepos_t EbmlBinary::UpdateSize(const ShouldWrite & writeFilter, bool /* bForceRender */)
{
  if (!CanWrite(writeFilter))
    return 0;

  return GetSize();
}

filepos_t EbmlBinary::ReadData(IOCallback & input, ScopeMode ReadFully)
{
  const auto SizeToRead = GetSize();
  if (Data != nullptr) {
    free(Data);
    Data = nullptr;
  }

  if (ReadFully == SCOPE_NO_DATA) {
    return SizeToRead;
  }

  if (!SizeToRead) {
    SetValueIsSet();
    return 0;
  }

  Data = (SizeToRead < std::numeric_limits<std::size_t>::max()) ? static_cast<binary *>(malloc(SizeToRead)) : nullptr;
  if (Data == nullptr)
    throw std::runtime_error("Error allocating data");
  input.readFully(Data, SizeToRead);
  SetValueIsSet();
  return SizeToRead;
}

bool EbmlBinary::operator==(const EbmlBinary & ElementToCompare) const
{
  return ((GetSize() == ElementToCompare.GetSize()) && (GetSize() == 0 || !memcmp(Data, ElementToCompare.Data, GetSize())));
}

} // namespace libebml
