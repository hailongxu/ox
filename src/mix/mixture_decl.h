
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "mixture_unitize.h"


#pragma once


___namespace2_begin(ox,mix)



/**
	typename sl, 一个类型列表必须是 ox::met::tslist<>
	typename ms, metas 你可以为mixture绑定元信息
	/// no using of the parameter : typename typeof_unit_tn, 每个元素也有相关管理信息,那么这个定义,如何从这个unit-type中获取实际使用的信息
	typename go, get-owner
	typename mlmd, method_lambda 对一个方法的具现,用来扩充数据信息用的
	typename ulmd, unit_lambda 把一个元素信息,包括管理信息,声称一个可以使用的单元类型
	typename diff, which is different with other type, so you can do something for certain type
	typename i, index 表示第一个类型的开始编号 should be zero
	typename rll, reversed left list, when we instantiate one element, the type instantiated will be pushed front to rll
		rll, is also a list too
*/



template
<
	typename sl,
	typename uz = unitize,
	typename ms = void,
	typename edi = void
>
struct mixture;


template <typename syms,size_t _j=-1>
struct saddr
{
	typedef syms i;
	static size_t const j = _j;
	template <size_t jj>
	struct change
	{
		typedef saddr<i,jj> type;
	};
};

template <size_t _i,size_t _j=-1>
struct index
{
	static size_t const i = _i;
	static size_t const j = _j;
	template <size_t jj>
	struct change
	{
		typedef index<i,jj> type;
	};
};

template <typename index>
struct is_index_type
{
	static bool const value = false;
};
template <size_t i,size_t j>
struct is_index_type <index<i,j>>
{
	static bool const value = true;
};

template <typename t>
struct tcct
{
	tcct(t const& v) : o(v) {}
	//operator t() { return o; }
	t const& value() const { return o; }
	t const& o;
};

template <typename t> tcct<t> tcc(t const& o) { return tcct<t>(o); }


struct ignore_type
{
	ignore_type unit() { return ignore_type(); }
	ignore_type const unit() const { return ignore_type(); }
	ignore_type next() {return ignore_type();}
	ignore_type const next() const {return ignore_type();}
	template <bool b> ignore_type dnext() {return ignore_type();}
	template <bool b> ignore_type const dnext() const {return ignore_type();}
};


inline bool operator==(const ignore_type&, const ignore_type&) {return true;}
inline bool operator!=(const ignore_type&, const ignore_type&) {return false;}
inline bool operator<(const ignore_type&, const ignore_type&) {return false;}
inline bool operator>(const ignore_type&, const ignore_type&) {return false;}
inline bool operator<=(const ignore_type&, const ignore_type&) {return false;}
inline bool operator>=(const ignore_type&, const ignore_type&) {return false;}



template <typename t>
struct membered : identity<t>
{
	static size_t const value = 2;
};

template <typename t>
struct stacked : identity<t>
{
	static size_t const value = 3;
};



/// features of vertical hierechy of mixture

template <typename s,typename t=void,typename n=void>
struct mlist
{
	typedef void _t_type;
	typedef s _t_syms;
	typedef n _t_next;
	typedef t unit;
	typedef ox::itot<0> idiff;
};
template <typename s,typename t,typename n>
struct mlist <s,membered<t>,n>
{
	typedef t _t_type;
	typedef s _t_syms;
	typedef n _t_next;
	typedef void unit;
	typedef ox::itot<2> idiff;
};

template <typename s,typename t,typename n>
struct mlist <s,stacked<t>,n>
{
	typedef s _t_syms;
	typedef t _t_type;
	typedef n _t_next;
	typedef void unit;
	typedef ox::itot<3> idiff;
};

template <typename ml,size_t i=0>
struct tmlist
{
	typedef typename ml::_t_type _t_type;
	typedef typename ml::_t_syms _t_syms;
	typedef typename ml::_t_next _t_next;
	typedef typename ml::unit unit;
	typedef typename ml::idiff idiff;
	static size_t const i = i;
};
template <size_t i>
struct tmlist <void,i>
{
	typedef void _t_type;
	typedef void _t_syms;
	typedef void _t_next;
	typedef void unit;
	typedef ox::itot<0> idiff;
	static size_t const i = 0;
};



___namespace2_end()




#include "../met/mod.h" /// tlist
#include "../met/type_for_param_passed.h" /// tlist



___namespace2_begin(ox,mix)


/// sl: the type list of data
/// i : the current position of type list
/// nextid: if has next nextid is 1, or nextid is 0
/// rl: the used list will be pushed front into the rll

template <typename sl,size_t i=0,size_t nextid=ox::met::is_void<typename sl::_t_next>::value?0:1,typename rl=void>
struct tlist
{
	typedef sl slist;
	typedef rl rlist;
	typedef typename sl::_t_type _t_type;
	typedef typename sl::_t_syms _t_syms;
	typedef typename sl::_t_meta _t_meta; /// need removed from ox::met::tslist ???
	typedef ox::itot<ox::met::is_void<_t_type>::value?0:1> idiff;
	static size_t const i = i;
	typedef ox::met::param_type_for_init<_t_type> wp0;
};



___namespace2_end()