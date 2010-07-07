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

#include "wq/core/string_list.h"

namespace wq {
namespace core {

// string_list class
string_list::string_list(const string& str, string::const_reference delim) : list<string>() {
    operator= ( from_string(str, delim) );
}

string_list::string_list(const_iterator first, const_iterator last) : list<string>(first, last) {

}

string_list& string_list::operator= (const string_list& r) {
    if(&r != this) {
        list<string>::operator= (r);
    }
    return *this;
}

string string_list::to_string(string::const_reference delim) const {
    string result;
    for(const_iterator iter = begin(); iter != end(); iter++) {
        result += (*iter);
        const_iterator tmp = iter;
        if(++tmp != end()) {
            result += delim;
        }
    }
    return result;
}

string_list string_list::from_string(const string& str, string::const_reference delim) {
    string_list result;
    if(!str.empty() && str.find(delim) == string::npos) {
        result.push_back(str);
    }
    else if(!str.empty()) {
        string::size_type last_pos = 0;
        string::size_type curr_pos = 0;

        // adding parts of string that ends with delim char
        while( (curr_pos = str.find(delim, last_pos)) != string::npos ) {
            string::const_iterator first = str.begin() + last_pos;
            string::const_iterator last = first + (curr_pos - last_pos);
            result.push_back( string(first, last) );
            last_pos = curr_pos + 1;
        }

        // after that we have to check/add last part of new list
        if( last_pos < str.size() ) {
            string::const_iterator first = str.begin() + last_pos;
            string::const_iterator last = first + (str.size() - last_pos);
            result.push_back( string(first, last) );
        }
    }
    return result;
}

}
}
