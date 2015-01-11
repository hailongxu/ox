
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/


/// 能否把数据类型也转化成简单类型
/// 数据项的syms的第一项不作函数化

#include "../ox/nsab.h"
#include "../ox/nothing.h"
#include "../met/_if_switch.h"
#include "../met/mod.h"
#include "../met/tslist.h"
#include "../met/type_for_param_passed.h"
#include "../cxx/tags.h"
#include "./mixture_unit.h"
#include "./mixture_decl.h"
#include "./mixture_method.h"
#include "./mixture_conv.h"
#include "./mixture_type_access.h"
#include "./mixture_conv.h"
#include "./mixture_param.h"


#pragma once


___namespace2_begin(ox,mix)




/*
	mixture's paramter
	sl : slist
	ms : ms
	unitize : unit-unitize
	rll : reversed left list
	go : get owner
	i : index/i

	mixture's meta 
	slist
	metas
	unitize
	index
	rllist
 */

///// if the tslist is a type do nothing, whatmore, we assert false here
//template <typename sl,typename ms,typename uz,typename diff,typename go/*rll*/,typename i>
//struct mixture
//	: unitize::ulmd::apply<tslisto<itot<0>,itot<1>,itot<0>,sl> ,itot<0>,i,mixture<sl,ms,uz,diff,go,i/*,rll*/> >::type
//{
//	___static_assert_same_type(i,itot<0>);
//	//___static_assert_same_type(go/*rll*/,void);
//
//	/// in fact, here will never comes, only when the sl is null
//	/// the element's meta, for the compatible with the tslist
//	typedef void _t_type;
//	typedef void _t_syms;
//	typedef void _t_meta;
//	typedef void _t_next;
//
//	/// mixture's meta
//	typedef sl slist;
//	typedef ms metas;
//	typedef unitize unitize;
//	typedef diff diff;
//	typedef i index;
//	//typedef rll rllist;
//	typedef go gowner;
//
//	typedef mixture self;
//	typedef self owner_type;
//
//	typedef sl slist_type;
//	typedef typename unitize::ulmd::apply<tslisto<itot<0>,itot<1>,itot<0>,sl>,itot<0>,go/*mixture<sl,ms,uz,diff,i,rll>*/,i >::type unit_type;
//	typedef typename met::param_type_for_init<slist_type>::type t0;
//
//
//	struct size { static size_t const value = 0; };
//
//
//	/// null default constructor
//	mixture()
//	{}
//	/// copy constructor
//	mixture(mixture const& other)
//		: unit_type(other.unit())
//	{}
//	/// template copy constructor
//	template <typename _sl, typename _ms, typename _uz,typename _di,typename _go/*_rll*/,typename _i>
//	mixture(mixture<_sl,_ms,_uz,_di,_go/*_rll*/,_i>const& other)
//		: unit_type(other.unit())
//	{}
//	///// template copy constructor : when the other is single type too
//	//template <typename _t, typename _ms, typename _uz,typename _di,typename _go/*_rll*/,typename _i>
//	//mixture(mixture<_t,_ms,_uz,_di,_go/*_rll*/,_i>const& other)
//	//	: unit_type(other.unit())
//	//{}
//	/// first element contructor
//	mixture(t0 p0)
//		: unit_type(ox::refof(p0))
//	{}
//
//	/// get unit base
//	unit_type& unit() { return *static_cast<unit_type*const>(this); }
//	unit_type const& unit() const { return *static_cast<unit_type const*const>(this); }
//	/// get next base, when not, return int, just as a non-mixture type
//	int next() {return 0;}
//	int const next() const {return 0;}
//
//	/// assign the value transferred to assign unit
//	/// at present, just assign to the first unit, later, we will assign a list to this
//	template <typename _t>
//	self& operator=(_t const& o) { unit()=o; return *this; }
//};
//
/// if the slist is an array
template <typename t,size_t n,typename ms>
struct mixture <t[n],void,ms,ox::tags::static_array>
{
	//___static_assert_same_type(i,itot<0>);
	//___static_assert_same_type(go/*rll*/,void);


	/// mixture's meta
	typedef t tsli [n];
	typedef void unitize;
	typedef ms metas;
	typedef ox::tags::static_array ediff;
	typedef void idiff;


	/// in fact, here will never comes, only when the sl is null
	/// the element's meta, for the compatible with the tslist
	typedef tsli _t_type;
	typedef void _t_syms;
	typedef void _t_meta;
	typedef void _t_next;

	typedef mixture self;
	typedef self owner_type;
	typedef _t_type unit_type;

