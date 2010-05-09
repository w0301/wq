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

#include "wq/core/exception.h"

namespace wq {
namespace core {

/*!
	\class exception
	\brief Base class for all exceptions.

	This is base class for all other exceptions classes in wq libraries.
	It also derives from std::excpetion, but only if wq is built with
	WQ_STD_COMPATIBILITY enabled. So if WQ_STD_COMPATIBILITY is enabled
	you can handle wq's exceptions like this:
	\code
		try {
			throw wq::core::exception();
		}
		catch(std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	\endcode
*/

/*!
	\brief Description string.

	This function returns description string for exception.
	It's suppose to be overridden in derived classes, to return
	right description string. Without overriding it returns
	string "Unspecified exception occurred.".
*/
const char* exception::what() const throw() {
	return "Unspecified exception occurred.";
}

}  // namespace core
}  // namespace wq
