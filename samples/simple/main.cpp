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

#include <iostream>

enum SimpleFlag {
	Flag1 = 0x01,
	Flag2 = 0x10
};

WQ_DECLARE_FLAGS(SimpleFlags, SimpleFlag);

int main() {
	SimpleFlags fl;
	fl |= Flag1;

	fl.set(Flag1);

	fl.set(Flag2);

	fl.unset(Flag2);

	if(fl.test(Flag1)) {
		std::cout << "Flag1 set!" << std::endl;
	}

	fl.set(Flag2);

	fl.unset(Flag1);

	if(fl.test(Flag2)) {
		std::cout << "Flag2 set!" << std::endl;
	}

	fl.unset(Flag2);

	return 0;
}
