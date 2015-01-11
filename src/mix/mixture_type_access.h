
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "../met/lambda0.h"
#include "../met/tslist.h"
#include "../met/tslist_syms.h"
#include "../met/mod.h"
#include "../met/type_for_param_passed.h"
#include "../cxx/tags.h"
#include "./mixture_decl.h"


#pragma once


___namespace2_begin(ox,mix)



template <typename slike_tn>
struct match_data_member
{
	typedef typename slike_tn::_t_type _t_type;
	static bool const value = !met::is_same<_t_type,void>::value;
};
template <typename slike_tn>
struct match_method_member
{
	static bool const value = true;
};

template <typename slike_tn>
struct match_any
{
	static bool const value = true;
};

template <typename precate_lmd>
struct class_to_precate
{
	typedef precate_lmd type;
};
template <>
struct class_to_precate <ox::tags::data>
{
	typedef met::lambda1<match_data_member>::type type;
};
template <>
struct class_to_precate <ox::tags::method>
{
	typedef met::lambda1<match_method_member>::type type;
};
template <>
struct class_to_precate <ox::tags::any>
{
	typedef met::lambda1<match_any>::type type;
};


/// to kinds
/// struct declare
template <size_t cate> struct char_to_certain;

template <> struct char_to_certain <'m'>
{
	template <typename type_tn>
	struct unitof
	{
		typedef typename type_tn::owner owner_type;
		typedef typename owner_type::unit_type type;
	};

	typedef met::lambda<unitof<ox::_1>> lambda;
	typedef int mod;
	typedef ox::tags::any precate;
};
template <> struct char_to_certain <'u'>
{
	template <typename type_tn>
	struct unitof
	{
		typedef typename type_tn::owner owner_type;
		typedef typename owner_type::unit_type type;
	};

	typedef met::lambda<unitof<ox::_1>> lambda;
	typedef int mod;
	typedef ox::tags::any precate;
};


template
<
	typename slistbox_tn,
	typename addr,
	typename presel_lmd,
	typename nextslist_tn=met::next_of_slist<_1>
>
struct at_of_range
	: met::at_of_range<slistbox_tn,addr::i,presel_lmd,nextslist_tn>
{};


template
<
	typename slistbox_tn,
	typename addr,
	typename presel_lmd,
	typename nextslist_tn=met::next_of_slist<_1>
>
struct at_of_method
	: met::at_of_range<typename slistbox_tn::method_type,addr::j,presel_lmd,nextslist_tn>
{};

template
<
	typename slike,
	typename addr,
	typename presel_lmd = met::__pri_byid::always_selected_lambda,
	typename ishit_lmd = met::__pri_byid::is_matched_lambda,
	typename next = met::next_of_slist<_1>
>
struct sat_of_method
	: met::sat_of_range<typename slike::method_type,ox::met::saddr<typename addr::i>,presel_lmd,ishit_lmd,next>
{};

template
<
	typename slistbox_tn,
	typename addr_tn,
	typename presel_lmd,
	typename ishit_lambda = typename met::iff<addr_tn::j==-1,met::__pri_byid::is_matched_data_tulist_lambda,met::__pri_byid::is_matched_tulist_lambda>::type,
	typename nextslist_tn = met::next_of_slist<_1>
>
struct sat_of_mixture_range
	: met::sat_of_mixture_range <slistbox_tn,ox::met::saddr<typename addr_tn::i>,presel_lmd,ishit_lambda,nextslist_tn>
{};


template <typename addr_tn>
struct baseof
{
	static size_t const value = -1;
};
template <size_t i,size_t j>
struct baseof <ox::mix::index<i,j>>
{
	static size_t const value = i;
};


/// because of the data part has two properties, data & data method, which has encapsulated by unit,
/// when we get unit from data part, which unit will we get, okay, we will get data unit firstly
/// if you wanna get method unit, please use method_typeof getter

/*
	if tag is D
		return result1
	if tag is M
		if next is -1
			return false;
	if tag is A
		if next is -1
			return result1

	/// second step
	return result2


	if (tag is D or A and next==-1) return result1
	else if (tag is M and next ==-1) return false;
*/

template
<
	typename slist_like_tn,
	typename addr_tn, 
	typename memberof_lambda, 
	typename mod_tn=int,
	typename precate = class_to_precate<ox::tags::any>::type/*,
	typename is_hit = met::__pri_byid::is_matched_tulist_lambda*/
>
struct member_typeat
{
	typedef typename met::rmv_con<slist_like_tn>::type slist_range_type;
	typedef precate precate_lmd; /// always true, 
	/// becuase the tag which has been decided at the position at the mixture

	typedef typename
		met::iff
		<
			ox::mix::is_index_type<addr_tn>::value,
			ox::mix::at_of_range<slist_range_type,addr_tn,precate_lmd>,
			ox::mix::sat_of_mixture_range<slist_range_type,addr_tn,precate_lmd>
		>
		::type
	result_type;

