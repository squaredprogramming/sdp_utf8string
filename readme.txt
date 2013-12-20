Squared'D Programming STL-Style UTF-8 String Class
2013-12-20

This software was written as part of a series of blog post on writing a UTF-8 string class that behaves like std::string. It has all of the same methods as std::string and overloads the cast operator so it can be cast to std::string and std::wstring. The class also supports C++ STL-style iterators. The iterators are constant only though because UTF-8 is a variable-sized type. It wouldn't be possible to supply a mutable reference to any character. Because of this, the iterator will always return an unsigned 32-bit int type or wchar_t on systems that use a 32-bit wchar_t type.

For more information:
Blogs: http://squaredprogramming.blogspot.com/
       www.gamedev.net/blog/1670-squaredds-journal/
Email: squaredprogramming@gmail.com

License
BSD License
Copyright (c) 2013, Dominque A Douglas
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.