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

#ifndef WQ_P_LOCALE_H
#define WQ_P_LOCALE_H

#include "wq/core/locale.h"

namespace wq {
namespace core {

// private_data for locale class
class WQ_EXPORT locale::private_data {
    public:
        struct data {


        };

        // construction
        private_data(locale::language, locale::country);
        private_data(const string&);
        private_data(const private_data&);

        // getting informations about system locale
        static string system_locale_name();
        static string system_locale_encoding();

    private:
        friend class locale;

        // index of language and territory
        wq::ushort m_lang_index, m_terr_index;

        // pointer to locale data declared in array below
        // if NULL data are taken from system (OS)
        data* m_data_ptr;

        // array that holds all locale::private_data::data
        static const locale::private_data::data sm_locales_data[];

        // array that holds language names (0 - full, 1 - shortcut)
        static const char* sm_lang_names[][2];

        // array that holds country/territory names (0 - full, 1 - shortcut)
        static const char* sm_terr_names[][2];
};


}  // namespace core
}  // namespace wq

#endif  // WQ_P_LOCALE_H
