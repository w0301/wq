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

#ifndef WQ_PRIVATE_PTR_H
#define WQ_PRIVATE_PTR_H

#include "wq/defs.h"
#include "wq/core/type_info.h"

namespace wq {
namespace core {

template<class T> class private_ptr {
    public:
        typedef T data_type;
        typedef T pointer_type;
        typedef pointer_type* pointer;
        typedef const pointer const_pointer;

        // creation
        private_ptr(pointer new_data = NULL) : m_ptr(new_data) { };
        private_ptr(const private_ptr& from) : m_ptr(new data_type(*from.data())) { };
        private_ptr& operator= (const private_ptr&);
        private_ptr& operator= (pointer);

        // destruction
        ~private_ptr() {
            if(m_ptr != NULL) {
                delete m_ptr;
            }
        };

        // check existence of data
        bool is_ok() const {
            return m_ptr != NULL;
        };

        // getting data
        pointer data() {
            return m_ptr;
        };
        const pointer data() const {
            return m_ptr;
        };
        const pointer const_data() const {
            return m_ptr;
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

    private:
        pointer m_ptr;
};

template<class T> private_ptr<T>& private_ptr<T>::operator= (const private_ptr<T>& r) {
    if(this != &r) {
        if(m_ptr != NULL) {
            delete m_ptr;
        }
        m_ptr = new data_type( *r.data() );
    }
    return *this;
}

template<class T> private_ptr<T>& private_ptr<T>::operator= (pointer r) {
    if(r != m_ptr) {
        if(m_ptr != NULL) {
            delete m_ptr;
        }
        m_ptr = new data_type(*r);
    }
    return *this;
}

// some defines for declaring private data for classes
#define WQ_PRIVATE_DATA()                                                \
        friend class private_data;                                       \
        private_data *p() {                                              \
            return reinterpret_cast<private_data*>( p_ptr.data() );           \
        };                                                              \
        const private_data *cp() const {                                 \
            return reinterpret_cast<const private_data*>( p_ptr.const_data() );     \
        };                                                              \
        const private_data *p() const {                                  \
            return reinterpret_cast<const private_data*>( p_ptr.data() );     \
        };

#define WQ_NEW_PRIVATE_DATA()                    \
        wq::core::private_ptr<private_data> p_ptr;     \
        typedef private_data base_private_data;       \
        WQ_PRIVATE_DATA();


}  // namespace core
}  // namespace wq

// defining as movable
WQ_MOVABLE_TEMPLATE_TYPE_1(private_ptr);

#endif  // WQ_PRIVATE_PTR_H
