
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../met/_if_switch.h"
#include "../met/lambda.h"
#include "../met/tslist.h"
#include "../cxx/tags.h"
#include "mixture_decl.h"
#include "mixture_method_decl.h"
#include "mixture_inf_decl.h"




#pragma once


___namespace2_begin(ox,mix)



/**
	typename sl, 一个类型列表必须是 ox::met::tslist<>
	typename ms, metas 你可以为mixture绑定元信息
	/// no using of the parameter : typename typeof_unit_tn, 每个元素也有相关管理信息,那么这个定义,如何从这个unit-type中获取实际使用的信息
	typename ulmd, unit_lambda 把一个元素信息,包括管理信息,声称一个可以使用的单元类型
	typename i, index 表示第一个类型的开始编号 should be zero
	typename rll, reversed left list, when we instantiate one element, the type instantiated will be pushed front to rll
		rll, is also a list too
*/


/*
	when we find ulmd is void && i is void, then we regard sl as a special type, a mixture-like type 
	not a slist too
*/

template <typename tsl>
struct tulist_lmd;
template <template<size_t>class tsl,size_t i>
struct tulist_lmd<tsl<i>>
{
	template<size_t ii>
	struct apply
	{
		typedef tsl<ii> type;
	};
	template<size_t ii>
	struct call : tsl<ii>
	{};
};



template <typename tslistw>
struct tslw_to_tsl
{
	typedef typename tslistw::_t_type _t_type;
	typedef typename tslistw::_t_syms _t_syms;
	typedef typename tslistw::_t_meta _t_meta;
	typedef typename tslistw::_t_next _t_next;
	typedef ox::tslist<_t_type,_t_syms,_t_meta,_t_next> type;
};


/*
	the parameter b is useful just at the first time
	if b is false, the first item will be ignored, we will get item until the idiff is not null, 
	get the valid data feature
*/

template <typename sl,bool b=true>
struct dfeature_next;

template <template<size_t>class tsl,size_t i>
struct dfeature_next<tsl<i>,false>
{
	typedef tsl<i> type;
};
template <template<size_t>class tsl,size_t i>
struct dfeature_next<tsl<i>,true>
{
	typedef typename dfeature_next<tsl<i-1>,!tsl<i-1>::idiff::value>::type type;
};
template <template<size_t>class tsl>
struct dfeature_next<tsl<0>,true>
{
	typedef tsl<0> type;
};
template <template<size_t>class tsl>
struct dfeature_next<tsl<0>,false>
{
	typedef tsl<0> type;
};


/// get next data from direct next item
template <typename _sl,typename uz,typename ms,typename edi> struct dnext_helper;
template <template<size_t>class feat,typename uz,typename ms,typename edi>
struct dnext_helper<feat<0>,uz,ms,edi>
{
	typedef ox::empty_type type;
	typedef ox::empty_type const const_type;
	typedef type retype;
	typedef const_type const_retype;
	template <typename t> static retype get(t*const thispointer) { return ox::empty_type(); }
	template <typename t> static const_retype get(t const*const thispointer) { return ox::empty_type(); }
};
template <template<size_t>class feat,size_t i,typename uz,typename ms,typename edi>
struct dnext_helper <feat<i>,uz,ms,edi>
{
	typedef mixture<feat<i>,uz,ms,edi> type;
	typedef type const const_type;
	typedef type& retype;
	typedef const_type& const_retype;
	/// feat<i> = dfeature_next<ifeature,b>::type
	/// t is mixture of ifeature 
	template <typename t> static retype get(t*const thispointer) { return *static_cast<type*const>(thispointer); }
	template <typename t> static const_retype get(t const*const thispointer) { return *static_cast<type const*const>(thispointer); }
};


template <typename t>
struct iof_syms
{
	typedef typename t::_t_syms _t_syms;
	template <typename t>
	struct index
	{
		static size_t const i = 0;
	};
	template <template<size_t>class list,size_t ii>
	struct index <list<ii>>
	{
		static size_t const i = ii;
	};
	static size_t const value = index<_t_syms>::i;
};


