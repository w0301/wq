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
#include <string>

namespace wq {
	using namespace core;
}

int main() {
	try {
		wq::string str("+ľščťžýáíé=");
		for(wq::string::const_reverse_iterator i = str.rbegin(); i != str.rend(); i++) {
			std::cout << *i << std::endl;
		}
		std::cout << std::endl << std::endl;
		for(wq::string::const_iterator i = str.begin(); i != str.end(); i++) {
			std::cout << *i << std::endl;
		}
	}
	catch(wq::exception& e) {
	    std::cout << e.what() << std::endl;
    }
	return 0;
}
