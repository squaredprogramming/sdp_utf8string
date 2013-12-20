// utf8string.h
// Copyright (c) 2013, Dominque A Douglas
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
//    in the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//
// squaredprogramming.blogspot.com
//
#pragma once

#ifndef UTF8STRINGHEADER
#define UTF8STRINGHEADER

#define SAFE_SIGNED_STRINGS

#include <memory.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdint>

#include "utf8utils.h"

namespace sd_utf8
{

template <class Alloc = std::allocator<_uchar8bit>>
class _utf8string
{
	public:
		// some types that we need to define to make this work like an stl object
		// internally this is an std string, but outwardly, it returns __char32bit
		typedef _char32bit			value_type;
		typedef _char32bit			*pointer;
		typedef const _char32bit	*const_pointer;
		typedef _char32bit			&reference;
		typedef const _char32bit	&const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;

		// declare our iterator
		// declare as template class so we don't have to write everything twice for the const_iterator
		template <class TBaseIterator>
		class value_reverse_iterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
		{
			public:
				TBaseIterator forward_iterator;

			public:
				// copy constructor
				value_reverse_iterator(const value_reverse_iterator &other)
					:forward_iterator(other.forward_iterator)
				{
				}

				// create from forward iterator
				value_reverse_iterator(const TBaseIterator &iterator)
					:forward_iterator(iterator)
				{
					int a;
					a = 5;
				}

				value_type operator*() const
				{
					TBaseIterator temp = forward_iterator;
					return *(--temp);
				}

				// does not check to see if it goes past the end
				// iterating past the end is undefined
				value_reverse_iterator &operator++()
				{
					--forward_iterator;

					return *this;
				}

				// does not check to see if it goes past the end
				// iterating past the end is undefined
				value_reverse_iterator operator++(int)
				{
					value_reverse_iterator copy(*this);

					// increment
					--forward_iterator;

					return copy;
				}

				// does not check to see if it goes past the end
				// iterating past begin is undefined
				value_reverse_iterator &operator--()
				{
					++forward_iterator;
					return *this;
				}

				// does not check to see if it goes past the end
				// iterating past begin is undefined
				value_reverse_iterator operator--(int)
				{
					value_reverse_iterator copy(*this);

					++forward_iterator;

					return copy;
				}

				bool operator == ( const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator == other.forward_iterator;
				}
 
				bool operator != (const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator != other.forward_iterator;
				}

				bool operator < ( const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator > other.forward_iterator;
				}
 
				bool operator > (const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator < other.forward_iterator;
				}
		
				bool operator <= ( const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator >= other.forward_iterator;
				}
 
				bool operator >= (const value_reverse_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return forward_iterator <= other.forward_iterator;
				}

		};

		template <class Ty>
		class utf8string_iterator : public std::iterator<std::bidirectional_iterator_tag, Ty>
		{
			private:
				const _uchar8bit *utf8string_buf;

				void inc()
				{
					// increments the iterator by one
					// result in undefined behavior (crashes) if already at the end 
					IncToNextCharacter(utf8string_buf);
				}

				void dec()
				{
					// decrements the iterator by one
					// result in undefined behavior (crashes) if already at the beginning
					DecToNextCharacter(utf8string_buf);
				}

			public:
				// b should be a null terminated string in UTF-8
				// if this is the end start_pos should be the index of the null terminator
				// start_pos should be the valid start of a character
				utf8string_iterator(const _uchar8bit *b, size_type start_pos)
					:utf8string_buf(b)
				{
					IncrementToPosition(utf8string_buf, start_pos);
				}

				// copy constructor
				utf8string_iterator(const utf8string_iterator &other)
					:utf8string_buf(other.utf8string_buf)
				{
				}

				// b should already point to the correct position in the string
				utf8string_iterator(const _uchar8bit *b)
					:utf8string_buf(b)
				{
				}

				value_type operator*() const
				{
					// returns the character currently being pointed to
					return UTF8CharToUnicode(utf8string_buf);
				}