template <typename instantiate_tn>
struct instantiate_box;

template
<
	template <typename,size_t,size_t,size_t,typename> class instantiate,
	typename tsl,size_t i,size_t iow,size_t b,typename tinf
>
struct instantiate_box <instantiate<tsl,i,iow,b,tinf>>
{
	template <typename _tsl,size_t _i,size_t _iow,size_t _b,typename _tinf>
	struct apply
	{
		typedef instantiate<_tsl,_i,_iow,_b,_tinf> type;
	};
};


template <typename ssl,typename tsl,typename uz,typename ms,typename edi>
struct methodlmd_apply
{
	typedef typename ssl::unit unit;
	typedef ox::met::lambda<tinf<ox::_1/*ssl*/,tsl,uz,ms,edi>> tinf_lmd;

	typedef
		typename met::iff
		<
			ssl::i==0,
			ox::identity<typename uz::data_unit>,
			met::_switch
			<
				met::_case<ssl::idiff::value==1,typename uz::data_method_unit>,
				met::_case<ssl::idiff::value==2||ssl::idiff::value==3,typename uz::extern_method_unit>,
				met::_case<true,typename uz::method_unit>
			>
		>
		::type::type
	default_unit;

	typedef typename
		met::iff
		<
			met::is_same<unit,void>::value,
			instantiate_box<default_unit>,
			instantiate_box<unit>
		>
		::type
	lambda;
	typedef /*typename*/ ssl/*::_t_syms*/ _t_syms;

	static const size_t sslidi = ssl::idiff::value;
	static const size_t tslidi = tsl::idiff::value;
	typedef tulist_lmd<ssl> ssllmd;
	typedef tulist_lmd<tsl> tsllmd;
	static size_t const i = ssllmd::call<0>::n-ssl::i; /// from zero
	static size_t const iow = tsllmd::call<0>::n-tsl::i; /// from zero
	static size_t const idi = (sslidi==2||sslidi==3)? sslidi:tslidi; /// idiff

	typedef typename lambda::apply<_t_syms,i,iow,idi,tinf_lmd>::type type;
};


template <typename feature>
struct feature_next;

/// when feature is data list
template <template<typename,size_t,size_t,typename>class tl,typename s,size_t i,size_t in,typename r/*,typename lmd*/>
struct feature_next <tl<s,i,in,r>>
{
public:
	template <typename sl>
	struct is_valid
	{
		static size_t const value = ox::met::is_void<typename sl::_t_next>::value ? 0: 1;
	};
	typedef tl<s,i,in,r> feature;
	typedef typename feature::slist slist;
	typedef typename feature::rlist rlist;

	typedef typename slist::_t_type _t_type;
	typedef typename slist::_t_syms _t_syms;
	typedef typename slist::_t_meta _t_meta;

	typedef typename slist::_t_next slist_next;
	typedef typename ox::met::push_front<rlist,_t_type,_t_syms,void>::type rlist_next;
	static size_t const i = feature::i+1;
	static size_t const nextid_1 = ox::met::is_void<slist_next>::value?0:1;
	static size_t const nextid = ox::met::iff<
		nextid_1,is_valid<slist_next>,ox::itot<0>>::type::value;

public:
	typedef tl<slist_next,i,nextid,rlist_next> type;
};

/// when list is method list
template <template<typename,size_t>class ml,typename sl,size_t i>
struct feature_next <ml<sl,i>>
{
	typedef typename ml<sl,i>::_t_next _t_next;
	typedef ml<_t_next,i+1> type;
};


/// get feature0 from any-data-list
template <typename feature>
struct feature0;
template <typename s,size_t i,size_t in,typename r>
struct feature0 <tlist<s,i,in,r>>
{
private:
	typedef tlist<s,i,in,r> feature;
	typedef typename feature::slist slist;
	typedef typename feature::rlist rlist;
	typedef typename ox::met::push_slist_front_backward<slist,rlist>::type slist0;
public:
	typedef tlist<slist0> type;
};



