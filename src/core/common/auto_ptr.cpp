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

/*!
    \class wq::core::auto_ptr
    \brief Auto deallocated pointer.

    This class implement date which are guarded by reference
    counting. This guard gives guaranty that data are deallocated
    only if there is no auto_ptr object that points to them. Data
    are also shared and are dispatched only if necessary this prohibit
    pointless copying of possibly big pieces of data.

    This class is totally thread safe, that means that reference
    counting used by this class is thread safe too (it's provided by
    atomic<int> and atomic<T*> classes).

    \sa atomic<int>, atomic<T*>
*/

/*!
    \fn wq::core::auto_ptr<T>::auto_ptr()
    \brief Creates an empty object.

    This constructor creates auto_ptr object that
    does not point to any date. If you want
    to set date use assignment operator or
    set() function.

    \sa set()
*/

/*!
    \fn wq::core::auto_ptr<T>::auto_ptr(pointer ptr)
    \brief Creates object with pointer.

    This constructor creates auto_ptr object that
    does point to date under \a ptr. Pointer \a ptr is
    not dereferenced and copied it's just assign to
    object's internal pointer. Function owners_count()
    returns \b 1 after this constructor.

    \param ptr New pointer that will be in \a this object.
    \sa set(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::auto_ptr(const auto_ptr& from)
    \brief Copy constructor.

    This constructor creates auto_ptr object that takes
    pointer from object \a from. This constructor do not
    copy data pointed by \a from's pointer it just assign
    \a from's pointer and increment pointer's reference
    counter.

    \param from Object which datas will be assigned.
    \sa set(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::operator=(pointer ptr)
    \brief Assignment operator.

    This operator unset current pointer by calling function
    unset() and after that assign new pointer \a ptr. After
    calling this operator owners_count() function returns \b 1.

    \param ptr Pointer that will be assigned.
    \sa set(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::operator=(const auto_ptr& from)
    \brief Assignment operator.

    This operator unset current data by calling function
    unset() and after that assign date from object \a from.
    Data pointed by \a from's pointer are not copied, they are only
    assigned and data's reference counter is incremented.

    \param from Object which pointer will be assigned.
    \sa set(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::~auto_ptr()
    \brief Destruct object.

    This destructor simply call function unset() which
    decrement number of data's references and if there
    is no references left it deletes data too.

    \sa unset()
*/

/*!
    \fn  wq::core::auto_ptr<T>::is_ok() const
    \brief Check existence of data.

    This function check if any data are assigned to object.

    \return \b True if data are assigned and \b false otherwise.
*/

/*!
    \fn  wq::core::auto_ptr<T>::data()
    \brief Returns pointer to data.

    This function calls function detach() to make sure
    that current data has only 1 owner and after that returns
    pointer to current data.

    \sa detach(), const_data(), data() const
*/

/*!
    \fn  wq::core::auto_ptr<T>::const_data() const
    \brief Returns pointer to data.

    This function returns constant pointer to current data without
    calling detach() function.

    \sa data() const, data()
*/

/*!
    \fn  wq::core::auto_ptr<T>::data() const
    \brief Returns pointer to data.

    This function returns constant pointer to current data without
    calling detach() function. This is equivalent to const_data()
    function.

    \sa const_data(), data()
*/

/*!
    \fn  wq::core::auto_ptr<T>::operator-> ()
    \brief Returns pointer to data.

    This operator call detach() function and
    returns pointer to current data. This operator allows this
    behavior of auto_ptr objects:
    \code
        wq::core::auto_ptr<my_class> ptr = new my_class;
        ptr->nonconst_func_in_my_class();
    \endcode

    \sa operator-> (), data(), data() const
*/

/*!
    \fn  wq::core::auto_ptr<T>::operator-> () const
    \brief Returns pointer to data.

    This operator returns constant pointer to current data without
    calling detach() function. This operator allows this
    behavior of auto_ptr objects:
    \code
        wq::core::auto_ptr<my_class> ptr = new my_class;
        ptr->const_func_in_my_class();
    \endcode

    \sa operator-> (), data(), data() const
*/

/*!
    \fn  wq::core::auto_ptr<T>::operator* ()
    \brief Returns reference to data.

    This operator call detach() function and
    returns reference to current data. This operator allows this
    behavior of auto_ptr objects:
    \code
        wq::core::auto_ptr<my_class> ptr = new my_class;
        func_with_my_class_reference_parameter( *ptr );
    \endcode

    \sa operator-> (), data(), data() const
*/

