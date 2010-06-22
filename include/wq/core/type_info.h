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

#include <typeinfo>

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
		static bool is_movable() {
			return false;
		};
		static const std::type_info& type_id() {
			return typeid(value_type);
		};
};

// all pointers are movable
template<class T> class type_info<T*> {
	public:
		typedef T* value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;

		static uint size() {
			return sizeof(value_type);
		};
		static bool is_movable() {
			return true;
		};
		static const std::type_info& type_id() {
			return typeid(value_type);
		};
};

// references are special
template<class T> class type_info<T&> {
	public:
		typedef T& value_type;

		static uint size() {
			return sizeof(value_type);
		};
		static bool is_movable() {
			return false;
		};
		static const std::type_info& type_id() {
			return typeid(value_type);
		};
};

// special declaration for void
template<> class type_info<void> {
	public:
		typedef void value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		static uint size() {
			return 0;
		};
		static bool is_movable() {
			return false;
		};
		static const std::type_info& type_id() {
			return typeid(value_type);
		};
};

// allowing moving for types by this macro
#define WQ_MOVABLE_TYPE(type)                             \
	namespace wq {	\
	namespace core {	\
	template<> class type_info<type> {		\
		public:		\
			typedef type value_type;	\
			typedef value_type* pointer;	\
			typedef const value_type* const_pointer;	\
			typedef value_type& reference;	\
			typedef const value_type& const_reference;	\
			static uint size() { return sizeof(value_type); };	\
			static bool is_movable() {	return true; };	\
			static const std::type_info& type_id() { return typeid(value_type); };	\
	};	}	}

#define WQ_MOVABLE_TEMPLATE_TYPE_1(type)                             \
    namespace wq {  \
    namespace core {    \
    template<class T> class type_info< type<T> > {      \
        public:     \
            typedef type<T> value_type;    \
            typedef value_type* pointer;    \
            typedef const value_type* const_pointer;    \
            typedef value_type& reference;  \
            typedef const value_type& const_reference;  \
            static uint size() { return sizeof(value_type); };  \
            static bool is_movable() { return true; }; \
            static const std::type_info& type_id() { return typeid(value_type); };  \
    };  }   }

}  // namespace core
}  // namespace wq

// moving is allowed for all standard types
WQ_MOVABLE_TYPE(char);
WQ_MOVABLE_TYPE(signed char);
WQ_MOVABLE_TYPE(unsigned char);

WQ_MOVABLE_TYPE(wchar_t);

WQ_MOVABLE_TYPE(bool);

WQ_MOVABLE_TYPE(signed short);
WQ_MOVABLE_TYPE(unsigned short);

WQ_MOVABLE_TYPE(signed int);
WQ_MOVABLE_TYPE(unsigned int);

WQ_MOVABLE_TYPE(signed long);
WQ_MOVABLE_TYPE(unsigned long);

WQ_MOVABLE_TYPE(signed long long);
WQ_MOVABLE_TYPE(unsigned long long);

WQ_MOVABLE_TYPE(float);
WQ_MOVABLE_TYPE(double);
WQ_MOVABLE_TYPE(long double);

#endif  // WQ_TYPE_INFO_H
