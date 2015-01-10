//
/////////////////////////////////////////////////////////////////////////////////
// 原创作者：徐海龙
// 创建时间：2007-5-12
// 历史信息：定义字符串相关的特性，以及通用类型
// 版权：版权归作者所有，本文件可以自由传播



#pragma once
#pragma warning (disable : 4996)

namespace ox
{
namespace character
{

enum is_case_sensitive_c
{
	__case_sensitive = 1,
	__case_insensitive,
};

template <typename character_tn> struct character_trait;

template <> struct character_trait<char>
{
	typedef char character;
	typedef unsigned char ucharacter;
};

template <> struct character_trait<wchar_t>
{
	typedef wchar_t character;
	typedef wchar_t ucharacter;
};

} // end of character
} // end of ox lib