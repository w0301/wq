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
	\brief Construct new exception object.

	This constructor doesn't do anything special. The biggest
	thing done by this constructor is calling of std::exception constructor.
*/
exception::exception() throw()
#if WQ_STD_COMPATIBILITY
	: std::exception()
#endif
{

}

/*!
	\brief Destruct exception object.

	Standard destructor for virtual class.
*/
exception::~exception() throw() {

}

/*!
	\brief Description string.

	This function returns description string for exception.
	It's suppose to be overridden in derived classes, to return
	right description string. Without overriding it returns
	string "unspecified exception".
*/
const char* exception::what() const throw() {
	return "unspecified exception";
}

/*!
	\class range_error
	\brief Range exception.

	This exception is thrown when an index or an iterator
	points to bad place.

	\sa wq::core::exception
*/

/*!
	\brief Construct new exception object.

	This constructor doesn't do anything special. The biggest
	thing done by this constructor is calling of std::out_of_range constructor.
*/
range_error::range_error() throw() : wq::core::exception()
#if WQ_STD_COMPATIBILITY
	, std::out_of_range("")
#endif
{

}

/*!
	\brief Destruct exception object.

	Standard destructor for virtual class.
*/
range_error::~range_error() throw() {

}

/*!
	\brief Description string.

	Description string for this kinf of exception
	is "index or iterator was out of range".
*/
const char* range_error::what() const throw() {
	return "index or iterator was out of range";
}

/*!
	\fn WQ_NO_THROW(...)
	\relates wq::core::exception
	\brief Disallow throwing of exception.

	This macro is used when somebody wants to prohibit throwing
	exceptions. It is used like this:
	\code
		WQ_NO_THROW(
			foo1();
			foo2();
		)
	\endcode
	Please bear in mind that WQ_NO_THROW section is stopped after
	first exception occurred, to override this use nested macros:
		\code
		WQ_NO_THROW(
			WQ_NO_THROW(throw_func());
			throw_func2();	// this function is called because of nested WQ_NO_THROW
		)
	\endcode
*/
}  // namespace core
}  // namespace wq
