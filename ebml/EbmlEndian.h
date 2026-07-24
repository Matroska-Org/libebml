// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
    \file
    \version \$Id: EbmlEndian.h 1298 2008-02-21 22:14:18Z mosu $
    \author Ingo Ralf Blum   <ingoralfblum @ users.sf.net>
    \author Lasse Kärkkäinen <tronic @ users.sf.net>
    \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ENDIAN_H
#define LIBEBML_ENDIAN_H

#include <algorithm>
#include <cstring>

#include "EbmlConfig.h" // contains _ENDIANESS_

namespace libebml {

enum endianess {
    big_endian,   ///< PowerPC, Alpha, 68000
    little_endian ///< Intel x86 platforms
};

/*!
    \class Endian
    \brief general class to handle endian-specific buffers
    \note don't forget to define/undefine _ENDIANESS_ to BIG_ENDIAN depending on your machine
*/
template<class TYPE, endianess ENDIAN> class Endian
{
    public:
      Endian() = default;

      Endian(const TYPE value)
      {
        memcpy(&platform_value, &value, sizeof(TYPE));
        process_endian();
      }

      inline Endian & Eval(const binary *endian_buffer)
      {
          //endian_value = *(TYPE *)(endian_buffer);
          memcpy(&endian_value, endian_buffer, sizeof(TYPE));  // Some (all?) RISC processors do not allow reading objects bigger than 1 byte from non-aligned addresses, and endian_buffer may point to a non-aligned address.
          process_platform();
          return *this;
      }

      inline void Fill(binary *endian_buffer) const
      {
          //*(TYPE*)endian_buffer = endian_value;
          memcpy(endian_buffer, &endian_value, sizeof(TYPE)); // See above.
      }

      inline operator const TYPE&() const { return platform_value; }
    //  inline TYPE endian() const   { return endian_value; }
      inline const TYPE &endian() const       { return endian_value; }
      inline size_t size() const   { return sizeof(TYPE); }
      inline bool operator!=(const binary *buffer) const {return *(reinterpret_cast<TYPE*>(buffer)) == platform_value;}

#if defined(EBML_STRICT_API)
    private:
#else
    protected:
#endif
      TYPE platform_value;
      TYPE endian_value;

      inline void process_endian()
      {
          endian_value = platform_value;
#ifdef WORDS_BIGENDIAN
          if (ENDIAN == little_endian)
#else  // _ENDIANESS_
          if (ENDIAN == big_endian)
#endif // _ENDIANESS_
            std::reverse(reinterpret_cast<uint8*>(&endian_value),reinterpret_cast<uint8*>(&endian_value+1));
      }

      inline void process_platform()
      {
          platform_value = endian_value;
#ifdef WORDS_BIGENDIAN
          if (ENDIAN == little_endian)
#else  // _ENDIANESS_
          if (ENDIAN == big_endian)
#endif // _ENDIANESS_
            std::reverse(reinterpret_cast<uint8*>(&platform_value),reinterpret_cast<uint8*>(&platform_value+1));
      }
};

} // namespace libebml

#endif // LIBEBML_ENDIAN_H
