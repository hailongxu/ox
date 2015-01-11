
/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 2009-3-24
 */

#include "../ox/nsab.h"
#include "../met/_if_switch.h"
#include "../met/tslist.h"
#include "./mixture_decl.h"

#pragma once


___namespace2_begin(ox,mix)


template <typename type_tn,typename idi=typename type_tn::idiff>
struct sizeof_tulist;

template <template<size_t>class tsl,size_t i,typename idi>
struct sizeof_tulist <tsl<i>,idi>
{
	static size_t const value =
		(idi::value ? 1 : 0 ) +
		sizeof_tulist<tsl<i-1>,typename tsl<i-1>::idiff>::value;
};
template <template<size_t>class tsl,typename idi>
struct sizeof_tulist <tsl<1>,idi>
{
	static size_t const value = idi::value?1:0;
};
template <template<size_t>class tsl,typename idi>
struct sizeof_tulist <tsl<0>,idi>
{
	static size_t const value = 0;
};

template <typename type_tn>
struct sizeof_mixture
{
	static size_t const value = sizeof_tulist<typename type_tn::ifeature>::value;
};


___namespace2_end()
