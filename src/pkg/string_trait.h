//
/////////////////////////////////////////////////////////////////////////////////
// 原创作者：徐海龙
// 创建时间：2007-5-12
// 历史信息：2007-7-11
// 版权：版权归作者所有，本文件可以自由传播

/// for ox_string_traits
#include <string>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// for ox_string_kit_traits
#include <vector> 

#include "../def/theory_type.h"
#include "./character_trait.h"
#include "./string_trait_of_length.h"
#include "./string_pod.h"
#include "./character_static_value.h"

#pragma once
#pragma warning (disable : 4996)


namespace ox
{
namespace str
{


template <typename character_tn>
struct string_traits {};

template <>
struct string_traits<char> : public string_traits_of_length<char>
{
	typedef ox::character::character_trait<char>::character character;

	static character * to_upper(character const * szstr)
	{
		return _strupr(const_cast<character*>(szstr));
	}
	static character * to_lower(character const * szstr)
	{
		return _strlwr(const_cast<character*>(szstr));
	}

	static character* strcat(character* str, character const* src)
	{
		return ::strcat(str,src);
	}
	static character* strncat(character* str, character const* src,size_t n)
	{
		return ::strncat(str,src,n);
	}
	static character* strcpy(character* dest,character const* src)
	{
		return ::strcpy(dest,src);
	}
	static character* strncpy(character* dest,character const* src,size_t n)
	{
		return ::strncpy(dest,src,n);
	}

	static bool is_in_upper_alpha(character c) { return c>='A' && c<='Z';	}
	static bool is_in_lower_alpha(character const & c){ return c>='a' && c<='z'; }
	static bool is_in_alpha(character const & c) { return is_in_lower_alpha(c) || is_in_upper_alpha(c); }
	static character & to_upper_self(character & c) { return c=(character)toupper(c); }
	static character & to_lower_self(character & c) { return c=(character)tolower(c); }
	static character to_upper(character const & c) { return (character)toupper(c); }
	static character to_lower(character const & c) { return (character)tolower(c); }
	template<ox::character::is_case_sensitive_c>
	static bool is_same_ch(character const & c1, character const & c2);
	template<> static bool is_same_ch<ox::character::__case_sensitive>(character const & c1, character const & c2)
	{ return c1==c2; }
	template<> static bool is_same_ch<ox::character::__case_insensitive>(character const & c1, character const & c2)
	{
		if (is_in_lower_alpha(c1))
			return (c1-c2=='a'-'A' || c1-c2 ==0)?true:false;
		else if (is_in_upper_alpha(c1))
			return (c2-c1=='a'-'A' || c2-c1 ==0)?true:false;
		else
			return c2==c1;
	}

	static character * strstr(character const * szstr, character const * szstr_tobefinded)
	{
		return const_cast<character*>(::strstr(szstr, szstr_tobefinded));
	}
	static int memcmp(character const * first_begin, character const * second_begin, size_t count)
	{
		return ::memcmp(first_begin,second_begin,count);
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc> static int strcmp(character const & cfirst, character const & csecond);
	template <> static int strcmp<ox::character::__case_sensitive>(character const & cfirst, character const & csecond)
		{return cfirst-csecond; }
	template <> static int strcmp<ox::character::__case_insensitive>(character const & cfirst, character const & csecond)
		{return is_in_alpha(cfirst) && is_in_alpha(csecond) ? to_lower(cfirst)-to_lower(csecond) : cfirst-csecond; }
	template <ox::character::is_case_sensitive_c> static int strcmp(character const * szfirst, character const * szsecond);
	template <> static int strcmp<ox::character::__case_sensitive>(character const * szfirst, character const * szsecond)
		{return ::strcmp(szfirst,szsecond);}
	template <> static int strcmp<ox::character::__case_insensitive>(character const * szfirst, character const * szsecond)
		{return ::stricmp(szfirst,szsecond);}
	template <ox::character::is_case_sensitive_c> static int strncmp(character const * first_begin, character const * second_begin, size_t count);
	template <> static int strncmp<ox::character::__case_sensitive>(character const * first_begin, character const * second_begin, size_t count)
		{return ::strncmp(first_begin,second_begin,count);}
	template <> static int strncmp<ox::character::__case_insensitive>(character const * first_begin, character const * second_begin, size_t count)
		{return ::strnicmp(first_begin,second_begin,count);}
	static unsigned long to_ulong(character const * szstr, size_t base, character const ** end)
	{
		return strtoul(szstr,(character**)(end),(int)base);
	}
	static long to_long(character const * szstr, size_t base, character const ** end)
	{
		return strtol(szstr,const_cast<character**>(end),(int)base);
	}
	static character * to_string(unsigned int ul_value, size_t base, character * result)
	{
		return _ultoa(ul_value, result,(int)base);
	}
	static character * to_string(int number, size_t base, character * result)
	{
		return _itoa(number, result,(int)base);
	}
	static character * to_string(unsigned long ul_value, size_t base, character * result)
	{
		return _ultoa(ul_value, result,(int)base);
	}
	static character * to_string(long l_value, size_t base, character * result)
	{
		return _ltoa(l_value, result,(int)base);
	}
	static int vsprintf(character *buffer, const character *format, va_list argptr)
	{
		return ::vsprintf(buffer,format,argptr);
	}
	static int vprintf(const character *format, va_list argptr)
	{
		return ::vprintf(format,argptr);
	}
	static int vscprintf(const character *format,va_list argptr)
	{
		return ::_vscprintf(format,argptr);
	}
};

template <>
struct string_traits<wchar_t> : public string_traits_of_length<wchar_t>
{
	typedef ox::character::character_trait<wchar_t>::character character;

