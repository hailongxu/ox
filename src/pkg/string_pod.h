/*	
	类名: shell_kit
	作者: 徐海龙
	创建时间：2007-5-12
	文件名: podstring.h
	版权：版权归作者所有，本文件可以自由传播
 */

#include "../def/theory_type.h"
#include "./string_const.h"
#include "./character_static_value.h"
#include "./string_trait_of_length.h"

#pragma once

namespace ox
{
namespace str
{
	//// form of interval : former closed latter open
	template <typename character_tn, constant_t::string_mode_c string_mode_tc, ox::theory::polarity_c polarity_tc>
	struct podstring_tt {};

	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_interval,ox::theory::__positive>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__positive;
		static const constant_t::string_mode_c __mode = constant_t::__string_interval;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		character * begin_;
		character * end_;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return end_; }
		character front()const { return *begin(); }
		character back()const { return *end(); }
		size_t size()const { return end_-begin_; }
		bool isend(const character * pcharacterpos)const { return pcharacterpos>=end_; }
		bool isund(const character * pcharpos)const { return pcharpos<end_; }
		bool isempty()const { return isend(begin());}
		bool isvalid()const { return begin()!=0 && end()!=0; }
		static podstring_t make(const character * begin, const character * end)
		{
			podstring_t podstring = {begin,end};
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			podstring.end_ = end;
			return podstring;
		}
	};

	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_interval,ox::theory::__negative>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__negative;
		static const constant_t::string_mode_c __mode = constant_t::__string_interval;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		character * begin_;
		character * end_;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return end_; }
		character front()const { return *begin(); }
		character back()const { return *end(); }
		size_t size()const { return begin_-end_; }
		bool isend(const character * pcharacterpos)const { return pcharacterpos<=end_; }
		bool isund(const character * pcharpos)const { return pcharpos>end_; }
		bool isempty()const { return isend(begin());}
		bool isvalid()const { return begin()!=0 && end()!=0; }
		static podstring_t make(const character * begin, const character * end)
		{
			podstring_t podstring = {begin,end};
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			podstring.end_ = end;
			return podstring;
		}
	};

	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_size,ox::theory::__positive>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__positive;
		static const constant_t::string_mode_c __mode = constant_t::__string_size;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		character * begin_;
		size_t length;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return begin_+length; }
		size_t size()const { return length; }
		character front()const { return *begin(); }
		character back()const { return *end(); }
		bool isend(const character * pcharacterpos)const { return pcharacterpos>=end(); }
		bool isund(const character * pcharpos)const { return pcharpos<end(); }
		bool isempty()const { return isend(begin());}
		bool isvalid()const { return begin()!=0 && end()!=0; }
		static podstring_t make(character * begin, character * end)
		{
			podstring_t podstring = {begin,end-begin};
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			podstring.length = end-begin;
			return podstring;
		}
	};
	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_size,ox::theory::__negative>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__negative;
		static const constant_t::string_mode_c __mode = constant_t::__string_size;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		character * begin_;
		size_t length;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return begin_-length; }
		size_t size()const { return length; }
		character front()const { return *begin(); }
		character back()const { return *end(); }
		bool isend(const character * pcharacterpos)const { return pcharacterpos<=end(); }
		bool isund(const character * pcharpos)const { return pcharpos>end(); }
		bool isempty()const { return isend(begin()); }
		bool isvalid()const { return begin()!=0 && end()!=0; }
		static podstring_t make(const character * begin, const character * end)
		{
			podstring_t podstring = {begin,begin-end};
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			podstring.length = begin-end;
			return podstring;
		}
	};

	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_null,ox::theory::__positive>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__positive;
		static const constant_t::string_mode_c __mode = constant_t::__string_null;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		typedef ox::character::static_value<character> static_value;
		character * begin_;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return begin_+size(); }
		character front()const { return * begin(); }
		character back()const { return static_value::null_character; }
		size_t size()const { return string_traits_of_length<character>::length<ox::theory::__positive>(begin_); }
		bool isend(const character * pcharacterpos)const { return *pcharacterpos==static_value::null_character; }
		bool isund(const character * pcharpos)const { return *pcharpos!=static_value::null_character; }
		bool isempty()const { return isend(begin()); }
		bool isvalid()const { return begin()!=0; }
		static podstring_t make(const character * begin, const character * end)
		{
			podstring_t podstring = {begin};
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			*end=0;
			return podstring;
		}
	};

	template <typename character_tn>
	struct podstring_tt<character_tn,constant_t::__string_null,ox::theory::__negative>
	{
		static const ox::theory::polarity_c __polarity = ox::theory::__negative;
		static const constant_t::string_mode_c __mode = constant_t::__string_null;
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef podstring_tt<character,__mode,__polarity> podstring_t;
		typedef ox::character::static_value<character> static_value;
		character * begin_;
		character * & refp_begin() { return begin_; }
		character * begin()const { return begin_; }
		character * end()const { return begin_-size(); }
		character front()const { return * begin(); }
		character back()const { return static_value::null_character; }
		size_t size()const { return string_traits_of_length<character>::length<ox::theory::__negative>(begin_); }
		bool isend(const character * pcharacterpos)const { return *pcharacterpos==static_value::null_character; }
		bool isund(const character * pcharpos)const { return *pcharpos!=static_value::null_character; }
		bool isempty()const { return isend(begin()); }
		bool isvalid()const { return begin()!=0; }
		static podstring_t make(const character * begin, const character * end)
		{
			podstring_t podstring = {begin};
			*end=0;
			return podstring;
		}
		static podstring_t & make(const character * begin, const character * end, podstring_t & podstring)
		{
			podstring.begin_ = begin;
			*end=0;
			return podstring;
		}
	};


	template <typename character_tn> struct string_tt
	{
		typedef typename ox::character::character_trait<character_tn>::character character;

		template <constant_t::string_mode_c string_mode_tc,ox::theory::polarity_c polarity_tc>
		struct type_tt
		{
			typedef podstring_tt<character,string_mode_tc,polarity_tc> string_t;
		};

		template <constant_t::string_mode_c string_mode_tc>
		struct type_mode_tt
		{
			typedef podstring_tt<character,string_mode_tc,ox::theory::__positive> posstring_t;
			typedef podstring_tt<character,string_mode_tc,ox::theory::__negative> negstring_t;
			template <ox::theory::polarity_c polarity_tc>
			struct type_polity_tt
			{
				typedef podstring_tt<character,string_mode_tc,polarity_tc> string_t;
			};
		};

		template <ox::theory::polarity_c polarity_tc>
		struct type_polity_tt
		{
			typedef podstring_tt<character,constant_t::__string_interval,polarity_tc> stringi_t;
			typedef podstring_tt<character,constant_t::__string_size,polarity_tc> stringn_t;
			typedef podstring_tt<character,constant_t::__string_null,polarity_tc> stringz_t;
			template <constant_t::string_mode_c string_mode_tc>
			struct type_mode_tt
			{
				typedef podstring_tt<character,string_mode_tc,polarity_tc> string_t;
			};
		};

		typedef typename type_tt<constant_t::__string_interval,ox::theory::__positive>::string_t stringi;
		typedef typename type_tt<constant_t::__string_size,ox::theory::__positive>::string_t stringn;
		typedef typename type_tt<constant_t::__string_null,ox::theory::__positive>::string_t stringz;

		typedef typename type_tt<constant_t::__string_interval,ox::theory::__negative>::string_t rstringi;
		typedef typename type_tt<constant_t::__string_size,ox::theory::__negative>::string_t rstringn;
		typedef typename type_tt<constant_t::__string_null,ox::theory::__negative>::string_t rstringz;
	};

	template <constant_t::string_mode_c string_mode_tc, ox::theory::polarity_c polarity_tc>
	struct tostring_tt
	{
		typedef podstring_tt<char,string_mode_tc,polarity_tc> astring_t;
		typedef podstring_tt<wchar_t,string_mode_tc,polarity_tc> wstring_t;

		template <typename character_tn>
		struct ctype_tt
		{
			typedef typename ox::character::character_trait<character_tn>::character character;
			typename podstring_tt<character,string_mode_tc,polarity_tc> cstring_t;
		};
	};

	typedef string_tt<char>:: stringi  astringi;	typedef string_tt<wchar_t>:: stringi  wstringi;
	typedef string_tt<char>:: stringn  astringn;	typedef string_tt<wchar_t>:: stringn  wstringn;
	typedef string_tt<char>:: stringz  astringz;	typedef string_tt<wchar_t>:: stringz  wstringz;
	typedef string_tt<char>::rstringi rastringi;	typedef string_tt<wchar_t>::rstringi rwstringi;
	typedef string_tt<char>::rstringn rastringn;	typedef string_tt<wchar_t>::rstringn rwstringn;
	typedef string_tt<char>::rstringz rastringz;	typedef string_tt<wchar_t>::rstringz rwstringz;


} // end of string
} // end of ox