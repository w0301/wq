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
	\class string::value_type
	\brief UTF-8 character handler.

	This class is used to point to existing character
	in any wq::core::string object ("reference object") or
	to just keep UTF-8 formated character ("normal object").

	\sa string
*/

/*!
	\fn string::value_type::value_type()
	\brief Default constructor.

	Creates empty object that must be initialized
	by assign operators.

	\sa operator=()
*/

/*!
	\brief Copy constructor.

	This constructor creates exact copy of \a from object.

	\sa value_type(), operator=(const value_type&)
*/
string::value_type::value_type(const value_type& from) :
		m_ptr(NULL), m_owner(from.m_owner), m_tempbuff(NULL) {
	if(m_owner != NULL) {
		m_ptr = from.m_ptr;
	}
	else if(!from.is_null()) {
		allocator_type alloc;
		size_type clen = from.bytes();
		m_ptr = alloc.allocate(clen + 1);
		alloc.copy(m_ptr, from.m_ptr, clen);
		alloc.construct(m_ptr + clen, '\0');
	}
}

/*!
	\brief Basic construction.

	This constructor constructs "normal object" which holds
	utf8 character. If \a c is not good encoded exception
	wq::core::encode_error is thrown.

	\param c Pointer to first byte of utf8 encoded character sequence.
	\param clen Length of character in bytes, if is \b -1 it is automatically determinate.
*/
string::value_type::value_type(const char* c, size_type clen) : m_ptr(NULL), m_owner(NULL), m_tempbuff(NULL) {
	if(c == NULL) {
		return;
	}
	if(clen == -1) {
		clen = strlen(c);
	}
	if(string::octets_count(c, clen) != 0) {
		allocator_type alloc;
		m_ptr = alloc.allocate(clen + 1);
		alloc.copy(m_ptr, c, clen);
		alloc.construct(m_ptr + clen, '\0');
	}
	else {
		throw encode_error();
	}
}

