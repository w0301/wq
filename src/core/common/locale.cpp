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

#include "wq/core/locale.h"
#include "wq/core/p/locale.h"

#include "wq/core/string.h"

#include <clocale>

#ifdef WQ_UNIX
    #include <langinfo.h>
#endif

namespace wq {
namespace core {

// locale::wq_data class
locale::wq_data::wq_data(locale::language lang, locale::country cou) :
        m_lang_index(lang), m_terr_index(cou), m_data_ptr(NULL) {

}

locale::wq_data::wq_data(const string& name) :
        m_lang_index(), m_terr_index(), m_data_ptr(NULL) {
    string::size_type _pos = name.find( string::value_type('_') );
    string lang_code = name.substr(0, _pos);
    string terr_code = name.substr(_pos + 1, name.size() - (_pos + 1));

    // finding language index
    for(wq::ushort i = 0; i != locale::last_language; i++) {
        if(string(sm_lang_names[i][1]).compare(lang_code, false) == 0) {
            m_lang_index = i;
        }
    }

    // finding territory index
    for(wq::ushort i = 0; i != locale::last_country; i++) {
        if(string(sm_terr_names[i][1]).compare(terr_code, false) == 0) {
            m_terr_index = i;
        }
    }
}

locale::wq_data::wq_data(const wq_data& from) :
        m_lang_index(from.m_lang_index), m_terr_index(from.m_terr_index), m_data_ptr(from.m_data_ptr) {

}

string locale::wq_data::system_locale_name() {
    // we have to cache current locale
    const char *tmp_lc = setlocale(LC_ALL, NULL);

    // get system default locale name
    string ret_val = string( setlocale(LC_ALL, ""));
    ret_val = ret_val.substr( 0, ret_val.find( string::value_type('.') ) );

    // renew old locale and return
    setlocale(LC_ALL, tmp_lc);
    return ret_val;
}

string locale::wq_data::system_locale_encoding() {
    // we have to cache current locale
    const char *tmp_lc = setlocale(LC_ALL, NULL);

    // set locale to system's one and get encoding
    setlocale(LC_ALL, "");
    string ret_val;
    #ifdef WQ_UNIX
        ret_val = string( nl_langinfo(CODESET) );
    #endif

    // renew old locale and return
    setlocale(LC_ALL, tmp_lc);
    return ret_val;
}

/*!
    \class locale
    \brief Localization support for library.

    This class use used for converting numbers to its string
    representations. It is also used to get informations about
    current or any other locale.
*/
WQ_SHARED_DATA_I(locale);
locale locale::sm_default_locale = locale::initial_default_locale();

/*!
    \brief Constructs new locale object.

    This constructor constructs new locale object that describes
    locale with language \a lang and with country \a cou.

    \param lang Language of new locale.
    \param cou Territory/country of new locale.
*/
locale::locale(language lang, country cou) :
        d_ptr(new wq_data(lang, cou)), m_encoding("UTF-8") {

}

locale::locale(const string& name) :
        d_ptr(new wq_data(name)), m_encoding("UTF-8") {

}

locale::locale(const locale& from) :
        d_ptr(from.d_ptr), m_encoding(from.m_encoding) {

}

locale& locale::operator= (const locale& r) {
    if(&r != this) {
        m_encoding = r.m_encoding;
        d_ptr = r.d_ptr;
    }
    return *this;
}

locale::~locale() {

}

// getters
locale::language locale::language_id() const {
    return language(d()->m_lang_index);
}

locale::country locale::country_id() const {
    return country(d()->m_terr_index);
}

string locale::language_name() const {
    return string( wq_data::sm_lang_names[d()->m_lang_index][0], utf8_encoder() );
}

string locale::language_shortcut() const {
    return string( wq_data::sm_lang_names[d()->m_lang_index][1], utf8_encoder() );
}

string locale::country_name() const {
    return string( wq_data::sm_terr_names[d()->m_terr_index][0], utf8_encoder() );
}

string locale::country_shortcut() const {
    return string( wq_data::sm_terr_names[d()->m_terr_index][1], utf8_encoder() );
}

string locale::name() const {
    if(d()->m_lang_index == C) {
        return string();
    }
    return language_shortcut() + '_' + country_shortcut();
}

text_encoder* locale::encoder(bool throwing) const {
    string enc = encoding();
    if(enc.compare("UTF-8", false) == 0 || enc.compare("UTF8", false) == 0) {
        return new utf8_encoder(throwing);
    }
    if(enc.compare("cp1250", false) == 0) {
        return new cp1250_encoder(throwing);
    }
    // this is out fallback encoding
    return new ascii_encoder(throwing);
}

// static functions
locale locale::system_locale() {
    locale ret_lc( wq_data::system_locale_name() );
    ret_lc.set_encoding( wq_data::system_locale_encoding() );
    return ret_lc;
}

locale locale::initial_default_locale() {
    locale ret_lc = system_locale();
    ret_lc.set_encoding( string("UTF-8") );
    return ret_lc;
}

} // namespace core
} // namespace wq
