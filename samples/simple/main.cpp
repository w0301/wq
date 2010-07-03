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

#include "wq/defs.h"
#include "wq/core/vector.h"
#include "wq/core/list.h"
#include "wq/core/atomic.h"
#include "wq/core/exception.h"
#include "wq/core/shared_ptr.h"
#include "wq/core/allocator.h"
#include "wq/core/type_info.h"
#include "wq/core/string.h"

#include <iostream>
#include <string>

namespace wq {
using namespace core;
}

int main() {
    try {
        wq::string str("článku");
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;

        str.append(5, "|");
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;

        str.insert(str.begin() +  2, 1, "\\");
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;

        str.erase(str.end() - 2, str.end());
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;

        str.replace(2, 4, wq::string("|<-->|"), 1, 4);
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;

        str.resize(9);
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;
        std::cout << str.utf8_str() << ", capacity >> " << str.capacity() << std::endl;

        str.reserve(43);
        std::cout << str.utf8_str() << ", capacity >> " << str.capacity() << std::endl;

        char* buff = new char[str.bytes() + 1];
        *(buff + str.copy(buff, str.size())) = '\0';
        std::cout << str.utf8_str() << " >> " << str.size() << std::endl;
        std::cout << buff << std::endl;

        wq::string str1 = "contents of str1";
        wq::string str2 = "contents of str2";
        std::cout << "str1: " << str1.utf8_str() << std::endl;
        std::cout << "str2: " << str2.utf8_str() << std::endl;
        str1.swap(str2);
        std::cout << "str1: " << str1.utf8_str() << std::endl;
        std::cout << "str2: " << str2.utf8_str() << std::endl;

        std::cout << "\n\n\n";

        wq::string::value_type val("�");  // �
        std::cout << val.utf8() << std::endl;
        std::cout << val.utf16().first << ":" << val.utf16().second << std::endl;
        std::cout << val.utf32() << std::endl;

        wq::string str3("ahoj");
        std::cout << str3.utf8_str() << std::endl;
        str3.append(" ako sa máš?");
        std::cout << str3.utf8_str() << std::endl;

        std::cout << "\n\n\n";

        const int from = 0;
        wq::string str10 = "ahoj ako ahoj máš";
        std::cout << str10.rfind("ako") << std::endl;
    }
    catch(wq::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
