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

#include "wq/defs.h"
#include "wq/core/vector.h"
#include "wq/core/atomic.h"
#include "wq/core/exception.h"

#include <iostream>

using namespace wq;

void exception_test() throw(core::exception) {
	throw core::exception();
}

void atomic_test() throw(core::exception) {
	{
		core::atomic<int> at(0);
		int cmp_val = 0;
		int new_val = 999;
		std::cout << "comparing " << at.val() << " with " << cmp_val << std::endl;
		std::cout << "changing val from: " << at.cmp_set(cmp_val, new_val);
		std::cout << " to: " << at.val() << std::endl;
		std::cout << at.inc(101) << " + 101 = ";
		std::cout << at.val() << std::endl;

		std::cout << "old val " << at.set(0);
		std::cout << " is set to new val - " << at.val() << std::endl;
	}
	std::cout << std::endl;
	{
		typedef core::atomic<unsigned long*> atomic_type;
		atomic_type at(NULL);
		atomic_type::pointer_type new_val = new atomic_type::value_type(10);

		std::cout << "old val is: " << at.val() << std::endl;
		std::cout << "old val is: " << at.set(new_val);
		std::cout << " and new val is: " << at.val() << std::endl;
	}
	std::cout << std::endl;
	{
		typedef core::atomic<int*> atomic_type;
		atomic_type at(NULL);
		atomic_type::pointer_type cmp_val = NULL;
		atomic_type::pointer_type new_val = new atomic_type::value_type();

		std::cout << "comparing " << at.val() << " with " << cmp_val << std::endl;
		std::cout << "setting val from: " << at.cmp_set(cmp_val, new_val);
		std::cout << " to: " << at.val() << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	try {
		atomic_test();
		WQ_NO_THROW(
			WQ_NO_THROW(
				exception_test();
			);
			std::cout << "ahoj, ako sa mas!" << std::endl;
		);
		exception_test();
	}
	catch(core::exception& e) {
	    std::cout << e.what() << std::endl;
    }

	any<int> a = 10;
	std::cout << a << std::endl;

	return 0;
}
