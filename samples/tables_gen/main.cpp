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

#include "wq/wq.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <clocale>
#include <algorithm>
#include <langinfo.h>

namespace wq {
    using namespace core;
}

// generator for locale tables
class locale_gen {
    public:
        locale_gen() { };

        void add_locale(const wq::string&);
        wq::string create_tables();

    private:
        void put_one_table(const wq::vector<wq::string_list>&, wq::string&, const wq::string& = "table_name");

        // data for tables
        wq::vector<wq::string_list> m_days_names;
        wq::vector<wq::string_list> m_ab_days_names;
        wq::vector<wq::string_list> m_months_names;
        wq::vector<wq::string_list> m_ab_months_names;

        // table with indexes of data in other table - one line == one locale
        struct locale_indexes {
            wq::ushort m_lang;
            wq::ushort m_terr;
            wq::ushort m_days;
            wq::ushort m_ab_days;
            wq::ushort m_months;
            wq::ushort m_ab_months;
        };
        wq::vector<locale_indexes> m_locale_indexes;
};

void locale_gen::add_locale(const wq::string& name) {
    // setting locale
    try {
        wq::locale wq_locale( name.substr( 0, name.find('.') ) );
        if( !name.empty() ) {
            setlocale( LC_ALL, name.c_str() );
        }
        else {
            setlocale(LC_ALL, "C");
        }

        locale_indexes new_indexes;
        new_indexes.m_lang = wq_locale.language_id();
        new_indexes.m_terr = wq_locale.country_id();

        // extracting informations from os's locale database
        // this most works only on unix
#ifdef WQ_UNIX
        wq::string_list to_add;

        // iterator that is used to determine index
        wq::vector<wq::string_list>::const_iterator iter;

        to_add.push_back( wq::string( nl_langinfo(DAY_1) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_2) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_3) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_4) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_5) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_6) ) );
        to_add.push_back( wq::string( nl_langinfo(DAY_7) ) );
        if( (iter = std::find(m_days_names.begin(), m_days_names.end(), to_add)) == m_days_names.end() ) {
            m_days_names.push_back(to_add);
            new_indexes.m_days = m_days_names.size() - 1;
        }
        else {
            new_indexes.m_days = iter - m_days_names.begin();
        }

        to_add.clear();
        to_add.push_back( wq::string( nl_langinfo(ABDAY_1) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_2) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_3) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_4) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_5) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_6) ) );
        to_add.push_back( wq::string( nl_langinfo(ABDAY_7) ) );
        if( (iter = std::find(m_ab_days_names.begin(), m_ab_days_names.end(), to_add)) == m_ab_days_names.end() ) {
            m_ab_days_names.push_back(to_add);
            new_indexes.m_ab_days = m_ab_days_names.size() - 1;
        }
        else {
            new_indexes.m_ab_days = iter - m_ab_days_names.begin();
        }

        to_add.clear();
        to_add.push_back( wq::string( nl_langinfo(MON_1) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_2) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_3) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_4) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_5) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_6) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_7) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_8) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_9) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_10) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_11) ) );
        to_add.push_back( wq::string( nl_langinfo(MON_12) ) );
        if( (iter = std::find(m_months_names.begin(), m_months_names.end(), to_add)) == m_months_names.end() ) {
            m_months_names.push_back(to_add);
            new_indexes.m_months = m_months_names.size() - 1;
        }
        else {
            new_indexes.m_months = iter - m_months_names.begin();
        }

        to_add.clear();
        to_add.push_back( wq::string( nl_langinfo(ABMON_1) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_2) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_3) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_4) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_5) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_6) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_7) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_8) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_9) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_10) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_11) ) );
        to_add.push_back( wq::string( nl_langinfo(ABMON_12) ) );
        if( (iter = std::find(m_ab_months_names.begin(), m_ab_months_names.end(), to_add)) == m_ab_months_names.end() ) {
            m_ab_months_names.push_back(to_add);
            new_indexes.m_ab_months = m_ab_months_names.size() - 1;
        }
        else {
            new_indexes.m_ab_months = iter - m_ab_months_names.begin();
        }
#endif
        m_locale_indexes.push_back( new_indexes );
    }
    catch(wq::locale_error& err) {
        return;
    }
}

