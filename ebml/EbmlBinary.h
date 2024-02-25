// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Julien Coloos  <suiryc @ users.sf.net>
*/
#ifndef LIBEBML_BINARY_H
#define LIBEBML_BINARY_H

#include <cstdlib>
#include <cstring>

#include "EbmlTypes.h"
#include "EbmlElement.h"

namespace libebml {

/*!
    \class EbmlBinary
    \brief Handle all operations on an EBML element that contains "unknown" binary data

  \todo handle fix sized elements (like UID of CodecID)
*/
class EBML_DLL_API EbmlBinary : public EbmlElement {
  public:
    EbmlBinary(const EbmlCallbacks &);
    EbmlBinary(const EbmlBinary & ElementToClone);
    EbmlBinary& operator=(const EbmlBinary & ElementToClone);
    ~EbmlBinary() override;

    bool SizeIsValid(std::uint64_t size) const override {return size < 0x7FFFFFFF;} // we don't mind about what's inside
    static inline bool SizeIsValid(std::uint64_t size) {return size < 0x7FFFFFFF;} // we don't mind about what's inside

    filepos_t RenderData(IOCallback & output, bool bForceRender, const ShouldWrite & writeFilter = WriteSkipDefault) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(const ShouldWrite & writeFilter = WriteSkipDefault, bool bForceRender = false) override;

    void SetBuffer(const binary *Buffer, const std::uint32_t BufferSize) {
      Data = const_cast<binary *>(Buffer);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    binary *GetBuffer() const {return Data;}

    void CopyBuffer(const binary *Buffer, const std::uint32_t BufferSize) {
      if (Data != nullptr)
        free(Data);
      Data = static_cast<binary *>(malloc(BufferSize * sizeof(binary)));
      memcpy(Data, Buffer, BufferSize);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    using EbmlElement::operator const EbmlId &;
    operator const binary &() const;

    bool IsDefaultValue() const override {
      return false;
    }

    bool operator==(const EbmlBinary & ElementToCompare) const;

  private:
    binary *Data{nullptr}; // the binary data inside the element
};

} // namespace libebml

#endif // LIBEBML_BINARY_H
