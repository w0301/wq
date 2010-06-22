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

// encode_error class
encode_error::encode_error() throw() : wq::core::exception() {

}

const char* encode_error::what() const throw() {
	return "encode error occurred";
}

// utf8_encoder class
string utf8_encoder::encode(const char* str, wq::size_t size) const {
    if(size == -1) {
        size = strlen(str);
    }
    string ret_val;

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
    return const_cast<char*>( str.utf8_str() );
}


}  // namespace core
}  // namespace wq