string::value_type::value_type(char c) : m_ptr(NULL), m_owner(NULL), m_tempbuff(NULL) {
	allocator_type alloc;
	m_ptr = alloc.allocate(2);
	alloc.construct(m_ptr, c);
	alloc.construct(m_ptr + 1, '\0');
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
string::value_type::value_type(string* owner, char* c) : m_ptr(c), m_owner(owner), m_tempbuff(NULL) {
	if(bytes() == 0) {
		throw encode_error();
	}
}

/*!
	\brief  Construction with constant owner.

	Just like value_type(string*, char*) but with 'const_cast' used.

	\sa value_type(string*, char*)
*/
string::value_type::value_type(const string* owner, char* c) :
		m_ptr(c), m_owner(const_cast<string*>(owner)), m_tempbuff(NULL) {
	if(bytes() == 0) {
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
	allocator_type alloc;
	if(m_owner == NULL && m_ptr != NULL) {
		alloc.deallocate(m_ptr);
	}
	if(m_tempbuff != NULL) {
		alloc.deallocate(m_tempbuff);
	}
}

/*!
	\fn string::value_type::bytes() const
	\brief Length of character.

	Function returns number of bytes in character
	handled by object.
*/

/*!
	\brief Next character.

	Function returns next character in string. If object
	is not "reference object" or there is no any other
	character default object is returned - is_null() returns true.
*/
string::value_type string::value_type::next() const {
	if(owner() == NULL) {
		return value_type();
	}
	if(m_ptr + bytes() < owner()->s()->m_last) {
		return value_type(m_owner, m_ptr + bytes());
	}
	return value_type(m_owner, owner()->s()->m_last);
}

string::value_type string::value_type::prev() const {
	if(owner() == NULL) {
		return value_type();
	}

	// finding previous character is more complex :(
	value_type ret = value_type(m_owner, m_ptr);
	size_type clen = 0;
	while(1) {
		clen++;
		ret.m_ptr--;
		if(ret.bytes() != 0) {
			break;
		}
	}
	if(m_ptr - clen > owner()->s()->m_start) {
		return value_type(m_owner, m_ptr - clen);
	}
	return value_type(m_owner, owner()->s()->m_start);
}

/*!
	\brief Assign operator.

	This operator copy \a r object to \a this object.

	\sa operator= (const char*)
*/
string::value_type& string::value_type::operator= (const value_type& r) {
	if(this != &r) {
		if(m_owner == NULL) {
            m_owner = r.m_owner;
            if(m_owner == NULL) {
                allocator_type alloc;
                if(!is_null()) {
                    alloc.deallocate(m_ptr);
                    m_ptr = NULL;
                }
                size_type clen = r.bytes();
                m_ptr = alloc.allocate(clen + 1);
                alloc.copy(m_ptr, r.m_ptr, clen);
                alloc.construct(m_ptr + clen, '\0');
            }
            else {
                m_ptr = r.m_ptr;
            }
		}
		else {
			// replacing + assure m_ptr validity
            difference_type dist = m_ptr - m_owner->s()->m_start;
            m_owner->s()->m_len += m_owner->lowl_replace(m_ptr, m_ptr + bytes(), r.m_ptr, r.bytes());
            m_ptr = m_owner->s()->m_start + dist;
		}
	}
	return *this;
}

/*!
	\brief Assign operator.

	This assign operator assign \a c character to \a this object.
	If \a this object is of "reference type" string's character is also
	changed.
*/
string::value_type& string::value_type::operator= (const char* c) {
	if(m_owner == NULL) {
		allocator_type alloc;
		if(m_ptr != NULL) {
			alloc.deallocate(m_ptr);
		}
		size_type clen = strlen(c);
		if(string::octets_count(c, clen) != 0) {
			m_ptr = alloc.allocate(clen + 1);
			alloc.copy(m_ptr, c, clen);
			alloc.construct(m_ptr + clen, '\0');
		}
		else {
			throw encode_error();
		}
	}
	else {
		// replacing + assure m_ptr validity
		difference_type dist = m_ptr - m_owner->s()->m_start;
		m_owner->s()->m_len += m_owner->lowl_replace(m_ptr, m_ptr + bytes(), c);
		m_ptr = m_owner->s()->m_start + dist;
	}
	return *this;
}

/*!
	\brief Assign operator.

	Assign character \a c as content of object.
*/
string::value_type& string::value_type::operator= (char c) {
	if(m_owner == NULL) {
		allocator_type alloc;
		if(m_ptr != NULL) {
			alloc.deallocate(m_ptr);
		}
		m_ptr = alloc.allocate(2);
		alloc.construct(m_ptr, c);
		alloc.construct(m_ptr + 1, '\0');
	}
	else {
		// replacing + assure m_ptr validity
		difference_type dist = m_ptr - m_owner->s()->m_start;
		m_owner->s()->m_len += m_owner->lowl_replace(m_ptr, m_ptr + 1, &c, 1);
		m_ptr = m_owner->s()->m_start + dist;
	}
	return *this;
}

string::value_type& string::value_type::rebind(const value_type& n) {
    if(owner() != NULL) {
        m_owner = n.m_owner;
        m_ptr = n.m_ptr;
        return *this;
    }
    return (*this) = n;
}

/*!
	\brief Compare operator.

	Compare character in \a this object with character
	in \a r object.

	\return Returns \b true when characters are identical.
*/
bool string::value_type::operator== (const value_type& r) const {
    if(m_ptr == r.m_ptr) {
        return true;
    }
    else if(m_ptr == NULL || r.m_ptr == NULL) {
         return false;
    }
	else if(this == &r) {
		return true;
	}
	size_type this_size = bytes();
	size_type r_size = r.bytes();
	if(this_size != r_size) {
		return false;
	}

	for(size_type i = 0; i != r_size; i++) {
		if(m_ptr[i] != r.m_ptr[i]) {
			return false;
		}
	}
	return true;
}

/*!
	\brief Compare operator.

	Compare character in \this object with character describes
	in \a r string.

	\return Returns \b true when characters are identical.
*/
bool string::value_type::operator== (const char* r) const {
	if(m_ptr == NULL || r == NULL) {
		return false;
	}
	size_type this_size = bytes();
	size_type r_size = strlen(r);
	if(this_size != r_size) {
		return false;
	}

	for(size_type i = 0; i != r_size; i++) {
		if(m_ptr[i] != r[i]) {
			return false;
		}
	}
	return true;
}

/*!
	\brief Compare operator.

	Compare character in \this object with character \a c.

	\return Returns \b true when characters are identical.
*/
bool string::value_type::operator== (char r) const {
	if(bytes() == 1 && *m_ptr == r) {
		return true;
	}
	return false;
}

/*!
	\fn bool string::value_type::operator!= (const value_type&) const
	\brief Compare operator.

	Convince operator - opposite of operator==.
*/

/*!
	\fn bool string::value_type::operator!= (const char*) const
	\brief Compare operator.

	Convince operator - opposite of operator==.
*/

/*!
	\fn bool string::value_type::operator!= (char) const
	\brief Compare operator.

	Convince operator - opposite of operator==.
*/

/*!
	\brief Converts to C string.

	This function converts character to sequence
	of char-s (bytes) and return a pointer to sequence.

	\sa operator const char*()
*/
const char* string::value_type::utf8() const {
	if(m_owner != NULL) {
		allocator_type alloc;
		if(m_tempbuff != NULL) {
			alloc.deallocate(m_tempbuff);
		}
		size_type clen = bytes();
		m_tempbuff = alloc.allocate(clen + 1);
		alloc.copy(m_tempbuff, m_ptr, clen);
		alloc.construct(m_tempbuff + clen, '\0');

		return m_tempbuff;
	}
	return m_ptr;
}

/*!
	\brief Conversion to char.

	This functions convert character to
	\a char type, however if character has more
	than 1 byte character \b '?' is returned and no
	exception is thrown. To handle all unicode characters
	use utf8() function.

	\sa utf8()
*/
char string::value_type::ch() const {
	if(bytes() == 1) {
		return *m_ptr;
	}
	return '?';
}

/*!
	\fn string::value_type::operator char() const
	\brief Implicit conversion.

	This operator allows implicit conversion to
	char type by calling ch() function.

	\sa ch()
*/

// string::iterator class
string::iterator string::iterator::operator+ (size_type n) const {
	iterator ret = *this;
	for( ; n != 0; n--) {
		ret.m_val.rebind(ret.m_val.next());
		// only first increment to last char is allowed!
		if(ret.m_val.is_last() && n > 1) {
			throw range_error();
		}
	}
	return ret;
}

string::iterator string::iterator::operator- (size_type n) const {
	iterator ret = *this;
	for( ; n != 0; n--) {
		ret.m_val.rebind(ret.m_val.prev());
		// only first decrement to first char is allowed!
		if(ret.m_val.is_first() && n > 1) {
			throw range_error();
		}
	}
	return ret;
}

// string::const_iterator class
string::const_iterator string::const_iterator::operator+ (size_type n) const {
	const_iterator ret = *this;
	for( ; n != 0; n--) {
		ret.m_val.rebind(ret.m_val.next());
		// only first increment to last char is allowed!
		if(ret.m_val.is_last() && n > 1) {
			throw range_error();
		}
	}
	return ret;
}

string::const_iterator string::const_iterator::operator- (size_type n) const {
	const_iterator ret = *this;
	for( ; n != 0; n--) {
		ret.m_val.rebind(ret.m_val.prev());
		// only first decrement to first char is allowed!
		if(ret.m_val.is_first() && n > 1) {
			throw range_error();
		}
	}
	return ret;
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
static const string::size_type npos = -1;

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
string::string(const char* str, size_type size) : m_tempbuff(NULL), s_ptr(new shared_data) {
	assign(str, size);
}

/*!
	\brief Copy constructor.

	Constructs string copying the context of other string.
	However constructor will perform shallow copy of string
	and later, if it is necessary, deep copy will be performed.
*/
string::string(const string& other) : m_tempbuff(NULL), s_ptr(other.s_ptr) {

}

string::string(size_type n, const_reference c) : m_tempbuff(NULL), s_ptr(new shared_data) {
    assign(n, c);
}

/*!
	\brief Destruction of string.

	Destructor delete all data created by string object.
*/
string::~string() {
	if(m_tempbuff != NULL) {
		s()->m_alloc.deallocate(m_tempbuff);
	}
}

string& string::operator= (const string& r) {
    if(&r != this) {
        s_ptr.set_data(r.s_ptr);
    }
    return *this;
}

/*!
	\brief Clear string.

	This function deletes contents of string and deallocates
	all allocated memory.
*/
void string::clear() {
    if(m_tempbuff != NULL) {
         s()->m_alloc.deallocate(m_tempbuff);
         m_tempbuff = NULL;
     }
    if(size() != 0) {
		s_ptr.set_data(new shared_data);
	}
}

void string::reserve(size_type least_size) {
    lowl_realloc(least_size);
}

void string::resize(size_type new_size, const_reference new_c) {
    if(new_size > size()) {
        append(new_size - size(), new_c);
    }
    else {
        erase(begin() + new_size, begin() + size());
    }
}

/*!
	\brief Returns character.

	This function returns character of string at index \a i.
	Character is returned in form of reference and can be manipulate
	by all functions in class value_type. If there is no character
	at index \a i exception wq::core::range_error is thrown.

	\sa operator[](), at(size_type) const;
*/
string::reference string::at(size_type i) {
	if(i > size()) {
		throw range_error();
	}
	return *(begin() + i);
}

/*!
	\brief Returns character.

	This constant form of at(size_type) function. However
	only constant operations are allowed on returned value.

	\sa operator[](), at(size_type)
*/
string::const_reference string::at(size_type i) const {
	if(i > size()) {
		throw range_error();
	}
	return *(begin() + i);
}

string& string::assign(const string& str) {
	s()->m_len = lowl_assign(str.s()->m_start, str.bytes());
	return *this;
}

string& string::assign(const string& str, size_type from, size_type size) {
    const_iterator start = str.begin() + from;
    const char* start_at = str.s()->m_start + start->ptr_index();
    size = (size > str.size() - from) ? str.size() - from : size;
    size = (start + size)->ptr() - start->ptr();
	s()->m_len = lowl_assign(start_at, size);
	return *this;
}

string& string::assign(const char* str, size_type size) {
	s()->m_len = lowl_assign(str, size);
	return *this;
}

string& string::assign(size_type n, const_reference c) {
	clear();
	lowl_realloc(n);
	for(; n != 0; n--) {
		s()->m_len += lowl_append(c.ptr(), c.bytes());
	}
	return *this;
}

string& string::append(const string& str) {
    s()->m_len += lowl_append(str.s()->m_start, str.bytes());
    return *this;
}

string& string::append(const string& str, size_type from, size_type size) {
    const_iterator start = str.begin() + from;
    const char* start_at = str.s()->m_start + start->ptr_index();
    size = (size > str.size() - from) ? str.size() - from : size;
    size = (start + size)->ptr() - start->ptr();
    s()->m_len += lowl_append(start_at, size);
    return *this;
}

string& string::append(const char* str, size_type size) {
    s()->m_len += lowl_append(str, size);
    return *this;
}

string& string::append(size_type n, const_reference c) {
    lowl_realloc(n);
    for(; n != 0; n--) {
        s()->m_len += lowl_append(c.ptr(), c.bytes());
    }
    return *this;
}

string& string::insert(size_type i, const string& str) {
    s()->m_len += lowl_insert(at(i).ptr(), str.s()->m_start, str.bytes());
    return *this;
}

string& string::insert(size_type i, const string& str, size_type from, size_type size) {
    const_iterator start = str.begin() + from;
    const char* start_at = str.s()->m_start + start->ptr_index();
    size = (size > str.size() - from) ? str.size() - from : size;
    size = (start + size)->ptr() - start->ptr();
    s()->m_len += lowl_insert(at(i).ptr(), start_at, size);
    return *this;
}

string& string::insert(size_type i, const char* str, size_type size) {
    s()->m_len += lowl_insert(at(i).ptr(), str, size);
    return *this;
}

string& string::insert(size_type i, const_reference c) {
    s()->m_len += lowl_insert(at(i).ptr(), c, c.bytes());
    return *this;
}

string& string::insert(size_type i, size_type n, const_reference c) {
    // we will use temporary buffer for this inserting, this avoid
    // repeated moving of memory
    string ins_str(n, c);
    s()->m_len += lowl_insert(at(i).ptr(), ins_str.s()->m_start, ins_str.bytes());
    return *this;
}

string::iterator string::insert(iterator iter, const string& str) {
    s()->m_len += lowl_insert(iter->ptr(), str.s()->m_start, str.bytes());
    return iter;
}

string::iterator string::insert(iterator iter, const char* str, size_type size) {
    s()->m_len += lowl_insert(iter->ptr(), str, size);
    return iter;
}

string::iterator string::insert(iterator iter, const_reference c) {
    s()->m_len += lowl_insert(iter->ptr(), c, c.bytes());
    return iter;
}

string::iterator string::insert(iterator iter, size_type n, const_reference c) {
    // we will use temporary buffer for this inserting, this avoid
    // repeated moving of memory
    string ins_str(n, c);
    s()->m_len += lowl_insert(iter->ptr(), ins_str.cs()->m_start, ins_str.bytes());
    return iter;
}

string& string::erase(size_type from, size_type n) {
    if(n == -1) {
        n = size() - from;
    }
    iterator start = begin() + from;
    iterator end = start + n;
    s()->m_len -= lowl_erase(start->ptr(), end->ptr());
    return *this;
}

string::iterator string::erase(iterator iter) {
    s()->m_len -= lowl_erase(iter->ptr(), (iter + 1)->ptr());
    return iter->ptr() >= cs()->m_last ? end() : iter;
}

string::iterator string::erase(iterator start_iter, iterator end_iter) {
    s()->m_len -= lowl_erase(start_iter->ptr(), end_iter->ptr());
    return start_iter->ptr() >= cs()->m_last ? end() : start_iter;
}

string& string::replace(size_type from, size_type n, const string& with) {
    iterator start = begin() + from;
    iterator end = start + n;
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), with.cs()->m_start, with.bytes());
    return *this;
}

string& string::replace(size_type from, size_type n, const string& with, size_type from2, size_type n2) {
    iterator start = begin() + from;
    iterator end = start + n;
    const_iterator start2 = with.begin() + from2;
    const_iterator end2 = start2 + n2;
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), start2->ptr(), (end2->ptr() - start2->ptr()));
    return *this;
}

