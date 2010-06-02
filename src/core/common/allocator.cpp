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

#include "wq/core/allocator.h"

namespace wq {
namespace core {

// bad_alloc class
bad_alloc::bad_alloc() throw() : wq::core::exception()
#if WQ_STD_COMPATIBILITY
	, std::bad_alloc()
#endif
{

};

bad_alloc::~bad_alloc() throw() {

}

const char* bad_alloc::what() const throw() {
	return "bad allocation";
}

}  // namespace core
}  // namespace wq
