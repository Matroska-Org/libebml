// Copyright © 2002-2004 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
*/
#ifndef LIBEBML_TYPES_H
#define LIBEBML_TYPES_H

#include <cstdint>

#include "ebml/EbmlConfig.h"

using binary = std::uint8_t;
using filepos_t = std::uint64_t;

#include "EbmlEndian.h" // binary needs to be defined

namespace libebml {

enum ScopeMode {
  SCOPE_PARTIAL_DATA = 0,
  SCOPE_ALL_DATA,
  SCOPE_NO_DATA
};

} // namespace libebml

#endif
