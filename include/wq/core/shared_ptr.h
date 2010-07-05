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

#ifndef WQ_SHARED_PTR_H
#define WQ_SHARED_PTR_H

#include "wq/defs.h"
#include "wq/core/atomic.h"
#include "wq/core/type_info.h"

namespace wq {
namespace core {

// class for handling implicitly shared pieces of data
template<class T> class shared_ptr {
	public:
        typedef T data_type;
        typedef T pointer_type;
		typedef pointer_type* pointer;
		typedef const pointer const_pointer;

		// creation
		shared_ptr() : m_ptr(NULL), m_count(NULL) { };
		shared_ptr(pointer new_data) : m_ptr(NULL), m_count(NULL) {
			set_data(new_data);
		};
		shared_ptr& operator= (pointer r) {
			set_data(r);
			return *this;
		};
		shared_ptr(const shared_ptr& from) : m_ptr(NULL), m_count(NULL) {
			set_data(from);
		};
		shared_ptr& operator= (const shared_ptr& r) {
			if(this != &r && m_ptr != r.m_ptr) {
				set_data(r);
			}
			return *this;
		};

		// destruction
		~shared_ptr() {
			unset_data();
		};

		// check existence of data
		bool is_ok() const {
			return m_ptr != NULL && m_count != NULL;
		};

		// getting data
		pointer data() {
			unshare_data();
			return m_ptr->val();
		};
		const pointer data() const {
			return m_ptr->val();
		};
		const pointer const_data() const {
			return m_ptr->val();
		};
		pointer operator-> () {
			return data();
		};
		const pointer operator-> () const {
			return data();
		};
		data_type& operator* () {
			return *data();
		};
		const data_type& operator* () const {
			return *data();
		};

		// getting data owners count
		int data_count() const {
			return is_ok() ? m_count->val() : 0;
		};

		// setting/unsetting data
		void set_data(pointer);
		void set_data(const shared_ptr&);
		void unset_data();

		// making data unique
		void unshare_data();

	private:
		// atomic classes which handle shared pointer
		wq::core::atomic<pointer>* m_ptr;
		wq::core::atomic<int>* m_count;
};

template<class T> void shared_ptr<T>::set_data(pointer new_ptr) {
	unset_data();

	// creating new data handlers and assigning pointer
	m_ptr = new wq::core::atomic<pointer>(new_ptr);
	m_count = new wq::core::atomic<int>(1);
}

template<class T> void shared_ptr<T>::set_data(const shared_ptr& from) {
	unset_data();

	if(from.is_ok()) {
		from.m_count->inc();
		m_count = from.m_count;
		m_ptr = from.m_ptr;
	}
}

template<class T> void shared_ptr<T>::unset_data() {
	if(is_ok()) {
		// atomic decrement and optionally deleting
		m_count->dec();
		if(data_count() == 0) {
			delete m_ptr;
			delete m_count;
		}
		m_ptr = NULL;
		m_count = NULL;
	}
}

template<class T> void shared_ptr<T>::unshare_data() {
	if(is_ok() && data_count() > 1) {
		// we have to copy existing data and set them to shared_ptr
		pointer new_data = new data_type( *( (*m_ptr).val() ) );
		set_data(new_data);
	}
}

// some defines for declaring data for classes
#define WQ_SHARED_DATA()                                                \
        friend class shared_data;                                       \
        shared_data *s() {                                              \
            return static_cast<shared_data*>( s_ptr.data() );           \
        };                                                              \
        const shared_data *cs() const {                                 \
            return static_cast<const shared_data*>( s_ptr.const_data() );     \
        };                                                              \
        const shared_data *s() const {                                  \
            return static_cast<const shared_data*>( s_ptr.data() );     \
        };

#define WQ_NEW_SHARED_DATA()                    \
        wq::core::shared_ptr<shared_data> s_ptr;     \
        typedef shared_data base_shared_data;		\
		WQ_SHARED_DATA();

}  // namespace core
}  // namespace wq

// defining as movable
WQ_MOVABLE_TEMPLATE_TYPE_1(shared_ptr);

#endif  // WQ_SHARED_PTR_H
