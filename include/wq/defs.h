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

#ifndef WQ_DEFS_H
#define WQ_DEFS_H

#include "wq/config.h"

#include <cstddef>

namespace wq {

// first of all - defines for exporting to DLL libs
#if WQ_SHARED_LIBS && defined(WQ_WINDOWS)
	#ifdef WQ_BUILDING
		#define WQ_EXPORT __declspec(dllexport)
	#elif
		#define WQ_EXPORT __declspec(dllimport)
	#endif  // WQ_BUILDING
#else
	#define WQ_EXPORT
#endif  // WQ_SHARED_LIBS

// define which should be use for disable copying of object
#define WQ_NO_COPY(className)	\
	private:	\
		className(const className&);	\
		className& operator= (const className&);

// now defining of integer types for each size
typedef signed char int8;
typedef unsigned char uint8;

typedef signed short int16;
typedef unsigned short uint16;

typedef signed int int32;
typedef unsigned int uint32;

typedef signed long long int64;
typedef unsigned long long uint64;

// some shortcuts
typedef uint32 uint;
typedef uint16 ushort;

// definition of type that will be used for size types in containers etc.
typedef std::size_t size_t;

// now define for type which is returned by pointer operations
typedef std::ptrdiff_t ptrdiff_t;

// class for setting flags similar to Qt's one
template<typename T> class flags {
	public:
		typedef T enum_type;

		// creations functions and operators
		flags() : m_flags(enum_type(0)) { };
		flags(uint val) : m_flags(val) { };
		flags(enum_type val) : m_flags(val) { };
		flags(const flags& r) : m_flags(r.m_flags) { };
		flags& operator= (const flags& r) {
			if(this != &r) {
				m_flags = r.m_flags;
			}
			return *this;
		};

		// getting value of all flags
		operator uint () {
			return m_flags;
		};
		uint value() {
			return m_flags;
		};
		const uint value() const {
			return m_flags;
		};

		// (un-)setting flag
		void set(enum_type flag) {
			m_flags |= flag;
		};
		void unset(enum_type flag) {
			m_flags ^= flag;
		};

		// testing for particular flag
		bool test(enum_type flag) const {
			return m_flags & flag;
		};
		bool operator! () const {
			return m_flags == 0;
		};

		// bit operators
		flags operator| (flags r) const {
			return flags(m_flags | r.m_flags);
		};
		flags operator& (flags r) const {
			return flags(m_flags & r.m_flags);
		};
		flags operator^ (flags r) const {
			return flags(m_flags ^ r.m_flags);
		};
		flags operator~ () const {
			return flags(~m_flags);
		};

		// bit operators with assignment
		flags& operator|= (flags r) {
			return ( (*this) = ((*this) | r) );
		};
		flags& operator&= (flags r) {
			return ( (*this) = ((*this) & r) );
		};
		flags& operator^= (flags r) {
			return ( (*this) = ((*this) ^ r) );
		};

	private:
		uint m_flags;
};

// class for pairs
template<class T1, class T2> class pair {
    public:
        typedef T1 first_type;
        typedef T2 second_type;
        pair() { };
        pair(first_type val1, second_type val2) : first(val1), second(val2) { };

        first_type first;
        second_type second;
};

template<class T1, class T2> pair<T1, T2> make_pair(T1 val1, T2 val2) {
    return pair<T1, T2>(val1, val2);
}

// macros for using flags class
#define WQ_DECLARE_FLAGS(newTypeForFlags, enumType)	\
	typedef wq::flags<enumType> newTypeForFlags;


}  // namespace wq


#endif  // WQ_DEFS_H
