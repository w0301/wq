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

#ifndef WQ_ATOMIC_H
#define WQ_ATOMIC_H

#include "wq/defs.h"

namespace wq {
namespace core {

// template class only for few types:
template<class T> class atomic;

// all functions return old value of variable
// for integer type
template<> class atomic<int> {
	public:
		atomic(int val = 0) : m_num(val) { };

		int set(int);
		int cmp_set(int, int);
		int inc(int = 1);
		int dec(int by = 1) {
			return inc(-by);
		};

		int val() const {
			return m_num;
		};

	private:
		volatile int m_num;
};

inline int atomic<int>::set(int new_val) {
	int old;
#if defined(__i386__) || defined(__x86_64__)
	asm __volatile__
    (
    	"lock xchgl %0, %1;"
    	: "=r" (old), "=m" (m_num)
    	: "0" (new_val)
    	:
    );
#endif
	return old;
}

inline int atomic<int>::cmp_set(int cmp_val, int new_val) {
#if defined(__i386__) || defined(__x86_64__)
	int old;
	asm __volatile__
    (
    	"lock cmpxchgl %2, %0;"
    	: "=m" (m_num), "=a" (old)
    	: "r" (new_val), "m" (m_num), "a" (cmp_val)
    	:
    );
#endif
	return old;
}

inline int atomic<int>::inc(int by) {
#if defined(__i386__) || defined(__x86_64__)
	asm __volatile__
	(
		"lock addl %1, %0;"
		: "=m" (m_num)
		: "ir" (by), "m" (m_num)
		:
    );
#endif
	return m_num - by;
}

// and for all pointers types
template<class T> class atomic<T*> {
	public:
		typedef T value_type;
		typedef T* pointer_type;

		atomic(pointer_type ptr = NULL) : m_ptr(ptr) { };
		~atomic() { };

		pointer_type set(pointer_type);
		pointer_type cmp_set(pointer_type, pointer_type);

		pointer_type val() const {
			return m_ptr;
		};

	private:
		volatile pointer_type m_ptr;
};

template<class T> inline T* atomic<T*>::set(pointer_type new_val) {
	pointer_type old;
#if defined(__i386__) || defined(__x86_64__)
	asm __volatile__
    (
	#if defined(__x86_64__)
		"lock xchgq %0, %1;"
	#else
		"lock xchgl %0, %1;"
	#endif
       	: "=r" (old), "=m" (m_ptr)
        : "0" (new_val)
		:
    );
#endif
	return old;
}

template<class T> inline T* atomic<T*>::cmp_set(pointer_type cmp_val, pointer_type new_val) {
	pointer_type old;
#if defined(__i386__) || defined(__x86_64__)
	asm __volatile__
    (
	#if defined(__x86_64__)
		"lock cmpxchgq %2, %0;"
	#else
		"lock cmpxchgl %2, %0;"
	#endif
       	: "=m" (m_ptr), "=a" (old)
       	: "r" (new_val), "m" (m_ptr), "a" (cmp_val)
        :
    );
#endif
	return old;
}

}  // namespace core
}  // namespace wq

#endif  // WQ_ATOMIC_H
