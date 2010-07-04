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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "wq/defs.h"

namespace wq {
    //using namespace core;
}

/*!
    This program takes file (argument for program) in format like this:
    \code
        # Comments are allowed
        # "8-bit encoding character number"\t"unicode number" #"unicode name of character"\n
        # For example:
        0x00    0x00 #NULL
        # ...
    \endcode
    Program's output is sent to standard output. The output is C/C++ like array
    which can be useful for programming *_encoder classes.
*/
int main(int argc, char* args[]) {
    if(argc == 1) {
        std::cout << "No unicode mapping file specified." << std::endl;
        return 1;
    }
    std::ifstream mapping_file(args[1]);
    if( !mapping_file.is_open() ) {
        std::cout << "Unicode mapping file does not exist." << std::endl;
        return 1;
    }
    std::string line_str;
    wq::uint32 mapp_arr[0xFF + 1];
    wq::uint32 mapp_arr_curr = 0;

    // getting data from file
    while( !mapping_file.eof() ) {
        getline(mapping_file, line_str);
        if(line_str.size() == 0 || line_str.at(0) == '#') {
            continue;
        }
        std::istringstream line_stream(line_str);
        line_stream.setf(std::ios::hex, std::ios::basefield);
        wq::uint32 mapp_arr_enc = 0;
        wq::uint32 mapp_arr_uni = 0;
        line_stream >> mapp_arr_enc >> mapp_arr_uni;
        mapp_arr[mapp_arr_curr] = mapp_arr_uni;
        mapp_arr_curr++;
    }

    // shaping data and writing them to output stream
    std::cout << "wq::uint32 array_name[] = {" << std::endl;
    for(wq::uint32 i = 0; i != mapp_arr_curr; i++) {
        std::cout << mapp_arr[i];
        if(i != mapp_arr_curr - 1) {
            std::cout << ", ";
        }
        if(i % 15 == 0 && i != 0) {
            std::cout << std::endl;
        }
    }
    std::cout << "};" << std::endl;

    return 0;
}
