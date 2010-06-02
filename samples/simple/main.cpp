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
#include "wq/core/list.h"
#include "wq/core/atomic.h"
#include "wq/core/exception.h"
#include "wq/core/shared_ptr.h"
#include "wq/core/allocator.h"
#include "wq/core/type_info.h"
#include "wq/core/string.h"

#include <iostream>

namespace wq {
	using namespace core;
}

int main() {
	try {
		wq::string str("rt");

		/*
		wq::string::iterator i = str.begin();
		for( ; i != str.end(); ) {
			std::cout << i->c_str() << std::endl;
			i = i + 1;
		}
		*/
		wq::string::value_type i = str.at(0);
		for( ; i != wq::string::value_type(); ) {
			std::cout << i.c_str() << std::endl;
			i.rebind(i.next());
		}
	}
	catch(wq::exception& e) {
	    std::cout << e.what() << std::endl;
    }
	return 0;
}
