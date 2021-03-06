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

#ifndef WQ_CORE_LOCALE_H
#define WQ_CORE_LOCALE_H

#include "wq/core/defs.h"
#include "wq/core/string.h"
#include "wq/core/auto_ptr.h"

namespace wq {
namespace core {

// exception for locales
class locale_error : public exception {
    public:
        locale_error() throw() : exception() { };
        const char* what() const throw();
};

// class that holds locale spefication
class WQ_EXPORT locale {
    public:
        class wq_data;

        // some enum - starting with capital because of names!
        enum language {
               C = 1,
               Abkhazian = 2,
               Afan = 3,
               Afar = 4,
               Afrikaans = 5,
               Albanian = 6,
               Amharic = 7,
               Arabic = 8,
               Armenian = 9,
               Assamese = 10,
               Aymara = 11,
               Azerbaijani = 12,
               Bashkir = 13,
               Basque = 14,
               Bengali = 15,
               Bhutani = 16,
               Bihari = 17,
               Bislama = 18,
               Breton = 19,
               Bulgarian = 20,
               Burmese = 21,
               Byelorussian = 22,
               Cambodian = 23,
               Catalan = 24,
               Chinese = 25,
               Corsican = 26,
               Croatian = 27,
               Czech = 28,
               Danish = 29,
               Dutch = 30,
               English = 31,
               Esperanto = 32,
               Estonian = 33,
               Faroese = 34,
               Fijian = 35,
               Finnish = 36,
               French = 37,
               Frisian = 38,
               Gaelic = 39,
               Galician = 40,
               Georgian = 41,
               German = 42,
               Greek = 43,
               Greenlandic = 44,
               Guarani = 45,
               Gujarati = 46,
               Hausa = 47,
               Hebrew = 48,
               Hindi = 49,
               Hungarian = 50,
               Icelandic = 51,
               Indonesian = 52,
               Interlingua = 53,
               Interlingue = 54,
               Inuktitut = 55,
               Inupiak = 56,
               Irish = 57,
               Italian = 58,
               Japanese = 59,
               Javanese = 60,
               Kannada = 61,
               Kashmiri = 62,
               Kazakh = 63,
               Kinyarwanda = 64,
               Kirghiz = 65,
               Korean = 66,
               Kurdish = 67,
               Kurundi = 68,
               Laothian = 69,
               Latin = 70,
               Latvian = 71,
               Lingala = 72,
               Lithuanian = 73,
               Macedonian = 74,
               Malagasy = 75,
               Malay = 76,
               Malayalam = 77,
               Maltese = 78,
               Maori = 79,
               Marathi = 80,
               Moldavian = 81,
               Mongolian = 82,
               Nauru = 83,
               Nepali = 84,
               Norwegian = 85,
               Occitan = 86,
               Oriya = 87,
               Pashto = 88,
               Persian = 89,
               Polish = 90,
               Portuguese = 91,
               Punjabi = 92,
               Quechua = 93,
               Rhaeto_romance = 94,
               Romanian = 95,
               Russian = 96,
               Samoan = 97,
               Sangho = 98,
               Sanskrit = 99,
               Serbian = 100,
               Serbo_croatian = 101,
               Sesotho = 102,
               Setswana = 103,
               Shona = 104,
               Sindhi = 105,
               Singhalese = 106,
               Siswati = 107,
               Slovak = 108,
               Slovenian = 109,
               Somali = 110,
               Spanish = 111,
               Sundanese = 112,
               Swahili = 113,
               Swedish = 114,
               Tagalog = 115,
               Tajik = 116,
               Tamil = 117,
               Tatar = 118,
               Telugu = 119,
               Thai = 120,
               Tibetan = 121,
               Tigrinya = 122,
               Tonga_language = 123,
               Tsonga = 124,
               Turkish = 125,
               Turkmen = 126,
               Twi = 127,
               Uigur = 128,
               Ukrainian = 129,
               Urdu = 130,
               Uzbek = 131,
               Vietnamese = 132,
               Volapuk = 133,
               Welsh = 134,
               Wolof = 135,
               Xhosa = 136,
               Yiddish = 137,
               Yoruba = 138,
               Zhuang = 139,
               Zulu = 140,
               Nynorsk = 141,
               Bosnian = 142,
               Divehi = 143,
               Manx = 144,
               Cornish = 145,
               Akan = 146,
               Konkani = 147,
               Ga = 148,
               Igbo = 149,
               Kamba = 150,
               Syriac = 151,
               Blin = 152,
               Geez = 153,
               Koro = 154,
               Sidamo = 155,
               Atsam = 156,
               Tigre = 157,
               Jju = 158,
               Friulian = 159,
               Venda = 160,
               Ewe = 161,
               Walamo = 162,
               Hawaiian = 163,
               Tyap = 164,
               Chewa = 165,
               last_language = Chewa + 1
           };
        enum country {
            any_country = 0,
            Afghanistan = 1,
            Albania = 2,
            Algeria = 3,
            American_Samoa = 4,
            Andorra = 5,
            Angola = 6,
            Anguilla = 7,
            Antarctica = 8,
            Antigua_and_Barbuda = 9,
            Argentina = 10,
            Armenia = 11,
            Aruba = 12,
            Australia = 13,
            Austria = 14,
            Azerbaijan = 15,
            Bahamas = 16,
            Bahrain = 17,
            Bangladesh = 18,
            Barbados = 19,
            Belarus = 20,
            Belgium = 21,
            Belize = 22,
            Benin = 23,
            Bermuda = 24,
            Bhutan = 25,
            Bolivia = 26,
            Bosnia_and_Herzegowina = 27,
            Botswana = 28,
            Bouvet_Island = 29,
            Brazil = 30,
            British_Indian_Ocean_Territory = 31,
            Brunei_Darussalam = 32,
            Bulgaria = 33,
            Burkina_Faso = 34,
            Burundi = 35,
            Cambodia = 36,
            Cameroon = 37,
            Canada = 38,
            Cape_Verde = 39,
            Cayman_Islands = 40,
            Central_African_Republic = 41,
            Chad = 42,
            Chile = 43,
            China = 44,
            Christmas_Island = 45,
            Cocos_Islands = 46,
            Colombia = 47,
            Comoros = 48,
            Democratic_Republic_of_Congo = 49,
            Peoples_Republic_of_Congo = 50,
            Cook_Islands = 51,
            Costa_Rica = 52,
            Ivory_Coast = 53,
            Croatia = 54,
            Cuba = 55,
            Cyprus = 56,
            Czech_Republic = 57,
            Denmark = 58,
            Djibouti = 59,
            Dominica = 60,
            Dominican_Republic = 61,
            East_Timor = 62,
            Ecuador = 63,
            Egypt = 64,
            El_Salvador = 65,
            Equatorial_Guinea = 66,
            Eritrea = 67,
            Estonia = 68,
            Ethiopia = 69,
            Falkland_Islands = 70,
            Faroe_Islands = 71,
            Fiji = 72,
            Finland = 73,
            France = 74,
            Metropolitan_France = 75,
            French_Guiana = 76,
            French_Polynesia = 77,
            French_Southern_Territories = 78,
            Gabon = 79,
            Gambia = 80,
            Georgia = 81,
            Germany = 82,
            Ghana = 83,
            Gibraltar = 84,
            Greece = 85,
            Greenland = 86,
            Grenada = 87,
            Guadeloupe = 88,
            Guam = 89,
            Guatemala = 90,
            Guinea = 91,
            Guinea_Bissau = 92,
            Guyana = 93,
            Haiti = 94,
            Heard_and_Mcdonald_Islands = 95,
            Honduras = 96,
            Hong_Kong = 97,
            Hungary = 98,
            Iceland = 99,
            India = 100,
            Indonesia = 101,
            Iran = 102,
            Iraq = 103,
            Ireland = 104,
            Israel = 105,
            Italy = 106,
            Jamaica = 107,
            Japan = 108,
            Jordan = 109,
            Kazakhstan = 110,
            Kenya = 111,
            Kiribati = 112,
            Democratic_Republic_of_Korea = 113,
            Republic_of_Korea = 114,
            Kuwait = 115,
            Kyrgyzstan = 116,
            Lao = 117,
            Latvia = 118,
            Lebanon = 119,
            Lesotho = 120,
            Liberia = 121,
            Libyan_Arab_Jamahiriya = 122,
            Liechtenstein = 123,
            Lithuania = 124,
            Luxembourg = 125,
            Macau = 126,
            Macedonia = 127,
            Madagascar = 128,
            Malawi = 129,
            Malaysia = 130,
            Maldives = 131,
            Mali = 132,
            Malta = 133,
            Marshall_Islands = 134,
            Martinique = 135,
            Mauritania = 136,
            Mauritius = 137,
            Mayotte = 138,
            Mexico = 139,
            Micronesia = 140,
            Moldova = 141,
            Monaco = 142,
            Mongolia = 143,
            Montserrat = 144,
            Morocco = 145,
            Mozambique = 146,
            Myanmar = 147,
            Namibia = 148,
            Nauru_Country = 149,
            Nepal = 150,
            Netherlands = 151,
            Netherlands_Antilles = 152,
            New_Caledonia = 153,
            NewZealand = 154,
            Nicaragua = 155,
            Niger = 156,
            Nigeria = 157,
            Niue = 158,
            Norfolk_Island = 159,
            Northern_Mariana_Islands = 160,
            Norway = 161,
            Oman = 162,
            Pakistan = 163,
            Palau = 164,
            Palestinian_Territory = 165,
            Panama = 166,
            Papua_New_Guinea = 167,
            Paraguay = 168,
            Peru = 169,
            Philippines = 170,
            Pitcairn = 171,
            Poland = 172,
            Portugal = 173,
            Puerto_Rico = 174,
            Qatar = 175,
            Reunion = 176,
            Romania = 177,
            Russian_Federation = 178,
            Rwanda = 179,
            Saint_Kitts_and_Nevis = 180,
            Saint_Lucia = 181,
            Saint_Vincent_and_Grenadines = 182,
            Samoa = 183,
            San_Marino = 184,
            Sao_Tome_and_Principe = 185,
            Saudi_Arabia = 186,
            Senegal = 187,
            Seychelles = 188,
            Sierra_Leone = 189,
            Singapore = 190,
            Slovakia = 191,
            Slovenia = 192,
            Solomon_Islands = 193,
            Somalia = 194,
            South_Africa = 195,
            South_Georgia_and_the_South_Sandwich_Islands = 196,
            Spain = 197,
            Sri_Lanka = 198,
            Saint_Helena = 199,
            Saint_Pierre_and_Miquelon = 200,
            Sudan = 201,
            Suriname = 202,
            Svalbard_and_Jan_Mayen_Islands = 203,
            Swaziland = 204,
            Sweden = 205,
            Switzerland = 206,
            Syrian_Arab_Republic = 207,
            Taiwan = 208,
            Tajikistan = 209,
            Tanzania = 210,
            Thailand = 211,
            Togo = 212,
            Tokelau = 213,
            Tonga = 214,
            Trinidad_and_Tobago = 215,
            Tunisia = 216,
            Turkey = 217,
            Turkmenistan = 218,
            Turks_and_Caicos_Islands = 219,
            Tuvalu = 220,
            Uganda = 221,
            Ukraine = 222,
            United_Arab_Emirates = 223,
            United_Kingdom = 224,
            United_States_of_America = 225,
            United_States_Minor_Outlying_Islands = 226,
            Uruguay = 227,
            Uzbekistan = 228,
            Vanuatu = 229,
            Vatican_City_State = 230,
            Venezuela = 231,
            Viet_Nam = 232,
            British_Virgin_Islands = 233,
            US_Virgin_Islands = 234,
            Wallis_and_Futuna_Islands = 235,
            Western_Sahara = 236,
            Yemen = 237,
            Yugoslavia = 238,
            Zambia = 239,
            Zimbabwe = 240,
            Serbia_and_Montenegro = 241,
            last_country = Serbia_and_Montenegro + 1
        };

        // creation and assignment
        locale(language = C, country = any_country);
        locale(const string&);
        locale(const locale&);
        locale& operator= (const locale&);

        // destruction
        ~locale();

        // getters of IDes
        language language_id() const;
        country country_id() const;

        // getters of name
        string language_name() const;
        string language_shortcut() const;
        string country_name() const;
        string country_shortcut() const;
        string name() const;
        const string& encoding() const {
            return m_encoding;
        };

        // other getters
        text_encoder* encoder(bool = true) const;

        // get system/default locale
        static locale system_locale();
        static const locale& default_locale() {
            return sm_default_locale;
        };
        static locale initial_default_locale();
        static void set_default_locale(const locale& lc) {
            sm_default_locale = lc;
        };


    protected:
        WQ_NEW_SHARED_DATA();
        WQ_SHARED_DATA_D();

    private:
        // setting of encoding is allow only threw system_locale() function
        string m_encoding;
        void set_encoding(const string& str) {
            m_encoding = str;
        };

        // holds default locale
        static locale sm_default_locale;
};

} // namespace core
} // namespace wq


#endif  // WQ_CORE_LOCALE_H
