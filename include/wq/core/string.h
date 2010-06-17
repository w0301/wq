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
				value_type(const char*, size_type = -1);
				value_type(char);
				value_type(string*, char*);
				value_type(const string*, char*);

				// destruction
				~value_type();

				// count of bytes of character
				size_type bytes() const {
					return m_owner == NULL ? strlen(m_ptr) : owner()->octets_count(m_ptr);
				};

				// index if m_ptr in string's array
				size_type ptr_index() const {
					return m_owner == NULL ? 0 : m_ptr - owner()->s()->m_start;
				};

				// return m_ptr as const pointer
				const char* ptr() const {
					return m_ptr;
				};

                // constant returning of m_owner
                const string* owner() const {
                    return m_owner;
                };

				// return true if no character is assigned
				bool is_null() const {
					return m_ptr == NULL;
				};

				// return if char is first/last in string
				bool is_first() const {
					return m_owner == NULL ? true : owner()->s()->m_start == m_ptr;
				};
				bool is_last() const {
					return m_owner == NULL ? true : owner()->s()->m_last == m_ptr;
				};

				// next/previous character in string
				value_type next() const;
				value_type prev() const;

				// assignment
				value_type& operator= (const value_type&);
				value_type& operator= (const char*);
				value_type& operator= (char);

                // change value without replacing in string
                value_type& rebind(const value_type&);

				// comparing
				bool operator== (const value_type&) const;
				bool operator== (const char*) const;
				bool operator== (char) const;
				bool operator!= (const value_type& r) const {
					return !((*this) == r);
				};
				bool operator!= (const char* r) const {
					return !(*this == r);
				};
				bool operator!= (char r) const {
					return !(*this == r);
				};

				// conversion
				const char* utf8() const;
				char ch() const;
				operator const char*() const {
					return utf8();
				};

			private:
				friend class string;
				string* owner() {
					return m_owner;
				};
				char* ptr() {
				    return m_ptr;
				};


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

		class const_iterator;
		//! Class which represent special iterator.
		class iterator {
			public:
				typedef string::value_type value_type;
				typedef string::reference reference;
				typedef reference* pointer;
				typedef string::difference_type difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;

				// creation and copying
				iterator() { };
				iterator(const value_type& val) : m_val(val) { };
				iterator(const iterator& from) : m_val(from.m_val) { };
				iterator& operator= (const iterator& r) {
					if(&r != this) {
						// we have to use rebind here!
						m_val.rebind(r.m_val);
					}
					return *this;
				};

				// converting
				reference* operator-> () {
					return &m_val;
				};
				const_reference* operator-> () const {
					return &m_val;
				};
				reference operator* () {
					return m_val;
				};
				const_reference operator* () const {
					return m_val;
				};

				// comparing
				bool operator== (const iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				bool operator== (const const_iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const const_iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const const_iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const const_iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const const_iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const const_iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				// distance of iterators
				difference_type operator- (const iterator& r) const {
					return m_val.ptr() - r.m_val.ptr();
				};
				difference_type operator- (const const_iterator& r) const {
					return m_val.ptr() - r.m_val.ptr();
				};

				// incrementing and decrementing
				// all this operations are const!
				iterator operator+ (size_type) const;
				iterator operator- (size_type) const;
				iterator& operator+= (size_type n) {
					return ( (*this) = ((*this) + n) );
				};
				iterator& operator-= (size_type n) {
					return ( (*this) = ((*this) - n) );
				};
				iterator& operator++ () {
					return ( (*this) += 1 );
				};
				iterator& operator-- () {
					return ( (*this) -= 1 );
				};
				iterator& operator++ (int) {
					return ( (*this) += 1 );
				};
				iterator& operator-- (int) {
					return ( (*this) -= 1 );
				};

			private:
				value_type m_val;
				friend class const_iterator;
		};

		//! Class which represent special constant iterator.
		class const_iterator {
			public:
				typedef string::value_type value_type;
				typedef string::reference reference;
				typedef string::value_type* pointer;
				typedef string::difference_type difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;

				// creation and copying
				const_iterator() { };
				const_iterator(const value_type& val) : m_val(val) { };
				const_iterator(const const_iterator& from) : m_val(from.m_val) { };
				const_iterator& operator= (const const_iterator& r) {
					if(&r != this) {
						// we have to use rebind here!
						m_val.rebind(r.m_val);
					}
					return *this;
				};
				const_iterator(const iterator& from) : m_val(from.m_val) { };
				const_iterator& operator= (const iterator& r) {
					// we have to use rebind here!
					m_val.rebind(r.m_val);
					return *this;
				};

				// converting
				const_reference* operator-> () const {
					return &m_val;
				};
				const_reference operator* () const {
					return m_val;
				};

				// comparing
				bool operator== (const iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				bool operator== (const const_iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const const_iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const const_iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const const_iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const const_iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const const_iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				// distance of iterators
				difference_type operator- (const iterator& r) const {
					return m_val.ptr() - r.m_val.ptr();
				};
				difference_type operator- (const const_iterator& r) const {
					return m_val.ptr() - r.m_val.ptr();
				};

				// incrementing and decrementing
				// all this operations are const!
				const_iterator operator+ (size_type) const;
				const_iterator operator- (size_type) const;
				const_iterator& operator+= (size_type n) {
					return ( (*this) = ((*this) + n) );
				};
				const_iterator& operator-= (size_type n) {
					return ( (*this) = ((*this) - n) );
				};
				const_iterator& operator++ () {
					return ( (*this) += 1 );
				};
				const_iterator& operator-- () {
					return ( (*this) -= 1 );
				};
				const_iterator& operator++ (int) {
					return ( (*this) += 1 );
				};
				const_iterator& operator-- (int) {
					return ( (*this) -= 1 );
				};

			private:
				value_type m_val;
				friend class iterator;
		};

		//! Reverse iterator type.
		typedef std::reverse_iterator<iterator> reverse_iterator;

		//! Constant reverse iterator type.
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		// construction
		string();
		string(const char*, size_type = -1);
		string(const string&);
		string(size_type, const_reference);

		// destruction
		~string();

	    // assignment operator
		string& operator= (const string&);

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

		// iterators
		iterator begin() {
			return iterator( value_type(this, cs()->m_start) );
		};
		iterator end() {
			return iterator( value_type(this, cs()->m_last) );
		};
		const_iterator begin() const {
			return const_iterator( value_type(this, s()->m_start) );
		};
		const_iterator end() const {
			return const_iterator( value_type(this, s()->m_last) );
		};

		// reverse iterators
		reverse_iterator rbegin() {
			return reverse_iterator( end() );
		};
		reverse_iterator rend() {
			return reverse_iterator( begin() );
		};
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator( end() );
		};
		const_reverse_iterator rend() const {
			return const_reverse_iterator( begin() );
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
		string& assign(const string&);
		string& assign(const string&, size_type, size_type);
		string& assign(const char*, size_type = -1);
		string& assign(size_type, const_reference);

		// appending
		string& append(const string&);
		string& append(const string&, size_type, size_type);
		string& append(const char*, size_type = -1);
		string& append(size_type, const_reference);

		// inserting
		string& insert(size_type, const string&);
		string& insert(size_type, const string&, size_type, size_type);
		string& insert(size_type, const char*, size_type = -1);
		string& insert(size_type, const_reference);
		string& insert(size_type, size_type, const_reference);
		iterator insert(iterator, const string&);
		iterator insert(iterator, const char*, size_type = -1);
		iterator insert(iterator, const_reference);
		iterator insert(iterator, size_type, const_reference);

		// erasing
		string& erase(size_type = 0, size_type = -1);
		iterator erase(iterator);
		iterator erase(iterator, iterator);

		// replacing - TODO later

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
		size_type bytes_left(const char* from) const {
			return s()->m_last - from;
		};
		static size_type octets_count(const char*, size_type);
		size_type octets_count(const char* c) const {
			return octets_count(c, bytes_left(c));
		};
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
