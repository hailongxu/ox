/*	
	类名: shell_kit
	作者: 徐海龙
	日期: 2005-6-20
	文件名: directory.h
	版权：版权归作者所有，本文件可以自由传播
 */


#pragma once

/// 定义空字符串，长度为零，只有

#include "./character_static_value.h"


namespace ox
{
namespace string
{

template <typename character_tn>
struct string_static_value;

template <>
struct string_static_value<char>
{
public:
	typedef char character_tn;
	typedef ox::character::character_trait<character_tn>::character character;
	static character * getp_c_null_string() { return ""; }
};

template <>
struct string_static_value<wchar_t>
{
public:
	typedef wchar_t character_tn;
	typedef ox::character::character_trait<character_tn>::character character;
	static character * getp_c_null_string() { return L""; }
};

//typedef character_static_value<char>

} // end of match
} // end of ox