void locale_gen::put_one_table(const wq::vector<wq::string_list>& table, wq::string& out_str, const wq::string& table_name) {
    char buffer[5];
    sprintf(buffer, "%d", table.front().size());
    out_str = out_str + "const char* locale::wq_data::" + table_name + "[][" + buffer + "] = {\n";
    for(wq::vector<wq::string_list>::const_iterator i = table.begin(); i != table.end(); i++) {
        out_str += "{";
        for(wq::string_list::const_iterator i2 = i->begin(); i2 != i->end(); i2++) {
            out_str = out_str + "\"" + *i2 + "\"";
            wq::string_list::const_iterator tmp = i2;
            if(++tmp != i->end()) {
                out_str += ", ";
            }
        }
        out_str += "}";
        if(i + 1 != table.end()) {
            out_str += ",\n";
        }
    }
    out_str += "\n};";
}

wq::string locale_gen::create_tables() {
    wq::string ret_str;
    char buffer[15];
    ret_str = ret_str + "locale::wq_data::data locale::wq_data::locales[] = {\n";
    for(wq::vector<locale_indexes>::const_iterator iter = m_locale_indexes.begin(); iter != m_locale_indexes.end(); iter++) {
        sprintf(buffer, "{%d, ", iter->m_lang);
        ret_str += buffer;
        sprintf(buffer, "%d, ", iter->m_terr);
        ret_str += buffer;
        sprintf(buffer, "%d, ", iter->m_days);
        ret_str += buffer;
        sprintf(buffer, "%d, ", iter->m_ab_days);
        ret_str += buffer;
        sprintf(buffer, "%d, ", iter->m_months);
        ret_str += buffer;
        sprintf(buffer, "%d}", iter->m_ab_months);
        ret_str += buffer;
        if(iter + 1 != m_locale_indexes.end()) {
            ret_str += ",";
        }
        ret_str += "\n";
    }
    ret_str += "};\n\n";

    put_one_table(m_days_names, ret_str, "days_names");
    ret_str += "\n\n";
    put_one_table(m_ab_days_names, ret_str, "ab_days_names");
    ret_str += "\n\n";
    put_one_table(m_months_names, ret_str, "months_names");
    ret_str += "\n\n";
    put_one_table(m_ab_months_names, ret_str, "ab_months_names");

    return ret_str;
}

// generator for "8bit to utf8" tables
class utf8bit_gen {
    public:
        utf8bit_gen() : m_mapp_arr_pos(0) { };

        void add_line(wq::string);
        wq::string create_table();

    private:
        wq::uint32 m_mapp_arr[0xFF + 1];
        wq::uint32 m_mapp_arr_pos;
};

void utf8bit_gen::add_line(wq::string str) {
    // parsing line data
    std::istringstream line_stream( str.std_str() );
    line_stream.setf(std::ios::hex, std::ios::basefield);
    wq::uint32 bit8_code = 0;
    wq::uint32 uni_code = 0;
    line_stream >> bit8_code >> uni_code;

    // adding data to object's array
    m_mapp_arr[m_mapp_arr_pos] = uni_code;
    m_mapp_arr_pos++;
}

wq::string utf8bit_gen::create_table() {
    wq::string ret_str;

    // shaping data and writing them to output stream
    ret_str += "wq::uint32 array_name[] = {\n";
    for(wq::uint32 i = 0; i != m_mapp_arr_pos; i++) {
        char buffer[100];
        sprintf(buffer, "%d", m_mapp_arr[i]);
        ret_str += buffer;
        if(i != m_mapp_arr_pos - 1) {
            ret_str += ", ";
        }
        if(i % 15 == 0 && i != 0) {
            ret_str += "\n";
        }
    }
    ret_str += "};";
    return ret_str;
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
    if(wq::string(args[1]) == "-u") {
        std::ifstream mapping_file(args[2]);
        if( !mapping_file.is_open() ) {
            std::cout << "Unicode mapping file does not exist." << std::endl;
            return 1;
        }

        // creating new generator
        utf8bit_gen generator;

        // getting data from file
        while( !mapping_file.eof() ) {
            std::string line_str;
            getline(mapping_file, line_str);
            if(line_str.size() == 0 || line_str.at(0) == '#') {
                continue;
            }
            generator.add_line( wq::string(line_str) );
        }
        std::cout << generator.create_table().locale_str() << std::endl;
    }
    if(wq::string(args[1]) == "-l") {
        locale_gen generator;
        generator.add_locale( wq::string() );
        for(int i = 0; i != argc - 2; i++) {
            generator.add_locale( wq::string( args[i + 2] ) );
        }
        std::cout << generator.create_tables().locale_str() << std::endl;
    }

    return 0;
}
