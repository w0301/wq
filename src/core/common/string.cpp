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

#include <cstring>

namespace wq {
namespace core {

// string::shared_data class
string::shared_data::shared_data(const shared_data& from) :
		m_start(NULL), m_last(NULL), m_end(NULL), m_len(0) {
	allocator_type::size_type size = from.m_last - from.m_start;
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
	append(str);
}


// private functions
// function will return number of octets of character at 'c'
string::size_type string::octets_count(char c) {
	/*
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
	return 4;
}

// function will return number of chars of str
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

// this function will append new utf8 chars to string
void string::append(const char* str, size_type str_size) {
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

}  // namespace core
}  // namespace wq
