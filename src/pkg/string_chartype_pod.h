/*	
	类名: c_string
	作者: 徐海龙
	日期: 2007-5-20
	文件名: string_chartype_pod.h
	版权：版权归作者所有，本文件可以自由传播
 */


#include "./string_chartype_def.h"
#include "./string_pod.h"

#pragma once

namespace ox
{
namespace str
{

	template <> struct get_string_chartype_tt<astringi> { typedef char character; };
	template <> struct get_string_chartype_tt<astringn> { typedef char character; };
	template <> struct get_string_chartype_tt<astringz> { typedef char character; };
	template <> struct get_string_chartype_tt<wstringi> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<wstringn> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<wstringz> { typedef wchar_t character; };
	
	template <> struct get_string_chartype_tt<const astringi> { typedef char character; };
	template <> struct get_string_chartype_tt<const astringn> { typedef char character; };
	template <> struct get_string_chartype_tt<const astringz> { typedef char character; };
	template <> struct get_string_chartype_tt<const wstringi> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<const wstringn> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<const wstringz> { typedef wchar_t character; };


	template <> struct c_string<astringz>
	{
		typedef astringz string_t;
		typedef const string_t const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(string_t & str) { return const_cast<c_string_t>(str.begin()); };
		static c_string_t get(const_string_t & str) { return const_cast<c_string_t>(str.begin()); };
	};
	template <> struct c_string<wstringz>
	{
		typedef wstringz string_t;
		typedef const string_t const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(string_t & str) { return const_cast<c_string_t>(str.begin()); };
		static c_string_t get(const_string_t & str) { return const_cast<c_string_t>(str.begin()); };
	};

} // end of match
} // end of ox