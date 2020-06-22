/****************************************************************************
** libebml : parse EBML files, see http://embl.sourceforge.net/
**
** Defines all exception classes to be used in libebml.
**
** Copyright (C) 2019 Michael Mohr.  All rights reserved.
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

#ifndef LIBEBML_EXCEPTIONS_H
#define LIBEBML_EXCEPTIONS_H

#include <exception>
#include "ebml_export.h"
#include "ebml/EbmlConfig.h"

START_LIBEBML_NAMESPACE

class EBML_DLL_API EbmlError : public std::exception
{
  private:
    const char *reason;
  public:
    explicit EbmlError(const char *why) { this->reason = why; }
    const char * what() const throw() override { return this->reason; }
};

END_LIBEBML_NAMESPACE

#endif //LIBEBML_EXCEPTIONS_H