				// does not check to see if it goes past the end
				// iterating past the end is undefined
				utf8string_iterator &operator++()
				{
					inc();

					return *this;
				}

				// does not check to see if it goes past the end
				// iterating past the end is undefined
				utf8string_iterator operator++(int)
				{
					utf8string_iterator copy(*this);

					// increment
					inc();

					return copy;
				}

				// does not check to see if it goes past the end
				// iterating past begin is undefined
				utf8string_iterator &operator--()
				{
					dec();
					return *this;
				}

				// does not check to see if it goes past the end
				// iterating past begin is undefined
				utf8string_iterator operator--(int)
				{
					utf8string_iterator copy(*this);

					dec();

					return copy;
				}

				bool operator == ( const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf == other.utf8string_buf;
				}
 
				bool operator != (const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf != other.utf8string_buf;
				}

				bool operator < ( const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf < other.utf8string_buf;
				}
 
				bool operator > (const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf > other.utf8string_buf;
				}
		
				bool operator <= ( const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf <= other.utf8string_buf;
				}
 
				bool operator >= (const utf8string_iterator &other) const
				{
					// just compare pointers
					// the programmer is responsible for making both iterators are for the same set of data
					return utf8string_buf >= other.utf8string_buf;
				}
		};

		// make our iterator types here
		typedef utf8string_iterator<value_type>			iterator;
		typedef utf8string_iterator<const value_type>	const_iterator;
		typedef value_reverse_iterator<iterator>		reverse_iterator;
		typedef value_reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		std::basic_string<_uchar8bit, std::char_traits<unsigned char>, Alloc> utfstring_data;

		// No longer needed. std::basic_string will take care of this for us
		// void growbuffer(size_type new_size, bool copy_data = true);
		// size_t recommendreservesize(size_type str_len);
		// void copystringtobuffer(const unsigned char *str, size_type str_len);

	public:
		// default constructor
		_utf8string<Alloc>()
		{
		}

		// build from a c string
		// undefined (ie crashes) if str is NULL
		_utf8string<Alloc>(const _char8bit *str)
		:utfstring_data((const _uchar8bit *)str)
		{
		}

		// build from a c string
		// undefined (ie crashes) if str is NULL
		_utf8string<Alloc>(const _uchar8bit *str)
		:utfstring_data(str)
		{
		}

		// construct from an unsigned char
		_utf8string<Alloc>(size_t n, _char32bit c)
		{
			utf8_encoding encoding;
			size_t encoding_size;

			GetUTF8Encoding(c, encoding, encoding_size);

			utfstring_data.reserve(encoding_size * n);

			for(size_t j = 0; j < n; j++)
			{
				for(size_t i = 0; i < encoding_size; ++i)
				{
					utfstring_data += encoding[i];
				}
			}
		}

		// construct from a normal char
		_utf8string<Alloc>(_uchar8bit c)
		:utfstring_data(1, c)
		{
		}

		// construct from a normal char
		_utf8string<Alloc>(_char8bit c)
		:utfstring_data(1, (_uchar8bit)c)
		{
		}

		// construct from a normal char
		_utf8string<Alloc>(_char16bit c)
		{
			utf8_encoding encoding;
			size_t encoding_size;

			GetUTF8Encoding(c, encoding, encoding_size);

			for(size_t i = 0; i < encoding_size; ++i)
			{
				utfstring_data += encoding[i];
			}
		}

		// construct from a normal char
		_utf8string<Alloc>(_char32bit c)
		{
			utf8_encoding encoding;
			size_t encoding_size;

			GetUTF8Encoding(c, encoding, encoding_size);

			for(size_t i = 0; i < encoding_size; ++i)
			{
				utfstring_data += encoding[i];
			}
		}

		// copy constructor
		_utf8string<Alloc>(const _utf8string<Alloc> &str)
		:utfstring_data(str.utfstring_data)
		{
		}

