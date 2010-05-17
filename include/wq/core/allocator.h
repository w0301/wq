/****************************************************************************
**
** Copyright (C) 2010 Richard Kakaš.
** All rights reserved.
** Contact: Richard Kakaš <richard.kakas@gmail.com>
**
** @LICENSE_START@
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** @LICENSE_END@
**
****************************************************************************/

#ifndef WQ_ALLOCATOR_H
#define WQ_ALLOCATOR_H

#include "wq/defs.h"
#include "wq/core/exception.h"
#include "wq/core/type_info.h"

namespace wq {
namespace core {

// basic allocator and also base class (or just pattern)
// for all other allocators in wq framework
template<class T> class allocator {
	public:

};


} // namespace core
} // namespace wq


#endif  // WQ_ALLOCATOR_H