/*!
    \fn  wq::core::auto_ptr<T>::operator* () const
    \brief Returns reference to data.

    This operator returns constant reference to current data without
    calling detach() function. This operator allows this
    behavior of auto_ptr objects:
    \code
        wq::core::auto_ptr<my_class> ptr = new my_class;
        func_with_my_class_const_reference_parameter( *ptr );
    \endcode

    \sa operator-> (), data(), data() const
*/

/*!
    \fn wq::core::auto_ptr<T>::owners_count() const
    \brief Number of references.

    This function returns number of references of current
    data.

    \sa detach()
*/

/*!
    \fn wq::core::auto_ptr<T>::set(pointer ptr)
    \brief Set current data.

    This function calls unset() function and after
    that it sets new data by assigning \a ptr to
    object's data pointer. Function owners_count()
    returns \b 1 after that.

    \warning Data pointed by \a ptr are not copied, pointer is assigned!

    \sa detach(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::set(const auto_ptr& from)
    \brief Set current data.

    This function calls unset() function and after
    that it sets new data by assigning \a from's data
    pointer to \a this object's data pointer. This functions
    also increment data's reference counter.

    \sa detach(), unset()
*/

/*!
    \fn wq::core::auto_ptr<T>::unset()
    \brief Unset data.

    This function decrements current data's reference
    counter and if it's \b 0 after this decrementing this
    function deletes data too. When no data are assigned this
    function does nothing.

    \sa set()
*/

/*!
    \fn  wq::core::auto_ptr<T>::detach()
    \brief Detach current data.

    This function checks number of current data's
    references and if this number is greater than 1
    it makes new data by copying old one and also makes
    new reference counter for new data.

    \sa data()
*/

/*!
    \fn WQ_NEW_SHARED_DATA()
    \relates wq::core::auto_ptr
    \brief Declares new shared data.

    If you use this macro in your class body declaration (usually in protected part) you get
    new \b auto_ptr object that points to instant of \b wq_date sub-class of your class (you have
    to implement this class too).

    New auto_ptr<wq_date> object have name \a d_ptr and you have to
    initialize it in your's class constructors.

    \sa wq::core::auto_ptr, auto_ptr_use, WQ_SHARED_DATA(), WQ_SHARED_DATA_D(), WQ_SHARED_DATA_I()
*/

/*!
    \fn WQ_SHARED_DATA()
    \relates wq::core::auto_ptr
    \brief Declares shared data for class.

    If you use this macro you get inline functions d(), d() const, cd() const. These functions
    return pointer to object of type \b wq_date class - constant versions return constant
    pointer.

    Please remember that you have to put declaration of \b wq_date class
    somewhere before this macro in class body, most likely in protected section.
    And declarations of \b wq_date mustn't be forward - wq_data's class body have
    to be in same file like this macro. To use forward declaration see macros
    WQ_SHARED_DATA_D() and WQ_SHARED_DATA_I().

    \sa wq::core::auto_ptr, auto_ptr_use, WQ_NEW_SHARED_DATA(), WQ_SHARED_DATA_D(), WQ_SHARED_DATA_I()
*/

/*!
    \fn WQ_SHARED_DATA_D()
    \relates wq::core::auto_ptr
    \brief Declares shared data for class.

    This macro is first part of non-inline version of macro WQ_SHARED_DATA().
    Using this macro you get same functions like after using WQ_SHARED_DATA() macro
    however you have to use WQ_SHARED_DATA_I() macro somewhere in source file.

    \sa wq::core::auto_ptr, auto_ptr_use, WQ_NEW_SHARED_DATA(), WQ_SHARED_DATA(), WQ_SHARED_DATA_I()
*/

/*!
    \fn WQ_SHARED_DATA_I(class_name)
    \relates wq::core::auto_ptr
    \brief Implement shared data for class.

    This macro is second part of non-inline version of macro WQ_SHARED_DATA().
    This macro implements functions declared by macro WQ_SHARED_DATA_D(). You
    have to use this macro in source file and use name of your class as only
    parameter of it.

    \sa wq::core::auto_ptr, auto_ptr_use, WQ_NEW_SHARED_DATA(), WQ_SHARED_DATA(), WQ_SHARED_DATA_D()
*/

