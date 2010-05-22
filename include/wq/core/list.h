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

#ifndef WQ_LIST_H
#define WQ_LIST_H

#include "wq/defs.h"

#include <list>

namespace wq {
namespace core {

template<class T, class Allocator = std::allocator<T> > class list : public std::list<T, Allocator> {
	public:
		typedef typename std::list<T, Allocator>::reference reference;
		typedef typename std::list<T, Allocator>::const_reference const_reference;
		typedef typename std::list<T, Allocator>::iterator iterator;
		typedef typename std::list<T, Allocator>::const_iterator const_iterator;
		typedef typename std::list<T, Allocator>::size_type size_type;
		typedef typename std::list<T, Allocator>::difference_type difference_type;
		typedef typename std::list<T, Allocator>::value_type value_type;
		typedef typename std::list<T, Allocator>::allocator_type allocator_type;
		typedef typename std::list<T, Allocator>::pointer pointer;
		typedef typename std::list<T, Allocator>::const_pointer const_pointer;
		typedef typename std::list<T, Allocator>::reverse_iterator reverse_iterator;
		typedef typename std::list<T, Allocator>::const_reverse_iterator const_reverse_iterator;

		explicit list(const Allocator& al = Allocator()) : std::list<T, Allocator>(al) { };
		explicit list(size_type n, const T& value= T(), const Allocator& al = Allocator()) :
				std::list<T, Allocator>(n, value, al) { };
		template<class InputIterator>
		list(InputIterator first, InputIterator last, const Allocator& al= Allocator()) :
				std::list<T, Allocator>(first, last, al) { };
		list(const list& r) : std::list<T, Allocator>(r) { };

		list& operator= (const list& r) {
			if(&r != this) {
				std::list<T, Allocator>::operator=(r);
			}
			return *this;
		};
};

}  // namespace core
}  // namespace wq


#endif  // WQ_LIST_H