template <typename ssl,typename tsl,typename uz,typename ms,typename edi>
struct method2lmd_apply
{
	typedef tinf<ox::_1/*ssl*/,tsl,uz,ms,edi> tinf;
	typedef typename ssl::unit unit;

	typedef
		typename met::iff
		<
			met::is_void<typename ssl::_t_next>::value,
			ox::identity<typename uz::data_unit>,
			met::_switch
			<
				met::_case<ssl::idiff::value==1,typename uz::data_method_unit>,
				met::_case<ssl::idiff::value==2||ssl::idiff::value==3,typename uz::extern_method_unit>,
				met::_case<true,typename uz::method_unit>
			>
		>
		::type::type
	default_unit;

	typedef typename
		met::iff
		<
			met::is_same<unit,void>::value,
			instantiate_box<default_unit>,
			instantiate_box<unit>
		>
		::type
	lambda;
	typedef /*typename*/ ssl/*::_t_syms*/ _t_syms;

	static const size_t sslidi = ssl::idiff::value;
	static const size_t tslidi = tsl::idiff::value;
	static size_t const i = ssl::i; /// from zero
	static size_t const iow = tsl::i; /// from zero
	static size_t const idi = (sslidi==2||sslidi==3)? sslidi:tslidi; /// idiff

	typedef typename lambda::apply<_t_syms,i,iow,idi,tinf>::type type;
};


