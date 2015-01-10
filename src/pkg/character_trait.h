//
/////////////////////////////////////////////////////////////////////////////////
// ԭ�����ߣ��캣��
// ����ʱ�䣺2007-5-12
// ��ʷ��Ϣ�������ַ�����ص����ԣ��Լ�ͨ������
// ��Ȩ����Ȩ���������У����ļ��������ɴ���



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