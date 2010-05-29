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

	\param c Pointer to first byte of utf8 encoded character sequence.
*/
string::value_type::value_type(const char* c) : m_ptr(NULL), m_owner(NULL) {
	if(string::octets_count(c[0]) != 0) {
		allocator_type alloc;
		size_type clen = strlen(c);
		m_ptr = alloc.allocate(clen);
		alloc.copy(m_ptr, c, clen);
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
	m_alloc.copy(start, from.m_start, size);

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
string::string() : m_tempbuff(NULL), s_ptr(new shared_data) {

}

/*!
	\brief Constructs string.

	Constructor constructs string with the given context.
*/
string::string(const char* str) : m_tempbuff(NULL), s_ptr(new shared_data) {
	s()->m_len += lowl_append(str);
}

/*!
	\brief Copy constructor.

	Constructs string copying the context of other string.
	However constructor will perform shallow copy of string
	and later, if it is necessary, deep copy will be performed.
*/
string::string(const string& other) : m_tempbuff(NULL), s_ptr(other.s_ptr) {

}

string::~string() {
	if(m_tempbuff != NULL) {
		s()->m_alloc.deallocate(m_tempbuff);
	}
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

const char* string::utf8_str() const {
	if(m_tempbuff != NULL) {
		s()->m_alloc.deallocate(m_tempbuff);
	}
	m_tempbuff = s()->m_alloc.allocate(bytes() + 1);
	s()->m_alloc.copy(m_tempbuff, s()->m_start, bytes());
	s()->m_alloc.construct(m_tempbuff + bytes(), '\0');

	return m_tempbuff;
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

// this function realloc string by realloc algorithm + ensure 'size' bytes are free
void string::lowl_realloc(size_type size) {
	// resize only if it is needed
	size_type old_capacity = s()->m_end - s()->m_start;
	size_type old_size = s()->m_last - s()->m_start;
	if(old_capacity < old_size + size) {
		size_type new_capacity = old_capacity == 0 ? size :
								(old_capacity * 2 >= old_size + size ?
								 old_capacity * 2 : old_size + size);
		s()->m_start = s()->m_alloc.reallocate(s()->m_start, old_capacity, new_capacity);
		s()->m_end = s()->m_start + new_capacity;
		s()->m_last = s()->m_start + old_size;
	}
}

// this function appends new utf8 chars to string
string::size_type string::lowl_append(const char* str, size_type str_size) {
	if(str_size == -1) {
		str_size = strlen(str);
	}
	lowl_realloc(str_size);
	s()->m_last = s()->m_alloc.copy(s()->m_last, str, str_size);

	// returning length of inserted string
	return chars_count(str, str_size);
}

// this function delete string contents and assign the new one
string::size_type string::lowl_assign(const char* str, size_type str_size) {
	clear();
	return lowl_append(str, str_size);
}

// this function inserts 'str' at 'at' position (moving all chars after 'at' (including 'at') to right)
string::size_type string::lowl_insert(char* at, const char* str, size_type str_size) {
	if(str_size == -1) {
		str_size = strlen(str);
	}

	// appending 'str_size' zero characters + assure validity of 'at'
	difference_type dist = at - s()->m_start;
	lowl_realloc(str_size);
	at = s()->m_start + dist;

	// copying data to right
	s()->m_last = s()->m_alloc.ocopy(at + str_size, at, s()->m_last - at);

	// copying new data to right place
	s()->m_alloc.copy(at, str, str_size);

	// returning length
	return chars_count(str, str_size);
}

// delete characters between two pointers that points to bytes in string
string::size_type string::lowl_erase(char* first, char* last) {
	if(first == NULL || last == NULL) {
		return 0;
	}
	// rewriting deleted data
	s()->m_last = s()->m_alloc.ocopy(first, last, s()->m_last - last);

	// erased length
	return chars_count(first, last - first - 1);
}

// replace bytes between 'first' and 'last' with str
string::size_type string::lowl_replace(char* first, char* last, const char* str, size_type str_size) {
	if(str_size == -1) {
		str_size = strlen(str);
	}
	size_type rep_size = last - first;

	// first length that will be erased
	size_type ret = chars_count(str, str_size) - chars_count(first, last - first);

	// the most obvious way to do that - I am lazy to make it shorter :)
	if(rep_size == str_size) {
		s()->m_alloc.copy(first, str, rep_size);
	}
	else if(rep_size < str_size) {
		s()->m_alloc.copy(first, str, rep_size);
		lowl_insert(first + rep_size, str + rep_size, str_size - rep_size);
	}
	else if(rep_size > str_size) {
		first = s()->m_alloc.copy(first, str, str_size);
		lowl_erase(first, first + (rep_size - str_size));
	}
	return ret;
}

}  // namespace core
}  // namespace wq
