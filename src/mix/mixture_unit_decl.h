
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
	2012-9-22
	作者：徐海龙
	加入了ett的支持
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