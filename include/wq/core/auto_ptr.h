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

#ifndef WQ_CORE_AUTO_PTR_H
#define WQ_CORE_AUTO_PTR_H

#include "wq/core/defs.h"
#include "wq/core/atomic.h"
#include "wq/core/type_info.h"

namespace wq {
namespace core {

// class for handling implicitly shared pieces of data
template<class T> class auto_ptr {
	public:
        typedef T data_type;
        typedef T pointer_type;
		typedef pointer_type* pointer;
		typedef const pointer const_pointer;

		// creation
		auto_ptr() : m_ptr(NULL), m_count(NULL) { };
		auto_ptr(pointer new_data) : m_ptr(NULL), m_count(NULL) {
			set(new_data);
		};
		auto_ptr& operator= (pointer r) {
			set(r);
			return *this;
		};
		auto_ptr(const auto_ptr& from) : m_ptr(NULL), m_count(NULL) {
			set(from);
		};
		auto_ptr& operator= (const auto_ptr& r) {
			if(this != &r && m_ptr != r.m_ptr) {
				set(r);
			}
			return *this;
		};

		// destruction
		~auto_ptr() {
			unset();
		};

		// check existence of data and other checks.
		bool is_ok() const {
			return m_ptr != NULL && m_count != NULL;
		};

		// getting data
		pointer data() {
	        detach();
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
		int owners_count() const {
			return is_ok() ? m_count->val() : 0;
		};

		// setting/unsetting data
		void set(pointer);
		void set(const auto_ptr&);
		void unset();

		// making data unique
		void detach();

	private:
		// atomic classes which handle shared pointer
		wq::core::atomic<pointer>* m_ptr;
		wq::core::atomic<int>* m_count;
};

template<class T> void auto_ptr<T>::set(pointer new_ptr) {
	unset();

	// creating new data handlers and assigning pointer
	m_ptr = new wq::core::atomic<pointer>(new_ptr);
	m_count = new wq::core::atomic<int>(1);
}

template<class T> void auto_ptr<T>::set(const auto_ptr& from) {
	unset();

	if(from.is_ok()) {
		from.m_count->inc();
		m_count = from.m_count;
		m_ptr = from.m_ptr;
	}
}

template<class T> void auto_ptr<T>::unset() {
	if(is_ok()) {
		// atomic decrement and optionally deleting
		m_count->dec();
		if(owners_count() == 0) {
			delete m_ptr;
			delete m_count;
		}
		m_ptr = NULL;
		m_count = NULL;
	}
}

template<class T> void auto_ptr<T>::detach() {
	if(is_ok() && owners_count() > 1) {
		// we have to copy existing data and set them to auto_ptr
		pointer new_data = new data_type( *( (*m_ptr).val() ) );
		set(new_data);
	}
}

// defines for using wq internal data classes etc.
#define WQ_NEW_SHARED_DATA() ::wq::core::auto_ptr<wq_data> d_ptr;
#define WQ_SHARED_DATA()                                                \
    friend class wq_data;                                       \
    inline wq_data* d() {                                              \
        return static_cast<wq_data*>( d_ptr.data() );           \
    };                                                              \
    inline const wq_data* cd() const {                                 \
        return static_cast<const wq_data*>( d_ptr.const_data() );     \
    };                                                              \
    inline const wq_data* d() const {                                  \
        return static_cast<const wq_data*>( d_ptr.data() );     \
    };

#define WQ_SHARED_DATA_D()                      \
    friend class wq_data;                \
    wq_data* d();                        \
    const wq_data* cd() const;           \
    const wq_data* d() const;

#define WQ_SHARED_DATA_I(cls)                                                \
    cls::wq_data* cls::d() {                                              \
        return static_cast<wq_data*>( d_ptr.data() );           \
    };                                                              \
    const cls::wq_data* cls::cd() const {                                 \
        return static_cast<const wq_data*>( d_ptr.const_data() );     \
    };                                                              \
    const cls::wq_data* cls::d() const {                                  \
        return static_cast<const wq_data*>( d_ptr.data() );     \
    };

}  // namespace core
}  // namespace wq

// defining as movable
WQ_MOVABLE_TEMPLATE_TYPE_1(auto_ptr);

#endif  // WQ_CORE_AUTO_PTR_H
