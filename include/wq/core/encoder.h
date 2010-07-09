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

#ifndef WQ_ENCODER_H
#define WQ_ENCODER_H

#include "wq/core/defs.h"
#include "wq/core/exception.h"
#include "wq/core/auto_ptr.h"

namespace wq {
namespace core {

class string;
class utf8_encoder;

// exception that indicates error during encoding
class WQ_EXPORT encode_error : public wq::core::exception {
	public:
		encode_error() throw();
		const char* what() const throw();
};

// base class of text encoding classes between 8-bit encodings
class WQ_EXPORT text_encoder {
    public:
        // just abstract class
        text_encoder(bool thexce = true) : m_thexce(thexce)  { };
        text_encoder(const text_encoder& from) : m_thexce(from.is_throwing())  { };
        text_encoder& operator= (const text_encoder& from) {
            m_thexce = from.is_throwing();
            return *this;
        };
        virtual ~text_encoder() { };

        bool is_throwing() const {
            return m_thexce;
        };
        void set_throwing(bool thexce) {
            // TODO: make text_encoder::set_throwing thread-safe
            m_thexce = thexce;
        };

        virtual string encode(const char*, wq::size_t = -1) const;
        virtual char* decode(const string&, wq::size_t* = NULL) const {
            return NULL;
        };

        // manipulating with default encoder
        static const text_encoder& system_encoder(bool = true);
        static const text_encoder& wq_encoder(bool = true);
        static const text_encoder& default_encoder(bool = true);
        static void set_default_encoder(const text_encoder& enc) {
            sm_default_encoder = const_cast<text_encoder&>(enc);
        };

    private:
        bool m_thexce;
        static text_encoder& sm_default_encoder;
};

// encoder for UTF-8 strings
class WQ_EXPORT utf8_encoder : public text_encoder {
    public:
        utf8_encoder(bool thexce = true) : text_encoder(thexce) { };

        virtual ~utf8_encoder() { };

        virtual string encode(const char*, wq::size_t = -1) const;
        virtual char* decode(const string&, wq::size_t* = NULL) const;
};

// encoder for CP1250 strings
class WQ_EXPORT cp1250_encoder : public text_encoder {
    public:
        cp1250_encoder(bool thexce = true) : text_encoder(thexce) { };
        cp1250_encoder(const cp1250_encoder& from) : text_encoder(from)  { };
        cp1250_encoder& operator= (const cp1250_encoder& from) {
            text_encoder::operator= (from);
            return *this;
        };
        virtual ~cp1250_encoder() { };

        virtual string encode(const char*, wq::size_t = -1) const;
        virtual char* decode(const string&, wq::size_t* = NULL) const;

    private:
        static const wq::uint32 sm_mapping_array[];
};

// encoder for ASCII strings
class WQ_EXPORT ascii_encoder : public text_encoder {
    public:
        ascii_encoder(bool thexce = true) : text_encoder(thexce) { };
        ascii_encoder(const ascii_encoder& from) : text_encoder(from)  { };
        ascii_encoder& operator= (const ascii_encoder& from) {
            text_encoder::operator= (from);
            return *this;
        };
        virtual ~ascii_encoder() { };

        virtual string encode(const char*, wq::size_t = -1) const;
        virtual char* decode(const string&, wq::size_t* = NULL) const;
};

// default encoder - just inline function
inline static const text_encoder& default_encoder(bool thexce = true) {
#ifdef WQ_BUILDING
    // if we are building we always use wq default encoding!
    return text_encoder::wq_encoder(thexce);
#else
    return text_encoder::default_encoder(thexce);
#endif
};

}  // namespace core
}  // namespace wq


#endif  // WQ_ENCODER_H
