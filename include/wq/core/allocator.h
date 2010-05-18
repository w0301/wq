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

#ifndef WQ_ALLOCATOR_H
#define WQ_ALLOCATOR_H

#include "wq/defs.h"
#include "wq/core/exception.h"
#include "wq/core/type_info.h"

#include <cstring>

#if WQ_STD_COMPATIBILITY
	#include <new>
#endif

namespace wq {
namespace core {

// very bad exception, nobody wants to catch it, trust me!!!
class bad_alloc :
	public wq::core::exception
#if WQ_STD_COMPATIBILITY
	, public std::bad_alloc
#endif
{
	public:
		bad_alloc() throw();
		virtual ~bad_alloc() throw();

		virtual const char* what() const throw();
};

// it is good to be sure that some types won't be allocated
template<class T> class allocator;
template<class T> class allocator<T&>;

// void specialization of class - TODO
template<> class allocator<void>;

// basic allocator and also base class (or just pattern)
// for all other allocators in wq framework
template<class T> class allocator {
	public:
		// some definitions from STL
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef wq::size_t size_type;
		typedef wq::ptrdiff_t difference_type;

		// basic construction
		allocator() { };
		allocator(const allocator&) { };
		allocator& operator= (const allocator&) {
			return *this;
		};

		// construction from allocator for other types
		template<class To> allocator(const allocator<To>&) { };
		template<class To> allocator& operator= (const allocator<To>&) {
			return *this;
		};

		// some return functions, pointless but declared in STL allocator
		// and mine is suppose to by compatible
		pointer address(reference ref) {
			return &ref;
		};
		const_pointer address(const_reference ref) const {
			return &ref;
		};
		size_type max_size() const {
			return size_type(-1);
		};

		// now functions for allocating
		pointer allocate(size_type n, void* = NULL) {
			if(n > max_size()) {
				throw bad_alloc();
			}
			return static_cast<pointer>( ::operator new(type_info<value_type>::size() * n) );
		};
		void deallocate(pointer mem, size_type = -1) {
			if(mem == NULL) {
				throw bad_alloc();
			}
			::operator delete(mem);
		};

		void construct(pointer p, const_reference val) {
			new (static_cast<void*>(p)) value_type(val);
		};
		void destroy(pointer p) {
			p->~value_type();
		};

		// operating with memory
		pointer copy(const_pointer, size_type, pointer);
		pointer ocopy(const_pointer, size_type, pointer);
};

template<class T> T* allocator<T>::copy(const_pointer mem, size_type n, pointer dest) {
	if(type_info<value_type>::is_moveable()) {
		// this is not overlap safe copying
		memcpy(static_cast<void*>(dest), static_cast<const void*>(mem), n);
	}
	else {
		// same as above but slower (not a movable type) and in my words :)
		pointer tmp_dest = dest;
		const_pointer last = mem + n;
		for( ; mem != last; mem++, tmp_dest++) {
			construct(tmp_dest, *mem);
		}
	}
	return dest;
};

template<class T> T* allocator<T>::ocopy(const_pointer mem, size_type n, pointer dest) {
	if(type_info<value_type>::is_moveable()) {
		// overlap safe copying
		memmove(static_cast<void*>(dest), static_cast<const void*>(mem), n);
	}
	else {
		// for not-movable I must write some lines :(
		pointer buffer = allocate(n);
		for(size_type i = 0; i != n; i++) {
			construct(buffer + i, *(mem + i));
		}

		pointer tmp_dest = dest;
		const_pointer tmp_buff = buffer;
		const_pointer buff_last = tmp_buff + n;
		for( ; tmp_buff != buff_last; tmp_buff++, tmp_dest++) {
			// destination memory must be empty
			construct(tmp_dest, *tmp_buff);
		}

		for(size_type i = 0; i != n; i++) {
			destroy(buffer + i);
		}
		deallocate(buffer);
	}
	return dest;
};

}  // namespace core
}  // namespace wq


#endif  // WQ_ALLOCATOR_H
