// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/
#ifndef LIBEBML_VERSION_H
#define LIBEBML_VERSION_H

#include <string>

#include "EbmlConfig.h"

namespace libebml {

#define LIBEBML_VERSION 0x020000

extern const EBML_DLL_API std::string EbmlCodeVersion;
extern const EBML_DLL_API std::string EbmlCodeDate;

/*!
  \todo Closer relation between an element and the context it comes from (context is an element attribute ?)
*/

} // namespace libebml

#endif // LIBEBML_VERSION_H
