// Copyright © 2002-2010 Steve Lhomme.
// SPDX-License-Identifier: LGPL-2.1-or-later

/*!
  \file
  \version \$Id: EbmlVersion.cpp 639 2004-07-09 20:59:14Z mosu $
  \author Steve Lhomme     <robux4 @ users.sf.net>
*/

#include "ebml/EbmlVersion.h"

namespace libebml {

const std::string EbmlCodeVersion = "1.4.6";

// Up to version 1.3.3 this library exported a build date string. As
// this made the build non-reproducible, replace it by a placeholder to
// remain API compatible.
const std::string EbmlCodeDate = "Unknown";

} // namespace libebml
