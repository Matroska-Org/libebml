// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/

#include <cassert>
#include <algorithm>

#include "ebml/EbmlMaster.h"
#include "ebml/EbmlStream.h"
#include "ebml/MemIOCallback.h"

namespace libebml {

EbmlMaster::EbmlMaster(const EbmlCallbacks & classInfo, bool bSizeIsknown)
 :EbmlElement(classInfo, 0)
{
  SetSizeInfinite(!bSizeIsknown);
  SetValueIsSet();
  ProcessMandatory();
}

EbmlMaster::EbmlMaster(const EbmlMaster & ElementToClone)
 :EbmlElement(ElementToClone)
 ,bChecksumUsed(ElementToClone.bChecksumUsed)
 ,Checksum(ElementToClone.Checksum)
{
  SetSizeInfinite(!IsFiniteSize());
  ElementList.reserve(ElementToClone.ListSize());
  // add a clone of the list
  for (const auto& e : ElementToClone.ElementList)
    ElementList.push_back(e->Clone());
}

EbmlMaster::~EbmlMaster()
{
  for (auto Element : ElementList) {
    delete Element;
  }
}

/*!
  \todo handle exception on errors
  \todo write all the Mandatory elements in the Context, otherwise assert
*/
filepos_t EbmlMaster::RenderData(IOCallback & output, bool bForceRender, const ShouldWrite & writeFilter)
{
  filepos_t Result = 0;

  if (!bForceRender) {
    assert(CheckMandatory());
  }

  if (!bChecksumUsed) { // old school
    for (auto Element : ElementList) {
      if (!Element->CanWrite(writeFilter))
        continue;
      Result += Element->Render(output, writeFilter, false ,bForceRender);
    }
  } else { // new school
    MemIOCallback TmpBuf(GetSize() - 6);
    for (auto Element : ElementList) {
      if (!Element->CanWrite(writeFilter))
        continue;
      Element->Render(TmpBuf, writeFilter, false ,bForceRender);
    }
    std::uint64_t memSize = TmpBuf.GetDataBufferSize();
    const binary *memStart = TmpBuf.GetDataBuffer();
    while (memSize != 0) {
      const auto fillSize = static_cast<std::uint32_t>(std::min<std::uint64_t>(std::numeric_limits<std::uint32_t>::max(), memSize));
      Checksum.FillCRC32(memStart, fillSize);
      memStart += fillSize;
      memSize -= fillSize;
    }
    Result += Checksum.Render(output, writeFilter, false ,bForceRender);
    output.writeFully(TmpBuf.GetDataBuffer(), TmpBuf.GetDataBufferSize());
    Result += TmpBuf.GetDataBufferSize();
  }

  return Result;
}

/*!
  \todo We might be able to forbid elements that don't exist in the context
*/
bool EbmlMaster::PushElement(EbmlElement & element)
{
  try {
    ElementList.push_back(&element);
    return true;
  } catch(...) {
  }
  return false;
}

std::uint64_t EbmlMaster::UpdateSize(const ShouldWrite & writeFilter, bool bForceRender)
{
  if (!CanWrite(writeFilter))
    return 0;

  SetSize_(0);

  if (!IsFiniteSize())
    return (0-1);

  if (!bForceRender) {
    assert(CheckMandatory());
    }

  for (auto Element : ElementList) {
    if (!Element->CanWrite(writeFilter))
      continue;
    Element->UpdateSize(writeFilter, bForceRender);
    const std::uint64_t SizeToAdd = Element->ElementSize(writeFilter);
#if !defined(NDEBUG)
    if (static_cast<std::int64_t>(SizeToAdd) == (0-1))
      return (0-1);
#endif // !NDEBUG
    SetSize_(GetSize() + SizeToAdd);
  }
  if (bChecksumUsed) {
    SetSize_(GetSize() + Checksum.ElementSize());
  }

  return GetSize();
}

filepos_t EbmlMaster::WriteHead(IOCallback & output, unsigned int nSizeLength, const ShouldWrite& writeFilter)
{
  SetSizeLength(nSizeLength);
  return RenderHead(output, false, writeFilter);
}

filepos_t EbmlMaster::ReadData(IOCallback & input, ScopeMode scope)
{
  int upper = 0;
  EbmlStream aStream(input);
  EbmlElement *Parent = nullptr;
  Read(aStream, Context(), upper, Parent, false, scope);
  assert(Parent == nullptr);
  assert(upper == 0);

  input.setFilePointer(GetEndPosition(), seek_beginning);
  return GetSize();
}

/*!
  \note Hopefully no global element is mandatory
  \todo should be called for ALL EbmlMaster element on construction
*/
bool EbmlMaster::ProcessMandatory()
{
  const EbmlSemanticContext & MasterContext = EBML_CONTEXT(this);

  for (unsigned int EltIdx = 0; EltIdx < EBML_CTX_SIZE(MasterContext); EltIdx++) {
    if (EBML_CTX_IDX(MasterContext,EltIdx).IsMandatory() && EBML_CTX_IDX(MasterContext,EltIdx).IsUnique()) {
//      assert(EBML_CTX_IDX(MasterContext,EltIdx).Create != NULL);
      if (PushElement(EBML_SEM_CREATE(EBML_CTX_IDX(MasterContext,EltIdx))))
        continue;

      while (EltIdx-- != 0)
      {
        if (EBML_CTX_IDX(MasterContext,EltIdx).IsMandatory() && EBML_CTX_IDX(MasterContext,EltIdx).IsUnique())
        {
          // element that have been pushed should be removed
          auto *todelete = ElementList.back();
          ElementList.pop_back();
          delete todelete;
        }
      }
      return false;
    }
  }
  return true;
}

bool EbmlMaster::CheckMandatory() const
{
  const EbmlSemanticContext & MasterContext = EBML_CONTEXT(this);

  for (unsigned int EltIdx = 0; EltIdx < EBML_CTX_SIZE(MasterContext); EltIdx++) {
    if (EBML_CTX_IDX(MasterContext,EltIdx).IsMandatory()) {
      const auto & semcb = EBML_CTX_IDX_INFO(MasterContext,EltIdx);
      if (FindElt(semcb) == nullptr) {
        const bool hasDefaultValue = semcb.HasDefault();

#if !defined(NDEBUG)
        // you are missing this Mandatory element
//         const char * MissingName = semcb.GetName();
#endif // !NDEBUG
        if (!hasDefaultValue)
          return false;
      }
    }
  }

  return true;
}

EbmlElement *EbmlMaster::FindElt(const EbmlCallbacks & Callbacks) const
{
  auto it = std::find_if(ElementList.begin(), ElementList.end(), [&](const EbmlElement *Element)
    { return EbmlId(*Element) == EBML_INFO_ID(Callbacks); });

  return it != ElementList.end() ? *it : nullptr;
}

EbmlElement *EbmlMaster::FindFirstElt(const EbmlCallbacks & Callbacks, bool bCreateIfNull)
{
  auto e = FindElt(Callbacks);
  if (e)
    return e;

  if (bCreateIfNull) {
    // add the element
    EbmlElement *NewElt = &EBML_INFO_CREATE(Callbacks);
    if (NewElt == nullptr)
      return nullptr;

    if (!PushElement(*NewElt)) {
      delete NewElt;
      NewElt = nullptr;
    }
    return NewElt;
  }

  return nullptr;
}

EbmlElement *EbmlMaster::FindFirstElt(const EbmlCallbacks & Callbacks) const
{
  return FindElt(Callbacks);
}

/*!
  \todo only return elements that are from the same type !
  \todo the element might be the unique in the context !
*/
EbmlElement *EbmlMaster::FindNextElt(const EbmlElement & PastElt, bool bCreateIfNull)
{
  auto it = std::find(ElementList.begin(), ElementList.end(), &PastElt);
  if (it != ElementList.end()) {
    it = std::find_if(it + 1, ElementList.end(), [&](auto &&element) {
      return EbmlId(PastElt) == EbmlId(*element);
    });

    if (it != ElementList.end())
      return *it;
  }

  if (bCreateIfNull) {
    // add the element
    EbmlElement *NewElt = &(PastElt.CreateElement());

    if (!PushElement(*NewElt)) {
      delete NewElt;
      NewElt = nullptr;
    }
    return NewElt;
  }

  return nullptr;
}

EbmlElement *EbmlMaster::FindNextElt(const EbmlElement & PastElt) const
{
  auto it = std::find(ElementList.begin(), ElementList.end(), &PastElt);
  if (it != ElementList.end()) {
    it = std::find_if(it + 1, ElementList.end(), [&](auto &&element) {
      return EbmlId(PastElt) == EbmlId(*element);
    });

    if (it != ElementList.end())
      return *it;
  }

  return nullptr;
}

EbmlElement *EbmlMaster::AddNewElt(const EbmlCallbacks & Callbacks)
{
  // add the element
  EbmlElement *NewElt = &EBML_INFO_CREATE(Callbacks);
  if (NewElt == nullptr)
    return nullptr;

  if (!PushElement(*NewElt)) {
    delete NewElt;
    NewElt = nullptr;
  }
  return NewElt;
}

void EbmlMaster::Sort()
{
  std::sort(ElementList.begin(), ElementList.end(), EbmlElement::CompareElements);
}

/*!
  \brief Method to help reading a Master element and all subsequent children quickly
  \todo add an option to discard even unknown elements
  \todo handle when a mandatory element is not found
*/
void EbmlMaster::Read(EbmlStream & inDataStream, const EbmlSemanticContext & sContext, int & UpperEltFound, EbmlElement * & FoundElt, bool AllowDummyElt, ScopeMode ReadFully)
{
  if (ReadFully == SCOPE_NO_DATA)
    return;

  EbmlElement * ElementLevelA;
  // remove all existing elements, including the mandatory ones...
  for (auto Element : ElementList) {
    delete Element;
  }
  ElementList.clear();
  std::uint64_t MaxSizeToRead;

  if (IsFiniteSize())
    MaxSizeToRead = GetSize();
  else
    MaxSizeToRead = 0x7FFFFFFF;

  // read blocks and discard the ones we don't care about
  if (MaxSizeToRead > 0)
  {
    inDataStream.I_O().setFilePointer(GetSizePosition() + GetSizeLength(), seek_beginning);
    ElementLevelA = inDataStream.FindNextElement(sContext, UpperEltFound, MaxSizeToRead, AllowDummyElt);
    while (ElementLevelA != nullptr && UpperEltFound <= 0 && MaxSizeToRead > 0) {
      if (IsFiniteSize() && ElementLevelA->IsFiniteSize())
        MaxSizeToRead = GetEndPosition() - ElementLevelA->GetEndPosition(); // even if it's the default value
      if (!AllowDummyElt && ElementLevelA->IsDummy()) {
        if (ElementLevelA->IsFiniteSize()) {
          ElementLevelA->SkipData(inDataStream, sContext);
          delete ElementLevelA; // forget this unknown element
        } else {
          delete ElementLevelA; // forget this unknown element
          break;
        }
      } else {
        try {
          ElementLevelA->Read(inDataStream, EBML_CONTEXT(ElementLevelA), UpperEltFound, FoundElt, AllowDummyElt, ReadFully);
        } catch (...) {
          if (ElementLevelA == FoundElt) {
            UpperEltFound = 0;
            FoundElt = nullptr;
          }
          delete ElementLevelA;
          throw;
        }

        // Discard elements that couldn't be read properly if
        // SCOPE_ALL_DATA has been requested. This can happen
        // e.g. if block data is defective.
        bool DeleteElement = true;

        if (ElementLevelA->ValueIsSet() || (ReadFully != SCOPE_ALL_DATA)) {
          ElementList.push_back(ElementLevelA);
          DeleteElement = false;
        }

        // just in case
        if (ElementLevelA->IsFiniteSize()) {
          ElementLevelA->SkipData(inDataStream, EBML_CONTEXT(ElementLevelA));
          if (DeleteElement)
            delete ElementLevelA;
        } else {
          if (DeleteElement)
            delete ElementLevelA;

          if (UpperEltFound) {
            --UpperEltFound;
            if (UpperEltFound > 0 || MaxSizeToRead <= 0)
              goto processCrc;
            ElementLevelA = FoundElt;
          }

          break;
        }
      }

      if (UpperEltFound > 0) {
        UpperEltFound--;
        if (UpperEltFound > 0 || MaxSizeToRead <= 0)
          goto processCrc;
        ElementLevelA = FoundElt;
        continue;
      }

      if (UpperEltFound < 0) {
        UpperEltFound++;
        if (UpperEltFound < 0)
          goto processCrc;
      }

      if (MaxSizeToRead <= 0)
        goto processCrc;// this level is finished

      ElementLevelA = inDataStream.FindNextElement(sContext, UpperEltFound, MaxSizeToRead, AllowDummyElt);
    }
    if (UpperEltFound > 0) {
      FoundElt = ElementLevelA;
    }
  }
processCrc:

  auto CrcItr =
      std::find_if(ElementList.begin(), ElementList.end(), [=](auto &&element) {
        return EbmlId(*element) == EBML_ID(EbmlCrc32);
      });
  if (CrcItr != ElementList.end()) {
    bChecksumUsed = true;
    // remove the element
    Checksum = *(static_cast<EbmlCrc32 *>(*CrcItr));
  }

  if (bChecksumUsed)
  {
    delete *CrcItr;
    Remove(CrcItr);
  }

  SetValueIsSet();
}

void EbmlMaster::Remove(std::size_t Index)
{
  if (Index < ElementList.size()) {
    ElementList.erase(ElementList.begin() + Index);
  }
}

void EbmlMaster::Remove(EBML_MASTER_ITERATOR & Itr)
{
  ElementList.erase(Itr);
}

void EbmlMaster::Remove(EBML_MASTER_RITERATOR & Itr)
{
  ElementList.erase(Itr.base());
}

bool EbmlMaster::VerifyChecksum() const
{
  if (!bChecksumUsed)
    return true;

  EbmlCrc32 aChecksum;
  /// \todo remove the Checksum if it's in the list
  /// \todo find another way when not all default values are saved or (unknown from the reader !!!)
  MemIOCallback TmpBuf(GetSize() - 6);
  for (auto Element : ElementList) {
    Element->Render(TmpBuf, WriteAll, false, true);
  }
  std::uint64_t memSize = TmpBuf.GetDataBufferSize();
  const binary *memStart = TmpBuf.GetDataBuffer();
  while (memSize != 0) {
    const auto fillSize = static_cast<std::uint32_t>(std::min<std::uint64_t>(std::numeric_limits<std::uint32_t>::max(), memSize));
    aChecksum.FillCRC32(memStart, fillSize);
    memStart += fillSize;
    memSize -= fillSize;
  }

  return (aChecksum.GetCrc32() == Checksum.GetCrc32());
}

bool EbmlMaster::InsertElement(EbmlElement & element, std::size_t position)
{
  if ((ElementList.empty()) && position)
    return false;

  ElementList.insert(ElementList.begin() + position, &element);
  return true;
}

bool EbmlMaster::InsertElement(EbmlElement & element, const EbmlElement & before)
{
  auto Itr = std::find(ElementList.begin(), ElementList.end(), &before);
  if (Itr == ElementList.end())
    return false;

  ElementList.insert(Itr, &element);
  return true;
}


} // namespace libebml
