
/**
	2010-11-10
	���ߣ��캣��
	����һ�����Ӽ򵥵ĸ��������loki�µ�typelist
	����������ԭ����mixture������ԭ�����Ǹ�mixture�ǻ��������͵ģ��ܸ��ӣ��������
	����������д��һ������򵥵ĵ��������Ƚ�����
	2012-9-22
	���ߣ��캣��
	������ett��֧��
*/

#include "../ox/nsab.h"



#pragma once


___namespace2_begin(ox,mix)


/**
	general instantiation of any type
*/





/// does we need to difine the condition when the sl is not a sl
/// or we implement in mixture directly
//template <typename sl,typename ms,typename ulmd,typename i,typename rll>
//template <typename type,size_t i,size_t iow,size_t b,typename oww>
//struct instantiate_of_type;
//
//
//
//template <typename sym,size_t i,size_t iow,size_t b,typename oww>
//struct instantiate_of_syms;



template <typename sym,size_t i,size_t iow,size_t idi,typename inf>
struct mixture_data;



template <typename sym,size_t i,size_t iow,size_t idi,typename inf>
struct method_default;



template <typename sym,size_t i,size_t iow,size_t idi,typename inf>
struct data_method_default;



template <typename sym,size_t i,size_t iow,size_t idi,typename inf>
struct extern_method_default;





___namespace2_end()