/*!
    \page auto_ptr_use Thousands ways how to use wq::core::auto_ptr<> class.

    Class wq::core::auto_ptr is class for holding pointers that are automatically
    deallocated, learn more in class description. This class is so wonderful that
    it is used in one of the key features of wq framework.

    \section shared_data Implicit shared data
    This mechanism use used to achieve fast copy of objects. If you
    use this mechanism for your classes, you will get almost immediate
    copy constructor and assignment operator returns. This speed is achieved
    by differencing of copies to "shallow copy" and "deep copy". Shallow copy
    is performed when calling copy constructor and/or assignment operator.
    Shallow copying means that only pointer to data class is copied. Deep copy
    is performed when data are suppose to be changed - in all non-const functions
    that dereference shared data pointer. Deep copying means that copy constructor
    of shared data class is called and new instance of shared data class is created.

    \subsection shared_data_impl Implementing implicit shared data for own class
    Implementing of shared data mechanism for own classes is very easy.
    If you want to do so you just have to use few macros and declare one
    class. But a picture is worth a thousand words:\n
    In "include/foo.h" file:
    \code
    #include "wq/core/auto_ptr.h"

    // declaration of class that use implicit shared data
    class foo {
        public:
            foo();
            foo(const foo&);
            foo& operator= (const foo&);

        protected:
            // class that contains all shared variables etc.
            class wq_data {
                public:
                    // this constructor will be called by hand,
                    // so it don't have to be without parameters
                    wq_data();

                    // copy constructor is necessity because
                    // it is needed by auto_ptr class
                    wq_data(const wq_data&);

                    // destructor have to delete all data
                    // allocated by this class
                    ~wq_data();

                    // pointer to very big piece of data
                    char* m_data;
            }

            // declaring new auto_ptr object which points
            // to wq_data object, this is needed because
            // we don't have any ancestor that declares wq_data
            // class, if we have such ancestor class we can
            // inherit our wq_data class from ancestor's one
            // and don't use WQ_NEW_SHARED_DATA macro
            WQ_NEW_SHARED_DATA();

            // declaring functions that are used to
            // return direct pointers to wq_data object
            // this macro declare those functions as inline
            // functions to make them normal use macro pair
            // WQ_SHARED_DATA_D() and in source file WQ_SHARED_DATA_I(class_name)
            WQ_SHARED_DATA();
    };
    \endcode
    In "src/foo.h" file:
    \code
    #include "foo.h"

    //- foo::wq_data class implementation
    foo::wq_data::wq_data() : m_data(NULL) {
        // creating of data comes here
        // for example: m_data = new char[10];
    }
    foo::wq_data::wq_data(const wq_data& from) : m_data(NULL) {
        // deep copy of data is performed here
        // for example: strcpy(m_data, from.m_data, 10);
    }
    foo::wq_data::~wq_data() {
        // data are deallocated here
        // for example: delete []m_data;
    }

    //- foo class implementation
    // notice 'd_ptr' initializing, this can be
    // and have to be done only if WQ_NEW_SHARED_DATA
    // macro was called otherwise this will be called by
    // ancestor constructor which will be called here
    foo::foo() : d_ptr(new wq_data) {

    }

    // copy constructor of d_ptr is called, this
    // constructor perform shallow copy only
    foo::foo(const foo& from) : d_ptr( from.d_ptr ) {

    }

    // assignment operator of d_ptr is called, this
    // operator perform shallow copy only
    foo& foo::operator= (const foo& from) {
        if(&from != this) {
            d_ptr = from.d_ptr;
        }
        return *this;
    }

    // 'd_ptr' is declared as static object
    // that means that we don't have to call
    // any delete operator in destructor
    foo::~foo() {

    }
    \endcode

    \subsection shared_private_data_impl Implementing private implicit shared data for own class
    Making your shared data private means that declaration of wq_data class
    won't be visible for user of your class. This is achieved by declaring
    wq_data's class body in different header file which won't be released
    with other library's header files, of course this header file have to
    be included only in source files of your library. In wq there is 'p/'
    subdirectory in all include directories for these private headers of
    the same name like public one. I propose you to use same system.
    Using this system have few advantages: smaller compilation time, better
    binary compatibility and some others. However it has some disadvantages:
    you have to use WQ_SHARED_DATA_D() macro in class body and WQ_SHARED_DATA_I(class_name)
    in source file and your class mustn't have inline destructor.



    \sa WQ_NEW_SHARED_DATA(), WQ_SHARED_DATA(), wq::core::auto_ptr
*/
