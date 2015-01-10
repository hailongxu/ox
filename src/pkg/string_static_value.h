/*	
	����: shell_kit
	����: �캣��
	����: 2005-6-20
	�ļ���: directory.h
	��Ȩ����Ȩ���������У����ļ��������ɴ���
 */


#pragma once

/// ������ַ���������Ϊ�㣬ֻ��

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