	typedef typename met::param_type_for_init<_t_type>::type t0;

	/// get unit base
	unit_type& unit() { return _m_array; }
	unit_type const& unit() const { return _m_array; }

	ox::empty_type next() {return ox::empty_type();}
	ox::empty_type const next() const {return ox::empty_type();}
	
	unit_type _m_array;

	/// assign the value transferred to assign unit
	/// at present, just assign to the first unit, later, we will assign a list to this
	template <typename _t>
	self& operator=(_t const& o) { unit()=o; return *this; }
};


/*
t: t
s: syms_tn
m: m
tn: type_of_next_tn
sn: ids_of_next_tn
mn: meta_of_next_tn
nn: next_of_next_tn
ms: ms of mixture
unitize: unit-of-lambda
i: index
rll: reversed left list
go: get owner
*/

/// the type is tslist, get the type and instantiate it
template <template<size_t>class tsl,size_t i,typename uz,typename ms,typename edi>
struct mixture<tsl<i>,uz,ms,edi>
	: methodlmd_apply<typename tsl<i>::_t_syms,tsl<i>,uz,ms,edi>::type
	, mixture<tsl<i-1>,uz,ms,edi>
{
	static size_t const index = i;
	/// mixture's meta
	typedef tsl<index> ifeature;
	typedef uz unitize;
	typedef ms metas;
	typedef edi ediff;
	typedef typename ifeature::idiff idiff;

	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_type _t_type;
	typedef typename ifeature::_t_syms _t_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef mixture<tsl<i-1>,uz,ms,edi> _t_next;

	typedef mixture self;
	typedef mixture<tsl<tsl<0>::n>,uz,ms,edi> owner_type;

	typedef typename tulist_lmd<_t_syms>::apply<0>::type ssl0;

	typedef typename methodlmd_apply<typename tsl<i>::_t_syms,tsl<i>,uz,ms,edi>::type method_type;
	typedef typename methodlmd_apply<ssl0,tsl<i>,uz,ms,edi>::type unit_type;
	typedef typename ox::itot<tsl<0>::n> size;



	template <bool b> struct dnext_helper
		: ox::mix::dnext_helper<typename dfeature_next<ifeature,b>::type,uz,ms,edi>
	{};

	/// null default constructor
	self()
	{}
	/// copy constructor
	self(self const& _other)
		: method_type(_other.unit())
		, _t_next(_other.next())
	{}
	/*
	c o n /// current other next
	1 1 0
	1 0 1
	0 0 x
	0 1 x
	*/
	/// template copy constructor
	template <typename _sl,typename _uz,typename _ms,typename _ed>
	self(tcct<mixture<_sl,_uz,_ms,_ed>> _other)
		: method_type(_other.value().dnext<idiff::value && !_sl::idiff::value>().unit())
		, _t_next(tcc(_other.value().dnext<idiff::value>().next()))
	{}
	
	/// first elment do nothing, mainly due to support alias init2 params
	/// when alias mode is *, we can accept &, in case of 
	/// if the begin type is not empty, ignore is empty
	/// else  ignore is identity<empty>
	/// that is to say, if you input empty instance, do nothing

	self(tcct<ox::empty_type>const&)
	{}
	/// 1 element contructor
	template <typename t0>
	self(t0 p0)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p0))),ox::itot<idiff::value?0:1>())
	{}
	/// 2 elements constructor
	template <typename t0,typename t1>
	self(t0 p0,t1 p1)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?1:2>())
	{}
	/// 3 elements constructor
	template <typename t0,typename t1,typename t2>
	self (t0 p0,t1 p1,t2 p2)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?2:3>())
	{}
	/// 4 elements constructor
	template <typename t0,typename t1,typename t2,typename t3>
	self (t0 p0,t1 p1,t2 p2,t3 p3)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p3),ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?3:4>())
	{}
	/// 5 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4>
	self (t0 p0,t1 p1,t2 p2,t3 p3,t4 p4)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p4),ox::refof(p3),ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?4:5>())
	{}
	/// 6 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5>
	self (t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p5),ox::refof(p4),ox::refof(p3),ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?5:6>())
	{}
	/// 7 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	self (t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p6),ox::refof(p5),ox::refof(p4),ox::refof(p3),ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?6:7>())
	{}
	/// 8 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	self (t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7)
		: method_type(ox::refof(p0))
		, _t_next(allparams(param(ox::refof(p7),ox::refof(p6),ox::refof(p5),ox::refof(p4),ox::refof(p3),ox::refof(p2),ox::refof(p1),ox::refof(p0))),ox::itot<idiff::value?7:8>())
	{}
	/// set constructor
	template <typename t,size_t ii>
	self (ox::mix::tallparams<t> p,ox::itot<ii> const&)
		: method_type(p.o.o.v<ii-1>())
		, _t_next(p,ox::itot<idiff::value?ii-1:ii>())
	{}
	template <typename t>
	self (ox::mix::tallparams<t> p,ox::itot<0> const&)
	{}


	/// get unit base
	unit_type& unit() { return *static_cast<unit_type*const>(this); }
	unit_type const& unit() const { return *static_cast<unit_type const*const>(this); }
	/// get next base
	_t_next& next() { return *static_cast<_t_next*const>(this); }
	_t_next const& next() const { return *static_cast<_t_next const*const>(this); }

	template <bool b> typename dnext_helper<b>::retype dnext() { return dnext_helper<b>::get(this); }
	template <bool b> typename dnext_helper<b>::const_retype dnext() const { return dnext_helper<b>::get(this); }

	/// assign the value transferred to assign unit
	/// at present, just assign to the first unit, later, we will assign a list to this
	template <typename _t>
	self& operator=(_t const& o)
	{
		unit()=o;
		return *this;
	}
	self& operator=(self const& _other)
	{
		unit() = _other.unit();
		dnext() = _other.dnext();
		return *this;
	}
	template <typename _sl,typename _uz,typename _ms,typename _ed>
	self& operator=(tcct<mixture<_sl,_uz,_ms,_ed>> _other)
	{
		assign(
			dnext<!idiff::value>(),
			_other.value().dnext<!_sl::idiff::value>());
		return *this;
	}

	template <typename t1,typename t2>
	static void assign(t1& _this_sync,t2 const& _other_sync)
	{
		_this_sync.unit() = _other_sync.unit();
		assign(_this_sync.dnext<true>(),_other_sync.dnext<true>());
	}
	template <typename t1>
	static void assign(t1&,ox::empty_type const&)
	{}
	template <typename t2>
	static void assign(ox::empty_type const&,t2 const&)
	{}
	static void assign(ox::empty_type const&,ox::empty_type const&)
	{}
};

