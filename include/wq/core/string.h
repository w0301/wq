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

#ifndef WQ_STRING_H
#define WQ_STRING_H

#include "wq/defs.h"
#include "wq/core/encoder.h"
#include "wq/core/allocator.h"
#include "wq/core/shared_ptr.h"

#include <iterator>

#if WQ_STD_COMPATIBILITY
	#include <string>
#endif

namespace wq {
namespace core {

// class for handling all strings in wq, with unicode support of course
class WQ_EXPORT string {
	public:
		//! Type of allocator used for string objects.
		typedef wq::core::allocator<char> allocator_type;

		//! Type which handle indexes etc. in string objects.
		typedef allocator_type::size_type size_type;

		//! Type which describe result of math operations over iterators.
		typedef allocator_type::difference_type difference_type;

		// class which holds one unicode character - used only when getting value
		class value_type {
			public:
				// basic construction
				value_type();
				value_type(int val);
				value_type(wq::uint32 val) : m_val(val) { };
				value_type(char, const text_encoder& = default_encoder());
				value_type(const char*, bool = true);
				value_type(wq::uint16, wq::uint16, bool = true);

				// coping
				value_type(const value_type&);
				value_type& operator= (const value_type&);

				// convince functions
				size_type bytes() const {
				    return octets_count(m_val);
				};

                // assignment
				value_type& operator= (const char* c) {
				    return operator= ( value_type(c) );
				}
				value_type& operator= (wq::uint32 val) {
				    return operator= ( value_type(val) );
				};
				value_type& operator= (char c) {
				    return operator= ( value_type(c, default_encoder()) );
				};

                // comparing
				bool operator== (char r) const {
				    return m_val <= 0xFF && m_val == wq::uint32(r);
				};
                bool operator== (wq::uint32 r) const {
                    return m_val == r;
                };
				bool operator== (const char* r) const {
				    return m_val == decode_utf8(r, true);
				};
				bool operator== (const value_type& r) const {
                    return m_val == r.m_val;
                };

				// reverse comparing
                bool operator!= (char r) const {
                    return !(*this == r);
                };
                bool operator!= (wq::uint32 r) const {
                    return !(*this == r);
                };
                bool operator!= (const char* r) const {
                    return !(*this == r);
                };
                bool operator!= (const value_type& r) const {
                    return !((*this) == r);
                };

                // conversion
                char ascii() const;
                const char* utf8() const;
                wq::pair<wq::uint16, wq::uint16> utf16() const;
                wq::uint32 utf32() const {
                    return m_val;
                };

                // implicit conversion
                operator const char*() const {
                    return utf8();
                };

                // special characters
                static value_type repl_char() {
                    return value_type(0xFFFD);
                };
                static value_type delim_char() {
                    return value_type(0x7C);
                };

                // characters properties etc.
                struct uc_properties {
                    ushort category : 8;
                    ushort line_break_class : 8;
                    ushort direction : 8;
                    ushort combining_class :8;
                    ushort joining : 2;
                    signed short digit_value : 6;
                    ushort unicode_version : 4;
                    ushort lower_case_special : 1;
                    ushort upper_case_special : 1;
                    ushort title_case_special : 1;
                    ushort case_fold_special : 1;
                    signed short mirror_diff : 16;
                    signed short lower_case_diff : 16;
                    signed short upper_case_diff : 16;
                    signed short title_case_diff : 16;
                    signed short case_fold_diff : 16;
                    ushort grapheme_break : 8;
                    ushort word_break : 8;
                    ushort sentence_break : 8;
                };
                static const uc_properties* get_uc_properties(wq::uint32);

                // testing character's category
                enum uc_category {
                    no_category,

                    mark_non_spacing,          //   Mn
                    mark_spacing_combining,    //   Mc
                    mark_enclosing,           //   Me

                    number_decimalDigit,      //   Nd
                    number_letter,            //   Nl
                    number_other,             //   No

                    separator_space,          //   Zs
                    separator_line,           //   Zl
                    separator_paragraph,      //   Zp

                    other_control,            //   Cc
                    other_format,             //   Cf
                    other_surrogate,          //   Cs
                    other_privateUse,         //   Co
                    other_not_assigned,        //   Cn

                    letter_uppercase,         //   Lu
                    letter_lowercase,         //   Ll
                    letter_titlecase,         //   Lt
                    letter_modifier,          //   Lm
                    letter_other,             //   Lo

