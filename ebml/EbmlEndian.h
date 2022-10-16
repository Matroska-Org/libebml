/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** <file/class description>
**
** Copyright (C) 2002-2010 Steve Lhomme.  All rights reserved.
**
** This file is part of libebml.
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
    \author Ingo Ralf Blum   <ingoralfblum @ users.sf.net>
    \author Lasse Kärkkäinen <tronic @ users.sf.net>
    \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_ENDIAN_H
#define LIBEBML_ENDIAN_H

#if !defined(WORDS_BIGENDIAN)
#if defined(_MSC_VER)
#include <intrin.h>     // _byteswap_uint64
// #include <immintrin.h>  // Intel _loadbe_i64 / _storebe_i64 depends on the CPU
#endif // MSVC
#endif // !BIGENDIAN

#include <algorithm>
#include <cstring>

#include "EbmlConfig.h" // contains _ENDIANESS_

#if defined(__linux__)
#include <endian.h>
#if __BYTE_ORDER == __LITTLE_ENDIAN
#undef WORDS_BIGENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#define WORDS_BIGENDIAN 1
#endif
#else // !LINUX
// automatic endianess detection working on GCC
#if !defined(WORDS_BIGENDIAN)
#if (defined (__arm__) && ! defined (__ARMEB__)) || defined (__i386__) || defined (__i860__) || defined (__ns32000__) || defined (__vax__) || defined (__amd64__) || defined (__x86_64__)
#undef WORDS_BIGENDIAN
#elif defined (__sparc__) || defined (__alpha__) || defined (__PPC__) || defined (__mips__) || defined (__ppc__) || defined (__BIG_ENDIAN__)
#define WORDS_BIGENDIAN 1
#else // other CPU
// not automatically detected, put it yourself
#undef WORDS_BIGENDIAN // for my testing platform (x86)
#endif
#endif // not autoconf
#endif

#if defined(WORDS_BIGENDIAN) && defined(BUILD_LITTLE_ENDIAN)
#error mismatching endianess between C++ compiler anc CMake
#endif
#if !defined(WORDS_BIGENDIAN) && defined(BUILD_BIG_ENDIAN)
#error mismatching endianess between C++ compiler anc CMake
#endif

namespace libebml {

namespace endian {


#if defined(WORDS_BIGENDIAN)

// nothing to do
#define swap_big(T)  (T)

#elif defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)

// optimized little-endian
static inline std::int64_t swap_big(std::int64_t value)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap64(value);
#else // _MSC_VER
    return _byteswap_uint64(value);
#endif
}

static inline std::int32_t swap_big(std::int32_t value)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap32(value);
#else // _MSC_VER
    return _byteswap_ulong(value);
#endif
}

static inline std::int16_t swap_big(std::int16_t value)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_bswap16(value);
#else // _MSC_VER
    return _byteswap_ushort(value);
#endif
}

#else  // !GCC && !CLANG && !MSVC

// generic version
template<class T>
static inline T swap_big(T value)
{
    std::reverse(reinterpret_cast<std::uint8_t*>(&value),reinterpret_cast<std::uint8_t*>(&value+1));
    // TODO support C++23 std::byteswap
    return value;
}

#endif // !GCC && !CLANG && !MSVC

static inline std::int64_t from_big64(const binary *big_ptr)
{
    std::int64_t result;
    memcpy(&result, big_ptr, sizeof(result));
    return swap_big(result);
}

static inline std::int32_t from_big32(const binary *big_ptr)
{
    std::int32_t result;
    memcpy(&result, big_ptr, sizeof(result));
    return swap_big(result);
}

static inline std::int16_t from_big16(const binary *big_ptr)
{
    std::int16_t result;
    memcpy(&result, big_ptr, sizeof(result));
    return swap_big(result);
}

static inline void to_big64(std::int64_t value, binary out[8])
{
    value = swap_big(value);
    memcpy(out, &value, sizeof(value));
}

static inline void to_big32(std::int32_t value, binary out[4])
{
    value = swap_big(value);
    memcpy(out, &value, sizeof(value));
}

static inline void to_big16(std::int16_t value, binary out[2])
{
    value = swap_big(value);
    memcpy(out, &value, sizeof(value));
}

} // namespace endian

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

      explicit Endian(const TYPE value)
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

      inline explicit operator const TYPE&() const { return platform_value; }
    //  inline TYPE endian() const   { return endian_value; }
      inline const TYPE &endian() const       { return endian_value; }
      inline std::size_t size() const   { return sizeof(TYPE); }
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
            std::reverse(reinterpret_cast<std::uint8_t*>(&endian_value),reinterpret_cast<std::uint8_t*>(&endian_value+1));
      }

      inline void process_platform()
      {
          platform_value = endian_value;
#ifdef WORDS_BIGENDIAN
          if (ENDIAN == little_endian)
#else  // _ENDIANESS_
          if (ENDIAN == big_endian)
#endif // _ENDIANESS_
            std::reverse(reinterpret_cast<std::uint8_t*>(&platform_value),reinterpret_cast<std::uint8_t*>(&platform_value+1));
      }
};

using big_int16 = Endian<std::int16_t, big_endian>;
using big_int32 = Endian<std::int32_t, big_endian>;
using big_int64 = Endian<std::int64_t, big_endian>;
using big_uint16 = Endian<std::uint16_t, big_endian>;
using big_uint32 = Endian<std::uint32_t, big_endian>;
using big_uint64 = Endian<std::uint64_t, big_endian>;

} // namespace libebml

#endif // LIBEBML_ENDIAN_H
