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

// string::value_type class
#define WQ_UC_PROP_INDEX(ucs4) \
    (ucs4 < 0x11000 \
    ? (string::value_type::sm_property_trie[string::value_type::sm_property_trie[ucs4>>5] + (ucs4 & 0x1f)]) \
    : (string::value_type::sm_property_trie[string::value_type::sm_property_trie[((ucs4 - 0x11000)>>8) + 0x880] + (ucs4 & 0xff)]))

/*!
    \brief Gets properties of character.

    This function returns properties of unicode character.

    \param ucs4 Character which's properties will be returned.
    \return Structure with properties.
*/
const string::value_type::uc_properties* string::value_type::get_uc_properties(wq::uint32 ucs4) {
    int index = WQ_UC_PROP_INDEX(ucs4);
    return string::value_type::sm_properties + index;
}

/*!
	\class string::value_type
	\brief Unicode character handler.

    This class holds Unicode character and allows opration
    with it. This class is used inly when returning characters
    from string.

	\sa string
*/

/*!
	\brief Default constructor.

	Creates empty object that must be initialized
	by assign operators.

	\sa operator=()
*/
string::value_type::value_type() : m_val(0) {

}

/*!
	\brief Copy constructor.

	This constructor creates exact copy of \a from object.

	\sa value_type(), operator=(const value_type&)
*/
string::value_type::value_type(const value_type& from) : m_val(from.m_val) {

}

/*!
    \brief Basic construction.

    Constructs unicode character from character in 8-bit
    encoding.

    \param c ASCII encoded character.
    \param enc Encoder which will be used to encode character to unicode.
*/
string::value_type::value_type(char c, const text_encoder& enc) :
        m_val( enc.encode(&c, 1).at(0).utf32() ) {

}

/*!
    \brief Basic construction.

    Constructs unicode character from 32-bit number.

    \param c Number (code) of unicode character.
*/
string::value_type::value_type(int c) : m_val(c) {

}

/*!
	\brief Basic construction.

	This constructor object that holds unicode character. Unicode
	character is described in \a c buffer which is encoded in UTF-8.

	\param c Pointer to first byte of UTF-8 encoded character sequence.
	\param throw_err If \b true all encode errors of \a c are reported by throwing
	encode_error exception, otherwise those errors are not reported but character
	will be equal to repl_char() after error.
*/
string::value_type::value_type(const char* c, bool throw_err) : m_val( decode_utf8(c, throw_err) ) {

}

/*!
    \brief Basic construction.

    This constructor object that holds unicode character. Unicode
    character is described in \a c buffer which is encoded in UTF-16.

    \param c Pointer to first byte of UTF-16 encoded character sequence.
    Sequence must end with number \b 0.
    \param throw_err If \b true all encode errors of \a c are reported by throwing
    encode_error exception, otherwise those errors are not reported but character
    will be equal to repl_char() after error.
*/
string::value_type::value_type(wq::uint16 w1, wq::uint16 w2, bool throw_err) :
        m_val( decode_utf16(w1, w2, throw_err) ) {

}

/*!
    \fn string::value_type::operator= (char)
    \brief Assign operator.

    Assigns character \a c as content of object.
*/

/*!
    \fn string::value_type::operator= (wq::uint32)
    \brief Assign operator.

    Assigns character \a c as content of object.
*/

/*!
    \fn string::value_type::operator= (const char*)
    \brief Assign operator.

    This assigns operator assign \a c character to \a this object.
    If \a this object is of "reference type" string's character is also
    changed.
*/

/*!
    \brief Assign operator.

    This operator copy \a r object to \a this object.

    \sa operator= (const char*)
*/
string::value_type& string::value_type::operator= (const value_type& r) {
    if(this != &r) {
        m_val = r.m_val;
    }
    return *this;
}

/*!
    \fn bool string::value_type::operator== (const value_type&) const
    \brief Compare operator.

    Compare character in \a this object with character
    in \a r object.

    \return Returns \b true when characters are identical.
*/

