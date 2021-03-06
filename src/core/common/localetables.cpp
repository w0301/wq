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

#include "wq/core/p/locale.h"

namespace wq {
namespace core {

// names and shortcuts of all languages
const char* locale::wq_data::sm_lang_names[][2] = {
    {"", ""},   // undefined
    {"C", ""},
    {"Abkhazian", "ab"},
    {"Afan", "om"},
    {"Afar", "aa"},
    {"Afrikaans", "af"},
    {"Albanian", "sq"},
    {"Amharic", "am"},
    {"Arabic", "ar"},
    {"Armenian", "hy"},
    {"Assamese", "as"},
    {"Aymara", "ay"},
    {"Azerbaijani", "az"},
    {"Bashkir", "ba"},
    {"Basque", "eu"},
    {"Bengali", "bn"},
    {"Bhutani", "dz"},
    {"Bihari", "bh"},
    {"Bislama", "bi"},
    {"Breton", "br"},
    {"Bulgarian", "bg"},
    {"Burmese", "my"},
    {"Byelorussian", "be"},
    {"Cambodian", "km"},
    {"Catalan", "ca"},
    {"Chinese", "zh"},
    {"Corsican", "co"},
    {"Croatian", "hr"},
    {"Czech", "cs"},
    {"Danish", "da"},
    {"Dutch", "nl"},
    {"English", "en"},
    {"Esperanto", "eo"},
    {"Estonian", "et"},
    {"Faroese", "fo"},
    {"Fijian", "fj"},
    {"Finnish", "fi"},
    {"French", "fr"},
    {"Frisian", "fy"},
    {"Gaelic", "gd"},
    {"Galician", "gl"},
    {"Georgian", "ka"},
    {"German", "de"},
    {"Greek", "el"},
    {"Greenlandic", "kl"},
    {"Guarani", "gn"},
    {"Gujarati", "gu"},
    {"Hausa", "ha"},
    {"Hebrew", "he"},
    {"Hindi", "hi"},
    {"Hungarian", "hu"},
    {"Icelandic", "is"},
    {"Indonesian", "id"},
    {"Interlingua", "ia"},
    {"Interlingue", "ie"},
    {"Inuktitut", "iu"},
    {"Inupiak", "ik"},
    {"Irish", "ga"},
    {"Italian", "it"},
    {"Japanese", "ja"},
    {"Javanese", "jv"},
    {"Kannada", "kn"},
    {"Kashmiri", "ks"},
    {"Kazakh", "kk"},
    {"Kinyarwanda", "rw"},
    {"Kirghiz", "ky"},
    {"Korean", "ko"},
    {"Kurdish", "ku"},
    {"Kurundi", "rn"},
    {"Laothian", "lo"},
    {"Latin", "la"},
    {"Latvian", "lv"},
    {"Lingala", "ln"},
    {"Lithuanian", "lt"},
    {"Macedonian", "mk"},
    {"Malagasy", "mg"},
    {"Malay", "ms"},
    {"Malayalam", "ml"},
    {"Maltese", "mt"},
    {"Maori", "mi"},
    {"Marathi", "mr"},
    {"Moldavian", "mo"},
    {"Mongolian", "mn"},
    {"Nauru", "na"},
    {"Nepali", "ne"},
    {"Norwegian", "nb"},
    {"Occitan", "oc"},
    {"Oriya", "or"},
    {"Pashto", "ps"},
    {"Persian", "fa"},
    {"Polish", "pl"},
    {"Portuguese", "pt"},
    {"Punjabi", "pa"},
    {"Quechua", "qu"},
    {"Rhaeto_romance", "rm"},
    {"Romanian", "ro"},
    {"Russian", "ru"},
    {"Samoan", "sm"},
    {"Sangho", "sg"},
    {"Sanskrit", "sa"},
    {"Serbian", "sr"},
    {"Serbo_croatian", "sh"},
    {"Sesotho", "st"},
    {"Setswana", "tn"},
    {"Shona", "sn"},
    {"Sindhi", "sd"},
    {"Singhalese", "si"},
    {"Siswati", "ss"},
    {"Slovak", "sk"},
    {"Slovenian", "sl"},
    {"Somali", "so"},
    {"Spanish", "es"},
    {"Sundanese", "su"},
    {"Swahili", "sw"},
    {"Swedish", "sv"},
    {"Tagalog", "tl"},
    {"Tajik", "tg"},
    {"Tamil", "ta"},
    {"Tatar", "tt"},
    {"Telugu", "te"},
    {"Thai", "th"},
    {"Tibetan", "bo"},
    {"Tigrinya", "ti"},
    {"Tonga", "to"},
    {"Tsonga", "ts"},
    {"Turkish", "tr"},
    {"Turkmen", "tk"},
    {"Twi", "tw"},
    {"Uigur", "ug"},
    {"Ukrainian", "uk"},
    {"Urdu", "ur"},
    {"Uzbek", "uz"},
    {"Vietnamese", "vi"},
    {"Volapuk", "vo"},
    {"Welsh", "cy"},
    {"Wolof", "wo"},
    {"Xhosa", "xh"},
    {"Yiddish", "yi"},
    {"Yoruba", "yo"},
    {"Zhuang", "za"},
    {"Zulu", "zu"},
    {"Nynorsk", "nn"},
    {"Bosnian", "bs"},
    {"Divehi", "dv"},
    {"Manx", "gv"},
    {"Cornish", "kw"},
    {"Akan", "ak"},
    {"Konkani", "kok"},
    {"Ga", "gaa"},
    {"Igbo", "ig"},
    {"Kamba", "kam"},
    {"Syriac", "syr"},
    {"Blin", "byn"},
    {"Geez", "gez"},
    {"Koro", "kfo"},
    {"Sidamo", "sid"},
    {"Atsam", "cch"},
    {"Tigre", "tig"},
    {"Jju", "kaj"},
    {"Friulian", "fur"},
    {"Venda", "ve"},
    {"Ewe", "ee"},
    {"Walamo", "wa"},
    {"Hawaiian", "haw"},
    {"Tyap", "kcg"},
    {"Chewa", "ny"}
};

// names and shortcuts of all territories
const char* locale::wq_data::sm_terr_names[][2] = {
    {"Any country", ""},
    {"Afghanistan", "AF"},
    {"Albania", "AL"},
    {"Algeria", "DZ"},
    {"American Samoa", "AS"},
    {"Andorra", "AD"},
    {"Angola", "AO"},
    {"Anguilla", "AI"},
    {"Antarctica", "AQ"},
    {"Antigua and Barbuda", "AG"},
    {"Argentina", "AR"},
    {"Armenia", "AM"},
    {"Aruba", "AW"},
    {"Australia", "AU"},
    {"Austria", "AT"},
    {"Azerbaijan", "AZ"},
    {"Bahamas", "BS"},
    {"Bahrain", "BH"},
    {"Bangladesh", "BD"},
    {"Barbados", "BB"},
    {"Belarus", "BY"},
    {"Belgium", "BE"},
    {"Belize", "BZ"},
    {"Benin", "BJ"},
    {"Bermuda", "BM"},
    {"Bhutan", "BT"},
    {"Bolivia", "BO"},
    {"Bosnia and Herzegowina", "BA"},
    {"Botswana", "BW"},
    {"Bouvet Island", "BV"},
    {"Brazil", "BR"},
    {"British Indian Ocean Territory", "IO"},
    {"Brunei Darussalam", "BN"},
    {"Bulgaria", "BG"},
    {"Burkina Faso", "BF"},
    {"Burundi", "BI"},
    {"Cambodia", "KH"},
    {"Cameroon", "CM"},
    {"Canada", "CA"},
    {"Cape Verde", "CV"},
    {"Cayman Islands", "KY"},
    {"Central African Republic", "CF"},
    {"Chad", "TD"},
    {"Chile", "CL"},
    {"China", "CN"},
    {"Christmas Island", "CX"},
    {"Cocos Islands", "CC"},
    {"Colombia", "CO"},
    {"Comoros", "KM"},
    {"Democratic Republic of Congo", "CD"},
    {"Peoples Republic of Congo", "CG"},
    {"Cook Islands", "CK"},
    {"Costa Rica", "CR"},
    {"Ivory Coast", "CI"},
    {"Croatia", "HR"},
    {"Cuba", "CU"},
    {"Cyprus", "CY"},
    {"Czech Republic", "CZ"},
    {"Denmark", "DK"},
    {"Djibouti", "DJ"},
    {"Dominica", "DM"},
    {"Dominican Republic", "DO"},
    {"East Timor", "TL"},
    {"Ecuador", "EC"},
    {"Egypt", "EG"},
    {"El Salvador", "SV"},
    {"Equatorial Guinea", "GQ"},
    {"Eritrea", "ER"},
    {"Estonia", "EE"},
    {"Ethiopia", "ET"},
    {"Falkland Islands", "FK"},
    {"Faroe Islands", "FO"},
    {"Fiji", "FJ"},
    {"Finland", "FI"},
    {"France", "FR"},
    {"Metropolitan France", "FX"},
    {"French Guiana", "GF"},
    {"French Polynesia", "PF"},
    {"French Southern Territories", "TF"},
    {"Gabon", "GA"},
    {"Gambia", "GM"},
    {"Georgia", "GE"},
    {"Germany", "DE"},
    {"Ghana", "GH"},
    {"Gibraltar", "GI"},
    {"Greece", "GR"},
    {"Greenland", "GL"},
    {"Grenada", "GD"},
    {"Guadeloupe", "GP"},
    {"Guam", "GU"},
    {"Guatemala", "GT"},
    {"Guinea", "GN"},
    {"Guinea Bissau", "GW"},
    {"Guyana", "GY"},
    {"Haiti", "HT"},
    {"Heard and Mcdonald Islands", "HM"},
    {"Honduras", "HN"},
    {"Hong Kong", "HK"},
    {"Hungary", "HU"},
    {"Iceland", "IS"},
    {"India", "IN"},
    {"Indonesia", "ID"},
    {"Iran", "IR"},
    {"Iraq", "IQ"},
    {"Ireland", "IE"},
    {"Israel", "IL"},
    {"Italy", "IT"},
    {"Jamaica", "JM"},
    {"Japan", "JP"},
    {"Jordan", "JO"},
    {"Kazakhstan", "KZ"},
    {"Kenya", "KE"},
    {"Kiribati", "KI"},
    {"Democratic Republic of Korea", "KP"},
    {"Republic of Korea", "KR"},
    {"Kuwait", "KW"},
    {"Kyrgyzstan", "KG"},
    {"Lao", "LA"},
    {"Latvia", "LV"},
    {"Lebanon", "LB"},
    {"Lesotho", "LS"},
    {"Liberia", "LR"},
    {"Libyan Arab Jamahiriya", "LY"},
    {"Liechtenstein", "LI"},
    {"Lithuania", "LT"},
    {"Luxembourg", "LU"},
    {"Macau", "MO"},
    {"Macedonia", "MK"},
    {"Madagascar", "MG"},
    {"Malawi", "MW"},
    {"Malaysia", "MY"},
    {"Maldives", "MV"},
    {"Mali", "ML"},
    {"Malta", "MT"},
    {"Marshall Islands", "MH"},
    {"Martinique", "MQ"},
    {"Mauritania", "MR"},
    {"Mauritius", "MU"},
    {"Mayotte", "YT"},
    {"Mexico", "MX"},
    {"Micronesia", "FM"},
    {"Moldova", "MD"},
    {"Monaco", "MC"},
    {"Mongolia", "MN"},
    {"Montserrat", "MS"},
    {"Morocco", "MA"},
    {"Mozambique", "MZ"},
    {"Myanmar", "MM"},
    {"Namibia", "NA"},
    {"Nauru Country", "NR"},
    {"Nepal", "NP"},
    {"Netherlands", "NL"},
    {"Netherlands Antilles", "AN"},
    {"New Caledonia", "NC"},
    {"NewZealand", "NZ"},
    {"Nicaragua", "NI"},
    {"Niger", "NE"},
    {"Nigeria", "NG"},
    {"Niue", "NU"},
    {"Norfolk Island", "NF"},
    {"Northern Mariana Islands", "MP"},
    {"Norway", "NO"},
    {"Oman", "OM"},
    {"Pakistan", "PK"},
    {"Palau", "PW"},
    {"Palestinian Territory", "PS"},
    {"Panama", "PA"},
    {"Papua New Guinea", "PG"},
    {"Paraguay", "PY"},
    {"Peru", "PE"},
    {"Philippines", "PH"},
    {"Pitcairn", "PN"},
    {"Poland", "PL"},
    {"Portugal", "PT"},
    {"Puerto Rico", "PR"},
    {"Qatar", "QA"},
    {"Reunion", "RE"},
    {"Romania", "RO"},
    {"Russian Federation", "RU"},
    {"Rwanda", "RW"},
    {"Saint Kitts and Nevis", "KN"},
    {"Saint Lucia", "LC"},
    {"Saint Vincent and Grenadines", "VC"},
    {"Samoa", "WS"},
    {"San Marino", "SM"},
    {"Sao Tome and Principe", "ST"},
    {"Saudi Arabia", "SA"},
    {"Senegal", "SN"},
    {"Seychelles", "SC"},
    {"Sierra Leone", "SL"},
    {"Singapore", "SG"},
    {"Slovakia", "SK"},
    {"Slovenia", "SI"},
    {"Solomon Islands", "SB"},
    {"Somalia", "SO"},
    {"South Africa", "ZA"},
    {"South Georgia and the South Sandwich Islands", "GS"},
    {"Spain", "ES"},
    {"Sri Lanka", "LK"},
    {"Saint Helena", "SH"},
    {"Saint Pierre and Miquelon", "PM"},
    {"Sudan", "SD"},
    {"Suriname", "SR"},
    {"Svalbard and Jan Mayen Islands", "SJ"},
    {"Swaziland", "SZ"},
    {"Sweden", "SE"},
    {"Switzerland", "CH"},
    {"Syrian Arab Republic", "SY"},
    {"Taiwan", "TW"},
    {"Tajikistan", "TJ"},
    {"Tanzania", "TZ"},
    {"Thailand", "TH"},
    {"Togo", "TG"},
    {"Tokelau", "TK"},
    {"Tonga", "TO"},
    {"Trinidad and Tobago", "TT"},
    {"Tunisia", "TN"},
    {"Turkey", "TR"},
    {"Turkmenistan", "TM"},
    {"Turks and Caicos Islands", "TC"},
    {"Tuvalu", "TV"},
    {"Uganda", "UG"},
    {"Ukraine", "UA"},
    {"United Arab Emirates", "AE"},
    {"United Kingdom", "GB"},
    {"United States of America", "US"},
    {"United States Minor Outlying Islands", "UM"},
    {"Uruguay", "UY"},
    {"Uzbekistan", "UZ"},
    {"Vanuatu", "VU"},
    {"Vatican City State", "VA"},
    {"Venezuela", "VE"},
    {"Viet Nam", "VN"},
    {"British Virgin Islands", "VG"},
    {"US Virgin Islands", "VI"},
    {"Wallis and Futuna Islands", "WF"},
    {"Western Sahara", "EH"},
    {"Yemen", "YE"},
    {"Yugoslavia", "YU"},
    {"Zambia", "ZM"},
    {"Zimbabwe", "ZW"},
    {"Serbia and Montenegro", "CS"}
};



}  // namespace core
}  // namespace wq
