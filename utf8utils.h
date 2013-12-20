// utf8utils.h
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
//
// Change log
// 2013-12-10: - fixed bug in DecToNextCharacter()
//             - changed out_size type in GetUTF8Encoding() to size_t
//             - fixed bug in IncrementToPosition()
//
// 2012-12-12  - added GetCharPosFromBufferPosition()
//
#pragma once

#ifndef UTF8UTILSHEADER
#define UTF8UTILSHEADER

#include <cstdint>

namespace sd_utf8
{

// To make things easier I'll define some types that I can use that can be forced to the same size on all platforms.
// You'll have to define WCHAR32BITS on platforms where wchar_t is 4 bytes.

// #define WCHAR32BITS

#ifndef WCHAR32BITS

// because signed or unsigned is not mandated for char or wchar_t in the standard,
// always use the char and wchar_t types or we may get compiler errors when using
// some standaard function

typedef char _char8bit; // for ASCII and UTF8
typedef unsigned char _uchar8bit; // for ASCII and UTF8
typedef wchar_t _char16bit; // for USC2
typedef std::uint32_t _char32bit; // for UTF32

#else

typedef char __char8bit; // for ASCII and UTF8
typedef unsigned char __uchar8bit; // for ASCII and UTF8
typedef std::uint16_t __char16bit; // for USC2
typedef wchar_t __char32bit; // for UTF32

#endif

typedef _uchar8bit utf8_encoding[4];

/// \brief Generates a UTF8 encoding
/// This function generates a UTF-8 encoding from a 32 bit UCS-4 character.
/// This is being provided as a static method so it can be used with normal std::string objects
/// default_order is true when the byte order matches the system
inline void GetUTF8Encoding(_char32bit in_char, utf8_encoding &out_encoding, size_t &out_size, bool default_order = true)
{
	// check the order byte order and reorder if neccessary
	if(default_order == false)
	{
		in_char = ((in_char & 0x000000ff) << 24) + ((in_char & 0x0000ff00) << 8) + ((in_char & 0x00ff0000) >> 8) + ((in_char & 0xff000000) >> 24);
	}

	if(in_char < 0x80)
	{
		// 1 byte encoding
		out_encoding[0] = (char)in_char;
		out_size = 1;
	}
	else if(in_char < 0x800)
	{
		// 2 byte encoding
		out_encoding[0] = 0xC0 + ((in_char & 0x7C0) >> 6);
		out_encoding[1] = 0x80 + (in_char & 0x3F);
		out_size = 2;
	}
	else if(in_char < 0x10000)
	{
		// 3 byte encoding
		out_encoding[0] = 0xE0 + ((in_char & 0xF000) >> 12);
		out_encoding[1] = 0x80 + ((in_char & 0xFC0) >> 6);
		out_encoding[2] = 0x80 + (in_char & 0x3F);
		out_size = 3;
	}
	else
	{
		// 4 byte encoding
		out_encoding[0] = 0xF8 + ((in_char & 0x1C0000) >> 18);
		out_encoding[1] = 0x80 + ((in_char & 0x3F000) >> 12);
		out_encoding[2] = 0x80 + ((in_char & 0xFC0) >> 6);
		out_encoding[3] = 0x80 + (in_char & 0x3F);
		out_size = 4;
	}
}

/// \brief Generates a UTF8 encoding
/// This function generates a UTF-8 encoding from a 16 bit UCS-2 character.
/// This is being provided as a static method so it can be used with normal std::string objects
/// default_order is true when the byte order matches the system
inline void GetUTF8Encoding(_char16bit in_char, utf8_encoding &out_encoding, size_t &out_size, bool default_order = true)
{
	// check the order byte order and reorder if neccessary
	if(default_order == false)
	{
		in_char = ((in_char & 0x00ff) << 8) + ((in_char & 0xff00) >> 8);
	}

	// to reduce redundant code and possible bugs from typingg errors, use 32bit version
	GetUTF8Encoding((_char32bit)in_char, out_encoding, out_size, true);
}

/// \brief Converts a UTF8 encoded character into a 32 bit single code
/// The input should represent a single unicode character and does not need to be null terminated.
inline _char32bit UTF8CharToUnicode(const _uchar8bit *utf8data)
{
	if(utf8data[0] < 0x80)
	{
		return (_char32bit)utf8data[0];
	}
	else if(utf8data[0] < 0xE0)
	{
		// 2 bytes
		return ((utf8data[0] & 0x1F) << 6) + (utf8data[1] & 0x3F);
	}
	else if (utf8data[0] < 0xF0)
	{
		// 3 bytes
		return ((utf8data[0] & 0xF) << 12) + ((utf8data[1] & 0x3F) << 6) + (utf8data[2] & 0x3F);
	}
	else
	{
		// 4 bytes
		return ((utf8data[0] & 0x7) << 18) + ((utf8data[1] & 0x3F) << 12) + ((utf8data[2] & 0x3F) << 6) + (utf8data[3] & 0x3F);
	}
}

// increments a pointer to a UTF-8 encoded string to the next character
// undefined behavior if pointer doesn't point to valid UTF-8 data
inline void IncToNextCharacter(const _uchar8bit *&utf8data)
{
	// increments the iterator by one
	// result in undefined behavior (crashes) if already at the end 
	if(*utf8data < 0x80) ++utf8data;
	else if(*utf8data < 0xE0)	utf8data += 2;		// 2 bytes
	else if (*utf8data < 0xF0) utf8data += 3;		// 3 bytes
	else utf8data += 4;		// 4 bytes
}

// decrements a pointer to a UTF-8 encoded string to the previous character
// undefined behavior if pointer doesn't point to valid UTF-8 data or is
// already at the beginning of the string
inline void DecToNextCharacter(const _uchar8bit *&utf8data)
{
	// decrements the iterator by one
	// result in undefined behavior (crashes) if already at the beginning
	do
	{
			// first go back one
		--utf8data;
		// keep looking until the tops bits are not 10
	} while((*utf8data & 0xc0) == 0x80);
}


// increments a utf-8 string pointer to a position
// sets the pointer to the null-terminator if the position is off the string
// Behavior is undefined is string doesn't point to a properly formated UTF-8 string.
inline void IncrementToPosition(const _uchar8bit *&utf8data, size_t pos)
{
	size_t cur_index = 0;

	for(size_t cur_index = 0; (*utf8data != 0) && (cur_index < pos); )
	{
		IncToNextCharacter(utf8data);
		
		++cur_index;
	}
}

// scans a UTF-8 encoded string and returns the actual begining in the buffer of the
// character at pos
// undefined is pos is out of range or if string doesn't point to a properly formated UTF-8 string.
inline size_t GetBufferPosition(const _uchar8bit *string, size_t pos)
{
	const _uchar8bit *string_at_pos = string;
	IncrementToPosition(string_at_pos, pos);

	return (size_t)(string_at_pos - string);
}

// Get's the character's position from the buffer position
inline size_t GetCharPosFromBufferPosition(const _uchar8bit *string, size_t buffer_pos)
{
	const _uchar8bit *end_addr = &string[buffer_pos];
	size_t pos = 0;
	while(string < end_addr)
	{
		IncToNextCharacter(string);
		++pos;
	}
	return pos;
}

// reads a non-encoded unicode string to get the minium buffer size needed to encoded it in UTF-8
template <class T>
inline size_t GetMinimumBufferSize(const T *string)
{
	size_t min_size = 0;
	while(*string != 0)
	{
		if(*string < 0x80) min_size += 1;
		else if (string < 2048) min_size += 2;
		else if (string < 65536) min_size += 3;
		else min_size += 4;

		++string;
	}
}

// use a template method because the 16bit and 32 bit implementations are identical
// except for the type
template <typename char_type, typename Alloc>
inline void MakeUTF8StringImpl(const char_type* instring, std::basic_string<_uchar8bit, Alloc> &out, bool appendToOut)
{
	// first empty the string
	if(!appendToOut) out.clear();

	bool default_order = true; // the string uses the same byte order as the system
	int start = 0; // index of the first real character in the string

	// check for byte order mark
	if(instring[0] == 0xfffe)
	{
		default_order = false;
		++start;
	}
	else if(instring[0] == 0xfeff)
	{
		// jump past the byte order mark
		++start;
	}

	// loop until null terminator is reached
	for(int i = start; instring[i] != 0; ++i)
	{
		utf8_encoding cur_encoding;
		size_t encoding_size;

		// convert to UTF-8
		GetUTF8Encoding(instring[i], cur_encoding, encoding_size, default_order);

		// add to the std::string
		for(size_t j = 0; j < encoding_size; ++j)
		{
			out += cur_encoding[j];
		}
	}
}

// make this method private to restrict which types can be called
// use a template method because the 16bit and 32 bit implementations are identical
// except for the type
// out should point to a buffer large enough to hold the data
template <typename char_type>
inline void MakeUTF8StringImpl(const char_type* instring, _uchar8bit *out)
{
	bool default_order = true; // the string uses the same byte order as the system
	int start = 0; // index of the first real character in the string

	// check for byte order mark
	if(instring[0] == 0xfffe)
	{
		default_order = false;
		++start;
	}
	else if(instring[0] == 0xfeff)
	{
		// jump past the byte order mark
		++start;
	}

	// loop until null terminator is reached
	for(int i = start; instring[i] != 0; ++i)
	{
		utf8_encoding cur_encoding;
		int encoding_size;

		// convert to UTF-8
		GetUTF8Encoding(instring[i], cur_encoding, encoding_size, default_order);

		// add to the std::string
		for(int j = 0; j < encoding_size; ++j)
		{
			*out = cur_encoding[j];
			++out;
		}
	}
}

/// \brief Converts a null-terminated string to UTF-8
/// This function generates a UTF-8 encoding from a 16 bit null terminated string and places
/// it inside a std::string.
/// This is being provided so it can be used with normal std::string objects
template <typename Alloc>
inline void MakeUTF8String(const _char16bit* instring_UCS2, std::basic_string<_uchar8bit, std::char_traits<unsigned char>, Alloc> &out, bool appendToOut = false)
{
	// call the template method
	MakeUTF8StringImpl(instring_UCS2, out, appendToOut);
}

/// \brief Converts a null-terminated string to UTF-8
/// This function generates a UTF-8 encoding from a 16 bit null terminated string and places
/// it inside a std::string.
/// This is being provided as a static method so it can be used with normal std::string objects
template <typename Alloc>
inline void MakeUTF8String(const _char32bit* instring_UCS4, std::basic_string<_uchar8bit, std::char_traits<unsigned char>, Alloc> &out, bool appendToOut = false)
{
	// call the template method
	MakeUTF8StringImpl(instring_UCS4, out, appendToOut);
}

inline size_t GetNumCharactersInUTF8String(const _uchar8bit *utf8data)
{
	size_t count = 0;
	while(*utf8data != 0)
	{
		++count;
		IncToNextCharacter(utf8data);
	}
	return count;
}

}

#endif