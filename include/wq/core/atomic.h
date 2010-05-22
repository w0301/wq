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
		typedef int value_type;

		// creation
		atomic(int val = 0) : m_num(val) { };
		atomic& operator= (int val) {
			set(val);
			return *this;
		};

		atomic(const atomic& r) : m_num(r.val()) { };
		atomic& operator= (const atomic& r) {
			if(this != &r) {
				set(r.val());
			}
			return *this;
		};

		// operating with value
		int set(int);
		int cmp_set(int, int);
		int inc(int = 1);
		int dec(int by = 1) {
			return inc(-by);
		};
		int val() const {
			return m_num;
		};

		// some operators
		atomic& operator+= (int by) {
			inc(by);
			return *this;
		};
		atomic& operator-= (int by) {
			dec(by);
			return *this;
		};
		atomic& operator++ (int) {
			return ((*this) += 1);
		};
		atomic& operator-- (int by) {
			return ((*this) -= 1);
		};

		// not atomic comparing
		bool operator== (int r) const {
			return val() == r;
		};
		bool operator!= (int r) const {
			return val() != r;
		};

		// implicit conversion
		operator int() const {
			return val();
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
		typedef value_type* pointer;
		typedef const pointer const_pointer;

		// creation
		atomic(pointer ptr = NULL) : m_ptr(ptr) { };
		atomic& operator= (pointer val) {
			set(val);
			return *this;
		};

		atomic(const atomic& r) : m_ptr(r.val()) { };
		atomic& operator= (const atomic& r) {
			if(this != &val) {
				set(r.val());
			}
			return *this;
		};

		// destruction
		~atomic() {
			unset();
		};

		// operating with pointer
		pointer set(pointer);
		pointer cmp_set(pointer, pointer);

		// atomic unset and deleting of value
		void unset() {
			if(m_ptr != NULL) {
				delete set(NULL);
			}
		};

		// implicit conversion
		operator pointer() const {
			return val();
		};
		pointer val() {
			return m_ptr;
		};
		const pointer val() const {
			return m_ptr;
		};
		pointer operator-> () {
			return val();
		};
		const pointer operator-> () const {
			return val();
		};
		value_type& operator* () {
			return *val();
		};
		const value_type& operator* () const {
			return *val();
		};

		// not atomic comparing
		bool operator== (pointer r) {
			return val() == r;
		};
		bool operator!= (pointer r) {
			return val() != r;
		};

	private:
		volatile pointer m_ptr;
};

template<class T> inline T* atomic<T*>::set(pointer new_val) {
	pointer old;
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

template<class T> inline T* atomic<T*>::cmp_set(pointer cmp_val, pointer new_val) {
	pointer old;
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
