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
#include "wq/core/exception.h"

#include <iostream>

using namespace wq;

int main() {
	try {
		throw wq::core::exception();
	}
	catch(std::exception& e) {
	    std::cout << e.what() << std::endl;
    }

	return 0;
}
