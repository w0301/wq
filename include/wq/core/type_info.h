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

#ifndef WQ_TYPE_INFO_H
#define WQ_TYPE_INFO_H

#include "wq/defs.h"

namespace wq {
namespace core {

// this class is used for determine move ability of types
template<class T> class type_info {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		static uint size() {
			return sizeof(value_type);
		};
		static bool is_moveable() {
			return false;
		};
};

// all pointers are movable
template<class T> class type_info<T*> {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		static uint size() {
			return sizeof(value_type);
		};
		static bool is_moveable() {
			return true;
		};
};

// special declaration for void and void* types
template<> class type_info<void>;
template<> class type_info<void*> {
	public:
		typedef void* value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		static uint size() {
			return sizeof(value_type);
		};
		static bool is_moveable() {
			return true;
		};
};

// allowing moving for types by this macro
#define WQ_MOVEABLE_TYPE(type)                              \
    template <> bool type_info<type>::is_moveable() {       \
        return true;                                        \
    }                                                       \
    template <> bool type_info<const type>::is_moveable() { \
        return true;                                        \
    }

// allowing for all standard types
WQ_MOVEABLE_TYPE(char);
WQ_MOVEABLE_TYPE(signed char);
WQ_MOVEABLE_TYPE(unsigned char);

WQ_MOVEABLE_TYPE(wchar_t);

WQ_MOVEABLE_TYPE(bool);

WQ_MOVEABLE_TYPE(short);
WQ_MOVEABLE_TYPE(unsigned short);

WQ_MOVEABLE_TYPE(int);
WQ_MOVEABLE_TYPE(unsigned int);

WQ_MOVEABLE_TYPE(long);
WQ_MOVEABLE_TYPE(unsigned long);

WQ_MOVEABLE_TYPE(long long);
WQ_MOVEABLE_TYPE(unsigned long long);

WQ_MOVEABLE_TYPE(float);
WQ_MOVEABLE_TYPE(double);
WQ_MOVEABLE_TYPE(long double);

}  // namespace core
}  // namespace wq


#endif  // WQ_TYPE_INFO_H
