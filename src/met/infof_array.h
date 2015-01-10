
/**
	2008-12-7
	���ߣ��캣��
	�������Ԫ��������ƣ����ṩԪ��������Ԫ�����������������������������
	2009-8-23
	���ߣ��캣��
	��������������ĸ������������Ҫ�߱�������
*/

#include "../ox/nsab.h"
#include "../ox/nothing.h"


#pragma once

___namespace2_begin(ox,met)



/// arr info
template <typename t>
struct inf_of_arr
{
	typedef ox::null_type type;
	static size_t const value = 0;
};
template <typename t, size_t n>
struct inf_of_arr <t[n]>
{
	typedef t type;
	static size_t const value = n;
};



___namespace2_end()