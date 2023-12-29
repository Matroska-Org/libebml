// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#include "ebml/EbmlVoid.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

DEFINE_EBML_CLASS_ORPHAN(EbmlVoid, 0xEC, 1, "EBMLVoid")

EbmlVoid::EbmlVoid()
  :EbmlBinary(EbmlVoid::ClassInfos)
{
  SetValueIsSet();
}

filepos_t EbmlVoid::RenderData(IOCallback & output, bool /* bForceRender */, ShouldWrite /* writeFilter */)
{
  // write dummy data by 4KB chunks
  static binary DummyBuf[4*1024];

  std::uint64_t SizeToWrite = GetSize();
  while (SizeToWrite > 4*1024) {
    output.writeFully(DummyBuf, 4*1024);
    SizeToWrite -= 4*1024;
  }
  output.writeFully(DummyBuf, SizeToWrite);
  return GetSize();
}

std::uint64_t EbmlVoid::ReplaceWith(EbmlElement & EltToReplaceWith, IOCallback & output, bool ComeBackAfterward, ShouldWrite writeFilter)
{
  EltToReplaceWith.UpdateSize(writeFilter);
  if (HeadSize() + GetSize() < EltToReplaceWith.GetSize() + EltToReplaceWith.HeadSize()) {
    // the element can't be written here !
    return INVALID_FILEPOS_T;
  }
  if (HeadSize() + GetSize() - EltToReplaceWith.GetSize() - EltToReplaceWith.HeadSize() == 1) {
    // there is not enough space to put a filling element
    return INVALID_FILEPOS_T;
  }

  const std::uint64_t CurrentPosition = output.getFilePointer();

  output.setFilePointer(GetElementPosition());
  EltToReplaceWith.Render(output, writeFilter);

  if (HeadSize() + GetSize() - EltToReplaceWith.GetSize() - EltToReplaceWith.HeadSize() > 1) {
    // fill the rest with another void element
    EbmlVoid aTmp;
    aTmp.SetSize_(HeadSize() + GetSize() - EltToReplaceWith.GetSize() - EltToReplaceWith.HeadSize() - 1); // 1 is the length of the Void ID
    const std::size_t HeadBefore = aTmp.HeadSize();
    aTmp.SetSize_(aTmp.GetSize() - CodedSizeLength(aTmp.GetSize(), aTmp.GetSizeLength()));
    const std::size_t HeadAfter = aTmp.HeadSize();
    if (HeadBefore != HeadAfter) {
      aTmp.SetSizeLength(CodedSizeLength(aTmp.GetSize(), aTmp.GetSizeLength()) - (HeadAfter - HeadBefore));
    }
    aTmp.RenderHead(output, false, writeFilter); // the rest of the data is not rewritten
  }

  if (ComeBackAfterward) {
    output.setFilePointer(CurrentPosition);
  }

  return GetSize() + HeadSize();
}

std::uint64_t EbmlVoid::Overwrite(const EbmlElement & EltToVoid, IOCallback & output, bool ComeBackAfterward, ShouldWrite writeFilter)
{
  //  EltToVoid.UpdateSize(bWithDefault);
  if (EltToVoid.GetElementPosition() == 0) {
    // this element has never been written
    return 0;
  }
  if (EltToVoid.GetSize() + EltToVoid.HeadSize() <2) {
    // the element can't be written here !
    return 0;
  }

  const std::uint64_t CurrentPosition = output.getFilePointer();

  output.setFilePointer(EltToVoid.GetElementPosition());

  // compute the size of the voided data based on the original one
  SetSize(EltToVoid.GetSize() + EltToVoid.HeadSize() - 1); // 1 for the ID
  SetSize(GetSize() - CodedSizeLength(GetSize(), GetSizeLength()));
  // make sure we handle even the strange cases
  //std::uint32_t A1 = GetSize() + HeadSize();
  //std::uint32_t A2 = EltToVoid.GetSize() + EltToVoid.HeadSize();
  if (GetSize() + HeadSize() != EltToVoid.GetSize() + EltToVoid.HeadSize()) {
    SetSize(GetSize()-1);
    SetSizeLength(CodedSizeLength(GetSize(), GetSizeLength()) + 1);
  }

  if (GetSize() != 0) {
    RenderHead(output, false, writeFilter); // the rest of the data is not rewritten
  }

  if (ComeBackAfterward) {
    output.setFilePointer(CurrentPosition);
  }

  return EltToVoid.GetSize() + EltToVoid.HeadSize();
}

} // namespace libebml
