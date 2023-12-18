/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
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
*/

#include <array>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "ebml/EbmlElement.h"
#include "ebml/EbmlMaster.h"
#include "ebml/EbmlStream.h"
#include "ebml/EbmlVoid.h"
#include "ebml/EbmlDummy.h"
#include "ebml/EbmlContexts.h"

namespace libebml {

/*!
  \todo handle more than CodedSize of 5
*/
int CodedSizeLength(std::uint64_t Length, unsigned int SizeLength, bool bSizeIsFinite)
{
  unsigned int CodedSize;
  if (bSizeIsFinite) {
    // prepare the head of the size (000...01xxxxxx)
    // optimal size
    if (Length < 127) // 2^7 - 1
      CodedSize = 1;
    else if (Length < 16383) // 2^14 - 1
      CodedSize = 2;
    else if (Length < 2097151L) // 2^21 - 1
      CodedSize = 3;
    else if (Length < 268435455L) // 2^28 - 1
      CodedSize = 4;
    else CodedSize = 5;
  } else {
    if (Length <= 127) // 2^7 - 1
      CodedSize = 1;
    else if (Length <= 16383) // 2^14 - 1
      CodedSize = 2;
    else if (Length <= 2097151L) // 2^21 - 1
      CodedSize = 3;
    else if (Length <= 268435455L) // 2^28 - 1
      CodedSize = 4;
    else CodedSize = 5;
  }

  if (SizeLength > 0 && CodedSize < SizeLength) {
    // defined size
    CodedSize = SizeLength;
  }

  return static_cast<int>(CodedSize);
}

int CodedValueLength(std::uint64_t Length, int CodedSize, binary * OutBuffer)
{
  int _SizeMask = 0xFF;
  OutBuffer[0] = 1 << (8 - CodedSize);
  for (int i=1; i<CodedSize; i++) {
    OutBuffer[CodedSize-i] = Length & 0xFF;
    Length >>= 8;
    _SizeMask >>= 1;
  }
  // first one use a OR with the "EBML size head"
  OutBuffer[0] |= Length & 0xFF & _SizeMask;
  return CodedSize;
}

std::uint64_t ReadCodedSizeValue(const binary * InBuffer, std::uint32_t & BufferSize, std::uint64_t & SizeUnknown)
{
  binary SizeBitMask = 1 << 7;
  std::uint64_t Result = 0x7F;
  unsigned int SizeIdx, PossibleSizeLength = 0;
  std::array<binary, 8> PossibleSize = {};

  SizeUnknown = 0x7F; // the last bit is discarded when computing the size
  for (SizeIdx = 0; SizeIdx < BufferSize && SizeIdx < 8; SizeIdx++) {
    if (InBuffer[0] & (SizeBitMask >> SizeIdx)) {
      // ID found
      PossibleSizeLength = SizeIdx + 1;
      SizeBitMask >>= SizeIdx;

      // Guard against invalid memory accesses with incomplete IDs.
      if (PossibleSizeLength > BufferSize)
        break;

      for (SizeIdx = 0; SizeIdx < PossibleSizeLength; SizeIdx++) {
        PossibleSize.at(SizeIdx) = InBuffer[SizeIdx];
      }
      for (SizeIdx = 0; SizeIdx < PossibleSizeLength - 1; SizeIdx++) {
        Result <<= 7;
        Result |= 0xFF;
      }

      Result = 0;
      Result |= PossibleSize[0] & ~SizeBitMask;
      for (unsigned int i = 1; i<PossibleSizeLength; i++) {
        Result <<= 8;
        Result |= PossibleSize.at(i);
      }

      BufferSize = PossibleSizeLength;

      return Result;
    }
    SizeUnknown <<= 7;
    SizeUnknown |= 0xFF;
  }

  BufferSize = 0;
  return 0;
}


EbmlCallbacks::EbmlCallbacks(EbmlElement & (*Creator)(), const EbmlId & aGlobalId, const char * aDebugName, const EbmlSemanticContext & aContext)
  :Create(Creator)
  ,GlobalId(aGlobalId)
  ,DebugName(aDebugName)
  ,Context(aContext)
{
  assert((Create!=nullptr) || !strcmp(aDebugName, "DummyElement"));
}

const EbmlSemantic & EbmlSemanticContext::GetSemantic(std::size_t i) const
{
  assert(i<Size);
  if (i<Size)
    return MyTable[i];

  std::stringstream ss;
  ss << "EbmlSemanticContext::GetSemantic: programming error: index i outside of table size (" << i << " >= " << Size << ")";
  throw std::logic_error(ss.str());
}


EbmlElement::EbmlElement(std::uint64_t aDefaultSize, bool bValueSet)
  :DefaultSize(aDefaultSize)
  , bValueIsSet(bValueSet)
{
  Size = DefaultSize;
}

/*!
  \todo this method is deprecated and should be called FindThisID
  \todo replace the new RawElement with the appropriate class (when known)
*/
EbmlElement * EbmlElement::FindNextID(IOCallback & DataStream, const EbmlCallbacks & ClassInfos, std::uint64_t MaxDataSize)
{
  std::array<binary, 4> PossibleId;
  int PossibleID_Length = 0;
  std::array<binary, 8> PossibleSize; // we don't support size stored in more than 64 bits
  std::uint32_t PossibleSizeLength = 0;
  std::uint64_t SizeUnknown = 0;
  std::uint64_t SizeFound = 0;
  bool bElementFound = false;

  binary BitMask;
  std::uint64_t aElementPosition = 0, aSizePosition = 0;
  while (!bElementFound) {
    // read ID
    aElementPosition = DataStream.getFilePointer();
    std::uint32_t ReadSize = 0;
    BitMask = 1 << 7;
    while (PossibleID_Length < 4) {
      if (!DataStream.read(&PossibleId.at(PossibleID_Length), 1))
        return nullptr;            // no more data

      ++ReadSize;
      ++PossibleID_Length;

      if (PossibleId[0] & BitMask) {
        // this is the last octet of the ID
        // check wether that's the one we're looking for
/*      if (PossibleID == EBML_INFO_ID(ClassInfos)) {
          break;
        } else {
          /// \todo This element should be skipped (use a context ?)
        }*/
        bElementFound = true; /// \todo not exactly the one we're looking for
        break;
      }
      BitMask >>= 1;
    }

    if (!bElementFound)
      return nullptr;

    // read the data size
    aSizePosition = DataStream.getFilePointer();
    std::uint32_t _SizeLength;
    do {
      if (PossibleSizeLength >= 8)
        // Size is larger than 8 bytes
        return nullptr;

      ReadSize += DataStream.read(&PossibleSize.at(PossibleSizeLength++), 1);
      _SizeLength = PossibleSizeLength;
      SizeFound =
          ReadCodedSizeValue(PossibleSize.data(), _SizeLength, SizeUnknown);
    } while (_SizeLength == 0);
  }

  const auto PossibleID = EbmlId(PossibleId.data(), PossibleID_Length);
  auto Result = [=] {
    if (PossibleID == EBML_INFO_ID(ClassInfos))
      return &EBML_INFO_CREATE(ClassInfos);
    return static_cast<EbmlElement *>(new EbmlDummy(PossibleID));
  }();
  Result->SetSizeLength(PossibleSizeLength);

  Result->Size = SizeFound;

  if (!Result->ValidateSize() || (SizeFound != SizeUnknown && MaxDataSize < Result->Size)) {
    delete Result;
    return nullptr;
  }

  // check if the size is not all 1s
  if (SizeFound == SizeUnknown) {
    // Size of this element is unknown
    // only possible for Master elements
    if (!Result->SetSizeInfinite()) {
      /// \todo the element is not allowed to be infinite
      delete Result;
      return nullptr;
    }
  } else Result->SetSizeInfinite(false);
  Result->ElementPosition = aElementPosition;
  Result->SizePosition = aSizePosition;

  return Result;
}


/*!
  \todo replace the new RawElement with the appropriate class (when known)
  \todo skip data for Dummy elements when they are not allowed
  \todo better check of the size checking for upper elements (using a list of size for each level)
  \param LowLevel Will be returned with the level of the element found compared to the context given
*/
EbmlElement * EbmlElement::FindNextElement(IOCallback & DataStream, const EbmlSemanticContext & Context, int & UpperLevel,
                                           std::uint64_t MaxDataSize, bool AllowDummyElt, unsigned int MaxLowerLevel)
{
  int PossibleID_Length = 0;
  std::array<binary, 16> PossibleIdNSize;
  int PossibleSizeLength;
  std::uint64_t SizeUnknown;
  int ReadIndex = 0; // trick for the algo, start index at 0
  std::uint32_t ReadSize = 0, IdStart = 0;
  std::uint64_t SizeFound;
  int SizeIdx;
  bool bFound;
  const int UpperLevel_original = UpperLevel;
  const std::uint64_t ParseStart = DataStream.getFilePointer();

  do {
    // read a potential ID
    do {
      assert(ReadIndex < 16);
      // build the ID with the current Read Buffer
      bFound = false;
      binary IdBitMask = 1 << 7;
      for (SizeIdx = 0; SizeIdx < ReadIndex && SizeIdx < 4; SizeIdx++) {
        if (PossibleIdNSize[0] & (IdBitMask >> SizeIdx)) {
          // ID found
          PossibleID_Length = SizeIdx + 1;
          IdBitMask >>= SizeIdx;
          bFound = true;
          break;
        }
      }
      if (bFound) {
        break;
      }

      if (ReadIndex >= 4) {
        // ID not found
        // shift left the read octets
        memmove(PossibleIdNSize.data(), &PossibleIdNSize[1], --ReadIndex);
        IdStart++;
      }

      if (MaxDataSize <= ReadSize)
        break;
      if (DataStream.read(&PossibleIdNSize.at(ReadIndex++), 1) == 0) {
        return nullptr; // no more data ?
      }
      ReadSize++;

    } while (!bFound);

    if (!bFound)
      // we reached the maximum we could read without a proper ID
      return nullptr;

    SizeIdx = ReadIndex;
    ReadIndex -= PossibleID_Length;

    // read the data size
    std::uint32_t _SizeLength;
    PossibleSizeLength = ReadIndex;
    while (true) {
      _SizeLength = PossibleSizeLength;
      SizeFound = ReadCodedSizeValue(&PossibleIdNSize.at(PossibleID_Length), _SizeLength, SizeUnknown);
      if (_SizeLength != 0) {
        bFound = true;
        break;
      }
      if (PossibleSizeLength >= 8) {
        bFound = false;
        break;
      }
      if (MaxDataSize <= ReadSize) {
        bFound = false;
        break;
      }
      if( DataStream.read( &PossibleIdNSize.at(SizeIdx++), 1 ) == 0 ) {
        return nullptr; // no more data ?
      }
      ReadSize++;
      PossibleSizeLength++;
    }

    if (bFound) {
      // find the element in the context and use the correct creator
      const auto PossibleID = EbmlId(PossibleIdNSize.data(), PossibleID_Length);
      EbmlElement * Result = CreateElementUsingContext(PossibleID, Context, UpperLevel, false, AllowDummyElt, MaxLowerLevel);
      ///< \todo continue is misplaced
      if (Result != nullptr) {
        if (AllowDummyElt || !Result->IsDummy()) {
          Result->SetSizeLength(_SizeLength);

          Result->Size = SizeFound;
          // UpperLevel values
          // -1 : global element
          //  0 : child
          //  1 : same level
          //  + : further parent
          if (Result->ValidateSize() && (SizeFound == SizeUnknown || UpperLevel > 0 || MaxDataSize == 0 ||
                                         MaxDataSize >= (IdStart + PossibleID_Length + _SizeLength + SizeFound))) {
            if (SizeFound != SizeUnknown || Result->SetSizeInfinite()) {
              Result->ElementPosition = ParseStart + IdStart;
              Result->SizePosition = Result->ElementPosition + PossibleID_Length;
              // place the file at the beggining of the data
              DataStream.setFilePointer(Result->SizePosition + _SizeLength);
              return Result;
            }
          }
        }
        delete Result;
      }
    }

    // recover all the data in the buffer minus one byte
    ReadIndex = SizeIdx - 1;
    memmove(PossibleIdNSize.data(), &PossibleIdNSize[1], ReadIndex);
    IdStart++;
    UpperLevel = UpperLevel_original;
  } while ( MaxDataSize >= ReadSize );

  return nullptr;
}

/*!
  \todo what happens if we are in a upper element with a known size ?
*/
EbmlElement * EbmlElement::SkipData(EbmlStream & DataStream, const EbmlSemanticContext & Context, EbmlElement * TestReadElt, bool AllowDummyElt)
{
  EbmlElement * Result = nullptr;
  if (bSizeIsFinite) {
    assert(TestReadElt == nullptr);
    assert(ElementPosition < SizePosition);
    DataStream.I_O().setFilePointer(SizePosition + CodedSizeLength(Size, SizeLength, bSizeIsFinite) + Size, seek_beginning);
    //    DataStream.I_O().setFilePointer(Size, seek_current);
  } else {
    /////////////////////////////////////////////////
    // read elements until an upper element is found
    /////////////////////////////////////////////////
    bool bEndFound = false;
    while (!bEndFound && Result == nullptr) {
      // read an element
      /// \todo 0xFF... and true should be configurable
      //      EbmlElement * NewElt;
      if (TestReadElt == nullptr) {
        int bUpperElement = 0; // trick to call FindNextID correctly
        Result = DataStream.FindNextElement(Context, bUpperElement, 0xFFFFFFFFL, AllowDummyElt);
      } else {
        Result = TestReadElt;
        TestReadElt = nullptr;
      }

      if (Result != nullptr) {
        unsigned int EltIndex;
        // data known in this Master's context
        for (EltIndex = 0; EltIndex < EBML_CTX_SIZE(Context); EltIndex++) {
          if (EbmlId(*Result) == EBML_CTX_IDX_ID(Context,EltIndex)) {
            // skip the data with its own context
            Result = Result->SkipData(DataStream, EBML_SEM_CONTEXT(EBML_CTX_IDX(Context,EltIndex)), nullptr);
            break; // let's go to the next ID
          }
        }

        if (EltIndex >= EBML_CTX_SIZE(Context)) {
          if (EBML_CTX_PARENT(Context) != nullptr) {
            Result = SkipData(DataStream, *EBML_CTX_PARENT(Context), Result);
          } else {
            assert(Context.GetGlobalContext != nullptr);
            if (Context != Context.GetGlobalContext()) {
              Result = SkipData(DataStream, Context.GetGlobalContext(), Result);
            } else {
              bEndFound = true;
            }
          }
        }
      } else {
        bEndFound = true;
      }
    }
  }
  return Result;
}

EbmlElement *EbmlElement::CreateElementUsingContext(const EbmlId & aID, const EbmlSemanticContext & Context,
                                                    int & LowLevel, bool IsGlobalContext, bool bAllowDummy, unsigned int MaxLowerLevel)
{
  unsigned int ContextIndex;
  EbmlElement *Result = nullptr;

  // elements at the current level
  for (ContextIndex = 0; ContextIndex < EBML_CTX_SIZE(Context); ContextIndex++) {
    if (aID == EBML_CTX_IDX_ID(Context,ContextIndex)) {
      return &EBML_SEM_CREATE(EBML_CTX_IDX(Context,ContextIndex));
    }
  }

  // global elements
  assert(Context.GetGlobalContext != nullptr); // global should always exist, at least the EBML ones
  const auto& tstContext = Context.GetGlobalContext();
  if (tstContext != Context) {
    LowLevel--;
    MaxLowerLevel--;
    // recursive is good, but be carefull...
    Result = CreateElementUsingContext(aID, tstContext, LowLevel, true, bAllowDummy, MaxLowerLevel);
    if (Result != nullptr) {
      return Result;
    }
    LowLevel++;
    MaxLowerLevel++;
  } else {
    return nullptr;
  }

  // parent elements
  if (EBML_CTX_MASTER(Context) != nullptr && aID == EBML_INFO_ID(*EBML_CTX_MASTER(Context))) {
    LowLevel++; // already one level up (same as context)
    return &EBML_INFO_CREATE(*EBML_CTX_MASTER(Context));
  }

  // check wether it's not part of an upper context
  if (EBML_CTX_PARENT(Context) != nullptr) {
    LowLevel++;
    MaxLowerLevel++;
    return CreateElementUsingContext(aID, *EBML_CTX_PARENT(Context), LowLevel, IsGlobalContext, bAllowDummy, MaxLowerLevel);
  }

  if (!IsGlobalContext && bAllowDummy) {
    LowLevel = 0;
    Result = new (std::nothrow) EbmlDummy(aID);
  }

  return Result;
}

/*!
  \todo verify that the size written is the same as the data written
*/
filepos_t EbmlElement::Render(IOCallback & output, bool bWithDefault, bool bKeepPosition, bool bForceRender)
{
  assert(bValueIsSet || (bWithDefault && DefaultISset())); // an element is been rendered without a value set !!!
  // it may be a mandatory element without a default value
  if (!bWithDefault && IsDefaultValue()) {
    return 0;
  }
#if !defined(NDEBUG)
  std::uint64_t SupposedSize = UpdateSize(bWithDefault, bForceRender);
#endif // !NDEBUG
  filepos_t result = RenderHead(output, bForceRender, bWithDefault, bKeepPosition);
  const std::uint64_t WrittenSize = RenderData(output, bForceRender, bWithDefault);
#if !defined(NDEBUG)
  if (static_cast<std::int64_t>(SupposedSize) != (0-1))
    assert(WrittenSize == SupposedSize);
#endif // !NDEBUG
  result += WrittenSize;
  return result;
}

/*!
  \todo store the position of the Size writing for elements with unknown size
  \todo handle exceptions on errors
  \todo handle CodeSize bigger than 5 bytes
*/
filepos_t EbmlElement::RenderHead(IOCallback & output, bool bForceRender, bool bWithDefault, bool bKeepPosition)
{
  if (EBML_ID_LENGTH((const EbmlId&)*this) <= 0 || EBML_ID_LENGTH((const EbmlId&)*this) > 4)
    return 0;

  UpdateSize(bWithDefault, bForceRender);

  return MakeRenderHead(output, bKeepPosition);
}

filepos_t EbmlElement::MakeRenderHead(IOCallback & output, bool bKeepPosition)
{
  std::array<binary, 4 + 8> FinalHead; // Class D + 64 bits coded size
  std::size_t FinalHeadSize;

  FinalHeadSize = EBML_ID_LENGTH((const EbmlId&)*this);
  EbmlId(*this).Fill(FinalHead.data());

  const int CodedSize = CodedSizeLength(Size, SizeLength, bSizeIsFinite);
  CodedValueLength(Size, CodedSize, &FinalHead.at(FinalHeadSize));
  FinalHeadSize += CodedSize;

  output.writeFully(FinalHead.data(), FinalHeadSize);
  if (!bKeepPosition) {
    ElementPosition = output.getFilePointer() - FinalHeadSize;
    SizePosition = ElementPosition + EBML_ID_LENGTH((const EbmlId&)*this);
  }

  return FinalHeadSize;
}

std::uint64_t EbmlElement::ElementSize(bool bWithDefault) const
{
  if (!bWithDefault && IsDefaultValue())
    return 0; // won't be saved
  return Size + EBML_ID_LENGTH((const EbmlId&)*this) + CodedSizeLength(Size, SizeLength, bSizeIsFinite);
}

bool EbmlElement::IsSmallerThan(const EbmlElement *Cmp) const
{
  return EbmlId(*this) == EbmlId(*Cmp);
}

bool EbmlElement::CompareElements(const EbmlElement *A, const EbmlElement *B)
{
  if (EbmlId(*A) == EbmlId(*B))
    return A->IsSmallerThan(B);

  return false;
}

void EbmlElement::Read(EbmlStream & inDataStream, const EbmlSemanticContext & /* Context */, int & /* UpperEltFound */, EbmlElement * & /* FoundElt */, bool /* AllowDummyElt */, ScopeMode ReadFully)
{
  ReadData(inDataStream.I_O(), ReadFully);
}

bool EbmlElement::ForceSize(std::uint64_t NewSize)
{
  if (bSizeIsFinite) {
    return false;
  }

  const int OldSizeLen = CodedSizeLength(Size, SizeLength, bSizeIsFinite);
  const std::uint64_t OldSize = Size;

  Size = NewSize;

  if (CodedSizeLength(Size, SizeLength, bSizeIsFinite) == OldSizeLen) {
    bSizeIsFinite = true;
    return true;
  }
  Size = OldSize;

  return false;
}

filepos_t EbmlElement::OverwriteHead(IOCallback & output, bool bKeepPosition)
{
  if (ElementPosition == 0) {
    return 0; // the element has not been written
  }

  const std::uint64_t CurrentPosition = output.getFilePointer();
  output.setFilePointer(GetElementPosition());
  const filepos_t Result = MakeRenderHead(output, bKeepPosition);
  output.setFilePointer(CurrentPosition);
  return Result;
}

filepos_t EbmlElement::OverwriteData(IOCallback & output, bool bKeepPosition)
{
  if (ElementPosition == 0) {
    return 0; // the element has not been written
  }

  auto HeaderSize = EbmlId(*this).GetLength() + CodedSizeLength(Size, SizeLength, bSizeIsFinite);
#if !defined(NDEBUG)
  auto DataSize   = GetSize();
#endif

  auto CurrentPosition = output.getFilePointer();
  output.setFilePointer(GetElementPosition() + HeaderSize);
  auto Result = RenderData(output, true, bKeepPosition);
  output.setFilePointer(CurrentPosition);
  assert(Result == DataSize);
  return Result;
}


std::uint64_t EbmlElement::VoidMe(IOCallback & output, bool bWithDefault) const
{
  if (ElementPosition == 0) {
    return 0; // the element has not been written
  }

  EbmlVoid Dummy;
  return Dummy.Overwrite(*this, output, true, bWithDefault);
}

} // namespace libebml