		/// \brief Constructs a UTF-8 string from an 16 bit character terminated string
		_utf8string<Alloc> (const _char16bit* instring_UCS2)
		{
			MakeUTF8StringImpl(instring_UCS2, utfstring_data, true);
		}

		/// \brief Constructs a UTF-8 string from an 32 bit character terminated string
		_utf8string<Alloc> (const _char32bit* instring_UCS4)
		{
			MakeUTF8StringImpl(instring_UCS4, utfstring_data, true);
		}

		/// \brief copy constructor from basic std::string
		_utf8string<Alloc>(const std::string &instring)
			:utfstring_data((const unsigned char *)instring.c_str())
		{
		}

		/// \brief copy constructor from basic std::string
		_utf8string<Alloc>(const std::wstring &instring)
		{
			MakeUTF8StringImpl(instring.c_str(), utfstring_data, true);
		}

		// destructor
		~_utf8string<Alloc>()
		{
		}



		// assignment operator
		// we can define assignment operators for all possible types such as char, const char *, etc,
		// but this is not neccessary. Because those constructors were provided, the compiler will be
		// able to build a _utf8string<Alloc> for those types and then call this overloaded operator.
		// if performance becomes an issue, the additional variations to this operator can be created
		_utf8string<Alloc>& operator= (const _utf8string<Alloc> &rvalue)
		{
			utfstring_data = rvalue.utfstring_data;

			return *this;
		}

		// move assignment operator
		// should move the data to this object and remove it from the old one
		_utf8string<Alloc>& operator= (_utf8string<Alloc> &&rvalue)
		{
			utfstring_data = std::move(rvalue.utfstring_data);

			return *this;
		}

		// capacity ------------------------------------------------------------
		// request a new buffer size
		// this will resize the buffer, but it will not shrink the buffer is new_size < reserve_size
		// not useful unless the actual size of the string is known
		void reserve(size_type new_size)
		{
			utfstring_data.reserve(new_size);
		}

		// returns the size of the string in characters
		// synonomous with length()
		size_type size() const
		{
			return GetNumCharactersInUTF8String(utfstring_data.c_str());
		}

		// returns the size of the string in characters
		// synonomous with size()
		size_type length() const
		{
			return size();
		}

		// resizes the length of the string, padding the string with c
		// if the size is greater than the current size
		void resize(size_type n, value_type c)
		{
			if(n < size())
			{
				// find the position to chop of the string
				size_type cut_pos = GetBufferPosition(utfstring_data.c_str(), n);

				// set the null terminator
				utfstring_data[cut_pos] = 0;
			}
			else if(n > size())
			{
				size_type diff = n - size();

				// convert c to UTF-8 to get the size
				utf8_encoding c_utf8;
				size_type c_real_size;

				GetUTF8Encoding(c, c_utf8, c_real_size);

				// total size to add
				size_type additional_space = c_real_size * diff;

				size_type cur_size = utfstring_data.length();

				// make sure the buffer is big enough
				// we must do this now because if an exception is thrown because of
				// memory, the value of the string shouldn't change
				utfstring_data.resize(additional_space + cur_size);

				for(size_type i = cur_size; i < (additional_space + cur_size); i += c_real_size)
				{
					for(size_type j = 0; j < c_real_size; ++j) utfstring_data[i+j] = c_utf8[j];
				}
			}
		}

		// resizes the length of the string, padding the string with null
		// if the size is greater than the current size
		void resize(size_type n)
		{
			resize(n, value_type());
		}

		// returns the size of the allocated buffer
		size_type capacity() const
		{
			return utfstring_data.capacity();
		}

		// clears the string, setting the size to 0
		void clear()
		{
			utfstring_data.clear();
		}

		// checks to see if the string is empty
		bool empty() const
		{
			return utfstring_data.empty();
		}

		void shrink_to_fit()
		{
			utfstring_data.shrink_to_fit();
		}

		// iterators ----------------------------------------------------------------------

