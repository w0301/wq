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
#include "wq/core/locale.h"

#include <cstring>

namespace wq {
namespace core {

// encode_error class
encode_error::encode_error() throw() : wq::core::exception() {

}

const char* encode_error::what() const throw() {
	return "encode error occurred";
}

// text_encoder class
auto_ptr<text_encoder> text_encoder::sm_default_encoder = NULL;

string text_encoder::encode(const char*, wq::size_t) const {
    return string();
}

/*!
    \brief System encoder.

    This function returns pointer to encoder that encode/decode
    strings to/from system's (os's) encoding.

    \param thexce \b True if you wish returned encoding to report errors by encode_error exception.
    \sa default_encoder(), wq_encoder()
*/
const text_encoder* text_encoder::system_encoder(bool thexce) {
    static auto_ptr<text_encoder> s_sys_encoder;
    s_sys_encoder = locale::system_locale().encoder(thexce);
    return s_sys_encoder.data();
}

/*!
    \brief Wq libraries encoder.

    This function returns pointer to encoder that is used
    in all wq's header and source files. This encoder is used
    during build of libraries to encode/decode strings => when
    building libraries this encoder is returned by wq::core::default_encoder().

    \param thexce \b True if you wish returned encoding to report errors by encode_error exception.
    \sa wq::core::default_encoder(), default_encoder(), system_encoder()
*/
const text_encoder* text_encoder::wq_encoder(bool thexce) {
    static auto_ptr<text_encoder> s_wq_encoder = new utf8_encoder();
    s_wq_encoder->set_throwing(thexce);
    return s_wq_encoder.data();
};

/*!
    \brief Default encoding.

    This functions returns pointer to encoder that is returned
    by wq::core::default_encoder() function when not building wq
    libraries. As default this encoder is same as system_encoder()
    however user can change it by set_default_encoder() function.

    \param thexce \b True if you wish returned encoding to report errors by encode_error exception.
    \sa wq_encoder(), system_encoder(), wq::core::default_encoder(), set_default_encoder()
*/
const text_encoder* text_encoder::default_encoder(bool thexce) {
    if(sm_default_encoder.is_ok()) {
        sm_default_encoder->set_throwing(thexce);
        return sm_default_encoder.data();
    }
    set_default_encoder( const_cast<text_encoder*>(system_encoder()) );
    return default_encoder(thexce);
}

// utf8_encoder class
string utf8_encoder::encode(const char* str, wq::size_t size) const {
    if(size == -1) {
        size = strlen(str);
    }
    string ret_val;
    ret_val.reserve(size);

    // we will go threw all chars and check if all are correct
    // if not we will add replacement char or throw exception (is_throwing func)
    wq::size_t i = 0;
    while(i < size) {
        string::value_type c;
        try {
            c = str + i;
        }
        catch(const encode_error&) {
            if(is_throwing()) {
                throw;
            }
            else {
                c = string::value_type::repl_char();
            }
        }
        ret_val.push_back(c);
        i += c.bytes();
    }

    return ret_val;
}

char* utf8_encoder::decode(const string& str, wq::size_t* out_size) const {
    if(out_size != NULL) {
        *out_size = str.bytes();
    }
    string::allocator_type alloc = str.get_allocator();
    char* ret = alloc.allocate(str.bytes() + 1);
    alloc.construct( alloc.copy(ret, str.data(), str.bytes()), '\0' );
    return ret;
}

// cp1250_encoder class
const wq::uint32 cp1250_encoder::sm_mapping_array[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
    46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90,
    91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105,
    106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
    121, 122, 123, 124, 125, 126, 127, 8364, 0, 8218, 0, 8222, 8230, 8224, 8225,
    0, 8240, 352, 8249, 346, 356, 381, 377, 0, 8216, 8217, 8220, 8221, 8226, 8211,
    8212, 0, 8482, 353, 8250, 347, 357, 382, 378, 160, 711, 728, 321, 164, 260,
    166, 167, 168, 169, 350, 171, 172, 173, 174, 379, 176, 177, 731, 322, 180,
    181, 182, 183, 184, 261, 351, 187, 317, 733, 318, 380, 340, 193, 194, 258,
    196, 313, 262, 199, 268, 201, 280, 203, 282, 205, 206, 270, 272, 323, 327,
    211, 212, 336, 214, 215, 344, 366, 218, 368, 220, 221, 354, 223, 341, 225,
    226, 259, 228, 314, 263, 231, 269, 233, 281, 235, 283, 237, 238, 271, 273,
    324, 328, 243, 244, 337, 246, 247, 345, 367, 250, 369, 252, 253, 355, 729
};

string cp1250_encoder::encode(const char* str, wq::size_t size) const {
    if(size == -1) {
        size = strlen(str);
    }
    string ret_val;
    ret_val.reserve(size);

    for(wq::size_t i = 0; i != size; i++) {
        // using char number to get unicode number from array
        // and pushing new character to string
        ret_val.push_back( string::value_type(sm_mapping_array[wq::uint32(str[i])]) );
    }
    return ret_val;
}

char* cp1250_encoder::decode(const string& str, wq::size_t* out_size) const {
    if(out_size != NULL) {
        *out_size = str.size();
    }
    string::allocator_type alloc = str.get_allocator();
    char* ret = alloc.allocate(str.size() + 1);

    string::const_iterator end_iter = str.end();
    wq::size_t i = 0;
    for(string::const_iterator iter = str.begin(); iter != end_iter; iter++, i++) {
        // it's not so easy - we have to find unicode character code in
        // array and determine index of that code (variable 'c')
        wq::uint32 c = 0;
        for( ; c != 0x100; c++) {
            if(sm_mapping_array[c] == iter->utf32()) {
                break;
            }
            if(c == 0xFF) {
                // there is now such character in table => we will report
                // error use character '?' if user don't want to report
                if(is_throwing()) {
                    throw encode_error();
                }
                else {
                    c = '?';
                    break;
                }
            }
        }
        // now simple construct new char in array
        alloc.construct(ret + i, char(c));
    }
    alloc.construct(ret + i, '\0');

    return ret;
}


}  // namespace core
}  // namespace wq