                    punctuation_connector,    //   Pc
                    punctuation_dash,         //   Pd
                    punctuation_open,         //   Ps
                    punctuation_close,        //   Pe
                    punctuation_initial_quote, //   Pi
                    punctuation_final_quote,   //   Pf
                    punctuation_other,        //   Po

                    symbol_math,              //   Sm
                    symbol_currency,          //   Sc
                    symbol_modifier,          //   Sk
                    symbol_other,             //   So

                    punctuation_dask = punctuation_dash // oops
                };
                uc_category category() const {
                    return static_cast<uc_category>(get_uc_properties(m_val)->category);
                };
                bool is_lower() const {
                    return category() == letter_lowercase;
                };
                bool is_upper() const {
                    return category() == letter_uppercase;
                };
                bool is_title() const {
                    return category() == letter_titlecase;
                };

                // getting lower/upper characters etc.
                value_type lower() const {
                    return value_type(m_val + get_uc_properties(m_val)->lower_case_diff);
                };
                value_type upper() const {
                    return value_type(m_val + get_uc_properties(m_val)->upper_case_diff);
                };
                value_type title() const {
                    return value_type(m_val + get_uc_properties(m_val)->title_case_diff);
                };

            private:
                friend class string;

                // decoding functions
                static size_type octets_count(char);
                static size_type octets_count(wq::uint32);
                static wq::uint32 decode_utf8(const char*, bool);
                static wq::uint32 decode_utf16(wq::uint16, wq::uint16, bool);

                // this holds number of unicode character
                wq::uint32 m_val;

                // temporary buffers for convert functions - ends with 0
                mutable char m_utf8tmp[5];

                // properties tables
                static const unsigned short sm_property_trie[];
                static const uc_properties sm_properties[];
		};

	    class iterator;
	    class const_iterator;

	    //! Constant reference type for class.
	    typedef const value_type& const_reference;

	    //! Reference type for class.
	    friend class reference;
	    class reference : public value_type {
	        public:
	            // construction
	            reference(string*, char*);
	            reference(const string*, char*);
	            reference(const reference&);

	            // assignment with replace in string
	            reference& operator= (const reference&);
	            reference& operator= (const_reference);
	            reference& operator= (const char* c) {
	                return operator= ( value_type(c) );
	            };
	            reference& operator= (char c) {
                    return operator= ( value_type(c) );
                };

	        private:
                // to which string will we point
                string* m_owner;

                // and to which character
                char* m_ptr;

                // some functions for iterator and string classes
                friend class string;
                friend class iterator;
                friend class const_iterator;

	            string* owner() {
	                return m_owner;
	            };
	            const string* owner() const {
	                return m_owner;
	            };
                char* ptr() {
                    return m_ptr;
                };
                const char* ptr() const {
                    return m_ptr;
                };
                bool is_first() const {
                    return ptr() == owner()->s()->m_start;
                };
                bool is_last() const {
                    return ptr() == owner()->s()->m_last;
                };
                void rebind(const reference&);
                reference next() const;
                reference prev() const;
		};

		//! Class which represent special iterator.
		class iterator {
			public:
				typedef string::value_type value_type;
				typedef string::reference reference;
				typedef const reference const_reference;
				typedef reference* pointer;
				typedef const reference* const_pointer;
				typedef string::difference_type difference_type;
				typedef std::bidirectional_iterator_tag iterator_category;

				// creation and copying
				iterator(const reference& val) : m_val(val) { };
				iterator(const iterator& from) : m_val(from.m_val) { };
				iterator& operator= (const iterator& r) {
					if(&r != this) {
						// we have to use rebind here!
						m_val.rebind(r.m_val);
					}
					return *this;
				};

				// converting
				pointer operator-> () {
					return &m_val;
				};
				const_pointer operator-> () const {
					return &m_val;
				};
				reference operator* () {
					return m_val;
				};
				const_reference operator* () const {
					return m_val;
				};

