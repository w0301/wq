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

#include "wq/defs.h"
#include "wq/core/exception.h"
#include "wq/core/shared_ptr.h"

namespace wq {
namespace core {

class string;

// exception that indicates error during encoding
class encode_error : public wq::core::exception {
	public:
		encode_error() throw();
		const char* what() const throw();
};

// base class of text encoding classes between 8-bit encodings
class text_encoder {
    public:
        // just abstract class
        text_encoder(bool thexce = true) : m_thexce(thexce)  { };
        virtual ~text_encoder() { };

        bool is_throwing() const {
            return m_thexce;
        };

        virtual string encode(const char*, wq::size_t = -1) const = 0;
        virtual char* decode(const string&, wq::size_t* = NULL) const = 0;

    private:
        bool m_thexce;
};

// encoder for UTF-8 strings
class utf8_encoder : public text_encoder {
    public:
        utf8_encoder(bool thexce = true) : text_encoder(thexce) { };
        virtual ~utf8_encoder() { };

        virtual string encode(const char*, wq::size_t = -1) const;
        virtual char* decode(const string&, wq::size_t* = NULL) const;
};


}  // namespace core
}  // namespace wq


#endif  // WQ_ENCODER_H
