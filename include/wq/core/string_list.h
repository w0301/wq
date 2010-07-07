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

#ifndef WQ_STRING_LIST_H
#define WQ_STRING_LIST_H

#include "wq/core/list.h"
#include "wq/core/string.h"

namespace wq {
namespace core {

class WQ_EXPORT string_list : public list<string> {
    public:
        // STL like typedef-ines
        typedef typename list<string>::reference reference;
        typedef typename list<string>::const_reference const_reference;
        typedef typename list<string>::iterator iterator;
        typedef typename list<string>::const_iterator const_iterator;
        typedef typename list<string>::size_type size_type;
        typedef typename list<string>::difference_type difference_type;
        typedef typename list<string>::value_type value_type;
        typedef typename list<string>::allocator_type allocator_type;
        typedef typename list<string>::pointer pointer;
        typedef typename list<string>::const_pointer const_pointer;
        typedef typename list<string>::reverse_iterator reverse_iterator;
        typedef typename list<string>::const_reverse_iterator const_reverse_iterator;

        // creation
        string_list() : list<string>() { };
        string_list(const string&, string::const_reference = string::value_type::delim_char());
        string_list(const_iterator, const_iterator);
        string_list(const string_list& from) : list<string>(from) { };

        // assignment
        string_list& operator= (const string_list&);

        // new functions for converting
        string to_string(string::const_reference = string::value_type::delim_char()) const;
        static string_list from_string(const string&, string::const_reference = string::value_type::delim_char());
};


} // namespace core
} // namespace wq

#endif  // WQ_STRING_LIST_H
