//
/////////////////////////////////////////////////////////////////////////////////
// 原创作者：徐海龙
// 创建时间：2007-5-12
// 历史信息：2007-7-11
// 简洁的字符串操作函数
// 版权：版权归作者所有，本文件可以自由传播

#if !defined _____ox_macro_file__string_simple_kit_____
#define _____ox_macro_file__string_simple_kit_____
#endif


#include <assert.h>
#include <time.h>
#include <string>
#include <vector>
#include <wchar.h>
#include <cstring>

#include "../kernel/delegate.h"
#include "./character_trait.h"
#include "./logical.h"
#include "./match_const.h"
#include "./string_trait.h"
#include "./string_meta_action.h"
#include "./string_pod.h"


#pragma once
#pragma warning (disable : 4996)


namespace ox
{
namespace string
{

template <typename character_tn>

class string_simple_kit
	: public string_kit_traits<character_tn>
{
public:
	typedef typename string_kit_traits::character character;
	typedef typename string_kit_traits::std_string std_string;
	typedef typename string_kit_traits::strings strings;
	static ox::character::is_case_sensitive_c const __casive =  ox::character::__case_sensitive;
	static ox::character::is_case_sensitive_c const __incasive =  ox::character::__case_insensitive;
public:
	template <ox::theory::polarity_c polarity_tc>
	static size_t length(character const * szstr) { return string_traits::length<polarity_tc>(szstr); }
	static size_t length(character const * szstr) { return string_traits::length(szstr); }
	static character const* end(character const* szstr) { return szstr+length(szstr); }
	static character* end(character* szstr) { return szstr+length(szstr); }
	static bool is_alphabet_upper(character c) { return c>='A'&&c<='Z'; }
	static bool is_alphabet_lower(character c) { return c>='a'&&c<='z'; }
	static bool is_alphabet(character c) { return is_alphabet_upper(c)||is_alphabet_lower(c); }
	static bool is_digital(character c) { return c>='0'&&c<='9'; }
	static character* strcat(character* str, character const* src)
	{
		return string_traits::strcat(str,src);
	}
	static character* strncat(character* str, character const* src)
	{
		return string_traits::strncat(str,src);
	}
	static character* strcpy(character* str, character const* src)
	{
		return string_traits::strcpy(str,src);
	}
	static character* strncpy(character* str, character const* src,size_t n)
	{
		return string_traits::strncpy(str,src,n);
	}
	static size_t cpy(character* str, character const* src)
	{
		string_traits::strcpy(str,src);
		return length(src);
	}
	static size_t cpyn(character* str, character const* src,size_t n)
	{
		string_traits::strncpy(str,src,n);
		size_t len = length(src);
		return len>n?len:n;
	}
	/// return next zero position
	static character* append(character* str,character const* src)
	{
		character* cp = str;
		while(*cp) ++cp;
		while(*cp++ = *src++);
		return --cp;
	}
	/// return next zero position
	static character* append(character* str,character const* src,size_t n)
	{
		while (*str) ++str;
		while (n-- && (*str = *src++)) ++str;
		*str = '\0';
		return str;
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static bool to_bool(
		std_string const & str,
		bool default_when_error=false)
	{
		return to_bool<case_sensitive_tc>(str.c_str(),0,0,default_when_error);
	};
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static bool to_bool(
		character * szstr,
		bool default_when_error=false)
	{
		return to_bool<case_sensitive_tc>(szstr,0,0,default_when_error);
	};
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static bool to_bool(
		std_string const & str,
		size_t * compare_count,
		size_t * valid_match_count,
		bool default_when_error=false)
	{
		return to_bool<case_sensitive_tc>(str.c_str(), compare_count, valid_match_count, default_when_error);
	}
	/**
		形如"[ \t\r\n]*(true|false)XXX"
		形如"[ \t\r\n]*(T|F)XXX"
		形如"[ \t\r\n]*(1|0)XXX"
		返回相应的bool变量
		如果不成功返回缺省值bdefault
		compare_count ：本次变量参与比较的字符个数
		valid_match_count : 有效的字符个数
	*/
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static bool to_bool(
		const character * buffer_begin,
		size_t * compare_count,
		size_t * valid_match_count,
		bool default_when_error=false)
	{
		static const character _true[] =
		{
			to_static_character<char,'t'>::character_value,
			to_static_character<char,'r'>::character_value,
			to_static_character<char,'u'>::character_value,
			to_static_character<char,'e'>::character_value,
			to_static_character<char,'\0'>::character_value,
		};
		static const character _false[] =
		{
			to_static_character<char,'f'>::character_value,
			to_static_character<char,'a'>::character_value,
			to_static_character<char,'l'>::character_value,
			to_static_character<char,'s'>::character_value,
			to_static_character<char,'e'>::character_value,
			null_character,
		};
		static const character _sep[] = 
		{
			to_static_character<char,' '>::character_value,
			to_static_character<char,'\t'>::character_value,
			to_static_character<char,'\r'>::character_value,
			to_static_character<char,'\n'>::character_value,
			null_character,
		};
		static const character _1 = to_static_character<char,'1'>::character_value;
		static const character _0 = to_static_character<char,'0'>::character_value;

		bool result = default_when_error;
		bool is_analyse_ok = false;
		size_t count = 0;
		assert(buffer_begin);
		if (!buffer_begin) return default_when_error;
		character * p = find_first_in<false>(buffer_begin,_sep);
		assert(p);

		// 1/0
		switch(*p)
		{
		case _1: result = true; count = 1; is_analyse_ok = true; goto label_return_result;
		case _0: result = false; count = 1; is_analyse_ok = true; goto label_return_result;
		}

		// true/false t/f T/F
		count = get_same_length<case_sensitive_tc>(p,_true);
		if (count>=1)
		{
			result = true;
			is_analyse_ok = true;
			goto label_return_result;
		}
		count = get_same_length<case_sensitive_tc>(p,_false);
		if (count>=1)
		{
			result = false;
			is_analyse_ok = true;
			goto label_return_result;
		}

		label_return_result: 

		if (compare_count) *compare_count = count;
		if (valid_match_count)
		{
			if (is_analyse_ok)
			{
				if (result)
					*valid_match_count = (count==sizeof(_true))?sizeof(_true):1;
				else
					*valid_match_count = (count==sizeof(_true))?sizeof(_false):1;
			}
		}
		return result;
	}
	static unsigned long to_ulong (
		const character * szstr,
		size_t base,
		const character ** end=0)
	{
		return string_traits::to_ulong(szstr,base,end);
	}
	static unsigned long to_ulong (
		std_string & str,
		size_t base,
		size_t * used_len=0)
	{
		character * end = 0;
		unsigned long r = string_traits::to_ulong(str.c_str(),base,&end);
		if (used_len) *used_len = end - str.c_str();
		return r;
	}
	static long to_long (
		const character * szstr,
		size_t base,
		const character ** end=0)
	{
		return string_traits::to_long(szstr,base,end);
	}
	static long to_long (
		std_string & str,
		size_t base,
		size_t * used_len=0)
	{
		character * end = 0;
		long r = string_traits::to_long(str.c_str(),base,&end);
		if (used_len) *used_len = end - str.c_str();
		return r;
	}
	/// integer_tn includes long/ulong int/uint
	template <typename integer_tn>
	static character * to_string(
		integer_tn ul_value,
		character * str_value,
		size_t base)
	{
		return string_traits::to_string(ul_value, base, str_value);
	}
	template <typename integer_tn>
	static std_string & to_string(
		integer_tn ul_value,
		std_string & str_value,
		size_t base)
	{
		character buffer[32]  = { string_traits::null_character };
		to_string<integer_tn>(ul_value, buffer, base);
		str_value.clear();
		return str_value.append(buffer);
	}
	template <typename integer_tn>
	static std_string to_string(integer_tn ul_value, size_t base)
	{
		character buffer[32]  = { null_character };
		return to_string<integer_tn>(ul_value, buffer, base);
	}
	static const character * to_string(bool boolean)
	{
		static const character* _true = "true";
		static const character* _false = "false";
		return boolean?_true:_false;
	}

	/// 9999[/.-]99[/.-]99[ \t\r\n]+99[:.,']99[:.,']99
	static tm to_tm(character const* strtime,char const* mode="yMdHms",size_t sepcount=6)
	{
		character ampm = 0;
		char const* b = mode;
		char const* bend = end(mode);
		const character* matched_end = 0;
		tm atm;
		memset(&atm,0,sizeof(atm));
		static character const ws[] = {' ','\t','\r','\n',0};
		static character const sepn[] = {'0','-','9',0};
		static character const sepc[] = {'a','-','z','A','-','Z',0};
		static character const sep2[] = {'l','L','2',0};

		for (;*b&&b<bend;++b)
		{
			switch(*b)
			{
			case 'y':
				atm.tm_year = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 'M':
				atm.tm_mon = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 'L': case 'l':
				if (*b=='L')
					atm.tm_mon = (int)to_month_fromlong(strtime,&matched_end);
				else
					atm.tm_mon = (int)to_month_fromshort(strtime,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 'd':
				atm.tm_mday = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 'H': case 'h':
				atm.tm_hour = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 'm':
				atm.tm_min = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case 's':
				atm.tm_sec = to_ulong(strtime,10,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			case '2':
				ampm = to_ampm(strtime,&matched_end);
				if (matched_end==strtime) break;
				strtime = matched_end;
				if (is_digital(*strtime)) break;
				if (is_in_chset_pattern<false>(*(b+1),sep2))
					strtime = skip_pattern<false>(strtime,sepn,6);
				else
					strtime = skip_pattern<false>(strtime,sepc,6);
				break;
			}
		}

		if (ampm=='p' && atm.tm_hour<12)
			atm.tm_hour += 12;
		
		return atm;
	}

	//static bool is_in_chset(const character & c, const character & c1, ...)
	//{

	//}
	//template <ox::op::op_logic_not_t logical_not>
	template <bool is_same_tc>
	static bool is_in_chset(
		const character & c,
		const character * szchset)
	{
		for (;*szchset!=c && *szchset!=0;szchset++);
		return lg::not<!is_same_tc>(*szchset!=0);
	}

	template <bool is_same_tc>
	static bool is_in_chset(
		const character & c,
		const character * chset_begin,
		const character * chset_end)
	{
		for (;*chset_begin!=c && chset_begin<chset_end;chset_begin++);
		return lg::not<!is_same_tc>(chset_begin<chset_end);
	}
	/// "a-zcuy1-9"
	template <bool is_same_tc>
	static bool is_in_chset_pattern(
		const character & c,
		const character * chset_begin,
		const character * chset_end)
	{
		if (!chset_begin || !chset_end) return false;
		if (chset_begin>=chset_end) return false;

		if (*chset_begin=='-')
		{
			if (*chset_begin==c)
				return true;
			++chset_begin;
		}

		for (;chset_begin<chset_end;chset_begin++)
		{
			if (*chset_begin=='\\')
			{
				++chset_begin;
				if (!(chset_begin<chset_end)) break;
				if (*chset_begin==c) break;
				continue;
			}
			if (*chset_begin=='-')
			{
				++chset_begin;
				if (!(chset_begin<chset_end)) break;
				character b=*(chset_begin-2);
				character e=*(chset_begin);
				if (c>=b || c<=e) break;
				continue;
			}
			if (*chset_begin==c) break;
		}
		return lg::not<!is_same_tc>(chset_begin<chset_end);
	}
	/// "a-zcuy1-9"
	template <bool is_same_tc>
	static bool is_in_chset_pattern(
		const character & c,
		const character * szchset)
	{
		if (!szchset) return false;
		if (!*szchset) return false;

		if (*szchset=='-')
		{
			if (*szchset==c)
				return true;
			++szchset;
		}

		for (;*szchset;szchset++)
		{
			if (*szchset=='\\')
			{
				++szchset;
				if (!*szchset) break;
				if (*szchset==c) break;
				continue;
			}
			if (*szchset=='-')
			{
				++szchset;
				if (!*szchset) break;
				character b=*(szchset-2);
				character e=*szchset;
				if (c>=b && c<=e) break;
				continue;
			}
			if (*szchset==c) break;
		}
		return lg::not<!is_same_tc>(0!=*szchset);
	}
	static character * to_upper(const character * szstr)
	{
		return string_traits::to_upper(szstr);
	}
	static character * to_lower(const character * szstr)
	{
		return string_traits::to_lower(szstr);
	}
	static const std_string & to_upper(const std_string & str)
	{
		const character * pstr = str.c_str();
		string_traits::to_upper(const_cast<character*>(pstr));
		return str;
	}
	static std_string & to_upper(std_string & str)
	{
		const character * pstr = str.c_str();
		string_traits::to_upper(const_cast<character*>(pstr));
		return str;
	}
	static const std_string & to_lower(const std_string & str)
	{
		const character * pstr = str.c_str();
		string_traits::to_lower(const_cast<character*>(pstr));
		return str;
	}
	static std_string & to_lower(std_string & str)
	{
		const character * pstr = str.c_str();
		string_traits::to_lower(const_cast<character*>(pstr));
		return str;
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static bool is_same_ch(character const & c1,character const & c2)
	{ return string_traits::is_same_ch<case_sensitive_tc>(c1,c2); }
	static character * trim(
		const character * buffer_begin,
		const character * buffer_end,
		const character ch,
		const character ch_replace=null_character)
	{
		trimright(buffer_end,ch,ch_replace);
		return trimleft(buffer_begin,ch,ch_replace);
	}
	static character * trimright(
		const character * rbuffer,
		const character ch,
		const character ch_replace=null_character)
	{
		for (; *rbuffer==ch; *const_cast<character*>(rbuffer)=ch_replace,rbuffer--);
		return rbuffer;
	}
	static character * trimleft(
		const character * buffer,
		const character ch,
		const character ch_replace=null_character)
	{
		for (; *buffer==ch; *const_cast<character*>(buffer)=ch_replace,buffer++);
		return buffer;
	}
	static character * trimleft(
		const character * szbuffer,
		const character * szchs,
		const character ch_replace=null_character)
	{
		for (; strch(szchs,*szbuffer); *const_cast<character*>(szbuffer)=ch_replace,szbuffer++);
		return szbuffer;
	}

	template <bool is_same_tc> static
	character const* skip(
		const character* buffer_begin,
		const character* buffer_end,
		const character* szcharset,
		size_t maxtimes=-1)
	{
		if (!buffer_begin || !buffer_end)
			return buffer_begin;
		if (buffer_begin>buffer_end)
			return buffer_begin;
		size_t times = 0;
		for(;buffer_begin<buffer_end;++buffer_begin)
		{
			if (!is_in_chset<is_same_tc>(*buffer_begin,szcharset))
				break;
			if (++times>=maxtimes)
				break;
		}
		return buffer_begin;
	}
	template <bool is_same_tc> static
	character const* rskip(
		const character* buffer_rbegin,
		const character* buffer_rend,
		const character* szcharset,
		size_t maxtimes=-1)
	{
		if (!buffer_rbegin || !buffer_rend)
			return buffer;
		if (buffer_rbegin<buffer_rend)
			return buffer_rbegin;
		size_t times = 0;
		for(;buffer_rbegin<buffer_end;--buffer_rbegin)
		{
			if (!is_in_chset<is_same_tc>(*buffer_begin,szcharset))
				break;
			if (++times>=maxtimes)
				break;
		}
		return buffer_rbegin;
	}
	template <bool is_same_tc> static
	character const* skip(
		const character* buffer,
		const character* szcharset,
		size_t maxtimes=-1)
	{
		size_t times = 0;
		if (!buffer) return 0;
		for(;*buffer;++buffer)
		{
			if (!is_in_chset<is_same_tc>(*buffer,szcharset))
				break;
			if (++times>=maxtimes)
				break;
		}
		return buffer;
	}
	template <bool is_same_tc> static
	character const* rskip(
		const character* rbuffer,
		const character* szcharset)
	{
		if (!buffer) return 0;
		for(;*rbuffer;--rbuffer)
		{
			if (!is_in_chset<is_same_tc>(*rbuffer,szcharset))
				break;
			if (++times>=maxtimes)
				break;
		}
		return rbuffer;
	}
	template <bool is_same_tc> static
	character const* skip_pattern(
		const character* buffer_begin,
		const character* buffer_end,
		const character* pattern,
		size_t maxtimes=-1)
	{
		if (!buffer_begin || !buffer_end)
			return buffer;
		if (buffer_begin>buffer_end)
			return buffer_begin;
		size_t times = 0;
		for(;buffer_begin<buffer_end;++buffer_begin)
		{
			if (!is_in_chset_pattern<is_same_tc>(*buffer_begin,pattern))
				break;
			if (++times>=maxtimes)
				break;
		}
		return buffer_begin;
	}
	template <bool is_same_tc> static
	character const* skip_pattern(
		const character* buffer,
		const character* pattern,
		size_t maxtimes=-1)
	{
		if (!buffer || !pattern)
			return buffer;
		size_t times = 0;
		for(;*buffer;++buffer)
		{
			if (!is_in_chset_pattern<is_same_tc>(*buffer,pattern))
				break;
			if (++times>=maxtimes)
				break;
		}
		return buffer;
	}

	// 所能匹配的最大长度
	// 一个简单参数
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * szstr1,
		const character * szstr2)
		{return get_same_length<ox::theory::__positive,case_sensitive_tc>(szstr1,szstr2);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_rsame_length(
		const character * szstr1,
		const character * szstr2)
		{return get_same_length<ox::theory::__negative,case_sensitive_tc>(szstr1,szstr2);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * str1_begin, const character * str1_end,
		const character * szstr2)
		{return get_same_length<ox::theory::__positive,case_sensitive_tc>(str1_begin,str1_end,szstr2);}
	static size_t get_same_length(
		const character * str1_begin, const character * str1_end,
		const character * szstr2)
		{return get_same_length<ox::theory::__negative,case_sensitive_tc>(str1_begin,str1_end,szstr2);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * str1_begin, const character * str1_end,
		const character * str2_begin, const character * str2_end)
		{return get_same_length<ox::theory::__positive,case_sensitive_tc>(str1_begin,str1_end,str2_begin,str2_end);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_rsame_length(
		const character * str1_begin, const character * str1_end,
		const character * str2_begin, const character * str2_end)
		{return get_same_length<ox::theory::__negative,case_sensitive_tc>(str1_begin,str1_end,str2_begin,str2_end);}
	/// most important function
	template <ox::character::is_case_sensitive_c case_sensitive_tc,typename podstring1_tn,typename podstring2_tn>
	static size_t pod_get_same_length(
		const podstring1_tn & podstr1,
		const podstring2_tn & podstr2)
	{
		typedef podstring1_tn podstring1_t;
		typedef podstring2_tn podstring2_t;
		typedef ox::string::met::operator_add<const character *, podstring1_t::__polarity> operator1_inc;
		typedef ox::string::met::operator_add<const character *, podstring2_t::__polarity> operator2_inc;
		typedef ox::string::met::operator_distance<size_t,const character *,podstring1_t::__polarity> operator_distance;

		operator1_inc inc_str1;
		operator2_inc inc_str2;
		inc_str1.element = podstr1.begin();
		inc_str2.element = podstr2.begin();
		
		for (;
			podstr1.isund(inc_str1.element) &&
			podstr2.isund(inc_str2.element) &&
			is_same_ch<case_sensitive_tc>(*inc_str1.element,*inc_str2.element);
			inc_str1++, inc_str2++);

		return operator_distance::distance(podstr1.begin(),inc_str1.element);
	}
	template <ox::theory::polarity_c string_direction_tc,ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * szstr1,
		const character * szstr2)
	{
		typedef typename string_tt<character>::type_polity_tt<string_direction_tc>::stringz_t stringz_t;
		stringz_t strz1 = {const_cast<character*>(szstr1)};
		stringz_t strz2 = {const_cast<character*>(szstr2)};
		return pod_get_same_length<case_sensitive_tc>(strz1,strz2);
	}
	template <ox::theory::polarity_c string_direction_tc,ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * str1_begin, const character * str1_end,
		const character * szstr2)
	{
		typedef typename string_tt<character>::type_polity_tt<string_direction_tc>::stringz_t stringz_t;
		typedef typename string_tt<character>::type_polity_tt<string_direction_tc>::stringi_t stringi_t;
		stringi_t str1 = {const_cast<character*>(str1_begin),const_cast<character*>(str1_end)};
		stringz_t str2 = {const_cast<character*>(szstr2)};
		return pod_get_same_length<case_sensitive_tc>(str1,str2);
	}
	template <ox::theory::polarity_c string_direction_tc,ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t get_same_length(
		const character * str1_begin, const character * str1_end,
		const character * str2_begin, const character * str2_end)
	{
		typedef typename string_tt<character>::type_polity_tt<string_direction_tc>::stringi_t stringi_t;
		stringi_t str1 = {const_cast<character*>(str1_begin),const_cast<character*>(str1_end)};
		stringi_t str2 = {const_cast<character*>(str2_begin),const_cast<character*>(str2_end)};
		return pod_get_same_length<case_sensitive_tc>(str1,str2);
	}
	template <bool is_same_tc>
	static character* strrch(
		const character* rbuffer_begin,
		const character* rbuffer_end,
		const character ch,
		const character sentinel,
		const character** ppend)
	{
		if (rbuffer_begin==0||rbuffer_end==0||rbuffer_end>rbuffer_begin) return 0;
		for (;rbuffer_begin>rbuffer_end && *rbuffer_begin!=sentinel; rbuffer_begin--)
			if (lg::not<!is_same_tc>(*rbuffer_begin==ch)) break;
		if (ppend) *ppend=rbuffer_begin-1;
		return (lg::not<!is_same_tc>(*rbuffer_begin==ch))?rbuffer_begin:0;
	}
	template <bool is_same_tc>
	static character * strrch(
		const character * rbuffer_begin,
		size_t rbuffer_len,
		const character ch,
		const character ** ppend=0)
	{
		return strrch<is_same_tc>(rbuffer_begin,rbuffer_begin-rbuffer_len,ch,ppend);
	}
	template <bool is_same_tc>
	static character * strrch(
		const character * rbuffer_begin,
		const character * rbuffer_end,
		const character ch,
		const character ** ppend=0)
	{
		if (rbuffer_begin==0||rbuffer_end==0||rbuffer_end>rbuffer_begin) return 0;
		for (;rbuffer_begin>rbuffer_end; rbuffer_begin--)
			if (lg::not<!is_same_tc>(*rbuffer_begin==ch)) break;
		if (ppend) *ppend=rbuffer_begin-1;
		return const_cast<character*>(lg::not<!is_same_tc>(*rbuffer_begin==ch)?rbuffer_begin:0);
	}
	template <bool is_same_tc>
	static character * strrch(
		const character * szbuffer,
		const character ch,
		const character ** ppend=0)
	{
		size_t len = std_character_traits::length(szbuffer);
		return strrch<is_same_tc>(szbuffer+len-1,len,ch,ppend);
	}
	/// ppend is the next of the matched hit
	/// something will be changed, please be caution!!!!!!!!
	/// !!!!!!!!!!!!!!!!!
	template <bool is_same_tc>
	static character * strch(
		const character * buffer,
		size_t buffer_len,
		const character ch,
		const character ** ppend=0)
	{
		return strch<is_same_tc>(buffer,buffer+buffer_len,ch,ppend);
	}

	template <bool is_same_tc>
	static character * strch(
		const character * buffer_begin,
		const character * buffer_end,
		const character ch,
		const character ** ppend=0)
	{
		assert(buffer_begin&&buffer_end&&buffer_begin<=buffer_end);
		//if (buffer_begin==0||buffer_end==0||buffer_begin>buffer_end)
		//	return 0;
		for (;
			buffer_begin<buffer_end && 
			lg::not<!is_same_tc>(*buffer_begin!=ch);
			//ox::op::logical<bool>::_do<logical_not>(*buffer_begin!=ch);
			++buffer_begin);
		if (ppend) *ppend = buffer_begin;
		if (buffer_begin>=buffer_end)
			return 0;
		return const_cast<character*>(buffer_begin);
	}
	////// begin
	template <
		bool is_same_tc,
		ox::theory::polarity_c string_direction_tc,
		ox::character::is_case_sensitive_c case_sensitive_tc,
		typename podstring_tn
	>
	static character * pod_strch(
		podstring_tn const & podstr,
		const character & c,
		const character ** ppend)
	{
		typedef podstring_tn podstring_t;
		typedef ox::string::met::operator_add<const character *, podstring_t::__polarity> operator_inc;
		operator_inc inc_str = {podstr.begin()};
		for (;
			podstr.isund(inc_str.element) &&
			lg::not<!is_same_tc>(!is_same_ch<case_sensitive_tc>(*inc_str.element,c))
			//ox::op::logical<bool>::_do<logical_not>(!is_same_ch<case_sensitive_tc>(*inc_str.element,c));
			inc_str++);
		return const_cast<character *>(inc_str.element);
	}
	////// end
	template <bool is_same_tc>
	static character * strch(
		const character * szbuffer,
		const character ch,
		const character ** ppend=0)
	{
		if (szbuffer==0)
			return 0;
		//for (; *szbuffer!=0 && ox::op::logical<bool>::_do<logical_not>(*szbuffer!=ch); szbuffer++);
		for (; *szbuffer!=0 && lg::not<!is_same_tc>(*szbuffer!=ch);szbuffer++);
		if (ppend) *ppend = szbuffer+1;
		if (*szbuffer==null_character) // in case of the char is the null char
			return 0;
		return const_cast<character*>(szbuffer);
	}

	/** 
		查找不在字符里的字串的第一个位置
	*/

	template <bool is_same_tc>
	static character const* find_first_in(
		const character * buffer_begin,
		const character * buffer_end,
		const character * szchset)
	{
		assert (buffer_begin!=0 && buffer_begin<=buffer_end && szchset!=0);
		for (; 
			is_in_chset<!is_same_tc>(*buffer_begin,szchset) && buffer_begin<buffer_end;
			buffer_begin++);
		return buffer_begin;
	}

	template <bool is_same_tc>
	static character const* find_first_in(
		const character * szbuffer,
		const character * szchset)
	{
		assert (szbuffer!=0 && szchset!=0);
		for (; is_in_chset<!is_same_tc>(*szbuffer,szchset) && *szbuffer!=0;
			szbuffer++);
		return szbuffer;
	}

	template <bool is_same_tc>
	static character const * find_first_in(
		const character * buffer_begin,
		const character * buffer_end,
		const character * chset_begin,
		const character * chset_end)
	{
		assert (buffer_begin!=0 && buffer_begin<buffer_end && chset_begin!=0 && chset_begin<chset_end);
		for (;
			buffer_begin<buffer_end &&
			is_in_chset<!is_same_tc>(*buffer_begin,chset_begin,chset_end);
			buffer_begin++);
		return buffer_begin;
	}

	/** 
		查找不在字符里的字串的第一个位置，从右边起
	*/
	//

	template <bool is_same_tc>
	static character const * find_last_in(
		const character * buffer_begin,
		const character * buffer_end,
		const character * szchset)
	{
		assert (buffer_begin!=0 && buffer_begin<buffer_end && szchset!=0);
		--buffer_begin,--buffer_end;
		for (; is_in_chset<!is_same_tc>(*buffer_end,szchset) && buffer_begin<buffer_end; buffer_end--);
		return buffer_end;
	}
	template <bool is_same_tc>
	static character const* find_last_in(
		const character * szbuffer,
		const character * szchset)
	{
		assert (szbuffer!=0 && szchset!=0);
		return find_last_in<is_same_tc>(szbuffer,end(szbuffer),szchset);
	}
	/// [)
	template <bool is_same_tc>
	static character const* find_last_in(
		const character * buffer_begin,	const character * buffer_end,
		const character * chset_begin,	const character * chset_end)
	{
		assert (buffer_begin!=0 && buffer_begin<buffer_end && chset_begin!=0 && chset_begin<chset_end);
		--buffer_begin,--buffer_end;
		for (; is_in_chset<!is_same_tc>(*buffer_end,chset_begin,chset_end) && buffer_begin<buffer_end; buffer_end--);
		return const_cast<character*>(buffer_end);
	}

	/// find str in str
	template
	<
		ox::character::is_case_sensitive_c case_sensitive_tc,
		typename podstring1_tn,
		typename podstring2_tn,
		typename podstringi_tn
	>
	static character * pod_strstr(
		const podstring1_tn & text,
		const podstring2_tn & pattern,
		podstringi_tn * result=0)
	{
		typedef podstring1_tn podstring1_t;
		typedef podstring2_tn podstring2_t;
		typedef podstringi_tn podstringi_t;
		typedef typename podstring1_t::character character;

		if (!text.isvalid()) goto label_exit_not_found;
		size_t max_match_count = 0;
		
		for (podstring1_t texti=text; text.isund(texti.begin()); texti.refp_begin()++)
		{
			if (is_same_ch<case_sensitive_tc>(texti.front(),pattern.front()))
			{
				max_match_count = pod_get_same_length<case_sensitive_tc>(texti,pattern);
				if (pattern.isend(pattern.begin()+max_match_count))
				{
					if (result)
					{
						result->refp_begin()=texti.begin();
						result->end_=result->begin()+max_match_count;
					}
					return texti.begin();
				}
			}
		}
		label_exit_not_found:
		if (result->refp_begin()) result->refp_begin()=0;
		if (result->end_) result->end_=0;
		return 0;
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character * strstr(
		const character * sztext,
		const character * szpattern,
		const character ** pp_match_end=0)
	{
		typedef typename podstring_tt<character,constant_t::__string_null, ox::theory::__positive> stringz_t;
		typedef typename podstring_tt<character,constant_t::__string_interval, ox::theory::__positive> stringi_t;
		stringz_t str1 = {const_cast<character*>(sztext)};
		stringz_t str2 = {const_cast<character*>(szpattern)};
		stringi_t result;
		character * ret = pod_strstr<case_sensitive_tc>(str1,str2,&result);
		if (pp_match_end) *pp_match_end=result.end();
		return ret;
	}

	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character * strstr
	(
		const character * text_begin, const character * text_end,
		const character * pattern_begin, const character * pattern_end,
		const character ** pp_match_end=0
	)
	{
		stringi str1 = {const_cast<character*>(text_begin),const_cast<character*>(text_end)};
		stringi str2 = {const_cast<character*>(pattern_begin),const_cast<character*>(pattern_end)};
		stringi result;
		character * ret = pod_strstr<case_sensitive_tc>(str1,str2,&result);
		if (pp_match_end) *pp_match_end=result.end();
		return ret;
	}
	/// 为stl::string类串进行设计的
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character * strstr
	(
		const character * sztext,
		const std_string & strpattern,
		const character ** pp_match_end=0
	)
	{
		return strstr<case_sensitive_tc>(sztext,strpattern.c_str(),pp_match_end);
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character * strstr
	(
		const character * text_begin,
		const character * text_end,
		const character * szpattern,
		const character ** pp_match_end=0
	)
	{
		return strstr<case_sensitive_tc>(
			text_begin,text_end,
			szpattern,end(szpattern),
			pp_match_end);
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character * strstr
	(
		const character * text_begin,
		const character * text_end,
		const std_string & strpattern,
		const character ** pp_match_end=0
	)
	{
		return strstr<case_sensitive_tc>(
			text_begin,text_end,
			strpattern.c_str(),strpattern.c_str()+strpattern.length(),
			pp_match_end);
	}

	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static character const* strstr
	(
		const character * text_begin, const character * text_end,
		const character * pattern_pre_begin, const character * pattern_pre_end,
		const character * pattern_post_begin, const character * pattern_post_end,
		const character ** key_end=0,
		const character ** pp_match_end=0
	)
	{
		const character* key_begin = 0;
		const character* begin = text_begin;
		const character* end = text_end;
		const character* matched_begin = 0;
		const character* matched_end = 0;
		if (key_end) *key_end = 0;
		matched_begin = strstr<case_sensitive_tc>(begin,end,pattern_pre_begin,pattern_pre_end,&matched_end);
		if (!matched_begin) return 0;
		if (pp_match_end) *pp_match_end = matched_end;
		begin = matched_end;
		key_begin = begin;
		matched_begin = strstr<case_sensitive_tc>(begin,end,pattern_post_begin,pattern_post_end,&matched_end);
		if (matched_begin)
			if (key_end) *key_end = matched_begin;
		return key_begin;
	}

	/// mem_erase
	static void mem_erase(character const* begin,character const* end,character const* begin_erased,character const* end_erased)
	{
		assert (begin<=end);
		assert (begin<=begin_erased);
		assert (end>=end_erased);
		memmove((void*)begin_erased,end_erased,(char*)end-(char*)end_erased);
	}
	/// mem_erase
	static void mem_erase(character const* begin,size_t size,character const* begin_erased,size_t size_erased)
	{
		assert (begin<=begin_erased);
		assert (begin+size>=begin_erased+size_erased);
		size_t size_moved = size-size_erased-(begin_erased-begin);
		memmove((void*)begin_erased,begin_erased+size_erased,size_moved);
	}

	/// 字符串比较方法

	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strcmp(character const & cfirst, character const & csecond)
		{return string_traits::strcmp<case_sensitive_tc>(cfirst,csecond);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strcmp(character const * szfirst, character const * szsecond)
		{return string_traits::strcmp<case_sensitive_tc>(szfirst,szsecond);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strcmp(std_string const & strfirst, std_string const & strsecond)
		{return string_traits::strcmp<case_sensitive_tc>(strfirst.c_str(),strsecond.c_str());}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strcmp(character const * szfirst, stringi const & second_string, size_t * match_len=0)
	{
		size_t same_count = get_same_length<case_sensitive_tc>(second_string.begin(),second_string.end(),szfirst);
		if (match_len) *match_len = same_count;
		if (same_count<second_string.size())
			return strcmp<case_sensitive_tc>(szfirst[same_count],second_string.begin()[same_count]);
		else
			return szfirst[same_count]-null_character;
	}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strncmp(character const * szfirst, character const * szsecond,size_t count)
		{return string_traits::strncmp<case_sensitive_tc>(szfirst,szsecond,count);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strncmp(std_string const & strfirst, std_string const & strsecond, size_t count)
		{return string_traits::strncmp<case_sensitive_tc>(strfirst.c_str(),strsecond.c_str(),count);}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strncmp(character const * szfirst, stringi const & second_string)
		{return string_traits::strncmp<case_sensitive_tc>(szfirst,second_string.begin(),second_string.size());}
	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static int strncmp(stringi const & first_string, character const * szsecond)
		{return string_traits::strncmp<case_sensitive_tc>(first_string.begin(),szsecond,first_string.size());}


	static int stricmp(character const & cfirst, character const & csecond)
		{return string_traits::strcmp<ox::character::__case_insensitive>(cfirst,csecond);}
	static int stricmp(character const * szfirst, character const * szsecond)
		{return string_traits::strcmp<ox::character::__case_insensitive>(szfirst,szsecond);}
	static int stricmp(std_string const & strfirst, std_string const & strsecond)
		{return string_traits::strcmp<ox::character::__case_insensitive>(strfirst.c_str(),strsecond.c_str());}
	static int stricmp(character const * szfirst, stringi const & second_string, size_t * match_len=0)
	{
		size_t same_count = get_same_length<ox::character::__case_insensitive>(second_string.begin(),second_string.end(),szfirst);
		if (match_len) *match_len = same_count;
		if (same_count<second_string.size())
			return strcmp<ox::character::__case_insensitive>(szfirst[same_count],second_string.begin()[same_count]);
		else
			return szfirst[same_count]-null_character;
	}

	static int sprintf(character * buffer, const character * format, ...)
	{
		va_list arglist;
		va_start(arglist, format);
		int size = string_traits::vsprintf (buffer,format,arglist);
		va_end(arglist);
		return size;
	};

	static int sprintf(std_string& out,const character * format, ...)
	{
		int size = 0;
		va_list arglist;
		va_start(arglist, format);
		size = vsprintf(out,format,arglist);
		va_end(arglist);
		return size;
	}

	static int vscprintf(const character *format,va_list argptr)
	{
		return string_traits::vscprintf(format,argptr);
	}

	static int vsprintf(character *buffer, const character *format, va_list argptr)
	{
		return string_traits::vsprintf(buffer,format,argptr);
	}

	static int vsprintf(std_string& buffer, const character *format, va_list arglist)
	{
		int size = 0;
		do {
			size = vscprintf(format,arglist);
			if (size<0) break;
			buffer.resize(size);
			int size2 = vsprintf((character*)buffer.data(),format,arglist);
			assert (size==size2);
			size = size2;
		} while (0);
		return size;
	}

	static int printf (const character * format, ...)
	{
		va_list arglist;
		va_start(arglist, format);
		int size = string_traits::vprintf (format,arglist);
		va_end(arglist);
		return size;
	}

	static int printf2(character const* str, size_t count=1)
	{
		int ret = 0;
		for (size_t i=0;i<count;++i)
		{
			int ret2 = printf("%s",str);
			if (ret2<0) return ret2;
			ret += ret2;
		}
		return ret;
	}

	/// select the leftest hit position, and the longer is first at the same time
	/// input: [)
	/// begin: text from
	/// end: text to
	/// return:
	/// if we find any, return the hit position
	/// else if return 0
	/// matched_end：the actual matched end
	template <ox::character::is_case_sensitive_c casive>
	struct find_any
	{
		find_any()
			: seps(0), seps_count(0), index_hit(-1), length_hit(0)
		{}
		template <size_t n>
		find_any(character const* (&_seps) [n])
			: seps(_seps), seps_count(n), index_hit(-1), length_hit(0)
		{}

		character const*const* seps;
		size_t seps_count;
		size_t index_hit;
		size_t length_hit;

		template <size_t n>
		void set_seps(character const* (&_seps) [n])
		{
			 seps = _seps;
			 seps_count = n;
		}
		character const* operator()(character const*begin,character const*end,character const**matched_end)
		{
			index_hit = -1;
			character const* mbegin_hit = end;
			character const* mend_hit = end;
			character const* mbegin = 0;
			character const* mend = 0;
			character const* sep = seps[0];
			for (size_t i = 0; i < seps_count; i++,sep=seps[i])
			{
				size_t len = length(sep);
				mbegin = strstr<casive>(begin,end,sep,sep+len,&mend);
				if (mbegin==0) continue;
				/// okay, we finded
				/// select the leftest
				if (mbegin<mbegin_hit)
				{
					mbegin_hit = mbegin;
					mend_hit = mend;
					index_hit = i;
					length_hit = len;
				}
				/// select the longer
				else if (mbegin==mbegin_hit)
				{
					if (mend>mend_hit) mend_hit = mend;
					index_hit = i;
					length_hit = len;
				}
			}
			/// find nothing, set the matched_end to end
			if (index_hit==-1)
			{
				if (matched_end) *matched_end = end;
				return 0;
			}
			/// okay we finded
			if (matched_end) *matched_end = mend_hit;
			return mbegin_hit;
		}
	};

	template <ox::character::is_case_sensitive_c casive>
	struct find2_any
	{
		find2_any(character const*begin=0,character const*end=0)
			: text_begin(begin), text_end(end)
		{}

		character const* text_begin;
		character const* text_end;

		template <size_t n>
		character const* operator()(character const* (&seps) [n],character const**matched_end)
		{
			size_t length_hit = 0;
			size_t index_hit = -1;
			character const* mbegin_hit = text_end;
			character const* mend_hit = text_end;
			character const* mbegin = 0;
			character const* mend = 0;
			character const* sep = seps[0];
			size_t const seps_count = n;
			for (size_t i = 0; i < seps_count; i++,sep=seps[i])
			{
				size_t len = length(sep);
				mbegin = strstr<casive>(text_begin,text_end,sep,sep+len,&mend);
				if (mbegin==0) continue;
				/// okay, we finded
				/// select the leftest
				if (mbegin<mbegin_hit)
				{
					mbegin_hit = mbegin;
					mend_hit = mend;
					index_hit = i;
					length_hit = len;
				}
				/// select the longer
				else if (mbegin==mbegin_hit)
				{
					if (mend>mend_hit) mend_hit = mend;
					index_hit = i;
					length_hit = len;
				}
			}
			/// find nothing, set the matched_end to end
			if (index_hit==-1)
			{
				if (matched_end) *matched_end = text_end;
				return 0;
			}
			/// okay we finded
			if (matched_end) *matched_end = mend_hit;
			return mbegin_hit;
		}
	};

	template <ox::character::is_case_sensitive_c casive>
	struct sameto_any
	{
		sameto_any(character const*begin=0,character const*end=0)
			: text_begin(begin), text_end(end)
		{}

		character const* text_begin;
		character const* text_end;

		template <size_t n>
		size_t operator()(character const* (&seps) [n],size_t* index_hit=0)
		{
			size_t length_hit = 0;
			size_t index_hit2 = -1;
			character const* sep = seps[0];
			size_t const seps_count = n;
			for (size_t i = 0; i < seps_count; i++,sep=seps[i])
			{
				size_t len = get_same_length<casive>(text_begin,text_end,sep);
				if (len==0) continue;
				if (*(sep+len)==0) /// finded
				{
					index_hit2 = i;
					if (len>length_hit) length_hit = len;
				}
			}
			if (index_hit) *index_hit = index_hit2;
			return length_hit;
		}
	};

	static size_t to_month_fromlong(character const* str,character const**strnext=0)
	{
		return to_month_fromlong(str,end(str),strnext);
	}
	static size_t to_month_fromlong(character const* strb,character const* stre,character const**strnext=0)
	{
		static ox::character::is_case_sensitive_c const casive = ox::character::__case_insensitive;

		static character const m1[] = {'J','a','n','u','a','r','y',0};
		static character const m2[] = {'F','e','b','r','u','a','r','a','y',0};
		static character const m3[] = {'M','a','r','c','h',0};
		static character const m4[] = {'A','p','r','i','l',0};
		static character const m5[] = {'M','a','y',0};
		static character const m6[] = {'J','u','n','e',0};
		static character const m7[] = {'J','u','l','y',0};
		static character const m8[] = {'A','u','g','e','s','t',0};
		static character const m9[] = {'S','e','p','t','e','m','b','e','r',0};
		static character const m10[] = {'O','c','t','o','m','b','e','r',0};
		static character const m11[] = {'N','o','v','e','m','b','e','r',0};
		static character const m12[] = {'D','e','c','e','m','b','e','r',0};

		static character const* months [] =
			{&m1[0],&m2[0],&m3[0],&m4[0],&m5[0],&m6[0],&m7[0],&m8[0],&m9[0],&m10[0],&m11[0],&m12[0]};

		sameto_any<casive> sameto(strb,stre);
		size_t index = -1;
		size_t len = sameto(months,&index);
		if (index==-1)
		{
			if (strnext) *strnext = stre;
			return 0;
		}
		strb += len;
		if (strnext) *strnext = strb;
		if (strb==stre || !is_alphabet(*strb))
			return index==-1?0:index+1;
		return 0;
	}
	static size_t to_month_fromshort(character const* str,character const**strnext=0)
	{
		return to_month_fromshort(str,end(str),strnext);
	}
	static size_t to_month_fromshort(character const* strb,character const* stre,character const**strnext=0)
	{
		static ox::character::is_case_sensitive_c const casive = ox::character::__case_insensitive;

		static character const m1[] = {'J','a','n',0};
		static character const m2[] = {'F','e','b',0};
		static character const m3[] = {'M','a','r',0};
		static character const m4[] = {'A','p','r',0};
		static character const m5[] = {'M','a','y',0};
		static character const m6[] = {'J','u','n',0};
		static character const m7[] = {'J','u','l',0};
		static character const m8[] = {'A','u','g',0};
		static character const m9[] = {'S','e','p',0};
		static character const m10[] = {'O','c','t',0};
		static character const m11[] = {'N','o','v',0};
		static character const m12[] = {'D','e','c',0};

		static character const* months [] =
			{&m1[0],&m2[0],&m3[0],&m4[0],&m5[0],&m6[0],&m7[0],&m8[0],&m9[0],&m10[0],&m11[0],&m12[0]};

		character const* matched_end = 0;
		sameto_any<casive> sameto(strb,stre);
		size_t index = -1;
		size_t len = sameto(months,&index);
		if (index==-1)
		{
			if (strnext) *strnext = stre;
			return 0;
		}
		strb += len;
		if (strnext) *strnext = strb;
		if (index==-1) return 0;
		if (strb==stre || !is_alphabet(*strb))
			return index+1;
		return 0;
	}

	/// 'a':am, 'p':pm, '\0':error
	static character to_ampm(character const* strb,character const**strnext=0)
	{
		return to_ampm(strb,end(strb),strnext);
	}
	/// 'a':am, 'p':pm, '\0':error
	static character to_ampm(character const* strb,character const* stre,character const**strnext=0)
	{
		static ox::character::is_case_sensitive_c const casive = ox::character::__case_insensitive;

		static character const m1[] = {'a','m',0};
		static character const m2[] = {'p','m',0};

		static character const* months [] = {&m1[0],&m2[0]};

		character const* matched_end = 0;
		sameto_any<casive> sameto(strb,stre);
		size_t index = -1;
		size_t len = sameto(months,&index);
		if (index==-1)
		{
			if (strnext) *strnext = stre;
			return '\0';
		}
		
		strb += len;
		if (strnext) *strnext = strb;
		if (index==-1) return '\0';
		if (strb==stre || !is_alphabet(*strb))
			return index==0?'a':'p';
		return '\0';
	}

	/// 按单个字符进行分割
	static size_t split
	(
		std_string & strbuffer,
		strings & string_items,
		const character seperator,
		size_t count = -1
	)
	{
		return split(
			strbuffer.c_str(),
			strbuffer.length(),
			string_items,
			seperator,
			count);
	}
	static size_t split
	(
		const character * szbuffer,
		strings & string_items,
		const character seperator,
		size_t count = -1
	)
	{
		return split(
			szbuffer,
			szbuffer==0?0:std_character_traits::length(szbuffer),
			string_items,
			seperator,
			count);
	}
	static size_t split
	(
		const character * buffer,
		size_t buffer_len,
		strings & string_items,
		const character seperator,
		size_t count = -1
	)
	{
		if (buffer==0)
			return 0;

		for (const character * p = buffer, *p_end = buffer+buffer_len;p<p_end;)
		{
			if (p==0) break;
			if (*p==seperator)
			{
				p++;
				continue;
			}
			// ps : ptr_seprator
			const character * ps = strch<true>(p,p_end,seperator);
			if (ps==0)
			{
				string_items.push_back(std_string(p,p_end-p));
				break;
			}
			if (ps==p) continue;
			string_items.push_back(std_string(p,ps-p));
			p=ps+1;

			if (string_items.size()>=count)
			{
				string_items.push_back(std_string(p,p_end-p));
				break;
			}
		}

		return string_items.size();
	};

	
	struct spliter
	{
		spliter()
			: _m_buffer(0)
			, _m_buffer_len(0)
			, _m_seperators(0)
			, _m_seperator_count(0)
			, _m_count(-1)
			, _m_include_sep(false)
			, _m_sep_replaced(false)
			, _m_sepnew(0)
		{}

		spliter(
			character const* buffer,
			size_t buffer_len,
			character* seperators,
			size_t seperators_len,
			size_t count = -1
			)
			: _m_count(count)
			, _m_include_sep(false)
			, _m_sep_replaced(false)
			, _m_sepnew(0)
		{
			set_buffer(buffer,buffer_len);
			set_seperators(seperators,seperators_len);
		}

		spliter(
			character const* szbuffer,
			character const* seperators=0,
			size_t count = -1
			)
			: _m_count(count)
			, _m_include_sep(false)
			, _m_sep_replaced(false)
			, _m_sepnew(0)
		{
			set_buffer(szbuffer);
			set_seperators(seperators);
		}

		character const* _m_buffer;
		size_t _m_buffer_len;
		character const* _m_seperators;
		size_t _m_seperator_count;
		size_t _m_count;
		bool _m_include_sep;
		bool _m_sep_replaced;
		character _m_sepnew;

		void set_buffer(character const* buffer)
		{
			_m_buffer = buffer;
			_m_buffer_len = buffer==0?0:std_character_traits::length(buffer);
		}

		void set_buffer(character const* buffer, size_t len)
		{
			_m_buffer = buffer;
			_m_buffer_len = len;
		}

		void set_seperators(character const* seps)
		{
			_m_seperators = seps;
			_m_seperator_count = seps==0?0:std_character_traits::length(seps);
		}
		void set_seperators(character const* seps, size_t len)
		{
			_m_seperators = seps;
			_m_seperator_count = len;
		}
	
		void set_sepnew(character const& cnew)
		{
			_m_sep_replaced = true;
			_m_sepnew = cnew;
		}
		void unset_sep()
		{
			_m_sep_replaced = false;
			_m_sepnew = 0;
		}

		void add_item(strings & string_items,character const* item, size_t n)
		{
			string_items.push_back(std_string(item,n));
		}
		void add_item(nstrings & string_items,character const* item, size_t n)
		{
			nstring const ns = {const_cast<character*>(item),n};
			string_items.push_back(ns);
		}

		size_t operator()(strings & string_items,character const& seperator)
		{
			_m_seperators = &seperator;
			_m_seperator_count = 1;
			return operator()(string_items);
		}
		size_t operator()(nstrings & string_items,character const& seperator)
		{
			_m_seperators = &seperator;
			_m_seperator_count = 1;
			return operator()(string_items);
		}
		size_t operator()(strings & string_items,character const* seperator)
		{
			set_seperators(seperator);
			return operator()(string_items);
		}
		size_t operator()(nstrings & string_items,character const* seperator)
		{
			set_seperators(seperator);
			return operator()(string_items);
		}
		template <typename strings_tn>
		size_t operator()(strings_tn & string_items)
		{
			character const* buffer = _m_buffer;
			size_t buffer_len = _m_buffer_len;
			character const* sepbegin = _m_seperators;
			character const* sepend = sepbegin+_m_seperator_count-1;
			size_t count = _m_count;

			if (buffer==0) return 0;

			for (const character * p = buffer, *p_end = buffer+buffer_len;p<p_end;)
			{
				if (p==0) break;
				//if (*p==seperator)
				if (is_in_chset<true>(*p,sepbegin,sepend))
				{
					if (_m_sep_replaced) *const_cast<character*>(p) = _m_sepnew;
					p++;
					continue;
				}
				if (string_items.size()>=count)
				{
					add_item(string_items,p,p_end-p);
					break;
				}
				// ps : ptr_seprator
				const character* ps = find_first_in<true>(p,p_end,sepbegin,sepend);
				if (ps==0 || ps>p_end)
				{
					add_item(string_items,p,p_end-p);
					break;
				}
				if (ps==p) continue;
				size_t len = ps-p;
				if (_m_include_sep) len += 1;//_m_seperator_count;
				add_item(string_items,p,len);
				p=ps;

				//if (string_items.size()>=count)
				//{
				//	add_item(string_items,p,p_end-p);
				//	break;
				//}
			}

			return string_items.size();
		};
	};


	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t split
	(
		const character* szbuffer,
		const character* seperator,
		strings & string_items
	)
	{
		return split<case_sensitive_tc>(
			szbuffer,
			szbuffer==0?0:std_character_traits::length(szbuffer),
			seperator,
			string_items
			);
	}

	template <ox::character::is_case_sensitive_c case_sensitive_tc>
	static size_t split
	(
		const character * buffer,
		size_t buffer_len,
		const character* seperator,
		strings & string_items
	)
	{
		if (buffer==0)
			return 0;

		const character* match_end = 0;
		for (const character * p = buffer, *p_end = buffer+buffer_len;p<p_end;)
		{
			if (p==0 || *p==0) break;

			// ps : ptr_seprator
			const character * ps = strstr<case_sensitive_tc>(p,p_end,seperator,&match_end);
			if (match_end==0 || match_end>=p_end) 
			{
				string_items.push_back(std_string(p,p_end-p));
				break;
			}
			if (ps!=p)
			{
				string_items.push_back(std_string(p,ps-p));
			}
			p=match_end;
		}

		return string_items.size();
	};
	

	/// leadingtext_needed, 开始的标志前的数据是否要: 要
	/// posttext_needed, 最后的标志后的数据是否要: 要
	/// zero_needed, 如果解析的串的长度为零，是否忽略: 忽略
	struct split2
	{
		split2()
		{
			leadingtext_needed = true; //  开始的标志前的数据是否要
			posttext_needed = true; //  最后的标志后的数据是否要
			zero_needed = true; // 如果解析的串的长度为零，是否忽略
		}
		bool leadingtext_needed; //  开始的标志前的数据是否要
		bool posttext_needed; //  最后的标志后的数据是否要
		bool zero_needed; // 如果解析的串的长度为零，是否忽略

		typedef delegate<character const*(character const*begin,character const*end,character const**matched_end)> find_func;
		typedef delegate<bool(character const*begin,character const*finded,character const*end)> handle_func;
		find_func findnext;
		handle_func handle;


		/// 返回命中sep的次数
		size_t operator()
		(
			const character * buffer_begin,
			const character * buffer_end
		)
		{
			if (buffer_begin==0 || buffer_end<=buffer_begin)
				return 0;
			if (handle.is_empty())
				return 0;

			character const* begin = buffer_begin;
			character const* end = buffer_end;
			character const* finded = 0;
			character const* matched_end = begin;
			size_t count = 0;
			bool continuable = true;

			while(true)
			{
				finded = findnext(begin,end,&matched_end);
				if (!finded) break;
				count ++;
				continuable = handle(begin,finded,matched_end);
				if (!continuable) break;
				begin = matched_end;
				if (begin>=end) break;
			}

			///  处理后导文字
			if (continuable && !finded && end>begin)
				handle(begin,end,end);

			return count;
		}

		struct thisfindnext
		{
			typedef typename strings::iterator I;
			character const* operator()(character const*begin,character const*end,character const** matched_end)
			{
				/// get the minmum ptr in all the finded
				bool anyfinded = false;
				character const* minmum_found = end;
				character const* minmum_matched = 0;
				for (I i=seps.begin(); i!=seps.end(); ++i)
				{
					character const* matched = 0;
					character const* finded = strstr<ox::character::__case_sensitive>(begin,end,i->c_str(),&matched);
					if (finded) anyfinded = true;
					if (finded && finded<minmum_found)
					{
						minmum_found = finded;
						minmum_matched = matched;
					}
				}
				if (anyfinded)
				{
					if (matched_end) *matched_end = minmum_matched;
					return minmum_found;
				}
				return 0;
			}
			strings seps;
		};

		struct thishandle
		{
			struct record
			{
				character const*begin;
				character const*finded;
				character const*end;
			};
			typedef std::vector<record> result_type;
			bool operator()(character const*begin,character const*finded,character const*end)
			{
				record r = {begin,finded,end};
				result.push_back(r);
				return true;
			}
			result_type result;
		};
	};


	/// you just provide the handle
	template <ox::character::is_case_sensitive_c casive>
	struct split3
	{
		typedef typename split2::handle_func handle_func;

		split3() : handle(split.handle)
		{
			split.findnext.assign(&find);
		}
		template <size_t n>
		split3(character const* (&_seps) [n])
			: handle(split.handle)
			, find(_seps)
		{
			split.findnext.assign(&find);
		}

		size_t operator()(const character* buff_begin,const character* buff_end)
		{
			return split(buff_begin,buff_end);
		}

		find_any<casive> find;
		split2 split;
		handle_func& handle;
	};

	/// you nothing, just set some properties
	template <ox::character::is_case_sensitive_c casive>
	struct split4
	{
		struct string2
		{
			character const* begin;
			character const* end;
			character const* end2;
		};
		typedef std::vector<string2> string2s;

		struct handle4
		{
			handle4() : isset_1stsep_0(false) ,maxsize(0) ,count(0) {}
			bool operator()(character const*begin,character const*finded,character const*end)
			{
				string2 str = {begin,finded,end};
				strs.push_back(str);
				/// we later can let the seps done by user, we just done by default
				if (isset_1stsep_0 && finded && end>finded)
					*const_cast<character*>(finded) = 0;
				/// if we find there has one item left, exist the handle
				/// the last item will be done by the main function, because we donot know the end of text
				if ((++count)+1>=maxsize) return false;
				return true;
			}
			string2s strs;
			size_t maxsize, count;
			bool isset_1stsep_0;
		};

		split4() : out(handle.strs), isset_1stsep_0(handle.isset_1stsep_0)
		{
			split.handle.assign(&handle);
		}
		template <size_t n>
		split4(character const* (&_seps) [n])
			: split(_seps)
			, out(handle.strs)
			, isset_1stsep_0(handle.isset_1stsep_0)
		{
			split.handle.assign(&handle);
		}

		template <size_t n>
		void set_seps(character const* (&_seps) [n])
		{
			split.find.set_seps(_seps);
		}

		size_t operator()(character const* buff_begin,character const* buff_end,size_t maxsize=-1)
		{
			/// init the handle fields
			handle.maxsize = maxsize;
			handle.count = 0;

			/// if the maxsize == 1, we just return the whole text
			if (maxsize==1)
			{
				string2 str = {buff_begin,buff_end,buff_end};
				out.push_back(str);
				return 1;
			}

			/// start the split
			size_t count = split(buff_begin,buff_end);

			/// if we set the maxsize of the items, and the maxsize >= 2
			if (maxsize!=-1 && count+1==maxsize)
			{
				/// get the last matched end, if no more text, return
				if (out.back().end2 >= buff_end) return count;
				/// okay, we find the rest chars
				string2 str = {out.back().end2,buff_end,buff_end};
				out.push_back(str);
				++ count; /// increase the count
			}
			/// return the item.count
			//assert (count==out.size());
			return count;
		}

		bool& isset_1stsep_0;
		split3<casive> split;
		handle4 handle;
		string2s& out;
	};

	/**
		顺序分割，尽量分割，采取顺序相同最大化
	*/
	static size_t sequence_split
	(
		const character * buffer_begin,
		const character * buffer_end,
		strings const & sequence_seperators,
		strings & split_result_items,
		bool is_include_before_first_sep = true, //  开始的标志前的数据是否要
		bool is_include_after_last_sep = true, //  最后的标志后的数据是否要
		bool is_null_string_added = true // 如果解析的串的长度为零，是否忽略
	)
	{
		if (buffer_begin==0)
			return 0;

		const character * match_begin = buffer_begin;
		const character * match_end = 0;
		const character * match_next_begin = 0;
		const character * match_next_end = 0;
		strings::const_iterator i = sequence_seperators.begin();
		strings::const_iterator i_end = sequence_seperators.end();

		match_begin=strstr(match_begin,buffer_end,i++->c_str(),&match_end);
		if (match_begin==0)
			return 0; // 没有匹配
		if (is_include_before_first_sep && match_begin-buffer_begin>0)
			split_result_items.push_back(std_string(buffer_begin,match_begin-buffer_begin));

		for (; 
			i!=i_end && (match_next_begin = strstr(match_end,buffer_end,i->c_str(),&match_next_end));
			match_begin = match_next_begin, match_end = match_next_end, i++)
		{
			if (is_null_string_added || !(match_next_begin-match_end<=0))
				split_result_items.push_back(std_string(match_end,match_next_begin-match_end));
		}

		if (is_include_after_last_sep && match_end<buffer_end)
			split_result_items.push_back(std_string(match_end,buffer_end-match_end));

		return split_result_items.size();
	}

	static size_t sequence_split
	(
		const character * szbuffer,
		strings const & sequence_seperators,
		strings & split_result_items,
		bool is_include_before_first_sep = true, //  开始的标志前的数据是否要
		bool is_include_after_last_sep = true, //  最后的标志后的数据是否要
		bool is_null_string_added = true // 如果解析的串的长度为零，是否忽略
	)
	{
		if (szbuffer==0)
			return 0;

		const character * match_begin = szbuffer;
		const character * match_end = 0;
		const character * match_next_begin = 0;
		const character * match_next_end = 0;
		strings::const_iterator i = sequence_seperators.begin();
		strings::const_iterator i_end = sequence_seperators.end();

		match_begin=strstr(match_begin,i++->c_str(),&match_end);
		if (match_begin==0)
			return 0; // 没有匹配
		if (is_include_before_first_sep && match_begin-szbuffer>0)
			split_result_items.push_back(std_string(szbuffer,match_begin-szbuffer));

		for (; 
			i!=i_end && (match_next_begin = strstr(match_end,i->c_str(),&match_next_end));
			match_begin = match_next_begin, match_end = match_next_end, i++)
		{
			if (is_null_string_added || !(match_next_begin-match_end<=0))
				split_result_items.push_back(std_string(match_end,match_next_begin-match_end));
		}

		if (is_include_after_last_sep && *match_end!=0)
			split_result_items.push_back(std_string(match_end));

		return split_result_items.size();
	}

	static size_t sequence_split
	(
		const character * szbuffer,
		const character * sequence_seperators [],
		size_t seps_count,
		strings & split_result_items,
		bool is_include_before_first_sep = true, //  开始的标志前的数据是否要
		bool is_include_after_last_sep = true, //  最后的标志后的数据是否要
		bool is_null_string_added = true // 如果解析的串的长度为零，是否忽略
	)
	{ 
		if (szbuffer==0||sequence_seperators==0||seps_count==0)
			return 0;

		const character * match_begin = szbuffer;
		const character * match_end = 0;
		const character * match_next_begin = 0;
		const character * match_next_end = 0;
		size_t i = 0;

		match_begin=strstr(match_begin,sequence_seperators[i++],&match_end);
		if (match_begin==0)
			return 0; // 没有匹配
		if (is_include_before_first_sep)
			split_result_items.push_back(std_string(szbuffer,match_begin-szbuffer));

		for (; 
			i<seps_count && (match_next_begin = strstr(match_end,sequence_seperators[i],&match_next_end));
			match_begin = match_next_begin, match_end = match_next_end, i++)
		{
			if (is_null_string_added || !(match_next_begin-match_end<=0))
				split_result_items.push_back(std_string(match_end,match_next_begin-match_end));
		}

		if (is_include_after_last_sep && *match_end!=0)
			split_result_items.push_back(std_string(match_end));

		return split_result_items.size();
	}
	static size_t sequence_split
	(
		const character * buffer_begin,
		size_t count, // 字符个数，不是字节个数
		strings const & sequence_seperators,
		strings & split_result_items,
		bool is_include_before_first_sep = true, //  开始的标志前的数据是否要
		bool is_include_after_last_sep = true, //  最后的标志后的数据是否要
		bool is_null_string_added = true // 如果解析的串的长度为零，是否忽略
	)
	{
		return sequence_split (
			buffer_begin,
			buffer_begin+count,
			sequence_seperators,
			split_result_items,
			is_include_before_first_sep,
			is_include_after_last_sep,
			is_null_string_added);
	}
	static size_t sequence_split
	(
		std_string & str_source,
		strings const & sequence_seperators,
		strings & split_result_items,
		bool is_include_before_first_sep = true, //  开始的标志前的数据是否要
		bool is_include_after_last_sep = true, //  最后的标志后的数据是否要
		bool is_null_string_added = true // 如果解析的串的长度为零，是否忽略
	)
	{
		return sequence_split (
			str_source.c_str(),
			str_source.size(),
			sequence_seperators,
			split_result_items,
			is_include_before_first_sep,
			is_include_after_last_sep,
			is_null_string_added);
	}

	/// we add the currnet pointer int the text space
	/// if the range is error, will return 0, 
	/// or else return !0
	template <ox::character::is_case_sensitive_c casive>
	struct get_line
	{
		get_line()
			: text_begin(0), text_end(0), current(0)
		{}
		get_line(character const* b, character const* e)
			: text_begin(b), text_end(e), current(b)
		{}

		void set_range(character const* b, character const* e)
		{
			text_begin = b, text_end = e, current = b;
		}

		character const* operator()(character const** end)
		{
			/// the text is valid
			if (!text_begin || !text_end)
				return 0;
			if (current >= text_end)
				return 0;

			static character const r[2] = {'\r',0};
			static character const rn[3] = {'\r','\n',0};
			static character const n[2] = {'\n',0};
			/// set the default seps, if the seps is null
			static character const* seps [] = {r,rn,n};
			if (find.seps==0) find.set_seps(seps);

			/// start to find 
			character const* start = current;
			character const* mend = 0;
			character const* mbegin = find(current,text_end,&mend);
			/// get the end of the finded
			if (end) *end = mbegin?mbegin:text_end;
			/// update current to the next
			current = mend;
			/// return the this started
			return start;
		}
		find_any<casive> find;
		character const* text_begin;
		character const* text_end;
		character const* current;
	};

	template <ox::character::is_case_sensitive_c casive>
	struct foreach_line
	{
		typedef get_line<casive> get_line;
		typedef delegate<bool(character const*,character const*)> line_handle;

		bool operator()(character const* text,character const* text_end,line_handle const& handle)
		{
			_m_end = text;
			if (text==0 || text_end==0)
				return false;
			if (text_end<text)
				return false;

			//// prepare get-line
			_m_get_line.set_range(text,text_end);

			character const* end = 0;
			character const* begin = 0;

			while (true)
			{
				begin = _m_get_line(&end);
				if (!begin || !*begin) break;
				if (!handle(begin,end)) break;
			}

			_m_end = _m_get_line.current;

			return !begin || !*begin;
		}
		get_line _m_get_line;
		character const* _m_end;
	};
};


typedef string_simple_kit<char> astring_kit;
typedef string_simple_kit<wchar_t> wstring_kit;


} // end of string
} // end of ox lib