		// return iterator to the beginning of the list
		iterator begin()
		{
			return iterator(utfstring_data.c_str());
		}

		// return const_iterator to the beginning of the list
		const_iterator cbegin() const
		{
			// actually all iterators are const in this implementation
			return const_iterator(utfstring_data.c_str());
		}

		// return const_iterator to the beginning of the list
		const_iterator begin() const
		{
			return cbegin();
		}

		// return iterator to just after the last element of the list
		iterator end()
		{
			// this works because end will point to just after the end of the string which is the null terminator
			// and the basic_string that holds the data's length is the length of the buffer since sizeof(unsigned char) == 1
			const _uchar8bit *end_ptr = utfstring_data.c_str() + utfstring_data.length();

			return iterator(end_ptr);
		}

		// return iterator to just after the last element of the list
		const_iterator cend() const
		{
			const _uchar8bit *end_ptr = utfstring_data.c_str() + utfstring_data.length();

			return const_iterator(end_ptr);
		}

		const_iterator end() const
		{
			return cend();
		}

		// return iterator to the beginning of the list
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		// return const_iterator to the beginning of the list
		const_reverse_iterator crbegin() const
		{
			return const_reverse_iterator(end());
		}

		// return const_iterator to the beginning of the list
		const_reverse_iterator rbegin() const
		{
			return crbegin();
		}

		// return iterator to just after the last element of the list
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		// return iterator to just after the last element of the list
		const_reverse_iterator crend() const
		{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return crend();
		}

		// string operatios ---------------------------------------------------------------

		Alloc get_allocator() const
		{
			return utfstring_data.get_allocator();
		}

