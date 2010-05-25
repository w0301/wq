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

namespace wq {
namespace core {

// exception that indicates error during encoding
class encode_error : public wq::core::exception {
	public:
		encode_error() throw();
		const char* what() const throw();
};


}  // namespace core
}  // namespace wq


#endif  // WQ_ENCODER_H