string& string::replace(size_type from, size_type n, const char* str, size_type size) {
    iterator start = begin() + from;
    iterator end = start + n;
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), str, size);
    return *this;
}

string& string::replace(size_type from, size_type n, size_type count, const_reference c) {
    // we will use temporary buffer for this inserting, this avoid
    // repeated moving of memory
    string rep_str(count, c);

    iterator start = begin() + from;
    iterator end = start + n;
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), rep_str.cs()->m_start, rep_str.bytes());
    return *this;
}

string& string::replace(iterator start, iterator end, const string& with) {
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), with.cs()->m_start, with.bytes());
    return *this;
}

string& string::replace(iterator start, iterator end, const_iterator start2, const_iterator end2) {
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), start2->ptr(), end2->ptr() - start2->ptr());
    return *this;
}

string& string::replace(iterator start, iterator end, const char* str, size_type size) {
    s()->m_len += lowl_replace(start->ptr(), end->ptr(), str, size);
    return *this;
}

string& string::replace(iterator start, iterator end, size_type count, const_reference c) {
    // we will use temporary buffer for this inserting, this avoid
    // repeated moving of memory
    string rep_str(count, c);

    s()->m_len += lowl_replace(start->ptr(), end->ptr(), rep_str.cs()->m_start, rep_str.bytes());
    return *this;
}