/*!
    \fn bool string::value_type::operator== (const char*) const
    \brief Compare operator.

    Compare character in \this object with character describes
    in \a r UTF-8 encoded string.

    \return Returns \b true when characters are identical.
*/

/*!
    \fn bool string::value_type::operator== (char) const
    \brief Compare operator.

    Compare character in \this object with character \a c.

    \return Returns \b true when characters are identical.
*/

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
    \brief Converts to UTF-8 C string.

    This function converts character to sequence
    of char-s (bytes) and return a pointer to sequence.

    \sa operator const char*()
*/
const char* string::value_type::utf8() const {
    size_type oct_count = octets_count(m_val);
    if(oct_count == 0) {
        throw encode_error();
    }

    // encoding
    /*
       0000 0000-0000 007F | 0xxxxxxx
       0000 0080-0000 07FF | 110xxxxx 10xxxxxx
       0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
       0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    */
    if(oct_count == 1) {
        m_utf8tmp[0] = m_val;
    }
    else if(oct_count == 2) {
        m_utf8tmp[0] = (m_val >> 6) | 0xC0;
        m_utf8tmp[1] = (m_val & 0x3F) | 0x80;
    }
    else if(oct_count == 3) {
        m_utf8tmp[0] = (m_val >> 12) | 0xE0;
        m_utf8tmp[1] = ((m_val >> 6) & 0x3F) | 0x80;
        m_utf8tmp[2] = (m_val & 0x3F) | 0x80;
    }
    else if(oct_count == 4) {
        m_utf8tmp[0] = (m_val >> 18) | 0xF0;
        m_utf8tmp[1] = ((m_val >> 12) & 0x3F) | 0x80;
        m_utf8tmp[2] = ((m_val >> 6) & 0x3F) | 0x80;
        m_utf8tmp[3] = (m_val & 0x3F) | 0x80;
    }

    m_utf8tmp[oct_count] = '\0';
    return m_utf8tmp;
}

/*!
    \fn string::value_type::operator const char*() const
    \brief Implicit conversion.

    This operator allows implicit conversion to
    UTF-8 encoded string.

    \sa utf8()
*/

/*!
    \brief Converts to UTF-16.

    Returns character as representation of 1 or 2 16-bit unsigned
    integers. Integers are returned in wq::pair object and can be accessed
    by \a first and \a second members. Remember that \a second can be \b 0.

    \sa utf8(), utf32()
*/
wq::pair<wq::uint16, wq::uint16> string::value_type::utf16() const {
    wq::pair<wq::uint16, wq::uint16> ret_val(0, 0);
    wq::uint32 u = utf32();
    if(u < 0x10000) {
        ret_val.first = wq::uint16(u);
    }
    else {
        u = u - 0x10000;
        ret_val.first = (u >> 10) | 0xD800;
        ret_val.second = (u & 0x3FF) | 0xDC00;
    }
    return ret_val;
}

/*!
    \brief Conversion to ASCII char.

    This functions convert character to ASCII
    \a char type, however if character has more
    than 1 byte character \b '?' is returned and no
    exception is thrown. To handle all unicode characters
    use utf8() function.

    \sa utf8()
*/
char string::value_type::ascii() const {
    return bytes() == 1 ? char( utf32() ) : '?';
}

