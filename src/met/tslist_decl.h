


/**
	返回一个类型的自身的元函数
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"
#include "../ox/def_macro.h"
#include "../ox/tclasses.h"
#include "mod.h"
#include "type_for_param_passed.h"


#pragma once


___namespace1_begin(ox)


/*
	Two Rules:
	if the tslist is void, the tslist is null
	if the tslist is not void, the final next of tslist must be void
 */

template <typename type_tn, typename syms_tn=void, typename meta_tn=void, typename next_tn=void>
struct tslist
{
	//typedef ox::itot<idi==-1?(ox::met::is_same<t,void>::value?0:1):idi> idiff;

	typedef type_tn _t_type;
	typedef syms_tn _t_syms;
	typedef meta_tn _t_meta;

	typedef next_tn _t_next;


	///**
	//	self refector
	//*/
	//template <typename type_tn, typename next_tn>
	//struct refactor
	//{
	//	typedef tslist<type_tn,next_tn> type;
	//};
};





___namespace1_end()




___namespace2_begin(ox,met)

/**
@brief: 用ids进行标识类型，ids可以放在ext中
*/

template <typename type_tn, typename syms_tn=void, typename meta_tn=void>
struct ett
{
	typedef type_tn type;
	typedef syms_tn syms;
	typedef meta_tn meta;
};

template <typename derived_tn,typename super_tn>
struct metatt
{
	typedef derived_tn derived;
	typedef super_tn super;
};


/// 表示类型,因为在一个slist中,相同的类型会有多个
template <typename type_tn,size_t off=0,size_t _size=1>
struct taddr
{
	typedef type_tn type;
	static size_t const offset = off;
	static size_t const size = _size;
	template <size_t _off>
	struct reoffset
	{
		typedef taddr<type,_off,size> type;
	};
};

template <typename type_tn,size_t off=0,size_t _size=1>
struct saddr
{
	typedef type_tn type;
	static size_t const offset = off;
	static size_t const size = _size;
	template <size_t _off>
	struct reoffset
	{
		typedef saddr<type,_off,size> type;
	};
};

/// -1: the offset2 is not used
template <size_t off=-1,size_t _size=1>
struct index
{
	static size_t const offset = off;
	static size_t const size = _size;
	template <size_t _off>
	struct reoffset
	{
		typedef index<_off,size> type;
	};
};

template <typename index_tn>
struct is_index_type
{
	static bool const value = false;
};

template <size_t off,size_t size>
struct is_index_type <index<off,size>>
{
	static bool const value = true;
};

namespace ts
{
	template <typename tt>
	struct tof
	{
		typedef tt t;
	};
	template <typename t0,typename t1,typename t2>
	struct tof <ett<t0,t1,t2>>
	{
		typedef t0 t;
	};
	template <typename tt>
	struct sof
	{
		typedef void t;
	};
	template <typename t0,typename t1,typename t2>
	struct sof <ett<t0,t1,t2>>
	{
		typedef t1 t;
	};
	template <typename tt>
	struct mof
	{
		typedef void t;
	};
	template <typename t0,typename t1,typename t2>
	struct mof <ett<t0,t1,t2>>
	{
		typedef t2 t;
	};
}



___namespace2_end()