template <template<size_t>class tsl,typename uz,typename ms,typename edi>
struct mixture<tsl<1>,uz,ms,edi>
	: methodlmd_apply<typename tsl<1>::_t_syms,tsl<1>,uz,ms,edi>::type
{
	static size_t const index = 1;
	/// mixture's meta
	typedef tsl<index> ifeature;
	typedef uz unitize;
	typedef ms metas;
	typedef typename ifeature::idiff idiff;
	typedef edi ediff;

	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_type _t_type;
	typedef typename ifeature::_t_syms _t_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef void _t_next;

	typedef typename tulist_lmd<_t_syms>::apply<0>::type ssl0;
	typedef mixture self;
	typedef mixture<tsl<tsl<0>::n>,uz,ms,edi> owner_type;
	typedef typename methodlmd_apply<typename tsl<1>::_t_syms,tsl<1>,uz,ms,edi>::type method_type;
	typedef typename methodlmd_apply<ssl0,tsl<1>,uz,ms,edi>::type unit_type;
	typedef typename itot<ifeature::i> size;

	/// null default constructor
	self ()
	{}
	/// copy constructor
	self (mixture const& _other)
		: method_type(_other.unit())
	{}
	template <typename _sl,typename _uz,typename _ms,typename _edi>
	self (tcct<mixture<_sl,_uz,_ms,_edi>> _other)
		: method_type(_other.value().dnext<idiff::value && !_sl::idiff::value>().unit())
	{}
	///// when param is empty_type
	self (tcct<ox::empty_type>const&)
	{}
	/// 1 element contructor
	template <typename t0>
	self(t0 p0)
		: method_type(ox::refof(p0))
	{}
	/// 2 elements constructor
	template <typename t0,typename t1>
	self (t0 p0,t1 const&)
		: method_type(ox::refof(p0))
	{}
	/// 3 elements constructor
	template <typename t0,typename t1,typename t2>
	self (t0 p0,t1 const&,t2 const&)
		: method_type(ox::refof(p0))
	{}
	/// 4elements constructor
	template <typename t0,typename t1,typename t2,typename t3>
	self (t0 p0,t1 const&,t2 const&,t3 const&)
		: method_type(ox::refof(p0))
	{}
	/// 5 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4>
	self (t0 p0,t1 const&,t2 const&,t3 const&,t4 const&)
		: method_type(ox::refof(p0))
	{}
	/// 6 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5>
	self (t0 p0,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&)
		: method_type(ox::refof(p0))
	{}
	/// 7 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	self (t0 p0,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&,t6 const&)
		: method_type(ox::refof(p0))
	{}
	/// 8 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	self (t0 p0,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&,t6 const&,t7 const&)
		: method_type(ox::refof(p0))
	{}

	/// set constructor
	template <typename p,size_t ii>
	self (ox::mix::tallparams<p> p,ox::itot<ii> const&)
		: method_type(p.o.o.v<ii-1>())
	{}
	template <typename p>
	self (ox::mix::tallparams<p> p,ox::itot<0> const&)
	{}


	// get base unit
	unit_type& unit() { return *static_cast<unit_type*const>(this); }
	unit_type const& unit() const { return *static_cast<unit_type const*const>(this); }
	/// get next base, when not, return int, just as a non-mixture type
	ox::empty_type next() {return ox::empty_type();}
	ox::empty_type const next() const {return ox::empty_type();}
	/// next data-mixture
	template <bool b> ox::empty_type dnext() {return ox::empty_type();}
	template <bool b> ox::empty_type const dnext() const {return ox::empty_type();}

	///// assign the value transferred to assign unit
	///// at present, just assign to the first unit, later, we will assign a list to this
	template <typename _t>
	self& operator=(_t const& o)
	{
		unit()=o;
		return *this;
	}
	self& operator=(self const& _other)
	{
		unit() = _other.unit();
		return *this;
	}
	template <typename _sl,typename _uz,typename _ms,typename _ed>
	self& operator=(tcct<mixture<_sl,_uz,_ms,_ed>> _other)
	{
		assign(
			nextif<!!idiff::value>(),
			_other.value().nextif<!!idiff::value && !!_sl::idiff::value>());
		return *this;
	}
	template <typename t1,typename t2>
	static void assign(t1& _this_sync,t2 const& _other_sync)
	{
		_this_sync.unit() = _other_sync.unit();
	}
	template <typename t1>
	static void assign(t1&,ox::empty_type const&)
	{}
	template <typename t2>
	static void assign(ox::empty_type const&,t2 const&)
	{}
	static void assign(ox::empty_type const&,ox::empty_type const&)
	{}
};

