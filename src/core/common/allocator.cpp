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

// allocator<void> class
allocator<void>::pointer allocator<void>::allocate(size_type n, void*) {
    if(n > max_size()) {
        throw bad_alloc();
    }
    return malloc(type_info<value_type>::size() * n);
};

void allocator<void>::deallocate(pointer mem, size_type) {
    if(mem == NULL) {
        throw bad_alloc();
    }
    free(mem);
};

allocator<void>::pointer allocator<void>::reallocate(pointer mem, size_type old_size, size_type new_size) {
    if(new_size > max_size()) {
        throw bad_alloc();
    }
    return realloc(mem, new_size);
};

}  // namespace core
}  // namespace wq