		size_type find (const _utf8string<Alloc>& str, size_type pos = 0) const
		{
			size_type real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.find(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		size_type rfind (const _utf8string<Alloc>& str, size_type pos = 0) const
		{
			size_type real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.rfind(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		size_type find_first_of (const _utf8string<Alloc>& str, size_type pos = 0) const
		{
			size_type real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.find_first_of(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		size_type find_last_of (const _utf8string<Alloc>& str, size_type pos = std::string::npos) const
		{
			size_type real_pos;//
			if(size_type pos == std::string::npos) real_pos = pos;
			else real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.find_last_of(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		size_type find_first_not_of (const _utf8string<Alloc>& str, size_type pos = 0) const
		{
			size_type real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.find_first_not_of(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		size_type find_last_not_of (const _utf8string<Alloc>& str, size_type pos = std::string::npos) const
		{
			size_type real_pos;//
			if(size_type pos == std::string::npos) real_pos = pos;
			else real_pos = GetBufferPosition(utfstring_data.c_str(), pos);

			size_type found_pos = utfstring_data.find_last_not_of(str.utfstring_data, real_pos);

			// return the character position
			return GetCharPosFromBufferPosition(utfstring_data.c_str(), found_pos);
		}

		// returns a c-style null-terminated string
		const char *c_str() const
		{
			return (const char *)utfstring_data.c_str();
		}

		// returns a c-style null-terminated string
		const _uchar8bit *data() const
		{
			return c_str();
		}

		// copies a sub string of this string to s and returns the number of characters copied
		// if this string is shorter than len, as many characters as possible are copied
		// undefined behavior if the buffer pointed to by s is not long enough
		// UTF-8 version
		size_type copy (_uchar8bit *s, size_type len, size_type pos = 0) const
		{
			if(pos > size())
			{
				throw std::out_of_range("pos out of range");
			}

			size_type copy_to = pos + len;
			if(copy_to > size()) copy_to = size();

			size_type start, end, real_len;
			
			start = GetBufferPosition(utfstring_data.c_str(), pos);
			end   = GetBufferPosition(utfstring_data.c_str(), copy_to);

			real_len = end - start;

			// may give a warning because the MS C++ compiler has deprecated std::string::copy because of possible buffer overruns
			return utfstring_data.copy(s, real_len, start);
		}

		// copies a sub string of this string to s and returns the number of characters copied
		// if this string is shorter than len, as many characters as possible are copied
		// undefined behavior if the buffer pointed to by s is not long enough
		// outputs to UCS-2
		size_type copy (_char16bit *s, size_type len, size_type pos = 0) const
		{
			if(pos > size())
			{
				throw std::out_of_range("pos out of range");
			}

			size_type copy_to = pos + len;
			if(copy_to > size()) copy_to = size();

			// make end iterator here
			iterator substr_end(utfstring_data.c_str(), copy_to);

			// use the iterator, later the reason will become clear
			for(auto it = iterator(utfstring_data.c_str(), pos); it < substr_end; ++it)
			{
				*s++ = (_char16bit)*it;
			}

			return copy_to - pos;
		}

		// outputs to UCS-4
		size_type copy (_char32bit *s, size_type len, size_type pos = 0) const
		{
			if(pos > size())
			{
				throw std::out_of_range("pos out of range");
			}

			size_type copy_to = pos + len;
			if(copy_to > size()) copy_to = size();

			// make end iterator here
			iterator substr_end(utfstring_data.c_str(), copy_to);

			// use the iterator, later the reason will become clear
			for(auto it = iterator(utfstring_data.c_str(), pos); it < substr_end; ++it)
			{
				*s++ = (_char32bit)*it;
			}

			return copy_to - pos;
		}

		// access -------------------------------------------------------------------------------------
		// returns a const reference to the character at the index
		// doesn't throw exception. undefined if out of range
		// don't keep non-const version because this will always be const
		value_type operator[](size_type pos) const
		{
			const _uchar8bit *utf8data = utfstring_data.c_str();

			// increment to the correct location
			IncrementToPosition(utf8data, pos);


			return UTF8CharToUnicode(utf8data);
		}

		// returns a reference to the character at the index
		// will throw an exception if out of range
		// make this const since it can never change the string
		value_type at(size_type pos) const
		{
			// check range
			if(pos >= size())
			{
				throw std::out_of_range("subscript out of range");
			}
			// use operator defined above, will help us latere 
			return (*this)[pos];
		}

		// no-throw guarantee on non-empty strings. Undefined behavior on empty strings
		value_type back() const
		{
			return (*this)[size()-1];
		}

		// no-throw guarantee on non-empty strings. Undefined behavior on empty strings
		value_type front() const
		{
			UTF8CharToUnicode(utfstring_data.c_str());
		}

		// string operations -----------------------------------------------------------------------------
		_utf8string<Alloc> substr (size_type pos = 0, size_type len = std::string::npos) const
		{
			_utf8string<Alloc> temp;

			size_type end_pos = pos + len;
			if(end_pos > size()) end_pos = size() - pos;

			// use an iterator to copy the data
			for(auto it = const_iterator(utfstring_data.c_str(), pos); it < const_iterator(utfstring_data.c_str(), end_pos); ++it)
			{
				temp += (value_type) *it;
			}

			return temp;
		}

		// modifiers -------------------------------------------------------------------------------------

		// appends a string to the end of this one
		// we can define this operator for all possible types such as char, const char *, etc,
		// but this is not neccessary. Because those constructors were provided, the compiler will be
		// able to build a _utf8string<Alloc> for those types and then call this overloaded operator.
		// if performance becomes an issue, the additional variations to this operator can be created
		_utf8string<Alloc>& operator+= (const _utf8string<Alloc>& str)
		{
			// just use operator from std::basic_string
			utfstring_data += str.utfstring_data;

			return *this;
		}

		// for maximum compatibility with std::wstring add a cast operator
		operator std::wstring () const
		{
			// a temporary string
			std::wstring out;

			// grow the string so it won't have to resize
			out.reserve(length());

			// copy using our iterator
			for(const_iterator it = begin(); it < end(); ++it)
			{
				out += (_char16bit)*it;
			}

			return out;
		}

		// for maximum compatibility with std::wstring add a cast operator
		operator std::string () const
		{
			// a temporary string
			std::string out((const char *)utfstring_data.c_str());

			return out;
		}

		// assigns a new value from a UTF-8 or ASCII string
		_utf8string<Alloc> &assign(const _char8bit *str)
		{
			utfstring_data.assign((const _uchar8bit *)str);

			return *this;
		}

		// assigns a new value from a UTF-8 or ASCII string
		_utf8string<Alloc> &assign(const _uchar8bit *str)
		{
			utfstring_data.assign(str);

			return *this;
		}

		// assigns a new value from a character and count
		_utf8string<Alloc> &assign(size_t n, _uchar8bit c)
		{
			utfstring_data.assign(n, c);
		}

		// assigns a new value from a character and count
		_utf8string<Alloc> &assign(size_t n, _char8bit c)
		{
			utfstring_data.assign(n, (_uchar8bit)c);

			return *this;
		}


		// assigns a new value from a _utf8string
		_utf8string<Alloc> &assign(const _utf8string<Alloc> &str)
		{
			utfstring_data.assign(str.utfstring_data);

			return *this;
		}

		// assigns a new value from a _utf8string (MOVE(
		_utf8string<Alloc> &assign(const _utf8string<Alloc> &&str)
		{
			utfstring_data = std::move(str.utfstring_data);

			return *this;
		}

		// assigns a new value from a 16-bit character null terminated string
		_utf8string<Alloc> &assign(const _char16bit* instring_UCS2)
		{
			MakeUTF8StringImpl(instring_UCS2, utfstring_data, true);

			return *this;
		}

		// assigns a new value from a 32-bit character null terminated string
		_utf8string<Alloc> &assign(const _char32bit* instring_UCS4)
		{
			MakeUTF8StringImpl(instring_UCS4, utfstring_data, true);

			return *this;
		}

		// assigns a new value from a std::string
		_utf8string<Alloc> &assign(const std::string &instring)
		{
			utfstring_data.assign((const unsigned char *)instring.c_str());

			return *this;
		}

		// assigns a new value from a std::wstring
		_utf8string<Alloc> &assign(const std::wstring &instring)
		{
			MakeUTF8StringImpl(instring.c_str(), utfstring_data, true);

			return *this;
		}

		template <class InputIterator>
		_utf8string<Alloc> &assign (InputIterator first, InputIterator last)
		{
			// create a temporary string first so an excpetion won't alter the current value
			_utf8string<Alloc> temp;
			for(auto it = first; it < last; it++)
			{
				temp += (value_type)*it;
			}
			assign(temp);
		}

		// appends a character to the string
		void push_back(value_type c)
		{
			// just use resize to do the work
			resize(length() + 1, c);
		}

		// deletes the last character
		void pop_back()
		{
			// just use resize to do the work
			resize(length() - 1);
		}

		// inserts str right before character at position pos
		_utf8string<Alloc>& insert (size_type pos, const _utf8string<Alloc> & str)
		{
			// get the real position in the buffer
			size_type real_pos = sd_utf8::GetBufferPosition(utfstring_data.c_str(), pos);

			// just use the standard insert
			utfstring_data.insert(pos, str.utfstring_data);

			return *this;
		}

		_utf8string<Alloc>& insert (size_type pos, const _utf8string<Alloc>& str, size_type subpos, size_type sublen)
		{
			// create substring
			_utf8string<Alloc> temp = str.substr(subpos, sub_len);

			return insert(pos, temp);
		}

		_utf8string<Alloc>& erase (size_type pos = 0, size_type len = std::string::npos)
		{
			size_type real_pos = sd_utf8::GetBufferPosition(utfstring_data.c_str(), pos);

			if(len == std::string::npos) utfstring_data.erase(real_pos, len);
			else
			{
				size_type end_pos = pos + len;
				if(end_pos > size()) end_pos = size() - pos;

				size_type real_end_pos = sd_utf8::GetBufferPosition(utfstring_data.c_str(), end_pos);

				utfstring_data.erase(real_pos, real_end_pos - real_pos);
			}

			return *this;
		}

		_utf8string<Alloc>& replace (size_type pos, size_type len, const _utf8string<Alloc>& str)
		{
			// make copy so exceptions won't change string
			_utf8string<Alloc> temp_copy(*this);

			// erase
			temp_copy.erase(pos, len);

			// insert
			temp_copy.insert(pos, str);

			assign(temp_copy);

			return *this;
		}

		_utf8string<Alloc>& replace (size_type pos, size_type len, const _utf8string<Alloc>& str, size_type subpos, size_type sublen)
		{
			// make copy so exceptions won't change string
			_utf8string<Alloc> temp_copy(*this);
			_utf8string<Alloc> sub_str = str.substr(subpos, sublen);

			// erase
			temp_copy.erase(pos, len);

			// insert
			temp_copy.insert(pos, sub_str);

			assign(temp_copy);

			return *this;
		}

		_utf8string<Alloc>& replace (size_type pos, size_type len, size_type n, value_type c)
		{
			// make copy so exceptions won't change string
			_utf8string<Alloc> temp_copy(*this);
			_utf8string<Alloc> str(n, c);

			// erase
			temp_copy.erase(pos, len);

			// insert
			temp_copy.insert(pos, str);

			assign(temp_copy);

			return *this;
		}

		// swaps the contents of the strings
		void swap (_utf8string<Alloc>& str)
		{
			utfstring_data.swap(str.utfstring_data);
		}

		void KillEndingWhiteSpace()
		{
			if(utfstring_data.size())
			{
				size_type new_length = utfstring_data.size();
				unsigned char c = utfstring_data[new_length - 1];
				while(((c == '\r') || (c == '\n') || (c == '\t') || (c == ' ')) && (new_length > 0))
				{
					--new_length;
					c = utfstring_data[new_length - 1];
				}
				utfstring_data.resize(new_length);
			}
		}

		// comparison operators ---------------------------------------------------------------------------
		bool operator == ( const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data == other.utfstring_data;
		}
 
		bool operator != (const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data != other.utfstring_data;
		}

		bool operator < ( const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data < other.utfstring_data;
		}
 
		bool operator > (const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data > other.utfstring_data;
		}

		bool operator <= ( const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data <= other.utfstring_data;
		}
 
		bool operator >= (const _utf8string<Alloc> &other) const
		{
			// just compare pointers
			// the programmer is responsible for making both iterators are for the same set of data
			return utfstring_data >= other.utfstring_data;
		}

		// non-member function overloads ------------------------------------------------------------------

		// overload stream insertion so we can write to streams
		template <class Alloc>
		friend std::ostream& operator<<(std::ostream& os, const _utf8string<Alloc>& string)
		{
			// use from basic string
			// must cast to char otherwise stream will think it's a number
			os << (const char *)string.c_str();

			return os;
		}

		// overload stream extraction so we can write to streams
		template <class Alloc>
		friend std::istream& operator>>(std::istream& is, _utf8string<Alloc>& string)
		{
			// for now just use std::string because istream as defined can not read unsigned char
			std::string in;
			is >> in;

			string = in;

			return is;
		}

 		// we can define this operator for all possible types such as char, const char *, etc,
		// but this is not neccessary. Because those constructors were provided, the compiler will be
		// able to build a _utf8string<Alloc> for those types and then call this overloaded operator.
		// if performance becomes an issue, the additional variations to this operator can be created
		template <class Alloc>
		friend _utf8string<Alloc> operator + (const _utf8string<Alloc>& lhs, const _utf8string<Alloc>& rhs)
		{
			_utf8string<Alloc> out(lhs);
			out += rhs;

			return out;
		}

		static const size_type npos = -1;
};

typedef _utf8string<> utf8string;

}

#endif 