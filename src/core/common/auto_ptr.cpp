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
    \brief Holds auto deallocated pointer.

    You can use this class to hold pointer to any type.
    Please remember that data pointed by pointer will be
    dealloacated after \a auto_ptr object destruction.

    In wq this class is used to hold pointer to internal data of other
    classes in wq libraries. Those data are represented as sub-class
    of the data owner class. The sub-class has to be called \b private_data
    and it's declaration should be in different include file, I recommend to
    place it in sub-directory \b p of your include directory. And those files
    in \b p sub-directory are not released with other include files because
    they are need only by source files that include them to implement \b private_data
    class functions. This system is very good for multiplatform developing - you have
    one source file for every platform (implementing of \b private_data functions) and
    after that you have one common source file that just use \b private_data multiplatform
    functions.

    One can start using this system for it's class by calling WQ_NEW_PRIVATE_DATA()
    macro in protected part of class's body. Or if you have access to
    parent's \b private_data class you can use WQ_PRIVATE_DATA() macro insted and inherit
    your \b private_data class from parent's \b private_data class.

    <h2>Using this system looks like this:</h2>

    In include/foo.h file which is public header of our library:
    \code
    class foo {
        public:
            foo();
            ~foo();

        protected:
            class private_data;     // this is necessary <= macros do not do it!
            WQ_NEW_PRIVATE_DATA();  // if we have access to parents private_data this is
                                    // replaced by WQ_PRIVATE_DATA()
    };
    \endcode

    In include/p/foo.h file which is private header of our library:
    \code
    class foo::private_data {
        public:
            private_data();
            ~private_data();

            int sth_platform_depended();
    };
    \endcode

    In src/unix/foo.cpp file which is source compiled only on unix:
    \code
    #if WQ_UNIX  // to make sure we are on unix

    #include "p/foo.h"
    int foo::private_data::sth_platform_depended() {
        return 123321;  // imagine that this can be compiled only on unix :)
    }

    #endif  // WQ_UNIX
    \endcode

    In src/common/foo.cpp file which is source file compiled on all platform:
    \code
    #include "p/foo.h"
    #include "foo.h"

    // these functions of private_data class can be compile everywhere
    foo::private_data::private_data() {

    }
    foo::private_data::~private_data() {
        // destruction of data should be here
    }

    // now functions of our foo class
    foo::foo() : p_ptr(new private_data) {  // note p_ptr (declared by WQ_NEW_PRIVATE_DATA macro) creation

    }
    foo::~foo() {   // destruction of private_data object in p_ptr is automatic => destructor is empty

    }
    \endcode

    \sa WQ_NEW_PRIVATE_DATA(), WQ_PRIVATE_DATA(), shared_ptr
*/

/*!
    \fn WQ_NEW_PRIVATE_DATA()
    \brief Declares new private data.

    If you use this macro in your class body declaration (usually in protected part) you get
    new \b auto_ptr that points to instant of \b private_data sub-class of your class (you have
    to implement this class too). This macro automatically call WQ_PRIVATE_DATA() macro.
    If you use this macro you do not need access to class's parent's \b private_data class and you do not
    have to inherit your \b private_data from parent's one.

    This macro declare \a p_ptr object of type \b auto_ptr<private_data>.

    relates wq::core::auto_ptr
    \sa auto_ptr, WQ_PRIVATE_DATA()
*/

/*!
    \fn WQ_PRIVATE_DATA()
    \brief Declares private data for class.

    If you use this macro you get functions p(), p() const, cp() const. These functions
    return pointer to object of \b private_data class - constant versions return constant
    pointer.

    Please remember that you have to put declaration of \b private_data class
    somewhere before this macro in class body, most likely in protected section. However
    description of \b private_data class body should be in private header (see auto_ptr).

    \relates wq::core::auto_ptr
    \sa auto_ptr, WQ_NEW_PRIVATE_DATA()
*/
