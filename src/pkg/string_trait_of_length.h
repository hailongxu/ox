//
/////////////////////////////////////////////////////////////////////////////////
// 原创作者：徐海龙
// 创建时间：2007-5-12
// 历史信息：2007-7-11
// 版权：版权归作者所有，本文件可以自由传播



/// 为了和podstring的定义不产生循环引用的，所以另立了这个文件

/// for ox_string_traits
#include <string>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../def/theory_type.h"
#include "./character_trait.h"
#include "./character_static_value.h"

#pragma once
#pragma warning (disable : 4996)


namespace ox
{
namespace string
{


template <typename character_tn>
struct string_traits_of_length {};

template <>
struct string_traits_of_length<char>
{
	typedef ox::character::character_trait<char>::character character;

	/// return characters count
	static size_t length(character const * szstr)
	{
		return length<ox::theory::__positive>(szstr);
	}
	template <ox::theory::polarity_c polarity_tc> static size_t length(character const * szstr)
	{
		assert(szstr);
		return strlen(szstr);
	}
	template <>	static size_t length<ox::theory::__negative>(character const * szstr)
	{
		assert(szstr);
		const character *eos=szstr; 
		while(*eos--); 
		return szstr-eos-1;
	}
};

template <>
struct string_traits_of_length<wchar_t>
{
	typedef ox::character::character_trait<wchar_t>::character character;

	/// return characters count
	static size_t length(character const * szstr)
	{
		return length<ox::theory::__positive>(szstr);
	}
	template <ox::theory::polarity_c polarity_tc> static size_t length(character const * szstr)
	{
		assert(szstr);
		return wcslen(szstr);
	}
	template <>	static size_t length<ox::theory::__negative>(character const * szstr)
	{
		assert(szstr);
		const character *eos=szstr;
		while(*eos--);
		return szstr-eos-1;
	}
};


} // end of string
} // end of ox lib