///// next mixture
///// if current's idiff is not 0, the current is hit
///// else next as current until current's idiff is not 0
//template <typename t>
//struct next0_dmixture_type
//{
//	typedef void type;
//	typedef ox::empty_type type0;
//};
///// if current is not valid, find next
//template <typename mixture,size_t i>
//struct next0_dmixture_type_if0
//{
//	struct void_next { typedef void _t_next; };
//	typedef typename
//		met::iff
//		<
//			i==1 || i==0,
//			void_next,
//			mixture
//		>
//		::type::_t_next
//	mixture_next;
//
//	typedef typename next0_dmixture_type<mixture_next>::type type;
//};
//template <typename sl,typename uz,typename m,typename ed>
//struct next0_dmixture_type<mixture<sl,uz,m,ed>>
//{
//	typedef typename
//		met::iff
//		<
//			sl::idiff::value==1,
//			ox::identity<mixture<sl,uz,m,ed>>,
//			next0_dmixture_type_if0<mixture<sl,uz,m,ed>,sl::i>
//		>
//		::type::type
//	type;
//
//	typedef typename
//		met::iff
//		<
//			met::is_void<type>::value,
//			ox::identity<ox::empty_type>,
//			met::add<int const&>::to<type>
//		>
//		::type::type
//	type0;
//};
//
//
//template <typename rt>
//struct next_from_mixture
//{
//	template <typename t>
//	rt const& fromptr(t const* o)
//	{
//		return *static_cast<rt const*>(o);
//	}
//	template <typename t>
//	rt const& fromref(t const& o)
//	{
//		return *static_cast<rt const*>(&o);
//	}
//};
//template <typename rt>
//struct next_empty_from_mixture
//{
//	template <typename t>
//	rt fromptr(t const* o)
//	{
//		return rt();
//	}
//	template <typename t>
//	rt fromref(t const& o)
//	{
//		return rt();
//	}
//};
//
///// next mixture
//template <typename sl,typename uz,typename ms,typename e>
//typename next0_dmixture_type<mixture<sl,uz,ms,e>>::type0
//next0_dmixture(mixture<sl,uz,ms,e> const* o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next0_dmixture_type<t>::type mixturex;
//	typedef typename
//		met::iff
//		<
//			met::is_void<mixturex>::value,
//			next_empty_from_mixture<ox::empty_type>,
//			next_from_mixture<mixturex>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromptr(o);
//}
//template <typename sl,typename uz,typename ms,typename e>
//typename next0_dmixture_type<mixture<sl,uz,ms,e>>::type0
//next0_dmixture(mixture<sl,uz,ms,e> const& o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next0_dmixture_type<t>::type mixturex;
//	typedef typename
//		met::iff
//		<
//			met::is_void<mixturex>::value,
//			next_empty_from_mixture<ox::empty_type>,
//			next_from_mixture<mixturex>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromref(o);
//}
//
//
//template <typename t>
//struct next0_dunit_type
//{
//	struct empty_unit
//	{
//		typedef void unit_type;
//	};
//	typedef typename next0_dmixture_type<t>::type dmixture;
//	typedef typename
//		met::iff
//		<
//			met::is_void<dmixture>::value,
//			empty_unit,
//			dmixture
//		>
//		::type::unit_type
//	type;
//	typedef typename
//		met::iff
//		<
//			met::is_void<dmixture>::value,
//			ox::identity<ox::empty_type>,
//			met::add<int const&>::to<type>
//		>
//		::type::type
//	type0;
//};
//
//
//template <typename rt>
//struct next_unit_from_mixture
//{
//	template <typename t>
//	rt const& fromptr(t const* o)
//	{
//		return *static_cast<rt const*>(o);
//	}
//	template <typename t>
//	rt const& fromref(t const& o)
//	{
//		return *static_cast<rt const*>(&o);
//	}
//};
//template <typename rt>
//struct next_unit_empty_from_mixture
//{
//	template <typename t>
//	rt fromptr(t const* o)
//	{
//		return rt();
//	}
//	template <typename t>
//	rt fromref(t const& o)
//	{
//		return rt();
//	}
//};
//
//template <typename sl,typename uz,typename ms,typename e>
//typename next0_dunit_type<mixture<sl,uz,ms,e>>::type0
//next0_dunit(mixture<sl,uz,ms,e> const* o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next0_dunit_type<t>::type unitx;
//	typedef typename
//		met::iff
//		<
//			met::is_void<unitx>::value,
//			next_unit_empty_from_mixture<ox::empty_type>,
//			next_unit_from_mixture<unitx>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromptr(o);
//}
//
//template <typename sl,typename uz,typename ms,typename e>
//typename next0_dunit_type<mixture<sl,uz,ms,e>>::type0
//next0_dunit(mixture<sl,uz,ms,e> const& o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next0_dunit_type<t>::type unitx;
//	typedef typename
//		met::iff
//		<
//			met::is_void<unitx>::value,
//			next_unit_empty_from_mixture<ox::empty_type>,
//			next_unit_from_mixture<unitx>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromref(o);
//}
//
///// the direct next of the first valid mixture
//template <typename t>
//struct next_dmixture_type
//{
//	struct void_next { typedef void _t_next; };
//	typedef typename next0_dmixture_type<t>::type d0mixture;
//	/// get next
//	typedef typename
//		met::iff
//		<
//			met::is_void<d0mixture>::value,
//			void_next,
//			d0mixture
//		>
//		::type::_t_next
//	type;
//	/// get type0
//	typedef typename
//		met::iff
//		<
//			met::is_void<type>::value,
//			ox::identity<ox::empty_type>,
//			met::add<int const&>::to<type>
//		>
//		::type::type
//	type0;
//};
//
//template <typename sl,typename uz,typename ms,typename e>
//typename next_dmixture_type<mixture<sl,uz,ms,e>>::type0
//next_dmixture(mixture<sl,uz,ms,e> const* o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next_dmixture_type<t>::type mixturex;
//	typedef typename
//		met::iff
//		<
//			met::is_void<mixturex>::value,
//			next_empty_from_mixture<ox::empty_type>,
//			next_from_mixture<mixturex>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromptr(o);
//}
//template <typename sl,typename uz,typename ms,typename e>
//typename next_dmixture_type<mixture<sl,uz,ms,e>>::type0
//next_dmixture(mixture<sl,uz,ms,e> const& o)
//{
//	typedef mixture<sl,uz,ms,e> t;
//	typedef typename next_dmixture_type<t>::type mixturex;
//	typedef typename
//		met::iff
//		<
//			met::is_void<mixturex>::value,
//			next_empty_from_mixture<ox::empty_type>,
//			next_from_mixture<mixturex>
//		>
//		::type
//	call; /// if mixturex is empty_type (when all items is void), if mixturex is a mixture
//	return call().fromref(o);
//}





___namespace2_end()