template <template<size_t>class tsl,typename uz,typename ms,typename edi>
struct mixture<tsl<0>,uz,ms,edi>
	: methodlmd_apply<typename tsl<0>::_t_syms,tsl<0>,uz,ms,edi>::type
{
	static size_t const index = 0;
	/// mixture's meta
	typedef tsl<index> ifeature;
	typedef uz unitize;
	typedef ms metas;
	typedef typename ifeature::idiff idiff;
	typedef edi ediff;

	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_type _t_type;
	typedef typename ifeature::_t_syms _t_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef void _t_next;

	typedef typename tulist_lmd<_t_syms>::apply<0>::type ssl0;
	typedef mixture self;
	typedef mixture<tsl<tsl<0>::n>,uz,ms,edi> owner_type;
	typedef typename methodlmd_apply<typename tsl<0>::_t_syms,tsl<0>,uz,ms,edi>::type method_type;
	typedef typename methodlmd_apply<ssl0,tsl<0>,uz,ms,edi>::type unit_type;
	typedef typename itot<ifeature::i> size;


	/// null default constructor
	self ()
	{}
	/// copy constructor
	self (mixture const& _other)
		: method_type(_other.unit())
	{}
	template <typename _sl,typename _uz,typename _ms,typename _edi>
	self (tcct<mixture<_sl,_uz,_ms,_edi>> _other)
		: method_type(next0_dunit(_other.value()))
	{}
	self (tcct<ox::empty_type>const&)
	{}
	/// 1 element contructor
	template <typename t0>
	self (t0 const&)
	{}
	/// 2 elements constructor
	template <typename t0,typename t1>
	self (t0 const&,t1 const&)
	{}
	/// 3 elements constructor
	template <typename t0,typename t1,typename t2>
	self (t0 const&,t1 const&,t2 const&)
	{}
	/// 4elements constructor
	template <typename t0,typename t1,typename t2,typename t3>
	self (t0 const&,t1 const&,t2 const&,t3 const&)
	{}
	/// 5 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4>
	self (t0 const&,t1 const&,t2 const&,t3 const&,t4 const&)
	{}
	/// 6 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5>
	self (t0 const&,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&)
	{}
	/// 7 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	self (t0 const&,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&,t6 const&)
	{}
	/// 8 elements constructor
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	self (t0 const&,t1 const&,t2 const&,t3 const&,t4 const&,t5 const&,t6 const&,t7 const&)
	{}

	/// set constructor
	template <typename px,size_t ii>
	self (ox::mix::tparam<px>& p,ox::itot<ii>)
	{}


	// get base unit
	unit_type& unit() { return *static_cast<unit_type*const>(this); }
	unit_type const& unit() const { return *static_cast<unit_type const*const>(this); }
	/// get next base, when not, return int, just as a non-mixture type
	ox::empty_type next() {return ox::empty_type();}
	ox::empty_type const next() const {return ox::empty_type();}
	/// next mixture-data type
	template <bool b> ox::empty_type dnext() {return ox::empty_type();}
	template <bool b> ox::empty_type const dnext() const {return ox::empty_type();}

	///// assign the value transferred to assign unit
	///// at present, just assign to the first unit, later, we will assign a list to this
	template <typename _t>
	self& operator=(_t const& o)
	{
		unit()=o;
		return *this;
	}
	self& operator=(self const& _other)
	{
		return *this;
	}
	template <typename _sl,typename _uz,typename _ms,typename _ed>
	self& operator=(tcct<mixture<_sl,_uz,_ms,_ed>> _other)
	{
		return *this;
	}
};


