/*	
	作者: 徐海龙
	日期: 2007-5-20
	版权：版权归作者所有，本文件可以自由传播
 */

#include "./string_const.h"
#include "./character_static_value.h"

#pragma once

namespace ox
{
namespace str
{

	template <typename string_tn> struct get_string_chartype_tt;
	template <> struct get_string_chartype_tt<char *> { typedef char character; };
	template <> struct get_string_chartype_tt<wchar_t *> { typedef wchar_t character; };
	template <> struct get_string_chartype_tt<const char *> { typedef char character; };
	template <> struct get_string_chartype_tt<const wchar_t *> { typedef wchar_t character; };


	template <typename string_tn>
	struct c_string;

	template <> struct c_string<char*>
	{
		typedef char* string_t;
		typedef const char * const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		typedef const character* const_c_string_t;
		static c_string_t get(const_string_t str) { return const_cast<c_string_t>(str); };
		static c_string_t get(string_t str) { return const_cast<c_string_t>(str); };
	};
	template <> struct c_string<const char*>
	{
		typedef char* string_t;
		typedef const char * const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		typedef const character* const_c_string_t;
		static c_string_t get(const_string_t str) { return const_cast<c_string_t>(str); };
		static c_string_t get(string_t str) { return const_cast<c_string_t>(str); };
	};
	template <> struct c_string<wchar_t*>
	{
		typedef wchar_t* string_t;
		typedef const wchar_t * const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(const_string_t str) { return const_cast<c_string_t>(str); };
		static c_string_t get(string_t str) { return const_cast<c_string_t>(str); };
	};
	template <> struct c_string<const wchar_t*>
	{
		typedef wchar_t* string_t;
		typedef const wchar_t* const_string_t;
		typedef get_string_chartype_tt<string_t>::character character;
		typedef character* c_string_t;
		static c_string_t get(const_string_t str) { return const_cast<c_string_t>(str); };
		static c_string_t get(string_t str) { return const_cast<c_string_t>(str); };
	};

} // end of match
} // end of ox