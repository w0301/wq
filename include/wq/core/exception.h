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

#ifndef WQ_EXCEPTION_H
#define WQ_EXCEPTION_H

#include "wq/defs.h"

#if WQ_STD_COMPATIBILITY
	#include <exception>
#endif

namespace wq {
namespace core {

class WQ_EXPORT exception
#if WQ_STD_COMPATIBILITY
	: public std::exception
#endif
{
	public:
		exception() throw();
		virtual ~exception() throw();

		virtual const char* what() const throw();
};

#define WQ_NO_THROW(...) try { __VA_ARGS__ } catch(...) { }


}  // namespace core
}  // namespace wq


#endif  // WQ_EXCEPTION_H