template <typename sl,typename uz,typename ms,typename edi>
bool operator ==
(
	mixture<sl,uz,ms,edi>const& l,
	mixture<sl,uz,ms,edi>const& r
)
{
	return l.unit()==r.unit() && l.next()==r.next();
}

template <typename sl,typename uz,typename ms,typename edi,typename nonmixture>
bool operator ==
(
	mixture<sl,uz,ms,edi>const& l,
	nonmixture const& r
)
{
	return false;
}

template <typename nonmixture,typename sl,typename uz,typename ms,typename edi>
bool operator ==
(
	nonmixture const& l,
	mixture<sl,uz,ms,edi>const& r
)
{
	return false;
}




/// whether the two mixture is equal at value
template <typename sl,typename uz,typename ms,typename edi>
bool operator !=
(
	mixture<sl,uz,ms,edi>const& l,
	mixture<sl,uz,ms,edi>const& r
)
{
	return l.unit()!=r.unit() || l.next()!=r.next();
}

template <typename sl,typename uz,typename ms,typename edi,typename nonmixture>
bool operator !=
(
	mixture<sl,uz,ms,edi>const& l,
	nonmixture const& r
)
{
	return true;
}

template <typename nonmixture,typename sl,typename uz,typename ms,typename edi>
bool operator !=
(
	nonmixture const& l,
	mixture<sl,uz,ms,edi>const& r
)
{
	return true;
}






/// ----------------------------------------------
//// anther instantiate

/// the type is tslist, get the type and instantiate it
template <template<typename,size_t,size_t,typename>class tl,typename sl,size_t i,size_t nid,typename rl,typename uz,typename ms,typename edi>
struct mixture<tl<sl,i,nid,rl>,uz,ms,edi>
	//: methodlmd_apply<s,tsl<t,s,m,n>,uz,ms,edi>::type
	: mixture<typename feature_next<tl<sl,i,nid,rl>>::type,uz,ms,edi>
{
	/// mixture's meta
	typedef tl<sl,i,nid,rl> ifeature;
	typedef uz unitize;
	typedef ms metas;
	typedef edi ediff;
	typedef typename ifeature::idiff idiff;
	typedef typename feature_next<tl<sl,i,nid,rl>>::type feature_next;

	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_type _t_type;
	typedef typename ifeature::_t_syms _t_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef mixture<feature_next,uz,ms,edi> _t_next;

	//typedef mixture self;
	//typedef mixture<tsl<tsl<0>::n>,uz,ms,edi> owner_type;
};

template <template<typename,size_t,size_t,typename>class tl,typename sl,size_t i,typename rl,typename uz,typename ms,typename edi>
struct mixture<tl<sl,i,0,rl>,uz,ms,edi>
	: method2lmd_apply<tmlist<typename sl::_t_syms>,tl<sl,i,0,rl>,uz,ms,edi>::type
{
	/// mixture's meta
	typedef tl<sl,i,0,rl> ifeature;
	typedef uz unitize;
	typedef ms metas;
	typedef edi ediff;
	typedef typename ifeature::idiff idiff;

	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_type _t_type;
	typedef typename ifeature::_t_syms _t_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef void _t_next;

	typedef mixture self;
	////typedef mixture<tsl<tsl<0>::n>,uz,ms,edi> owner_type;
};




___namespace2_end()