/*!
    \brief Copies to buffer.

    This function copies data from string to \b char* buffer.
    Remember that this function doesn't append \b '\0' character.

    \param out_str Buffer in which data will be stored.
    \param n Number of characters to copy.
    \param from Index of first character that will be copied.
    \return This function returns number of bytes copied (no characters).
*/
string::size_type string::copy(char* out_str, size_type n, size_type from) const {
    const_iterator start = begin() + from;
    const_iterator end = start + n;
    size_type copied = 0;
    for( ; start != end; start++) {
        // for every character we have to copy all bytes
        size_type stop_after = start->bytes();
        for(size_type i = 0; i != stop_after; i++) {
            out_str[copied + i] = *(start->ptr() + i);
        }
        copied += stop_after;
    }
    return copied;
}

/*!
    \brief Swaps contents.

    This function swaps the contents between two string objects.

    \param with Object with which to swap contents.
*/
void string::swap(string& with) {
    // this is really fast because of implicit sharing
    string tmp_str = with;
    with = *this;
    *this = tmp_str;
}

/*!
    \brief Returns substring of string.

    This function returns string which contains particular
    sequence of string.

    \param from First character to choose for new string.
    \param n Number of character in new string.
    \return New string containing chosen sequence.
*/
string string::substr(size_type from, size_type n) const {
    string ret_str;
    ret_str.assign(*this, from, n);
    return ret_str;
}

