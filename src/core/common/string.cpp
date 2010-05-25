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

#include "wq/core/string.h"
#include "wq/core/encoder.h"

#include <cstring>

namespace wq {
namespace core {

// string::value_type class
/*!
	\brief Basic construction.

	This constructor constructs "normal object" which holds
	utf8 character. If \a c is not good encoded exception
	wq::core::encode_error is thrown.

	\param c Pointer to sequence of bytes of utf8 encoded character.
*/
string::value_type::value_type(const char* c) : m_ptr(NULL), m_owner(NULL) {
	if(string::octets_count(c[0]) != 0) {
		allocator_type alloc;
		size_type clen = strlen(c);
		m_ptr = alloc.allocate(clen);
		alloc.copy(c, clen, m_ptr);
	}
	else {
		throw encode_error();
	}
}

/*!
	\brief Construction with owner.

	This constructor constructs "reference object". This object
	refers to character in the string class. If \a c is not good
	encoded exception wq::core::encode_error is thrown.

	\param c Pointer to starting byte of character which has to be
	in string's range.
	\param owner Pointer to string object which owns \a c.
*/
string::value_type::value_type(char* c, string* owner) : m_ptr(c), m_owner(owner) {
	if(string::octets_count(c[0]) == 0) {
		throw encode_error();
	}
}

/*!
	\brief Destroy object.

	Destructor suppose to destroy object. It has to
	distinguish "reference object" and "normal object".
	First one is object that refer to character in existing
	wq::core::string class. An other one is object that just
	hold utf8 character and can be used to change characters
	in wq::core::string by '=' operator.
*/
string::value_type::~value_type() {
	if(m_owner == NULL) {
		allocator_type alloc;
		alloc.deallocate(m_ptr);
	}
}

// string::shared_data class
string::shared_data::shared_data(const shared_data& from) :
		m_start(NULL), m_last(NULL), m_end(NULL), m_len(0) {
	size_type size = from.m_last - from.m_start;
	char* start = m_alloc.allocate(size);
	m_alloc.copy(from.m_start, size, start);

	m_start = start;
	m_last = m_start + size;
	m_end = m_start + size;
	m_len = from.m_len;
}

string::shared_data::~shared_data() {
	if(m_start != NULL && m_last != NULL) {
		// it is not necessary to call destroy function for objects
		m_alloc.deallocate(m_start);
	}
}

// string class
/*!
	\brief Constructs string.

	This constructor constructs empty string.
*/
string::string() : s_ptr(new shared_data) {

}

/*!
	\brief Constructs string.

	Constructor constructs string with the given context.
*/
string::string(const char* str) : s_ptr(new shared_data) {
	lowl_append(str);
}

/*!
	\brief Copy constructor.

	Constructs string copying the context of other string.
	However constructor will perform shallow copy of string
	and later, if it is necessary, deep copy will be performed.
*/
string::string(const string& other) : s_ptr( other.s_ptr ) {

}

/*!
	\brief Clear string.

	This function deletes contents of string and deallocates
	all allocated memory.
*/
void string::clear() {
	if(size() != 0) {
		s_ptr.set_data(new shared_data);
	}
}

// private functions
// function will return number of octets of utf8 character 'c'
string::size_type string::octets_count(char c) {
	/*
	   octets specifications:
	   0000 0000-0000 007F | 0xxxxxxx
	   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
	   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
	   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	if( !(c & (1 << 7)) ) {
		return 1;
	}
	else if( c & (1 << 7) && c & (1 << 6) && !(c & (1 << 5)) ) {
		return 2;
	}
	else if( c & (1 << 7) && c & (1 << 6) && c & (1 << 5) && !(c & (1 << 4)) ) {
		return 3;
	}
	else if( c & (1 << 7) && c & (1 << 6) && c & (1 << 5) && c & (1 << 4) && !(c & (1 << 5)) ) {
		return 4;
	}
	return 0;
}

// function will return number of chars of 'str'
string::size_type string::chars_count(const char* str, size_type size) {
	if(size == -1) {
		size = strlen(str);
	}
	size_type ret_val = 0;
	size_type i = 0;
	while(i < size) {
		ret_val++;
		i += octets_count(str[i]);
	}
	return ret_val;
}

// this function appends new utf8 chars to string
void string::lowl_append(const char* str, size_type str_size) {
	if(str_size == -1) {
		str_size = strlen(str);
	}
	size_type old_size = s()->m_last - s()->m_start;
	size_type new_size = old_size == 0 ? str_size : (old_size * 2 >= old_size + str_size ?
													 old_size * 2 : old_size + str_size);

	s()->m_start = s()->m_alloc.reallocate(s()->m_start, old_size, new_size);
	s()->m_end = s()->m_start + new_size;
	s()->m_last = s()->m_start + old_size;
	s()->m_last = s()->m_alloc.copy(str, str_size, s()->m_last);

	// adding to len of string
	s()->m_len += chars_count(str, str_size);
}

// this function delete string contents and assign new one
void string::lowl_assign(const char* str, size_type str_size) {
	clear();
	lowl_append(str, str_size);
}

// this function inserts 'str' at 'at' position (moving all chars after 'at' (including 'at') to right)
void string::lowl_insert(char* at, const char* str, size_type str_size) {
	if(str_size == -1) {
		str_size = strlen(str);
	}
	size_type old_size = s()->m_last - s()->m_start;
	size_type new_size = old_size == 0 ? str_size : (old_size * 2 >= old_size + str_size ?
													 old_size * 2 : old_size + str_size);
	// reallocating memory to assure enough space + assure validity of 'at'
	difference_type dist = at - s()->m_start;
	s()->m_start = s()->m_alloc.reallocate(s()->m_start, old_size, new_size);
	s()->m_end = s()->m_start + new_size;
	s()->m_last = s()->m_start + old_size;
	at = s()->m_start + dist;

	// copying data to right / TODO: add function for moving to right to allocator
	s()->m_last = s()->m_alloc.ocopy(at, s()->m_last - at, at + str_size);

	// copying new data to right place
	s()->m_alloc.copy(str, str_size, at);

	// we should change length variable
	s()->m_len += chars_count(str, str_size);
}

}  // namespace core
}  // namespace wq
