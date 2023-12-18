// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id$
  \author Steve Lhomme     <robux4 @ users.sf.net>
  \author Julien Coloos  <suiryc @ users.sf.net>
*/
#ifndef LIBEBML_BINARY_H
#define LIBEBML_BINARY_H

#include <cstdlib>
#include <cstring>

#include "EbmlTypes.h"
#include "EbmlElement.h"

// ----- Added 10/15/2003 by jcsston from Zen -----
#if defined (__BORLANDC__) //Maybe other compilers?
  #include <mem.h>
#endif //__BORLANDC__
// ------------------------------------------------

namespace libebml {

/*!
    \class EbmlBinary
    \brief Handle all operations on an EBML element that contains "unknown" binary data

  \todo handle fix sized elements (like UID of CodecID)
*/
class EBML_DLL_API EbmlBinary : public EbmlElement {
  public:
    EbmlBinary();
    EbmlBinary(const EbmlBinary & ElementToClone);
    ~EbmlBinary() override;

    bool ValidateSize() const override {return IsFiniteSize() && GetSize() < 0x7FFFFFFF;} // we don't mind about what's inside

    filepos_t RenderData(IOCallback & output, bool bForceRender, bool bWithDefault = false) override;
    filepos_t ReadData(IOCallback & input, ScopeMode ReadFully = SCOPE_ALL_DATA) override;
    filepos_t UpdateSize(bool bWithDefault = false, bool bForceRender = false) override;

    void SetBuffer(const binary *Buffer, const uint32 BufferSize) {
      Data = const_cast<binary *>(Buffer);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    binary *GetBuffer() const {return Data;}

    void CopyBuffer(const binary *Buffer, const uint32 BufferSize) {
      if (Data != nullptr)
        free(Data);
      Data = static_cast<binary *>(malloc(BufferSize * sizeof(binary)));
      memcpy(Data, Buffer, BufferSize);
      SetSize_(BufferSize);
      SetValueIsSet();
    }

    operator const binary &() const;

    bool IsDefaultValue() const override {
      return false;
    }

    bool operator==(const EbmlBinary & ElementToCompare) const;

#if defined(EBML_STRICT_API)
  private:
#else
  protected:
#endif
    binary *Data; // the binary data inside the element
};

} // namespace libebml

#endif // LIBEBML_BINARY_H
