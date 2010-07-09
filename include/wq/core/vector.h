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

#ifndef WQ_VECTOR_H
#define WQ_VECTOR_H

#include "wq/core/defs.h"

#include <vector>

namespace wq {
namespace core {

template<class T, class Allocator = std::allocator<T> > class vector : public std::vector<T, Allocator> {
	public:
		typedef typename std::vector<T, Allocator>::reference reference;
		typedef typename std::vector<T, Allocator>::const_reference const_reference;
		typedef typename std::vector<T, Allocator>::iterator iterator;
		typedef typename std::vector<T, Allocator>::const_iterator const_iterator;
		typedef typename std::vector<T, Allocator>::size_type size_type;
		typedef typename std::vector<T, Allocator>::difference_type difference_type;
		typedef typename std::vector<T, Allocator>::value_type value_type;
		typedef typename std::vector<T, Allocator>::allocator_type allocator_type;
		typedef typename std::vector<T, Allocator>::pointer pointer;
		typedef typename std::vector<T, Allocator>::const_pointer const_pointer;
		typedef typename std::vector<T, Allocator>::reverse_iterator reverse_iterator;
		typedef typename std::vector<T, Allocator>::const_reverse_iterator const_reverse_iterator;

		explicit vector(const Allocator& al = Allocator()) : std::vector<T, Allocator>(al) { };
		explicit vector(size_type n, const T& value= T(), const Allocator& al = Allocator()) :
				std::vector<T, Allocator>(n, value, al) { };
		template<class InputIterator>
				vector(InputIterator first, InputIterator last, const Allocator& al= Allocator()) :
				std::vector<T, Allocator>(first, last, al) { };
		vector(const vector& r) : std::vector<T, Allocator>(r) { };

		vector& operator= (const vector& r) {
			if(&r != this) {
				std::vector<T, Allocator>::operator=(r);
			}
			return *this;
		};
};

}  // namespace core
}  // namespace wq


#endif  // WQ_VECTOR_H