	static character * to_upper(character const * szstr)
	{
		return wcsupr(const_cast<character*>(szstr));
	}
	static character * to_lower(character const * szstr)
	{
		return wcslwr(const_cast<character*>(szstr));
	}

	static character* strcat(character* str, character const* src)
	{
		return ::wcscat(str,src);
	}
	static character* strncat(character* str, character const* src,size_t n)
	{
		return ::wcsncat(str,src,n);
	}
	static character* strcpy(character* dest,character const* src)
	{
		wcscpy(dest,src);
	}
	static character* strncpy(character* dest,character const* src,size_t n)
	{
		wcsncpy(dest,src,n);
	}

	static bool is_in_upper_alpha(character c) { return c>=L'A' && c<=L'Z';	}
	static bool is_in_lower_alpha(character const & c){ return c>=L'a' && c<=L'z'; }
	static bool is_in_alpha(character const & c) { return is_in_lower_alpha(c) || is_in_upper_alpha(c); }
	static character & to_upper_self(character & c) { return c=towupper(c); }
	static character & to_lower_self(character & c) { return c=towlower(c); }
	static character to_upper(character const & c) { return towupper(c); }
	static character to_lower(character const & c) { return towlower(c); }
	template<ox::character::is_case_sensitive_c>
	static bool is_same_ch(character const & c1, character const & c2);
	template<> static bool is_same_ch<ox::character::__case_sensitive>(character const & c1, character const & c2)
	{ return c1==c2; }
	template<> static bool is_same_ch<ox::character::__case_insensitive>(character const & c1, character const & c2)
	{
		if (is_in_lower_alpha(c1))
			return (c1-c2==L'a'-L'A' || c1-c2 ==0)?true:false;
		else if (is_in_upper_alpha(c1))
			return (c2-c1==L'a'-L'A' || c2-c1 ==0)?true:false;
		else
			return c2==c1;
	}

	static character * strstr(character const * szstr, character const * szstr_tobefinded)
	{
		return const_cast<character*>(::wcsstr(szstr, szstr_tobefinded));
	}
	static int memcmp(character const * first_begin, character const * second_begin, size_t count)
	{
		return ::wmemcmp(first_begin,second_begin,count);
	}

	template <ox::character::is_case_sensitive_c case_sensitive_tc> static int strcmp(character const & cfirst, character const & csecond);
	template <> static int strcmp<ox::character::__case_sensitive>(character const & cfirst, character const & csecond)
		{return cfirst-csecond; }
	template <> static int strcmp<ox::character::__case_insensitive>(character const & cfirst, character const & csecond)
		{return is_in_alpha(cfirst) && is_in_alpha(csecond) ? to_lower(cfirst)-to_lower(csecond) : cfirst-csecond; }
	template <ox::character::is_case_sensitive_c>
	static int strcmp(character const * szfirst, character const * szsecond);
	template <> static int strcmp<ox::character::__case_sensitive>(character const * szfirst, character const * szsecond)
		{return ::wcscmp(szfirst,szsecond);}
	template <> static int strcmp<ox::character::__case_insensitive>(character const * szfirst, character const * szsecond)
		{return ::_wcsicmp(szfirst,szsecond);}
	template <ox::character::is_case_sensitive_c> static int strncmp(character const * first_begin, character const * second_begin, size_t count);
	template <> static int strncmp<ox::character::__case_sensitive>(character const * first_begin, character const * second_begin, size_t count)
		{return ::wcsncmp(first_begin,second_begin,count);}
	template <> static int strncmp<ox::character::__case_insensitive>(character const * first_begin, character const * second_begin, size_t count)
		{return ::_wcsnicmp(first_begin,second_begin,count);}

	static unsigned long to_ulong(character const * szstr, size_t base, character const** end)
	{
		return wcstoul(szstr,(character**)(end),(int)base);
	}
	static long to_long(character const * szstr, size_t base, character ** end)
	{
		return wcstol(szstr,const_cast<character**>(end),(int)base);
	}
	static character * to_string(unsigned int ul_value, size_t base, character * result)
	{
		return _ultow(ul_value, result,(int)base);
	}
	static character * to_string(int number, size_t base, character * result)
	{
		return _itow(number, result,(int)base);
	}
	static character * to_string(unsigned long ul_value, size_t base, character * result)
	{
		return _ultow(ul_value, result,(int)base);
	}
	static character * to_string(long l_value, size_t base, character * result)
	{
		return _ltow(l_value, result,(int)base);
	}
	static int vsprintf(character *buffer, const character *format, va_list argptr)
	{
		return vswprintf(buffer,format,argptr);
	}
	static int vprintf(const character *format, va_list argptr)
	{
		return ::vwprintf(format,argptr);
	}
	static int vscprintf(const character *format,va_list argptr)
	{
		return ::_vscwprintf(format,argptr);
	}
};


/// 字符串处理的通用规则

template <typename character_tn>
class string_kit_traits :
	public ox::character::static_value<character_tn>,
	public constant_t
{
public:
	typedef typename ox::character::character_trait<character_tn>::character character;
	typedef std::basic_string<character,std::char_traits<character>,std::allocator<character>> std_string;
	typedef typename ox::str::string_tt<character>::stringn nstring;
	typedef std::vector<std_string> strings;
	typedef std::vector<nstring> nstrings;
	typedef std::vector<character*> cstrings;
protected:
	typedef typename std::char_traits<character> std_character_traits;
	typedef string_traits<character> string_traits;

public:

	typedef string_tt<character> string_t;
	typedef typename string_t::stringi stringi;
	typedef typename string_t::stringn stringn;
	typedef typename string_t::stringz stringz;
	typedef typename string_t::rstringi rstringi;
	typedef typename string_t::rstringn rstringn;
	typedef typename string_t::rstringz rstringz;
};


} // end of string
} // end of ox lib