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

#ifndef WQ_STRING_H
#define WQ_STRING_H

#include "wq/defs.h"
#include "wq/core/allocator.h"
#include "wq/core/shared_ptr.h"

#include <iterator>

#if WQ_STD_COMPATIBILITY
	#include <string>
#endif

namespace wq {
namespace core {

// class for handling all strings in wq, with unicode support of course
class WQ_EXPORT string {
	public:
		//! Type of allocator used for string objects.
		typedef wq::core::allocator<char> allocator_type;

		//! Type which handle indexes etc. in string objects.
		typedef allocator_type::size_type size_type;

		//! Type which describe result of math operations over iterators.
		typedef allocator_type::difference_type difference_type;

		// class which points to one character of utf8 string
		class value_type {
			public:
				// construction
				value_type() : m_ptr(NULL), m_owner(NULL), m_tempbuff(NULL) { };
				value_type(const value_type&);
				value_type(const char*);
				value_type(string*, char*);
				value_type(const string*, char*);

				// destruction
				~value_type();

				// count of bytes of character
				size_type bytes() const {
					return m_owner == NULL ? strlen(m_ptr) : octets_count(m_ptr, m_owner->s()->m_last - m_ptr);
				};

				// next character in string
				value_type next() const;

				// assignment
				value_type& operator= (const value_type&);
				value_type& operator= (const char*);
				value_type& operator= (char);

				// comparing
				bool operator== (const value_type&) const;
				bool operator== (const char*) const;
				bool operator== (char) const;
				bool operator!= (const value_type& r) const {
					return !(*this == r);
				};
				bool operator!= (const char* r) const {
					return !(*this == r);
				};
				bool operator!= (char r) const {
					return !(*this == r);
				};

				// conversion
				const char* c_str() const;
				char ch() const;
				operator char() const {
					return ch();
				};

			private:
				// pointer to first byte of utf8 character
				char* m_ptr;

				// pointer to owner of character above
				string* m_owner;

				// temporary buffer for convert functions
				mutable char* m_tempbuff;
		};
		friend class value_type;

		//! Reference type for class.
		typedef value_type reference;

		//! Constant reference type for class.
		typedef const reference const_reference;

		//! Iterator type.
		typedef char* iterator;

		//! Constant iterator type.
		typedef const char* const_iterator;

		//! Reverse iterator type.
		typedef std::reverse_iterator<iterator> reverse_iterator;

		//! Constant reverse iterator type.
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		// construction
		string();
		string(const char*);
		string(const string&);

		// destruction
		~string();

		// manipulating with contents
		void clear();

		// size, capacity etc.
		size_type size() const {
			return s()->m_len;
		};
		size_type length() const {
			return size();
		};
		size_type capacity() const {
			return s()->m_end - s()->m_start;
		};
		size_type bytes() const {
			return s()->m_last - s()->m_start;
		};
		size_type max_size() const {
			return s()->m_alloc.max_size();
		};
		bool empty() const {
			return size() == 0;
		};

		// characters returning
		reference at(size_type);
		const_reference at(size_type) const;
		reference operator[] (size_type i) {
			return at(i);
		};
		const_reference operator[] (size_type i) const {
			return at(i);
		};

		// assigning

		// appending

		// inserting

		// erasing

		// replacing

		// converting
		const char* utf8_str() const;
		const char* c_str() const {
			return utf8_str();
		};

	private:
		class shared_data {
			public:
				shared_data() : m_start(NULL), m_last(NULL), m_end(NULL), m_len(0) { };
				shared_data(const shared_data&);
				~shared_data();

				char* m_start;
				char* m_last;
				char* m_end;
				size_type m_len;
				mutable allocator_type m_alloc;
		};

		// some private helpful functions
		static size_type octets_count(const char*, size_type);
		static size_type chars_count(const char*, size_type = -1);

		// low level functions - these functions do not touch
		// 's()->m_len' and if they return they return count
		// of characters inserted/deleted or new count - depends on kind of functions
		void lowl_realloc(size_type);
		size_type lowl_append(const char*, size_type = -1);
		size_type lowl_assign(const char*, size_type = -1);
		size_type lowl_insert(char*, const char*, size_type = -1);
		size_type lowl_erase(char*, char*);
		size_type lowl_replace(char*, char*, const char*, size_type = -1);

		// temp buffer for *_str functions
		mutable char* m_tempbuff;

	protected:
		WQ_NEW_SHARED_DATA();
};


}  // namespace core
}  // namespace wq


#endif  // WQ_STRING_H