/*!
	\brief Converts string.

	Returns utf8 null terminated string. Returned string
	is valid until next *_str() function call or until
	destruction if \a this object.

	\sa c_str()
*/
const char* string::utf8_str() const {
	if(m_tempbuff != NULL) {
		s()->m_alloc.deallocate(m_tempbuff);
	}
	m_tempbuff = s()->m_alloc.allocate(bytes() + 1);
	s()->m_alloc.copy(m_tempbuff, s()->m_start, bytes());
	s()->m_alloc.construct(m_tempbuff + bytes(), '\0');

	return m_tempbuff;
}

/*!
	\fn const char* string::c_str() const
	\brief Convert string.

	For now this is same as utf8_str() functions.

	\sa utf8_str()
*/

// private functions
// function will return number of octets of utf8 character 'c'
string::size_type string::octets_count(const char* arr, size_type size) {
	/*
	   octets specifications:
	   0000 0000-0000 007F | 0xxxxxxx
	   0000 0080-0000 07FF | 110xxxxx 10xxxxxx
	   0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
	   0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/
	char c = *arr;
	char c2 = size > 1 ? arr[1] : '\0';
	if(c2 != '\0') {
		if( !(c & (1 << 7)) ) {
			return 1;
		}
		else if( c & (1 << 7) && c & (1 << 6) && !(c & (1 << 5)) ) {
			return 2;
		}
		else if( c & (1 << 7) && c & (1 << 6) && c & (1 << 5) && !(c & (1 << 4)) ) {
			return 3;
		}
		else if( c & (1 << 7) && c & (1 << 6) && c & (1 << 5) && c & (1 << 4) && !(c & (1 << 4)) ) {
			return 4;
		}
		return 0;
	}
	return 1;
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
		size_type to_add = octets_count(str + i, size - i);
		if(to_add == 0) {
		    throw encode_error();
		}
		i += to_add;
	}
	return ret_val;
}

// this function realloc string by realloc algorithm + ensure 'size' bytes are free
void string::lowl_realloc(size_type size) {
	// resize only if it is needed
	size_type old_capacity = s()->m_end - s()->m_start;
	size_type old_size = s()->m_last - s()->m_start;
	if(old_capacity < old_size + size) {
	    // finding new capacity - always by multiplying with 2
	    size_type least_capacity = old_size + size;
	    size_type new_capacity = old_capacity == 0 ? 1 : old_capacity;
	    while(new_capacity < least_capacity) new_capacity = new_capacity * 2;

		s()->m_start = s()->m_alloc.reallocate(s()->m_start, old_capacity, new_capacity);
		s()->m_end = s()->m_start + new_capacity;
		s()->m_last = s()->m_start + old_size;
	}
}

// this function appends new utf8 chars to string
string::size_type string::lowl_append(const char* str, size_type str_size) {
    const char null_arr[] = "\0";
    if(str == NULL) {
        str_size = 1;
        str = null_arr;
    }
    else if(str_size == -1) {
        str_size = strlen(str);
    }

    lowl_realloc(str_size);
    s()->m_last = s()->m_alloc.copy(s()->m_last, str, str_size);

    // returning length of inserted string
    return chars_count(str, str_size);
}

// this function delete string contents and assign the new one
string::size_type string::lowl_assign(const char* str, size_type str_size) {
    s()->m_last = s()->m_start;
	return lowl_append(str, str_size);
}

// this function inserts 'str' at 'at' position (moving all chars after 'at' (including 'at') to right)
string::size_type string::lowl_insert(char* at, const char* str, size_type str_size) {
    const char null_arr[] = "\0";
    if(str == NULL) {
        str_size = 1;
        str = null_arr;
    }
    else if(str_size == -1) {
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
	// how many characters will we erase?
	size_type erased = chars_count(first, last - first);

	// rewriting deleted data
	s()->m_last = s()->m_alloc.ocopy(first, last, s()->m_last - last);

	// erased length
	return erased;
}

// replace bytes between 'first' and 'last' with str
string::size_type string::lowl_replace(char* first, char* last, const char* str, size_type str_size) {
	const char null_arr[] = "\0";
    if(str == NULL) {
	    str_size = 1;
	    str = null_arr;
	}
	else if(str_size == -1) {
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