// private static functions
string::size_type string::value_type::octets_count(char c) {
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

string::size_type string::value_type::octets_count(wq::uint32 c) {
    if(c <= 0x7F) {
        return 1;
    }
    else if(c <= 0x07FF) {
        return 2;
    }
    else if(c <= 0xFFFF) {
        return 3;
    }
    else if(c <= 0x10FFFF) {
        return 4;
    }
    return 0;
}

wq::uint32 string::value_type::decode_utf8(const char* seq, bool threrr) {
    size_type oct_count = octets_count(*seq);
    wq::uint32 ret_val = 0;
    if(oct_count == 0) {
        if(threrr) {
            throw encode_error();
        }
        else {
            return repl_char().utf32();
        }
    }

    // decoding sequence
    /*
       0000 0000-0000 007F | 0xxxxxxx
       0000 0080-0000 07FF | 110xxxxx 10xxxxxx
       0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
       0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    */
    if(oct_count == 1) {
        ret_val |= wq::uint32(*seq);
    }
    else if(oct_count == 2) {
        ret_val |= wq::uint8(*(seq + 1)) ^ 0x80;
        ret_val |= wq::uint32(wq::uint8(*seq) ^ 0xC0) << 6;
    }
    else if(oct_count == 3) {
        ret_val |= wq::uint8(*(seq + 2)) ^ 0x80;
        ret_val |= wq::uint32(wq::uint8(*(seq + 1)) ^ 0x80) << 6;
        ret_val |= wq::uint32(wq::uint8(*seq) ^ 0xE0) << 12;
    }
    else if(oct_count == 4) {
        ret_val |= wq::uint8(*(seq + 3)) ^ 0x80;
        ret_val |= wq::uint32(wq::uint8(*(seq + 2)) ^ 0x80) << 6;
        ret_val |= wq::uint32(wq::uint8(*(seq + 1)) ^ 0x80) << 12;
        ret_val |= wq::uint32(wq::uint8(*seq) ^ 0xF0) << 18;
    }

    return ret_val;
}

wq::uint32 string::value_type::decode_utf16(wq::uint16 w1, wq::uint16 w2, bool threrr) {
    if(w1 > 0xDFFF && w1 < 0xD800) {
        return wq::uint32(w1);
    }
    else if(!(w1 >= 0xD800 && w1 <= 0xDBFF)) {
        if(threrr) {
            throw encode_error();
        }
        else {
            return repl_char().utf32();
        }
    }
    else if(w2 == 0 || !(w2 >= 0xDC00 && w2 <= 0xDFFF)) {
        if(threrr) {
            throw encode_error();
        }
        else {
            return repl_char().utf32();
        }
    }
    return (((w1 ^ 0xD800) << 10) | (w2 ^ 0xDC00) + 0x10000);
}

// string::reference class
string::reference::reference(string* owner, char* ptr) :
        value_type(), m_owner(owner), m_ptr(ptr) {
    if(m_ptr == m_owner->cd()->m_last) {
        value_type::operator= ( wq::uint32(0) );
    }
    else {
        value_type::operator= (m_ptr);
    }
}

string::reference::reference(const string* owner, char* ptr) :
        value_type(), m_owner(const_cast<string*>(owner)), m_ptr(ptr) {
    if(m_ptr == m_owner->cd()->m_last) {
        value_type::operator= ( wq::uint32(0) );
    }
    else {
        value_type::operator= (m_ptr);
    }
}

string::reference::reference(const reference& from) :
        value_type(from), m_owner(from.m_owner), m_ptr(from.m_ptr) {

}

// assignment
string::reference& string::reference::operator= (const reference& r) {
    if(&r != this) {
        value_type::operator= (r);
        owner()->replace( iterator( reference(owner(), ptr()) ), iterator( reference(owner(), ptr() + bytes()) ),
                          r.ptr(), r.bytes());
    }
    return *this;
}

string::reference& string::reference::operator= (const_reference r) {
    if(&r != this) {
        value_type::operator= (r);
        owner()->replace( iterator( reference(owner(), ptr()) ), iterator( reference(owner(), ptr() + bytes()) ),
                          r.utf8(), r.bytes());
    }
    return *this;
}

// private functions
// new contents without changing string's data
void string::reference::rebind(const reference& n) {
    value_type::operator= (n);
    m_owner = n.m_owner;
    m_ptr = n.m_ptr;
}

// returns next character in string
string::reference string::reference::next() const {
    if(m_ptr + bytes() < owner()->d()->m_last) {
        return reference(m_owner, m_ptr + bytes());
    }
    return reference(m_owner, owner()->d()->m_last);
}

// returns previous character in string
string::reference string::reference::prev() const {
    // finding previous character is more complex :(
    reference ret = reference(m_owner, m_ptr);
    size_type clen = 0;
    while(1) {
        clen++;
        try {
            ret.rebind( reference( m_owner, (m_ptr - clen) ) );
        }
        catch(encode_error& err) {
            continue;
        }
        break;
    }
    if(m_ptr - clen > owner()->d()->m_start) {
        return reference(m_owner, m_ptr - clen);
    }
    return reference(m_owner, owner()->d()->m_start);
}

// string::iterator class
string::difference_type string::iterator::operator- (const const_iterator& r) const {
    const_iterator tmp = r;
    difference_type ret = 0;
    int add = tmp < *this ? 1 : -1;
    while(tmp != *this) {
        tmp += add;
        ret += add;
    }
    return ret;
}

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
string::difference_type string::const_iterator::operator- (const const_iterator& r) const {
    const_iterator tmp = r;
    difference_type ret = 0;
    int add = tmp < *this ? 1 : -1;
    while(tmp != *this) {
        tmp += add;
        ret += add;
    }
    return ret;
}

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

// string::wq_data class
string::allocator_type string::wq_data::m_alloc;

string::wq_data::wq_data(const wq_data& from) :
		m_start(NULL), m_last(NULL), m_end(NULL), m_len(0) {
	size_type size = from.m_last - from.m_start;
	char* start = m_alloc.allocate(size);
	m_alloc.copy(start, from.m_start, size);

	m_start = start;
	m_last = m_start + size;
	m_end = m_start + size;
	m_len = from.m_len;
}

string::wq_data::~wq_data() {
	if(m_start != NULL && m_last != NULL) {
		// it is not necessary to call destroy function for objects
		m_alloc.deallocate(m_start);
	}
}

// string class
const string::size_type string::npos = -1;

/*!
	\brief Constructs string.

	This constructor constructs empty string.
*/
string::string() : m_tempbuff(NULL), d_ptr(new wq_data) {

}

/*!
	\brief Constructs string.

	Constructor constructs string with the given context.
*/
string::string(const char* str, size_type size, const text_encoder& enc) :
        m_tempbuff(NULL), d_ptr(new wq_data) {
	assign( enc.encode(str, size) );
}

/*!
	\brief Copy constructor.

	Constructs string copying the context of other string.
	However constructor will perform shallow copy of string
	and later, if it is necessary, deep copy will be performed.
*/
string::string(const string& other) : m_tempbuff(NULL), d_ptr(other.d_ptr) {

}

string::string(size_type n, const_reference c) : m_tempbuff(NULL), d_ptr(new wq_data) {
    assign(n, c);
}

string::string(const_iterator first, const_iterator last) : m_tempbuff(NULL), d_ptr(new wq_data) {
    assign(first, last);
}

#if WQ_STD_COMPATIBILITY
string::string(const std::string& std_str, const text_encoder& enc) : m_tempbuff(NULL), d_ptr(new wq_data) {
    assign( enc.encode(std_str.data(), std_str.size()) );
}
#endif

/*!
	\brief Destruction of string.

	Destructor delete all data created by string object.
*/
string::~string() {
	set_tempbuff(NULL);
}

string& string::operator= (const string& r) {
    if(&r != this) {
        d_ptr.set(r.d_ptr);
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
         d()->m_alloc.deallocate(m_tempbuff);
         m_tempbuff = NULL;
     }
    if(size() != 0) {
		d_ptr.set(new wq_data);
	}
}

void string::reserve(size_type least_size) {
    // resize only if it is needed
    size_type old_capacity = d()->m_end - d()->m_start;
    size_type old_size = d()->m_last - d()->m_start;
    if(least_size == 0) {
        // now we want deallocate unneeded space
        d()->m_start = d()->m_alloc.reallocate(d()->m_start, old_capacity, old_size);
        d()->m_end = d()->m_start + old_size;
        d()->m_last = d()->m_start + old_size;
    }
    else if(old_capacity < old_size + least_size) {
        // finding new capacity - always by multiplying with 2
        size_type least_capacity = old_size + least_size;
        size_type new_capacity = old_capacity == 0 ? 1 : old_capacity;
        while(new_capacity < least_capacity) new_capacity = new_capacity * 2;

        d()->m_start = d()->m_alloc.reallocate(d()->m_start, old_capacity, new_capacity);
        d()->m_end = d()->m_start + new_capacity;
        d()->m_last = d()->m_start + old_size;
    }
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

string& string::assign(const string& str, size_type from, size_type size) {
    if(size == npos) {
        size = str.size();
    }
    size = (size > str.size() - from) ? (str.size() - from) : size;

    // when we are assigning full object we can simply assign
    // shared data only
    if(from == 0 && size == str.size()) {
        d_ptr.set(str.d_ptr);
    }
    else {
        clear();
        append(str, from, size);
    }
	return *this;
}

string& string::assign(size_type n, const_reference c) {
	clear();
	return append(n, c);
}

string& string::assign(const_iterator first, const_iterator last) {
    if(first.m_val.is_first() && last.m_val.is_last()) {
        // we can simply assign shared data
        d_ptr.set( first.m_val.owner()->d_ptr );
    }
    else {
        // we will append new data
        append(first, last);
    }
    return *this;
}

string& string::append(const string& str, size_type from, size_type size) {
    if(size == npos) {
        size = str.size();
    }
    size = (size > str.size() - from) ? (str.size() - from) : size;

    if(size > 0) {
        const_iterator first = str.begin() + from;
        const_iterator last = first + size;
        size_type bytes_size = last.ptr() - first.ptr();
        reserve(bytes_size);
        d()->m_last = d()->m_alloc.copy(end().ptr(), first.ptr(), bytes_size);
        d()->m_len += size;
    }
    return *this;
}

string& string::append(size_type n, const_reference c) {
    size_type c_bytes = c.bytes();
    const char* c_buff = c.utf8();
    reserve(n * c_bytes);
    for(; n != 0; n--) {
        d()->m_last = d()->m_alloc.copy(end().ptr(), c_buff, c_bytes);
        d()->m_len++;
    }
    return *this;
}

string& string::append(const_iterator first, const_iterator last) {
    size_type bytes_size = last.ptr() - first.ptr();
    if(bytes_size > 0) {
        reserve(bytes_size);
        d()->m_last = d()->m_alloc.copy(end().ptr(), first.ptr(), bytes_size);
        d()->m_len += last - first;
    }
    return *this;
}

string& string::insert(size_type i, const string& str, size_type from, size_type size) {
    if(size == npos) {
        size = str.size();
    }
    size = (size > str.size() - from) ? (str.size() - from) : size;

    if(size > 0) {
        const_iterator copy_from = str.begin() + from;
        const_iterator copy_to = copy_from + size;
        size_type insert_size = copy_to.ptr() - copy_from.ptr();

        reserve(insert_size);
        iterator cut_at = begin() + i;
        d()->m_last = d()->m_alloc.ocopy(cut_at.ptr() + insert_size, cut_at.ptr(), end().ptr() - cut_at.ptr());
        d()->m_alloc.copy(cut_at.ptr(), copy_from.ptr(), insert_size);
        d()->m_len += size;
    }
    return *this;
}

string::iterator string::insert(iterator iter, const string& str, size_type from, size_type size) {
    size_type dist = iter - begin();
    insert(dist, str, from, size);
    return begin() + dist;
}

string::iterator string::insert(iterator iter, const char* str, size_type size, const text_encoder& enc) {
    size_type dist = iter - begin();
    insert(dist, str, size, enc);
    return begin() + dist;
}

string::iterator string::insert(iterator iter, size_type n, const_reference c) {
    size_type dist = iter - begin();
    insert(dist, n, c);
    return begin() + dist;
}

string& string::erase(size_type from, size_type n) {
    if(n == npos) {
        n = size() - from;
    }
    if(n > 0) {
        iterator first = begin() + from;
        iterator last = first + n;
        d()->m_last = d()->m_alloc.ocopy(first.ptr(), last.ptr(), end().ptr() - last.ptr());
        d()->m_len -= n;
    }
    return *this;
}

string::iterator string::erase(iterator iter) {
    size_type dist = iter - begin();
    erase(dist, 1);
    try {
        iter = begin() + dist;
    }
    catch(const range_error&) {
        iter = end();
    }
    return iter;
}

string::iterator string::erase(iterator start_iter, iterator end_iter) {
    size_type dist1 = start_iter - begin();
    size_type dist2 = end_iter - begin();
    erase(dist1, dist2 - dist1);
    try {
        start_iter = begin() + dist1;
    }
    catch(const range_error&) {
        start_iter = end();
    }
    return start_iter;
}

string& string::replace(size_type from, size_type n, const string& with, size_type from2, size_type n2) {
    d_ptr.detach();
    if(n2 == npos) {
        n2 = with.size();
    }
    n2 = (n2 > with.size() - from2) ? (with.size() - from2) : n2;
    if(n == npos) {
        n = size();
    }
    n = (n > with.size() - from) ? (with.size() - from) : n;

    // now we will initialize all iterators that we will need
    iterator erase_from = begin() + from;
    iterator erase_to = erase_from + n;
    const_iterator insert_from = with.begin() + from2;
    const_iterator insert_to = insert_from + n2;

    // some sizes
    size_type erase_bytes = erase_to.ptr() - erase_from.ptr();
    size_type insert_bytes = insert_to.ptr() - insert_from.ptr();

    // things will be more complex because I want replacing as fast as possible
    if(erase_bytes == insert_bytes) {
        // just put it there
        d()->m_alloc.copy(erase_from.ptr(), insert_from.ptr(), insert_bytes);
    }
    else if(erase_bytes < insert_bytes) {
        // next step will make our erase iterators invalid (in some cases only)
        reserve(insert_bytes - erase_bytes);

        // so we have to count them again
        erase_from = begin() + from;
        erase_to = erase_from + n;

        // copy as many bytes as possible => erase_bytes bytes
        d()->m_alloc.copy(erase_from.ptr(), insert_from.ptr(), erase_bytes);

        // make space for other bytes
        d()->m_last = d()->m_alloc.ocopy(erase_to.ptr() + (insert_bytes - erase_bytes),
                                         erase_to.ptr(), end().ptr() - erase_to.ptr());

        // and put other bytes to right place
        d()->m_alloc.copy(erase_to.ptr(), insert_from.ptr() + erase_bytes, insert_bytes - erase_bytes);
    }
    else if(erase_bytes > insert_bytes) {
        // copy all bytes to right place
        d()->m_alloc.copy(erase_from.ptr(), insert_from.ptr(), insert_bytes);

        // and now copy data to left
        d()->m_last = d()->m_alloc.ocopy(erase_from.ptr() + insert_bytes, erase_to.ptr(),
                                         end().ptr() - erase_to.ptr());
    }

    // adding size
    d()->m_len += n2 - n;
    return *this;
}

int string::compare(size_type from1, size_type n1, const string& with, size_type from2, size_type n2, bool cs) const {
    if(n1 == npos) {
        n1 = size();
    }
    n1 = (n1 > size() - from1) ? (size() - from1) : n1;
    if(n2 == npos) {
        n2 = with.size();
    }
    n2 = (n2 > with.size() - from2) ? (with.size() - from2) : n2;

    const_iterator with_iter = with.begin() + from2;
    const_iterator start_iter = begin() + from1;
    const_iterator end_iter = start_iter + (n1 < n2 ? n1 : n2);
    while(start_iter != end_iter) {
        if( (cs && *start_iter != *with_iter) || (!cs && start_iter->lower() != with_iter->lower()) ) {
            return start_iter->utf32() - with_iter->utf32();
        }
        start_iter++;
        with_iter++;
    }
    return n1 == n2 ? 0 : -(n1 < n2 ? with_iter->utf32() : start_iter->utf32());
}

string::size_type string::find(const string& what, size_type from, bool cs) const {
    if(size() - from < what.size()) {
        return npos;
    }
    for(size_type start_at = from; start_at != size(); start_at++) {
        if(compare(start_at, what.size(), what, 0, npos, cs) == 0) {
            return start_at;
        }
    }
    return npos;
}

string::size_type string::rfind(const string& what, size_type from, bool cs) const {
    if(from == npos) {
        from = size() - 1;
    }
    from = from >= size() ? size() - 1 : from;
    if(from < what.size()) {
        return npos;
    }
    for(size_type start_at = from + what.size(); start_at >= 0; start_at--) {
        size_type pos = start_at - what.size();
        if(compare(pos, what.size(), what, 0, npos, cs) == 0) {
            return pos;
        }
    }
    return npos;
}

string::size_type string::find_first_of(const string& str, size_type pos, bool cs) const {
    const_iterator end_iter = end();
    const_iterator end_iter2 = str.end();
    for(const_iterator iter = begin() + pos; iter != end_iter; iter++) {
        for(const_iterator iter2 = str.begin(); iter2 != end_iter2; iter2++) {
            if( (cs && *iter == *iter2) || (!cs && iter->lower() == iter2->lower()) ) {
                return iter - begin();
            }
        }
    }
    return npos;
}

string::size_type string::find_first_not_of(const string& str, size_type pos, bool cs) const {
    const_iterator end_iter = end();
    const_iterator end_iter2 = str.end();
    for(const_iterator iter = begin() + pos; iter != end_iter; iter++) {
        size_type num_of_not = 0;
        for(const_iterator iter2 = str.begin(); iter2 != end_iter2; iter2++) {
            // counting of not found characters
            if( (cs && *iter != *iter2) || (!cs && iter->lower() != iter2->lower()) ) {
                num_of_not++;
            }
        }
        // returning if there was not character that is in str
        if(num_of_not == str.size()) {
            return iter - begin();
        }
    }
    return npos;
}

string::size_type string::find_last_of(const string& str, size_type pos, bool cs) const {
    if(pos == npos) {
        pos = size() - 1;
    }
    pos = pos >= size() ? size() - 1 : pos;
    const_iterator end_iter = begin();
    const_iterator end_iter2 = str.end();
    for(const_iterator iter = begin() + pos; iter != end_iter; iter--) {
        for(const_iterator iter2 = str.begin(); iter2 != end_iter2; iter2++) {
            if( (cs && *iter == *iter2) || (!cs && iter->lower() == iter2->lower()) ) {
                return iter - begin();
            }
        }
    }
    return npos;
}

string::size_type string::find_last_not_of(const string& str, size_type pos, bool cs) const {
    if(pos == npos) {
        pos = size() - 1;
    }
    pos = pos >= size() ? size() - 1 : pos;
    const_iterator end_iter = begin();
    const_iterator end_iter2 = str.end();
    for(const_iterator iter = begin() + pos; iter != end_iter; iter--) {
        size_type num_of_not = 0;
        for(const_iterator iter2 = str.begin(); iter2 != end_iter2; iter2++) {
            // counting of not found characters
            if( (cs && *iter != *iter2) || (!cs && iter->lower() != iter2->lower()) ) {
                num_of_not++;
            }
        }
        // returning if there was not character that is in str
        if(num_of_not == str.size()) {
            return iter - begin();
        }
    }
    return npos;
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
            out_str[copied + i] = *(start.ptr() + i);
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
    \fn string::utf8_str() const
	\brief Converts string.

	Returns utf8 null terminated string. Returned string
	is valid until next *_str() function call or until
	destruction if \a this object.

    \param err If \b true all encode errors are reported by \a encode_error
    exception. Otherwise problematic characters are replaced by string::value_type::repl_char().
	\sa c_str()
*/


/*!
	\fn const char* string::c_str() const
	\brief Convert string.

	For now this is same as utf8_str() functions.

	\sa utf8_str()
*/

}  // namespace core
}  // namespace wq
