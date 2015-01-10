/*	
	类名: c_string
	作者: 徐海龙
	日期: 2007-5-20
	文件名: string_chartype_stl.h
	版权：版权归作者所有，本文件可以自由传播
 */

#include "./string_chartype_def.h"
#include <string>

#pragma once

namespace ox
{
namespace string
{

	template <> struct get_string_chartype_tt<std::string> { typedef char character; };
	template <> struct get_string_chartype_tt<std::wstring> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<const std::string> { typedef char character; };
	template <> struct get_string_chartype_tt<const std::wstring> { typedef wchar_t character; };

	template <> struct c_string<std::string>
	{
		typedef std::string string_t;
		typedef const string_t const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(string_t & str) { return const_cast<c_string_t>(str.c_str()); };
		static c_string_t get(const_string_t & str) { return const_cast<c_string_t>(str.c_str()); };
	};
	template <> struct c_string<std::wstring>
	{
		typedef std::wstring string_t;
		typedef const string_t const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(string_t & str) { return const_cast<c_string_t>(str.c_str()); };
		static c_string_t get(const_string_t & str) { return const_cast<c_string_t>(str.c_str()); };
	};

} // end of match
} // end of ox