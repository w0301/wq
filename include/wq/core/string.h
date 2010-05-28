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
		// class which points to one character of utf8 string
		class value_type {
			public:
				value_type() : m_ptr(NULL), m_owner(NULL) { };
				value_type(const char*);
				value_type(char*, string*);

				~value_type();

			private:
				// pointer to first byte of utf8 character
				char* m_ptr;

				// pointer to owner of character above
				string* m_owner;
		};
		friend class value_type;

		typedef value_type reference;
		typedef const reference const_reference;
		typedef char* iterator;
		typedef const char* const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef wq::core::allocator<char> allocator_type;
		typedef allocator_type::size_type size_type;
		typedef allocator_type::difference_type difference_type;

		// construction
		string();
		string(const char*);
		string(const string&);

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

		// converting - TODO: need of temporally buffer - '\0' is needed
		const char* c_str() const {
			return s()->m_start == NULL ? "" : s()->m_start;
		};
		const char* utf8_str() const {
			return s()->m_start == NULL ? "" : s()->m_start;
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
				allocator_type m_alloc;
		};

		// some private helpful functions
		static size_type octets_count(char);
		static size_type chars_count(const char*, size_type = -1);
		void lowl_realloc(size_type);
		void lowl_append(const char*, size_type = -1);
		void lowl_append(char, size_type);
		void lowl_assign(const char*, size_type = -1);
		void lowl_insert(char*, const char*, size_type = -1);
		void lowl_insert(char*, char, size_type);
		void lowl_erase(char*, char*);
		void lowl_replace(char*, char*, const char*, size_type = -1);

	protected:
		WQ_NEW_SHARED_DATA();
};


}  // namespace core
}  // namespace wq


#endif  // WQ_STRING_H