	typedef typename result_type::owner first_owner;

	static bool const __is_result_okay = !met::is_same<first_owner,void>::value;
	static bool const __is_second = addr_tn::j!=-1;


	/// 规避, 当是method的时候,又没有指定第二个参数时,返回false
	struct void_result { typedef void owner; };

	typedef typename
		met::iff
		<
			__is_result_okay && __is_second,
			met::iff
			<
				ox::mix::is_index_type<addr_tn>::value,
				ox::mix::at_of_method<first_owner,addr_tn,precate_lmd>, /// we can set the fist as default
				ox::mix::sat_of_method<first_owner,addr_tn,precate_lmd>
			>,
			ox::identity<result_type>
		>
		::type::type
	final_type;

	typedef typename memberof_lambda::call<final_type>::type native_type;


	/// get plain_type
	typedef typename
		met::add_con<slist_like_tn>::to<native_type>::type
	plain_type;

	/// return
	typedef typename
		met::add<mod_tn>::to<plain_type>::type
	type;
};



template
<
	typename slist_like_tn,
	typename addr_tn,
	typename mod_tn=int
>
struct owner_typeat
{
	template <typename type_tn>
	struct ownerof
	{
		typedef typename type_tn::owner type;
	};
	typedef met::lambda<ownerof<ox::_1>> ownerof_lambda;
	typedef typename met::iff
		<
			addr_tn::j==-1,
			class_to_precate<ox::tags::data>,
			class_to_precate<ox::tags::any>
		>
		::type::type
	precate;

	typedef member_typeat<slist_like_tn,addr_tn,ownerof_lambda,mod_tn,precate> get;
	typedef typename get::type type;
};

/// get uint_type of mixture
template
<
	typename slist_like_tn,
	typename addr_tn,
	typename mod_tn=int
>
struct unit_typeat
{
	template <typename type_tn>
	struct unitof
	{
		typedef typename type_tn::owner owner_type;
		typedef typename owner_type::unit_type type;
	};
	typedef met::lambda<unitof<ox::_1>> unitof_lambda;
	typedef typename met::iff
		<
			addr_tn::j==-1,
			class_to_precate<ox::tags::data>,
			class_to_precate<ox::tags::any>
		>
		::type::type
	precate;

	typedef typename
		member_typeat<slist_like_tn,addr_tn,unitof_lambda,mod_tn,precate>::type
	type;
};


template
<
	typename slist_like_tn,
	typename addr_tn,
	typename mod_tn=int
>
struct data_typeat
{
	template <typename type_tn>
	struct getof
	{
		typedef typename type_tn::owner owner_type;
		typedef typename owner_type::unit_type::value_type type;
	};
	typedef met::lambda<getof<ox::_1>> getof_lambda;
	typedef typename class_to_precate<ox::tags::data>::type precate;

	typedef member_typeat<slist_like_tn,addr_tn,getof_lambda,mod_tn,precate> get;
	typedef typename get::type type;
};


template
<
	typename unit_tn,
	typename addr_tn,
	typename mod=int
>
struct unit_typefrom_unit
{
	typedef typename unit_tn::type derivedest;
	typedef typename unit_typeat<derivedest,addr_tn,mod>::type unit_type;
	typedef unit_type type;
};


///// from another view

/// return the unit, because there is just has unit and owner, owner for us
/// to use rarely
template
<
	typename slist_like_tn,
	typename addr_tn,
	typename mod_tn=int
>
struct method_typeat
{
	template <typename type_tn>
	struct unitof
	{
		typedef typename type_tn::owner owner_type;
		typedef owner_type type;
		//typedef typename owner_type::unit_type type;
	};
	typedef met::lambda<unitof<ox::_1>> unitof_lambda;

	typedef typename addr_tn::change<addr_tn::j==-1?0:addr_tn::j>::type addr;

	typedef member_typeat<slist_like_tn,addr,unitof_lambda,mod_tn> get;
	typedef typename get::type type;
};


///
/// cate : o,m,d,u
/// has 16 types
/// --------------------
/// o: owner
/// m: method
/// d: data = element
/// u, uint
/// o&: owner&
/// m&: method&
/// d&: data& = element&
/// u&: unit&
/// o*: owner*
/// m*: method*
/// d*: data* = element*
/// u*: unit*
/// oc: owner const
/// mc: method const
/// dc: data const = element const
/// uc, uint const
/// oc&: owner const&
/// mc&: method const&
/// dc&: data const& = element const&
/// uc&: unit const&
/// oc*: owner const*
/// mc*: method const*
/// dc*: data const* = element const*
/// uc*: unit const*
template <typename slike,typename addr,char cate='u'>
struct typeat
{
	typedef char_to_certain<cate> info;
	typedef typename info::lambda lambda;
	typedef typename info::mod mod;
	typedef typename info::precate precate;

	typedef typename
		member_typeat<slike,addr,lambda,mod>
		::type
	type;
};




___namespace2_end()