				// comparing
				bool operator== (const iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				bool operator== (const const_iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const const_iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const const_iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const const_iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const const_iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const const_iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				// distance of iterators
				difference_type operator- (const iterator& r) const {
				    return operator- ( const_iterator(r) );
				};
				difference_type operator- (const const_iterator&) const;

				// incrementing and decrementing
				// all this operations are const!
				iterator operator+ (size_type) const;
				iterator operator- (size_type) const;
				iterator& operator+= (size_type n) {
					return ( (*this) = ((*this) + n) );
				};
				iterator& operator-= (size_type n) {
					return ( (*this) = ((*this) - n) );
				};
				iterator& operator++ () {
					return ( (*this) += 1 );
				};
				iterator& operator-- () {
					return ( (*this) -= 1 );
				};
				iterator& operator++ (int) {
					return ( (*this) += 1 );
				};
				iterator& operator-- (int) {
					return ( (*this) -= 1 );
				};

			private:
				reference m_val;

                // functions for friends
                friend class string;
                friend class const_iterator;
                char* ptr() {
                    return m_val.ptr();
                };
                const char* ptr() const {
                    return m_val.ptr();
                };
		};

		//! Class which represent special constant iterator.
		class const_iterator {
			public:
                typedef string::value_type value_type;
                typedef string::reference reference;
                typedef const reference const_reference;
                typedef value_type* pointer;
                typedef const value_type* const_pointer;
                typedef string::difference_type difference_type;
                typedef std::bidirectional_iterator_tag iterator_category;

				// creation and copying
				const_iterator(const reference& val) : m_val(val) { };
				const_iterator(const const_iterator& from) : m_val(from.m_val) { };
				const_iterator& operator= (const const_iterator& r) {
					if(&r != this) {
						// we have to use rebind here!
						m_val.rebind(r.m_val);
					}
					return *this;
				};
				const_iterator(const iterator& from) : m_val(from.m_val) { };
				const_iterator& operator= (const iterator& r) {
					// we have to use rebind here!
					m_val.rebind(r.m_val);
					return *this;
				};

				// converting
				const_pointer operator-> () const {
					return &m_val;
				};
				const_reference operator* () const {
					return m_val;
				};

				// comparing
				bool operator== (const iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				bool operator== (const const_iterator& r) const {
					return m_val.ptr() == r.m_val.ptr();
				};
				bool operator!= (const const_iterator& r) const {
					return m_val.ptr() != r.m_val.ptr();
				};
				bool operator< (const const_iterator& r) const {
					return m_val.ptr() < r.m_val.ptr();
				};
				bool operator> (const const_iterator& r) const {
					return m_val.ptr() > r.m_val.ptr();
				};
				bool operator<= (const const_iterator& r) const {
					return m_val.ptr() <= r.m_val.ptr();
				};
				bool operator>= (const const_iterator& r) const {
					return m_val.ptr() >= r.m_val.ptr();
				};

				// distance of iterators
				difference_type operator- (const iterator& r) const {
					return operator- ( const_iterator(r) );
				};
				difference_type operator- (const const_iterator&) const;

				// incrementing and decrementing
				// all this operations are const!
				const_iterator operator+ (size_type) const;
				const_iterator operator- (size_type) const;
				const_iterator& operator+= (size_type n) {
					return ( (*this) = ((*this) + n) );
				};
				const_iterator& operator-= (size_type n) {
					return ( (*this) = ((*this) - n) );
				};
				const_iterator& operator++ () {
					return ( (*this) += 1 );
				};
				const_iterator& operator-- () {
					return ( (*this) -= 1 );
				};
				const_iterator& operator++ (int) {
					return ( (*this) += 1 );
				};
				const_iterator& operator-- (int) {
					return ( (*this) -= 1 );
				};

			private:
				reference m_val;

				// functions for friends
				friend class string;
				friend class iterator;
				const char* ptr() const {
				    return m_val.ptr();
				};
		};

		//! Reverse iterator type.
		typedef std::reverse_iterator<iterator> reverse_iterator;

		//! Constant reverse iterator type.
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		// construction
		string();
		string(const char*, size_type = npos, const text_encoder& = default_encoder());
		string(const string&);
		string(size_type, const_reference);
		string(const_iterator, const_iterator);

        #if WQ_STD_COMPATIBILITY
            string(const std::string&, const text_encoder& = default_encoder());
        #endif

		// destruction
		~string();

	    // assignment operators
		string& operator= (const string&);

		// manipulating with contents
		void clear();

		// size, capacity etc.
		size_type size() const {
			return s()->m_len;
		};
		size_type length() const {
			return size();
		};
		size_type capacity() const {
			return s()->m_end - s()->m_start;
		};
		size_type bytes() const {
			return s()->m_last - s()->m_start;
		};
		size_type max_size() const {
			return s()->m_alloc.max_size();
		};
		bool empty() const {
			return size() == 0;
		};

		// size, capacity manipulation
		void reserve(size_type = 0);
		void resize(size_type, const_reference = value_type());

		// iterators
		iterator begin() {
			return iterator( reference(this, cs()->m_start) );
		};
		iterator end() {
			return iterator( reference(this, cs()->m_last) );
		};
		const_iterator begin() const {
			return const_iterator( reference(this, s()->m_start) );
		};
		const_iterator end() const {
			return const_iterator( reference(this, s()->m_last) );
		};

		// reverse iterators
		reverse_iterator rbegin() {
			return reverse_iterator( end() );
		};
		reverse_iterator rend() {
			return reverse_iterator( begin() );
		};
		const_reverse_iterator rbegin() const {
			return const_reverse_iterator( end() );
		};
		const_reverse_iterator rend() const {
			return const_reverse_iterator( begin() );
		};

		// characters returning
		reference at(size_type);
		const_reference at(size_type) const;
		reference operator[] (size_type i) {
			return at(i);
		};
		const_reference operator[] (size_type i) const {
			return at(i);
		};

		// assigning
		string& assign(const string&, size_type = 0, size_type = npos);
		string& assign(const char* str, size_type size = npos, const text_encoder& enc = default_encoder()) {
		    return assign( string(str, size, enc) );
		};
		string& assign(size_type, const_reference);
		string& assign(const_iterator, const_iterator);

		// appending
		string& append(const string&, size_type = 0, size_type = npos);
		string& append(const char* str, size_type size = npos, const text_encoder& enc = default_encoder()) {
		    return append( string(str, size, enc) );
		};
		string& append(size_type, const_reference);
		string& append(const_iterator, const_iterator);

		// inserting
		string& insert(size_type, const string&, size_type = 0, size_type = npos);
		string& insert(size_type i, const char* str, size_type size = npos, const text_encoder& enc = default_encoder()) {
		    return insert( i, string(str, size, enc) );
		};
		string& insert(size_type i, size_type n, const_reference c) {
		    return insert( i, string(n, c) );
		}
		iterator insert(iterator, const string&, size_type = 0, size_type = npos);
		iterator insert(iterator, const char*, size_type = npos, const text_encoder& = default_encoder());
		iterator insert(iterator, size_type, const_reference);

		// erasing
		string& erase(size_type = 0, size_type = npos);
		iterator erase(iterator);
		iterator erase(iterator, iterator);

		// replacing - index versions (start position + size) and iterators version
		string& replace(size_type, size_type, const string&, size_type = 0, size_type = npos);
		string& replace(size_type from, size_type n, const char* str, size_type size = npos, const text_encoder& enc = default_encoder()) {
		    return replace( from, n, string(str, size, enc) );
		};
		string& replace(size_type from, size_type n, size_type count, const_reference c) {
		    return replace( from, n, string(count, c) );
		};
		string& replace(iterator from, iterator to, const string& str) {
		    return replace(from - begin(), to - from, str);
		};
		string& replace(iterator from, iterator to, const_iterator from2, const_iterator to2) {
		    return replace( from, to, string(from2, to2) );
		};
		string& replace(iterator from, iterator to, const char* str, size_type size = npos, const text_encoder& enc = default_encoder()) {
		    return replace(from - begin(), to - from, str, size, enc);
		};
		string& replace(iterator from, iterator to, size_type count, const_reference c) {
		    return replace(from - begin(), to - from, count, c);
		};

		// comparing
		int compare(size_type, size_type, const string&, size_type = 0, size_type = npos, bool = true) const;
		int compare(const string& with, size_type from = 0, size_type n = npos, bool cs = true) const {
		    return compare(0, npos, with, from, n, cs);
		};
		int compare(const char* str, bool cs = true, const text_encoder& enc = default_encoder()) const {
		    return compare(string(str, npos, enc), 0, npos, cs);
		};
		int compare(size_type from, size_type n, const char* str, size_type size = npos, bool cs = true, const text_encoder& enc = default_encoder()) const {
		    return compare(from, n, string(str, size, enc), 0, npos, cs);
		};

		// finding
		size_type find(const string&, size_type = 0, bool = true) const;
		size_type find(const char* s, size_type pos = 0, bool cs = true, const text_encoder& enc = default_encoder()) const {
		    return find(string(s, npos, enc), pos, cs);
		};
		size_type find(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
		    return find(string(s, n, enc), pos, cs);
		};
		size_type find(value_type c, size_type pos = 0, bool cs = true) const {
		    return find(string(1, c), pos, cs);
		};

	    size_type rfind(const string&, size_type = npos, bool = true) const;
	    size_type rfind(const char* s, size_type pos = npos, bool cs = true, const text_encoder& enc = default_encoder()) const {
	        return rfind(string(s, npos, enc), pos, cs);
	    };
	    size_type rfind(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
	        return rfind(string(s, n, enc), pos, cs);
	    };
	    size_type rfind(value_type c, size_type pos = npos, bool cs = true) const {
	        return rfind(string(1, c), pos, cs);
	    };

        size_type find_first_of(const string&, size_type = 0, bool = true) const;
        size_type find_first_of(const char* s, size_type pos = 0, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_first_of(string(s, npos, enc), pos, cs);
        };
        size_type find_first_of(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_first_of(string(s, n, enc), pos, cs);
        };
        size_type find_first_of(value_type c, size_type pos = 0, bool cs = true) const {
            return find_first_of(string(1, c), pos, cs);
        };

        size_type find_first_not_of(const string&, size_type = 0, bool = true) const;
        size_type find_first_not_of(const char* s, size_type pos = 0, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_first_not_of(string(s, npos, enc), pos, cs);
        };
        size_type find_first_not_of(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_first_not_of(string(s, n, enc), pos, cs);
        };
        size_type find_first_not_of(value_type c, size_type pos = 0, bool cs = true) const {
            return find_first_not_of(string(1, c), pos, cs);
        };

        size_type find_last_of(const string&, size_type = 0, bool = true) const;
        size_type find_last_of(const char* s, size_type pos = 0, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_last_of(string(s, npos, enc), pos, cs);
        };
        size_type find_last_of(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_last_of(string(s, n, enc), pos, cs);
        };
        size_type find_last_of(value_type c, size_type pos = 0, bool cs = true) const {
            return find_last_of(string(1, c), pos, cs);
        };

        size_type find_last_not_of(const string&, size_type = 0, bool = true) const;
        size_type find_last_not_of(const char* s, size_type pos = 0, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_last_not_of(string(s, npos, enc), pos, cs);
        };
        size_type find_last_not_of(const char* s, size_type pos, size_type n, bool cs = true, const text_encoder& enc = default_encoder()) const {
            return find_last_not_of(string(s, n, enc), pos, cs);
        };
        size_type find_last_not_of(value_type c, size_type pos = 0, bool cs = true) const {
            return find_last_not_of(string(1, c), pos, cs);
        };

		// other functions
		size_type copy(char*, size_type, size_type = 0) const;
		void swap(string&);
		string substr(size_type = 0, size_type = -1) const;

		// inline functions and operators for appending data
		void push_back(const_reference c) {
		    append(1, c);
		};
		string& operator+= (const string& str) {
		    return append(str);
		};
		string& operator+= (const char* str) {
		    return append(str);
		};
		string& operator+= (const_reference c) {
		    return append(c);
		};

		string operator+ (const string& str) {
            return string(*this).append(str);
        };
        string operator+ (const char* str) {
            return string(*this).append(str);
        };
        string operator+ (const_reference c) {
            return string(*this).append(c);
        };

		// converting
		const char* any_str(const text_encoder& enc) const {
		    return set_tempbuff( enc.decode(*this) );
		}
		const char* utf8_str(bool err = true) const {
		    return any_str( utf8_encoder(err) );
		};
		const char* c_str(bool err = true) const {
			return utf8_str(err);
		};

		// getters
		const char* data() const {
		    return s()->m_start;
		};
		allocator_type get_allocator() const {
		    return s()->m_alloc;
		};

	private:
		class shared_data {
			public:
				shared_data() : m_start(NULL), m_last(NULL), m_end(NULL), m_len(0) { };
				shared_data(const shared_data&);
				~shared_data();

				char* m_start;
				char* m_last;
				char* m_end;
				size_type m_len;
				static allocator_type m_alloc;
		};

		// temp buffer for *_str functions
		mutable char* m_tempbuff;

		char* set_tempbuff(char* buff) const {
		    if(m_tempbuff != NULL) {
		        s()->m_alloc.deallocate(m_tempbuff);
		    }
		    return (m_tempbuff = buff);
		};

	public:
		//! Constant that indicates greatest possible or automatic size.
		static const size_type npos;

	protected:
		WQ_NEW_SHARED_DATA();
};


}  // namespace core
}  // namespace wq

// define movable types
WQ_MOVABLE_TYPE(string);
WQ_MOVABLE_TYPE(string::value_type);
WQ_MOVABLE_TYPE(string::reference);
WQ_MOVABLE_TYPE(string::iterator);
WQ_MOVABLE_TYPE(string::const_iterator);

#endif  // WQ_STRING_H
