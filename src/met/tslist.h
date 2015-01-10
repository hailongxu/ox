
/**
	作者：徐海龙
	2010-11-10
	创建一个更加简单的概念，创建一个线性异质容器
*/

#include "../ox/nsab.h"
#include "../ox/nothing.h"
#include "../ox/tclasses.h"
#include "./mod.h"
#include "./_if_switch.h"
#include "./static_assert.h"
#include "./lambda.h"
#include "./lambda0.h"
#include "./infof.h"
#include "./tslist_decl.h"

#include "../cxx/tags.h"


#pragma once


___namespace2_begin(ox,met)


/// also provide a fun which can convert current output to next input
/// so, do later
template <typename act_tn, size_t count_tc>
struct do_repeatly
{
	typedef typename lambdate<act_tn>::type act;
	//typedef act
};

/*
	whether a type is tslist type
 */
template <typename type_tn>
struct is_slist
{
	static bool const value = false;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct is_slist <tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	static bool const value = true;
};

template <typename t>
struct is_tulist_like
{
	static bool const value = false;
};
template <template <size_t>class tsl,size_t i>
struct is_tulist_like <tsl<i>>
{
	static bool const value = true;
};

template <typename t>
struct is_tulmethod_like
{
	static bool const value = false;
};
template
<
	template <typename,typename,size_t,size_t,typename>class tuplemethod,
	typename t,typename uz,size_t i,size_t iow,typename ow
>
struct is_tulmethod_like <tuplemethod<t,uz,i,iow,ow>>
{
	static bool const value = true;
};


template <typename type_tn>
struct plurality
{
	static size_t const value = 0;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct plurality <tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	static size_t const value = 1;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename type2_tn,typename syms2_tn,typename meta2_tn,typename next_tn>
struct plurality <tslist<type_tn,syms_tn,meta_tn,tslist<type2_tn,syms2_tn,meta2_tn,next_tn>>>
{
	static size_t const value = 2;
};

template <typename type_tn,typename syms_tn=void>
struct slistify
{
	typedef typename
		iff
		<
			is_slist<type_tn>::value,
			type_tn,
			tslist<type_tn,syms_tn,void,void>
		>
		::type
	type;
};


template <typename slike_tn>
struct always_select
{
	static bool const value = true;
	typedef boolean<value> boolean;
};

/// reverse the slist
template <typename void_tn,typename prev_tn=void>
struct reverse
{
	typedef prev_tn type;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename prev_tn>
struct reverse <tslist<type_tn,syms_tn,meta_tn,next_tn>,prev_tn>
{
	typedef tslist<type_tn,syms_tn,meta_tn,prev_tn> next_type;
	typedef typename reverse<next_tn,next_type>::type type;
};

/*
	push_front(<int,void>, newtype) => <newtype, <int,void>>
	e.g.:
	slist = <a,b,c>
	new_type = d
	result = <d,a,b,c>
 */
template <typename slist_like_tn,typename newtype_tn,typename syms_tn=void,typename meta_tn=void,typename model_tn=tslist<_1,_2,_3,_4>>
struct push_front
{
private:
	typedef typename lambdate<slist_like_tn>::type lambda_slist;
	typedef typename lambdate<model_tn>::type lambda_model;

	typedef typename
		iff
		<
			lambda_slist::params_count>=4,
			lambda_slist,lambda_model
		>
		::type
	lambda;

public:
	typedef typename lambda::reapply<newtype_tn,syms_tn,meta_tn,slist_like_tn>::type type;
};

/// (<a,b,c>, N,4) ==> <N,N,N,N, a,b,c>
template <typename slist_like_tn,typename newtype_tn,typename syms_tn,typename meta_tn,size_t count_tc,typename model_tn=tslist<_1,_2,_3,_4>>
struct rpush_front
{
	typedef typename push_front<slist_like_tn,newtype_tn,syms_tn,meta_tn,model_tn>::type slist_type;
	typedef typename rpush_front<slist_type,newtype_tn,syms_tn,meta_tn,count_tc-1,model_tn>::type type;
};
template <typename slist_like_tn, typename newtype_tn,typename syms_tn,typename meta_tn,typename model_tn>
struct rpush_front <slist_like_tn,newtype_tn,syms_tn,meta_tn,0,model_tn>
{
	typedef slist_like_tn type;
};



/*
	
	Complexity = O(sizeof(tslist))

	e.g.:
	type = <a,b,c>
	tslist = <d,e,f>
	result = <f,e,d, a,b,c>

	/// index is the current index of tslist_tn being added
	template <typename type_tn,typename index>
	struct test
	{
		static bool const value = true; /// okay push
		/// static bool const value = false; /// donot push
	};
 */
/// when type tslist is void, return the inputed type
template <typename type_tn, typename tslist_tn, typename test_tn=void,typename index=itot<0>>
struct push_slist_front_backward
{
	typedef type_tn type;
};
template <typename type_tn, typename newtype_tn,typename syms_tn,typename meta_tn,typename newnext_tn, typename test_tn,typename index>
struct push_slist_front_backward <type_tn, tslist<newtype_tn,syms_tn,meta_tn,newnext_tn>,test_tn,index>
{
private:
	/// the default test, the result is always true
	template <typename slist_tn,typename _index>
	struct always_true { static bool const value = true; };

	typedef typename
		met::iff<met::is_void<test_tn>::value,always_true<_1,_2>,test_tn>::type
	test_type;
	typedef typename lambdate<test_type>::type test;

	typedef typename
		met::iff
		<
			test::recall<tslist<newtype_tn,syms_tn,meta_tn,newnext_tn>,index>::value,
			push_front<type_tn,newtype_tn,syms_tn,meta_tn>,
			identity<type_tn>
		>
		::type::type
	curr_type;

public:
	typedef typename
		push_slist_front_backward<curr_type,newnext_tn,test,typename index::inc<1>::type>::type
	type;
};



/*
template <typename slist_tn, typename index>
struct test
{
	//typedef typename slist_tn::_t_type _t_type;
	//typedef typename slist_tn::_t_syms _t_syms;
	//typedef typename slist_tn::_t_meta _t_meta;

	/// necessary
	static bool const value = true/false;
};
*/
/// support when tslist is not slist will be pushed to the front
template <typename type_tn,typename tslist_tn,typename test_tn=void>
struct push_front_backward
{
	/// the default test, the result is always true
	template <typename slist_tn,typename index>
	struct always_true { static bool const value = true; };

	typedef typename
		met::iff<met::is_void<test_tn>::value,always_true<_1,_2>,test_tn>::type
	test_type;
	typedef typename lambdate<test_type>::type test;

	template <typename _type_tn,typename _newtype_tn,typename _test_tn>
	struct push_type_front_backward
	{
		typedef typename
			iff
			<
				/// 为了让test调用统一化,所以把单个类型转成tslist
				_test_tn::recall< tslist<_newtype_tn,void,void,void>,itot<0> >::value,
				tslist<_newtype_tn,void,void,_type_tn>, type_tn
			>
			::type
		type;
	};

	typedef typename
		iff
		<
			met::is_slist<tslist_tn>::value,
			push_slist_front_backward<type_tn,tslist_tn,test>,
			push_type_front_backward<type_tn,tslist_tn,test>
		>
		::type::type
	type;
};


/// push back a type to tslist
/*
 	e.g.1, append slist:
	type = <a,b,c>
	tslist = <d,e,f>
	result = <a,b,c, d,e,f>

 	e.g.2, append one type:
	type = <a,b,c>
	tslist = d
	result = <a,b,c, d>

	if slist_tn is not slist type, we just regard it as an tail void
*/
template <typename slist_tn,typename newtype_tn,typename newsyms_tn=void,typename newmeta_tn=void>
struct push_back
{
	___static_assert(ox::met::is_void<slist_tn>::value);
	typedef tslist<newtype_tn,newsyms_tn,newmeta_tn,void> type;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename newtype_tn,typename newsyms_tn,typename newmeta_tn>
struct push_back <tslist<type_tn,syms_tn,meta_tn,next_tn>,newtype_tn,newsyms_tn,newmeta_tn>
{
	typedef
		tslist
		<
			type_tn,syms_tn,meta_tn,
			typename push_back<next_tn,newtype_tn,newsyms_tn,newmeta_tn>::type
		>
	type;
};
/// when added type is slist
template <typename type_tn, typename newtype_tn, typename newsyms_tn, typename newmeta_tn,typename newnext_tn>
struct push_back <type_tn,tslist<newtype_tn,newsyms_tn,newmeta_tn,newnext_tn>>
{
	typedef tslist<newtype_tn,newsyms_tn,newmeta_tn,newnext_tn> type;
};
template <typename type_tn, typename syms_tn,typename meta_tn,typename next_tn, typename newtype_tn, typename newsyms_tn, typename newmeta_tn,typename newnext_tn>
struct push_back <tslist<type_tn,syms_tn,meta_tn,next_tn>,tslist<newtype_tn,newsyms_tn,newmeta_tn,newnext_tn>>
{
private:
	typedef tslist<type_tn,syms_tn,meta_tn,next_tn> curslist;
	typedef tslist<newtype_tn,newsyms_tn,newmeta_tn,newnext_tn> newslist;
	typedef typename reverse<curslist>::type slist_reversed;
	typedef typename 
		push_front_backward <slist_reversed,newslist>::type
	slist_pushed_reversed;
public:
	typedef typename reverse<slist_pushed_reversed>::type type;
};


/// get size of tslist
/*
	template <tyename type_tn>
	struct test
	{
		static bool const value = true;
		/// true, the size will be increased
		/// false, donot
	};
 */
template <typename slist_tn, typename test_tn=void>
struct size
{
	___static_assert(met::is_nothing<slist_tn>::value);
	static size_t const value = 0;
	typedef itot<0> total;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename test_tn>
struct size <tslist<type_tn,syms_tn,meta_tn,next_tn>,test_tn>
{
	template <typename _test_tn>
	struct added_value
	{
		typedef typename lambdate<test_tn>::type lambda_test;
		static size_t const value = lambda_test::recall<type_tn,syms_tn,meta_tn>::value?1:0;
	};
	template <> struct added_value <void> { static size_t const value = 1; };
	template <> struct added_value <ox::null_type> { static size_t const value = 1; };
	static size_t const __added = added_value<test_tn>::value;
	static size_t const value = __added+size<next_tn>::value;
	typedef itot<1+size<next_tn>::total::value> total;
};


template <typename sl,typename ty,int i=0>
struct indexof
{
	static int const value = -1;
};

template <typename t,typename s,typename m,typename n,typename ty,int i>
struct indexof <tslist<t,s,m,n>,ty,i>
{
	static int const value = -i-1;
};
template <typename t,typename s,typename m,typename n,int i>
struct indexof <tslist<t,s,m,n>,t,i>
{
	static int const value = i;
};
template
<
	typename t,typename s,typename m,
	typename nt,typename ns,typename nm,typename nn,
	typename ty,int i
>
struct indexof <tslist<t,s,m,tslist<nt,ns,nm,nn>>,ty,i>
{
	static int const value = indexof<tslist<nt,ns,nm,nn>,ty,i+1>::value;
};
template
<
	typename t,typename s,typename m,
	typename nt,typename ns,typename nm,typename nn,
	int i
>
struct indexof <tslist<t,s,m,tslist<nt,ns,nm,nn>>,t,i>
{
	static int const value = i;
};

template <typename sl,typename ty,int i=0>
struct indexof_range
{
	typedef typename sl::_t_type  _t_type;
	typedef typename sl::_t_next _t_next;
	static int const value =
		iff
		<
			is_same<_t_type,type_tn>::value,
			ox::itot<i>,
			indexof_range<_t_next,type_tn,i+1>
		>
		::type::value;
};
template <typename ty,int i>
struct indexof_range <void,ty,i>
{
	static int const value = -1;
};


template <typename sl>
struct presel_tulist_always
{
	static bool const value = true;
};

template <typename slike,typename type_tn,typename fpresel=lambda<presel_tulist_always<ox::_1>>,int i=0>
struct indexof_tulist;
template <template <size_t>class slike,size_t ii,typename type_tn,typename fpresel,int i>
struct indexof_tulist<slike<ii>,type_tn,fpresel,i>
{
	static bool const selected = fpresel::call<slike<ii>>::value;

	typedef typename slike<ii>::_t_type _t_type;
	static int const value =
		iff
		<
			selected && is_same<_t_type,type_tn>::value,
			ox::itot<i>,
			indexof_tulist<slike<ii-1>,type_tn,fpresel,selected?i+1:i>
		>
		::type::value;
};
template <template <size_t>class slike,typename type_tn,typename fpresel,int i>
struct indexof_tulist<slike<1>,type_tn,fpresel,i>
{
	static bool const selected = fpresel::call<slike<1>>::value;

	typedef typename slike<1>::_t_type _t_type;
	static int const value = (selected && is_same<_t_type,type_tn>::value) ?i:-1;
};
template <template <size_t>class slike,typename type_tn,typename fpresel,int i>
struct indexof_tulist<slike<0>,type_tn,fpresel,i>
{
	typedef typename slike<0>::_t_type _t_type;
	static int const value = -1;
};

/// index of feature
template <typename feature,typename fishit,typename fpresel=lambda<presel_tulist_always<ox::_1>>,int i=0>
struct indexof_feature_if;
template <template <size_t>class feature,size_t ii,typename fishit,typename fpresel,int i>
struct indexof_feature_if<feature<ii>,fishit,fpresel,i>
{
	static bool const selected = fpresel::call<feature<ii>>::value;

	static int const value =
		iff
		<
			selected && fishit::call<feature<ii>>::value,
			ox::itot<i>,
			indexof_feature_if<feature<ii-1>,fishit,fpresel,selected?i+1:i>
		>
		::type::value;
};
template <template <size_t>class feature,typename fishit,typename fpresel,int i>
struct indexof_feature_if<feature<1>,fishit,fpresel,i>
{
	static bool const selected = fpresel::call<feature<1>>::value;

	static int const value = (selected && fishit::call<feature<1>>::value) ?i:-1;
};
template <template <size_t>class feature,typename fishit,typename fpresel,int i>
struct indexof_feature_if<feature<0>,fishit,fpresel,i>
{
	typedef typename feature<0>::_t_type _t_type;
	static int const value = -1;
};

template <typename feature,typename t,typename fpresel=lambda<presel_tulist_always<ox::_1>>>
struct indexof_feature
{
private:
	template <typename feat>
	struct hit
	{
		static bool const value = is_same<typename feat::_t_type,t>::value;
	};
public:
	static int const value = indexof_feature_if<feature,lambda<hit<ox::_1>>,fpresel>::value;
};


/**
	>=0 : the real offset of range
	< 0 : not found
		-1		: the slist is not a list type
		-(N+1)	: - ( |slist| + 1 )

	 -1 的计算和 -(N+1) 是同计算的
	 -1 ==> 1 ==> 1-1 ==> 0 = |slist| ==> is_null
	 -(N+1) ==> N+1 ==> N+1-1 ==> N ==> |slist| ==> not find, the null node
*/


/*
	>=0 : find one
	< 0 : find nothing, -R = the size of the slist
*/
template <typename slist_tn,typename type_tn>
struct xindexof
{
	struct hit_null
	{
		static int const value = -1;
	};
	static int const value =
		iff
		<
			is_slist<slist_tn>::value,
			indexof<slist_tn,type_tn>,
			hit_null
		>
		::type::value;
};

/// not implement, how to recognise slist is not a slike, such as no parameters
template <typename slike,typename ty>
struct xindexof_range
{
	___static_assert(false);
};

template <typename slike,typename type_tn,typename fpresel=lambda<presel_tulist_always<ox::_1>>>
struct xindexof_tulist
{
	static int const value = is_same<slike,type_tn>::value?0:-1;
};
template <template <size_t>class slike,size_t ii,typename type_tn,typename fpresel>
struct xindexof_tulist<slike<ii>,type_tn,fpresel>
{
	static int const value = indexof_tulist<slike<ii>,type_tn,fpresel>::value;
};


//template <typename feature,typename fishit,typename fpresel=lambda<presel_tulist_always<ox::_1>>>
//struct xindexof_feature
//{
//	static int const value = is_same<slike,type_tn>::value?0:-1;
//};
//template <template <size_t>class feature,size_t ii,typename fishit,typename fpresel>
//struct xindexof_feature<feature<ii>,type_tn,fpresel>
//{
//	static int const value = indexof_feature<slike<ii>,fishit,fpresel>::value;
//};





/**
	if the type is slist, 
	then we hit when any of elment of latter list type is included in former list
*/

/**
	>=0 : the real offset of range
	< 0 : not found
		-1		: the slist is not a list type
		-(N+1)	: - ( |slist| + 1 )

	 -1 的计算和 -(N+1) 是同计算的
	 -1 ==> 1 ==> 1-1 ==> 0 = |slist| ==> is_null
	 -(N+1) ==> N+1 ==> N+1-1 ==> N ==> |slist| ==> not find, the null node
*/
template <typename sl,typename tys,int i=0>
struct anyindexof
{
	static int const value = -1;
};
template <typename t,typename s,typename m,typename n,typename tys,int i>
struct anyindexof <tslist<t,s,m,n>,tys,i>
{
	static int const hit = indexof<tys,t>::value;
	static int const value = hit>=0 ? i : -i-1;
};
template <typename t,typename s,typename m,typename nt,typename ns,typename nm,typename nn,typename tys,int i>
struct anyindexof <tslist<t,s,m,tslist<nt,ns,nm,nn>>,tys,i>
{
	static int const hit = indexof<tys,t>::value;
	static int const value =
		iff<(hit>=0),itot<i>,anyindexof<tslist<nt,ns,nm,nn>,tys,i+1>>::type::value;
};


//// anyindexof tulist
template <typename sl,typename tys,typename fpresel=lambda<presel_tulist_always<ox::_1>>,int i=0>
struct anyindexof_tulist
{
	static int const value = -1;
};
template <template<size_t>class tsl,size_t ii,typename tys,typename fpresel,int i>
struct anyindexof_tulist <tsl<ii>,tys,fpresel,i>
{
	static bool const selected = fpresel::call<tsl<ii>>::value;
	typedef typename tsl<ii>::_t_type _t_type;
	static int const hit = iff<selected,xindexof_tulist<tys,_t_type>,itot<-1>>::type::value;
	static int const value =
		iff<(hit>=0),itot<i>,anyindexof_tulist<tsl<ii-1>,tys,fpresel,(selected?i+1:i)>>::type::value;
};
template <template<size_t>class tsl,typename tys,typename fpresel,int i>
struct anyindexof_tulist <tsl<1>,tys,fpresel,i>
{
	static bool const selected = fpresel::call<tsl<1>>::value;
	typedef typename tsl<1>::_t_type _t_type;
	static int const hit = iff<selected,xindexof_tulist<tys,_t_type>,itot<-1>>::type::value;
	static int const value = hit>=0 ? i : -i-1;
};
template <template<size_t>class tsl,typename tys,typename fpresel,int i>
struct anyindexof_tulist <tsl<0>,tys,fpresel,i>
{
	typedef typename tsl<0>::_t_type _t_type;
	static int const hit = -1;
	static int const value = hit>=0 ? i : -i-1;
};



/*
	the same as anyindexof, except:
	when the slist_tn is not a list, that is to say, when the anyindexof return -1,
	the anyxindexof handle the condtion, it will compare 
*/

template <typename sl,typename tys>
struct anyxindexof
{
	typedef typename slistify<tys>::type types;
	typedef typename slistify<sl>::type sl;

	typedef anyindexof<sl,types> middle;
	/// out
	static int const value = middle::value;
};

template <typename sl,typename tys,typename fpresel=lambda<presel_tulist_always<ox::_1>>>
struct anyxindexof_tulist
{
	static int const value = is_same<sl,tys>::value ? 0: -1;
};
template <typename sl,template<size_t>class tys,size_t ii2,typename fpresel>
struct anyxindexof_tulist<sl,tys<ii2>,fpresel>
{
	static int const value = anyindexof_tulist<tys<ii2>,sl,fpresel>::value>=0 ? 0: -1;
};

template <template<size_t>class tsl,size_t ii,typename tys,typename fpresel>
struct anyxindexof_tulist <tsl<ii>,tys,fpresel>
{
	//___static_assert_same_type(tsl<ii>,int);
	static int const value = anyindexof_tulist<tsl<ii>,tys,fpresel>::value;
};
template <template<size_t>class tsl,size_t ii,template<size_t>class tys,size_t ii2,typename fpresel>
struct anyxindexof_tulist <tsl<ii>,tys<ii2>,fpresel>
{
	static int const value = anyindexof_tulist<tsl<ii>,tys<ii2>,fpresel>::value;
};


template <typename range_tn>
struct next_of_slist
{
	typedef typename range_tn::_t_next type;
};

//
///// at of tslist [0..n]
///// this is simple implement, do not use the scan function

//template <size_t suffix_tn>
//struct at_of_slist<void,suffix_tn>
//{
//	typedef void owner;
//	typedef void type;
//	static size_t const suffix = suffix_tn;
//};

template
<
	typename slist_tn,size_t suffix_tc,
	typename presel_lmd_tn = lambda<always_select<_1> >
>
struct at_of_slist
{
	typedef presel_lmd_tn presel_lmd;
	/// _type_tn is _slist_tn::_t_type
	template <typename _slist_tn, typename _type_tn, size_t _suffix_tc>
	struct result
	{
		typedef _slist_tn owner;
		typedef _type_tn type;
		static size_t const value = _suffix_tc;
	};

	template <typename _slist_tn, size_t _suffix_tc>
	struct at_inside
	{
		typedef _slist_tn owner;
		typedef void type;
		static size_t const value = _suffix_tc;
	};
	template <typename type_tn, typename syms_tn, typename meta_tn, typename next_tn>
	struct at_inside <tslist<type_tn,syms_tn,meta_tn,next_tn>,0>
	{
		struct result_self
		{
			typedef tslist<type_tn,syms_tn,meta_tn,next_tn> owner;
			typedef type_tn type;
			static size_t const value = 0;
		};

		static bool const __is_selected = presel_lmd::call<tslist<type_tn,syms_tn,meta_tn,next_tn>>::value;
		typedef typename
			iff
			<
				__is_selected,
				result_self,
				at_inside<next_tn,0>
			>
			::type
		result;

		typedef typename result::owner owner;
		typedef typename result::type type;
		static size_t const value = 0;
	};

	template <typename type_tn, typename syms_tn, typename meta_tn, typename next_tn, size_t _suffix_tc>
	struct at_inside <tslist<type_tn,syms_tn,meta_tn,next_tn>,_suffix_tc>
	{
		static bool const __is_selected = presel_lmd::call<tslist<type_tn,syms_tn,meta_tn,next_tn>>::value;
		static size_t const __next_suffix = _suffix_tc - (__is_selected?1:0);
		typedef at_inside<next_tn,__next_suffix> _result;
		typedef typename _result::owner owner;
		typedef typename _result::type type;
		static size_t const value = _result::value;
	};

	typedef typename
		met::iff
		<
			met::is_slist<slist_tn>::value,
			at_inside<slist_tn,suffix_tc>,
			result<slist_tn,void,suffix_tc+1>
		>
		::type
	_result;
	typedef typename _result::owner owner;
	typedef typename _result::type type;
	typedef itot<suffix_tc-_result::value> suffix;
};


/// begin type of slist
template <typename slist_tn>
struct begin
{
	___static_assert(met::is_nothing<slist_tn>::value);
	typedef slist_tn type;
	typedef void syms_tn;
	typedef void meta;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct begin <tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	typedef type_tn type;
	typedef syms_tn syms;
	typedef meta_tn meta;
};

/// end type of slist
/// slist must be the type of slist-like

template <typename slist_tn,typename is_end_tn=void>
struct end
{
private:
	template <typename type_tn>
	struct is_end_default
	{
		static bool const value = is_nothing<type_tn>::value;
	};
	typedef typename
		iff
		<
			is_nothing<is_end_tn>::value,
			is_end_default<_1>,
			is_end_tn
		>
		::type
	is_end_type;
	typedef typename lambdate<is_end_type>::type is_ended;

	template <typename type_tn>
	struct id
	{
		typedef type_tn _t_type;
	};
	template <typename _owner_tn,typename _is_ended_tn>
	struct identity_of_end
	{
		typedef _owner_tn owner;
		typedef typename
			iff
			<
				_is_ended_tn::recall<owner>::value,
				id<void>,
				owner
			>
			::type::_t_type
		type;
	};

	template <typename slist_tn,typename prev_slist_tn,typename is_end_tn>
	struct endd
	{
		typedef typename
			iff
			<
				is_end_tn::recall<slist_tn>::value,
				identity_of_end<prev_slist_tn,is_end_tn>,
				endd <typename slist_tn::_t_next,slist_tn,is_end_tn>
			>
			::type
		result;
		/// force the compiler to unford the type
		typedef typename result::owner owner;
		typedef typename result::type type;
	};

public:
	//typedef typename endd<slist_tn,void,is_ended>::result result;
	typedef endd<slist_tn,void,is_ended> result;
	typedef typename result::type type;
	typedef typename result::owner owner;
	typedef typename owner::_t_syms syms;
	typedef typename owner::_t_meta meta;
};

/// <a,b,c> ==> <a const, b const, c const>
template <typename slist_tn>
struct add_con_to_slist
{
	typedef slist_tn type;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct add_con_to_slist<tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	typedef
		tslist
		<
			typename add_con<type_tn>::type,syms_tn,meta_tn,
			typename add_con_to_slist<next_tn>::type
		>
	type;
};

/// <a,b,c> ==> <a const, b const, c const>
template <typename slist_tn>
struct rmv_con_of_slist
{
	typedef slist_tn type;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct rmv_con_of_slist<tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	typedef
		tslist
		<
			typename rmv_con<type_tn>::type,syms_tn,meta_tn,
			typename rmv_con_of_slist<next_tn>::type
		>
	type;
};

/**
	is there any wrong? : whether the object type is removed ptr,
	because the ptr is behalfed as a type too.
	xxxxxxxxxxxxxxxxxxxxxxxxxxx
*/
template <typename slist_tn>
struct is_con_of_slist;


template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn>
struct is_con_of_slist <tslist<type_tn,syms_tn,meta_tn,next_tn>>
{
	typedef typename rmv_ref<type_tn>::type noref_type;
	typedef typename rmv_ptr<noref_type>::type noptr_type;
	static bool const value = is_con<noptr_type>::value;
};

template <typename type_tn,typename syms_tn,typename meta_tn,typename typenext_tn,typename symsnext_tn,typename metanext_tn,typename nextnext_tn>
struct is_con_of_slist <tslist<type_tn,syms_tn,meta_tn,tslist<typenext_tn,symsnext_tn,metanext_tn,nextnext_tn>>>
{
	static bool const value =
		met::iff
		<
			is_con<type_tn>::value,
			is_con_of_slist<tslist<typenext_tn,symsnext_tn,metanext_tn,nextnext_tn>>,
			boolean<false>
		>
		::type::value;
};


/*
	find a type
	t = <a, b, c, d >
	template <typename slist_tn,typename index>
	struct test
	{
		typedef typename slist_tn::_t_type _t_type;
		typedef typename slist_tn::_t_syms _t_syms;
		typedef typename slist_tn::_t_meta _t_meta;
		static size_t const __index = index::value;
		/// necessary
		static bool const value = ___is_type_same(type_tn,int);
	};

	find <t, test>
	result:
	type is the owner
	boolean indicats whether the test<type> is true
	index is the seq number started from 0 of the last execution
	if donot find, the index is the size of the slist
 */

template <typename slist_tn,typename test_tn,typename index_tn = itot<0>>
struct find_if
{
	struct result_type
	{
		typedef void type;
		typedef boolean<false> boolean;
		typedef index_tn index;
	};
	typedef typename result_type::type type;
	typedef typename result_type::index index;
};

template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename test_tn,typename index>
struct find_if<tslist<type_tn,syms_tn,meta_tn,next_tn>,test_tn,index>
{
	template <typename slist_tn,typename test_tn,typename index>
	friend struct find_if;
private:
	typedef typename lambdate<test_tn>::type test;
	template <typename _owner_tn,typename _index>
	struct finded
	{
		struct result_type
		{
			typedef _owner_tn type;
			typedef boolean<true> boolean;
			typedef _index index;
		};
	};
	typedef typename
		iff
		<
			test::recall<tslist<type_tn,syms_tn,meta_tn,next_tn>,index>::value,
			finded<tslist<type_tn,syms_tn,meta_tn,next_tn>,index>,
			find_if<next_tn,test,typename index::inc<1>::type>
		>
		::type::result_type
	result_type;

public:
	typedef typename result_type::type type;
	typedef typename result_type::boolean boolean;
	typedef typename result_type::index index;
};

/*
	find a type
	t = <a, b, c, d >
	find <t, c>
	result:
	type is the owner
	boolean indicats whether inputed type matches the element
	index is the seq number started from 0 of the last execution
	if donot find, the index is the size of the slist
 */
template <typename slist_tn,typename target_tn,typename index = itot<0>>
struct find
{
	struct result_type
	{
		typedef void type;
		typedef itot<-1> index;
	};
	typedef boolean<false> boolean;
	typedef typename result_type::type type;
	typedef typename result_type::index index;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename target_tn,typename index>
struct find<tslist<type_tn,syms_tn,meta_tn,next_tn>,target_tn,index>
{
	typedef typename
		find<next_tn,target_tn,typename index::inc<1>::type>::result_type
	result_type;
	typedef boolean<false> boolean;
	typedef typename result_type::type type;
	typedef typename result_type::index index;
};
template <typename next_tn,typename syms_tn,typename meta_tn,typename target_tn,typename index_tn>
struct find<tslist<target_tn,syms_tn,meta_tn,next_tn>,target_tn,index_tn>
{
	struct result_type
	{
		typedef boolean<true> boolean;
		typedef tslist<target_tn,syms_tn,meta_tn,next_tn> type;
		typedef index_tn index;
	};
};


/*
	template <typename slist_tn,typename index_tn>
	struct mod
	{
		/// get the current index
		static long const index = index_tn::value;

		/// needed below
		/// whether deleted
		typedef boolean<true> erased;
		/// if this is valid, the modified type
		typedef typename slist_tn::_t_type type;
		typedef typename slist_tn::_t_syms syms;
		typedef typename slist_tn::_t_meta meta;
		/// whether break this scan
		typedef boolean<true> going;
	};
	the first modify<,>, we just use the boolean
	which decide the first is valid
	other fields we donot care

	last: if the go to the end of the slist
	type: the final modified slist
	next: the whole list apart from the catched slist
	catched: the size of we handled and return true
	erased: the size of we erased in the catched events
	taken: the size of type = catched - erased
 */
template <typename slist_tn,typename modify_tn,long index_tc=0,long erased_tc=0>
struct modify_if
{
	/// the slist_tn is not slist type, if index==0
	/// boolean means whether reaches the next of the last
	typedef boolean<true/*index_tc!=0*/> last;
	typedef slist_tn type; /// not using
	typedef slist_tn next;
	typedef itot<index_tc> catched; /// use covered instead
	typedef itot<erased_tc> erased;
	typedef itot<index_tc-erased_tc> taken;
};
template <typename type_tn,typename syms_tn,typename meta_tn,typename next_tn,typename modify_tn,long index_tc,long erased_tc>
struct modify_if<tslist<type_tn,syms_tn,meta_tn,next_tn>,modify_tn,index_tc,erased_tc>
{
	typedef typename lambdate<modify_tn>::type modi_lambda;

	typedef typename modi_lambda::reapply<tslist<type_tn,syms_tn,meta_tn,next_tn>,itot<index_tc> >::type modiout;

	//// every field of abort will be gotten at the last
	struct abort_modify_if
	{
		typedef boolean<false> last;
		typedef void type;
		typedef void next;
		typedef void taken;
		typedef void catched;
		typedef void erased;
	};

	/// get the next modfied_if result
	typedef typename
		iff
		<
			/// whether this is enabled
			modiout::going::value,
			modify_if<next_tn,modiout,index_tc+1,(modiout::erased::value?erased_tc+1:erased_tc)>,
			abort_modify_if
		>
		::type
	modified_if_next;


	/// all bellowed is output

	/// form the modified slist
	typedef typename
		iff
		<
			/// whether this modification action is enabled
			modiout::going::value,
			typename iff
			<
				modiout::erased::value,
				typename modified_if_next::type,
				tslist<typename modiout::type,typename modiout::syms,typename modiout::meta,typename modified_if_next::type>
			>::type,
			void
		>
		::type
	type;

	typedef typename
		iff
		<
			/// whether this is enabled
			modiout::going::value,
			typename modified_if_next::catched,
			itot<index_tc>
		>
		::type
	catched;

	typedef itot<erased_tc> erased;

	typedef itot<catched::value-erased::value> taken;

	typedef typename
		iff
		<
			/// whether this is enabled
			modiout::going::value,
			typename modified_if_next::last,
			boolean<false>
		>
		::type
	last;

	typedef typename
		iff
		<
			/// whether this is enabled
			modiout::going::value,
			typename modified_if_next::next,
			tslist<type_tn,syms_tn,meta_tn,next_tn>
		>
		::type
	next;
};


/// <<a,b,c>, <d,e,f>, <g>> ==> <a,b,c, d,e,f, g>
/*
	ditto as push_front_backward:

	template <typename slist_tn, typename index>
	struct test
	{
		static bool const value = true; /// okay push
		/// static bool const value = false; /// donot push
	};
 */
template <typename slist_of_slist_tn,typename test_tn=void>
struct linkall
{
private:
	/// make the default test
	template <typename slist_tn,typename index>
	struct always_true  { static bool const value = true; };

	typedef typename
		met::iff<met::is_void<test_tn>::value,always_true<void,void>,test_tn>::type
	test_raw_type;
	typedef typename lambdate<test_raw_type>::type test_type;


	/// inside linkall action
	template <typename _slist_of_slist_tn,typename slist_prev_tn,typename test_tn>
	struct linkall_reversed
	{
		typedef slist_prev_tn type;
	};
	template <typename slist_tn,typename slist_syms_tn,typename slist_meta_tn,typename slist_of_slist_next_tn,typename slist_prev_tn,typename _test_tn>
	struct linkall_reversed <tslist<slist_tn,slist_syms_tn,slist_meta_tn,slist_of_slist_next_tn>,slist_prev_tn,_test_tn>
	{
		typedef typename
			push_front_backward<slist_prev_tn,slist_tn,_test_tn>::type
		slist_prev_type;
		typedef typename
			linkall_reversed<slist_of_slist_next_tn,slist_prev_type,_test_tn>::type
		type;
	};

	typedef typename
		linkall_reversed<slist_of_slist_tn,void,test_type>::type
	slist_reversed;

public:
	/// reverse the result
	typedef typename
		reverse<slist_reversed>::type
	type;
};


/// merge two slist
/// for example:
/// <a1,<b1,<c1,0>>> + <a2,<b2,<c2,0>>>
/// == > <<a1,<a2,0>>,<<b1,<b2,0>>,<<c1,<c2,0>>,0>>>
template <typename slist1_tn,typename slist2_tn>
struct merge_slist
{
	typedef void type;
};
template <typename type1_tn,typename syms1_tn,typename meta1_tn,typename next1_tn,typename type2_tn,typename syms2_tn,typename meta2_tn,typename next2_tn>
struct merge_slist <tslist<type1_tn,syms1_tn,meta1_tn,next1_tn>,tslist<type2_tn,syms2_tn,meta2_tn,next2_tn>>
{
	typedef tslist<type1_tn,syms1_tn,meta1_tn,tslist<type2_tn,syms2_tn,meta2_tn,void>> _t_type;
	typedef typename merge_slist<next1_tn,next2_tn>::type next_merge_type;
	typedef tslist<_t_type,void,void,next_merge_type> type;
};
template <typename type2_tn,typename syms2_tn,typename meta2_tn,typename next2_tn>
struct merge_slist <void,tslist<type2_tn,syms2_tn,meta2_tn,next2_tn>>
{
	typedef tslist<ox::null_type,tslist<type2_tn,syms2_tn,meta2_tn,void>> _t_type;
	typedef typename merge_slist<void,next2_tn>::type next_merge_type;
	typedef tslist<_t_type,void,void,next_merge_type> type;
};
template <typename type1_tn,typename syms1_tn,typename meta1_tn,typename next1_tn>
struct merge_slist <tslist<type1_tn,syms1_tn,meta1_tn,next1_tn>,void>
{
	typedef tslist<type1_tn,syms1_tn,meta1_tn,tslist<ox::null_type,void,void,void>> _t_type;
	typedef typename merge_slist<next1_tn,void>::type next_merge_type;
	typedef tslist<_t_type,void,void,next_merge_type> type;
};


/// remove handle prototype
/*
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct handle
	{
		typedef slist_tn::_t_type xxx;
		typedef slist_tn::_t_syms xxx;
		typedef slist_tn::_t_meta xxx;
		static bool const value = false;
		typede xxx type; /// the type will be put in prev_tested_tn
	};
 */
/// remove of slist
template
<
	typename slist_tn,
	typename is_removed_test_tn,
	typename slike_lambda_tn = typename relambda<slist_tn>::full,
	typename tested_before_tn = void
>
struct remove
{
private:
	typedef typename lambdate<is_removed_test_tn>::type test_lambda;
	typedef typename lambdate<slike_lambda_tn>::type slike_lambda;

	template
	<
		typename _slist_tn,
		size_t _suffix/*=0*/,
		size_t _prev_size/*=0*/,
		typename _prev_tn/*=void*/,
		typename _prev_removed_tn/*=void*/,
		typename _prev_tested/*=void*/
	>
	struct _remove
	{
		typedef typename _slist_tn::_t_type curr_type;
		typedef typename _slist_tn::_t_syms curr_syms;
		typedef typename _slist_tn::_t_meta curr_meta;
		typedef _slist_tn slist_type;
		typedef typename test_lambda::reapply<slist_type,itot<_suffix>,_prev_tested>::type tested;
		static bool const __is_removable = tested::value;

		typedef typename
			iff<__is_removable,_prev_tn,typename slike_lambda::apply<curr_type,curr_syms,curr_meta,_prev_tn>::type>
			::type
		curr_slist;

		typedef typename
			iff<__is_removable,typename slike_lambda::apply<curr_type,curr_syms,curr_meta,_prev_removed_tn>::type,_prev_removed_tn>
			::type
		curr_removed;

		typedef typename _slist_tn::_t_next next_type;
		static size_t const __curr_size = __is_removable?_prev_size:_prev_size+1;

		typedef
			_remove<next_type,_suffix+1,__curr_size,curr_slist,curr_removed,tested>
		_removed_result;
		typedef typename _removed_result::type type;
		typedef typename _removed_result::removed removed;
		typedef typename _removed_result::last_tested last_tested;
		static size_t const suffix = _removed_result::suffix;
		static size_t const prev_size = _removed_result::prev_size;
	};
	template
	<
		size_t _suffix,
		size_t _prev_size,
		typename _prev_tn,
		typename _prev_removed_tn,
		typename _prev_tested_tn
	>
	struct _remove
	<
		void,_suffix,_prev_size,
		_prev_tn,_prev_removed_tn,_prev_tested_tn
	>
	{
		typedef _prev_tn type;
		typedef _prev_removed_tn removed;
		typedef _prev_tested_tn last_tested;
		static size_t const suffix = _suffix;
		static size_t const prev_size = _prev_size;
	};

	typedef _remove<slist_tn,0,0,void,void,tested_before_tn> removed_result;
	typedef typename removed_result::type reversed_slist;
	typedef typename removed_result::removed reversed_slist_removed;
public:
	typedef typename reverse<reversed_slist>::type type;
	typedef typename reverse<reversed_slist_removed>::type removed;
	typedef typename removed_result::last_tested last_tested;
	typedef itot<removed_result::suffix> suffix;
	typedef itot<removed_result::prev_size> size;
};
/// when the input type is void
template
<
	typename is_removed_test_tn,
	typename slike_lambda_tn,
	typename tested_before_tn
>
struct remove <void,is_removed_test_tn,slike_lambda_tn,tested_before_tn>
{
	typedef void type;
	typedef void removed;
	typedef slike_lambda_tn slike_lambda;
	typedef tested_before_tn last_tested;
	typedef itot<-1> suffix;
	typedef itot<0> size;
};



/*
	/// range the current range
	/// params you can customed the type
	template <typename range_tn, typename suffix_tn, typename params_tn>
	struct handle
	{
		typedef .... type; /// the type is used as the next params
	};
 */

/*
	if the range is void, then exit with the null tuple: <type=void, result=started_param, range=ditto as 1st>
	else if prev result's continue is disabled, then return the input params <input_range, input_params, ditto as 1st>
	else call the handle and get the result, 
 */
template
<
	typename range_tn,
	typename handle_tn,
	typename params_tn,
	typename suffix_tn = itot<0>,
	typename next_tn = next_of_slist<_1>
>
struct scan
{
	template <typename,typename,typename,typename,typename>
	friend struct scan;
private:
	typedef typename lambdate<handle_tn>::type lambda_handle;
	typedef typename lambdate<next_tn>::type lambda_next;

	typedef params_tn prev_result;
	static bool const __is_this_enabled = prev_result::__is_continue;

	/// get current result
	struct break_result_of_handle
	{
		struct type
		{
			static bool const __is_continue = false;
		};
	};

	/// 获取当前的执行结果，如果需要执行的话
	typedef typename
		iff
		<
			__is_this_enabled,
			typename lambda_handle::template call<range_tn,suffix_tn,params_tn>,
			break_result_of_handle
		>
		::type::type
	this_result;
	/// get whether or not to go to the next action
	static bool const __is_next_enabled = this_result::__is_continue;

	/// get next range if the next enabled, else return the current range
	typedef typename
		iff
		<
			__is_next_enabled,
			typename lambda_next::template call<range_tn>,
			identity<range_tn>
		>
		::type::type
	range_of_next;

	/// break result of scan, because we just get just the two elements from scan itself
	//template <typename _range_tn,typename _suffix_tn,typename _result_tn>
	//struct scan_of_break
	//{
	//	typedef _range_tn range_type;// <=> this_range;
	//	typedef _suffix_tn suffix;
	//	typedef _result_tn result;
	//};
	/// this result of scan
	struct scan_of_break
	{
		typedef range_tn range_type;// <=> this_range;
		typedef suffix_tn suffix;
		typedef params_tn result;
	};

	typedef typename
		iff
		<
			__is_next_enabled,
			scan
			<
				range_of_next,
				lambda_handle,
				/// next params / e.g. this result
				this_result,
				itot<suffix_tn::value+1>,
				lambda_next
			>,
			/// should be this, not be the above
			scan_of_break
		>
		::type
	scan_of_next;

	/// out result
public:
	typedef typename scan_of_next::range_type range_type;
	typedef typename scan_of_next::suffix suffix;
	/// result is the param
	typedef typename scan_of_next::result result;
	typedef range_type type;
};
template <typename lambda_hande_tn,typename params_tn,typename suffix_tn,typename lambda_next_tn>
struct scan <void,lambda_hande_tn,params_tn,suffix_tn,lambda_next_tn>
{
	template <typename,typename,typename,typename,typename>
	friend struct scan;
private:
	typedef void this_range_type;
	typedef params_tn this_result;
public:
	/// normally, compilior will never run here
	typedef this_range_type type;
	typedef this_range_type range_type;
	typedef suffix_tn suffix;
	typedef params_tn result;
};


/*
	template <typename slist_tn, typename suffix_tn, typename last_handled_tn>
	struct handle
	{
		/// necessary, whether the cur_type_tn will be removed
		static bool const __is_removed = false;

		/// ___t2i(suffix_tn)
		/// your actions
		typedef typename slist_tn::_t_type cur_type;

		typedef ....(cur_type) type; /// necessary, the handled type
		typedef ... slist_tn::_t_syms syms;  /// necessary, the handled type
		typedef ... slist_tn::_t_meta meta; /// necessary, the handled type
		//.....

		/// necessary, whether the next handle is continue
		static bool const __is_continue = true;
	};
	/// the started null handled
	template <typename slist_tn, typename last_handled_tn>
	struct handle <slist_tn,itot<-1>,last_handled_tn>
	{
		/// necessary, whether the cur_type_tn will be removed
		static bool const __is_removed = false;

		/// suffix_tn::value is the long type
		/// your actions
		typedef typename slist_tn::_t_type cur_type;

		typedef ....(cur_type) type; /// necessary, the handled type
		typedef ... slist_tn::_t_syms syms;  /// necessary, the handled type
		typedef ... slist_tn::_t_meta meta; /// necessary, the handled type
		//.....

		/// necessary, whether the next handle is continue
		static bool const __is_continue = true;
	};
 */
template
<
	typename range_tn,
	typename handle_tn,/// <p1,p2,p3>
	bool is_start_continue_tc = true, /// is_started is setted by the range is slist or slist-like and while the range is not empty
	typename next_tn = next_of_slist<void>
>
struct apply
{
private:
	typedef typename lambdate<handle_tn>::type lambda_handle;
	typedef typename lambdate<next_tn>::type lambda_next;

private:
	/*
		这个可以修改一下吧，加入一个参数，以更好的支持对批量类型操作，或者把scan修改下
		lambda-handle是对历经每一个元素时，处理过程
		这个handle的result中的__is_continue，表示是否是进入到下一次处理
		当然对于一个元素的处理，也会自然有上一次的处理结果来决定，是否进行本次（第一次）handle
		这个第一次的上一次，我们成为started状态，由apply2的参数给出，如果，这个参数为false，
		那么handle就不会被调用，就直接退出了
		最大的不同时和apply是可以中间退出
	 */
	template
	<
		typename range_tn,
		typename handle_tn,/// <p1,p2,p3>
		bool is_start_continue_tc = true, /// is_started is setted by the range is slist or slist-like and while the range is not empty
		typename next_tn = next_of_slist<void>
	>
	struct apply_on_line
	{
	private:
		typedef typename lambdate<handle_tn>::type lambda_handle;
		typedef typename lambdate<next_tn>::type lambda_next;

		/// construct param struct for handle
		template <bool is_continue_tc,typename slist_reversed_tn,int index_tc,typename lambda_handled_last_tn>
		struct params
		{
			/// necessary member __is_continue
			static bool const __is_continue = is_continue_tc;
			typedef slist_reversed_tn slist_reversed_type;
			static int const __index = index_tc;
			typedef lambda_handled_last_tn handled_last_type;
		};
		template <typename slist_tn, typename type_tn, typename syms_tn, typename meta_tn, bool is_removed_tc>
		struct push_front
		{
			typedef slist_tn type;
		};
		template <typename slist_tn, typename type_tn, typename syms_tn, typename meta_tn>
		struct push_front <slist_tn,type_tn,syms_tn,meta_tn,false>
		{
			typedef typename met::push_front<slist_tn,type_tn,syms_tn,meta_tn>::type type;
		};

		/// construct handle struct, the type is the only result of scan
		template <typename slist_tn,typename suffix_tn,typename params_tn>
		struct scan_handle
		{
			//typedef typename slist_tn::_t_type this_type;
			typedef typename slist_tn this_type;
			typedef typename params_tn::handled_last_type handled_last_type;
			static int const __this_index = params_tn::__index;
			typedef typename
				lambda_handle::reapply<this_type,itot<__this_index>,handled_last_type>::type
			result_type;
			typedef typename result_type::type this_type_handled;
			typedef typename result_type::syms this_syms_handled;
			typedef typename result_type::meta this_meta_handled;
			static bool const __is_removed = result_type::__is_removed;
			static bool const __is_continue = result_type::__is_continue;
			typedef typename params_tn::slist_reversed_type slist_reversed_last;
			/// according to the different result, we take different action
			typedef typename
				push_front<slist_reversed_last,this_type_handled,this_syms_handled,this_meta_handled,__is_removed>::type
			slist_reversed_new;

			typedef params<__is_continue,slist_reversed_new,__this_index+1,result_type> type;
		};

		static bool const __is_slist = met::is_slist<range_tn>::value;
		static bool const __is_start_continue
			= (is_start_continue_tc&&__is_slist)?true:is_start_continue_tc;
		static int const __start_suffix = __is_start_continue?0:-1;
		typedef typename lambda_handle::reapply<tslist<ox::null_type,void,void>,itot<-1>,void>::type null_lambda_handle;

		/// start the action of scan
		typedef typename
			scan
			<
				range_tn, lambda<scan_handle<void,void,void>>,
				/// the prev of first hanle result, i.e. params
				params
				<
					__is_start_continue,void,__start_suffix,
					/// if range is not empty, but is_started is false, the nexline is the result
					/// and the result's type is the virtual handle's result
					null_lambda_handle//started_lambda_handle
				>,
				itot<0>,
				lambda_next
			>
		_scan;

	public:
		typedef typename _scan::suffix suffix;
		/// in fact, the result is the params
		typedef typename _scan::result result;
		/// lambda_handled last type of itself
		typedef typename result::handled_last_type type;
		/// return the final slist transformed
		typedef typename result::slist_reversed_type slist_reversed_type;
	};

private:
	/// excecute
	typedef apply_on_line<range_tn,lambda_handle,is_start_continue_tc,lambda_next> last_second_type;
	typedef typename last_second_type::slist_reversed_type slist_reversed_type;

	/// the execute result
public:
	typedef typename last_second_type::result result;
	typedef typename reverse<slist_reversed_type>::type slist_type;
	typedef typename last_second_type::type type; /// last handled type
};


/// just for transform 
/*
	template <typename slist_tn, typename suffix_tn>
	struct handle
	{
		typedef slist_tn::_t_type type;
		typedef slist_tn::_t_syms syms;
		typedef slist_tn::_t_meta meta;
	};
 */
template
<
	typename range_tn,
	typename handle_tn,
	typename next_tn = next_of_slist<_1>
>
struct modify
{
private:
	typedef typename lambdate<handle_tn>::type lambda_handle;
	typedef typename lambdate<next_tn>::type lambda_next;

	/// construct param struct for handle
	template <typename _range_tn>
	struct params
	{
		/// necessary member __is_continue
		static bool const __is_continue = true;
		typedef _range_tn slist_reversed_type;
	};
	/// construct handle struct, the type is the only result of scan
	template <typename slist_tn,typename suffix_tn,typename params_tn>
	struct handle
	{
		//typedef typename slist_tn::_t_type this_type;
		typedef typename slist_tn this_type;
		typedef typename lambda_handle::reapply<this_type,suffix_tn>::type this_type_result;
		typedef typename this_type_result::type this_type_new;
		typedef typename this_type_result::syms this_syms_new;
		typedef typename this_type_result::meta this_meta_new;
		typedef typename params_tn::slist_reversed_type slist_prev;
		typedef typename
			push_front<slist_prev,this_type_new,this_syms_new,this_meta_new>::type
		slist_reversed_now;
		typedef params<slist_reversed_now> type;
	};

	/// start the action of scan
	typedef typename
		scan
		<
			range_tn,
			handle<_1,_2,_3>,
			params<void>,
			itot<0>,
			lambda_next
		>
	_scan;

	/// in fact, the result is the params
	typedef typename _scan::result result;
public:
	/// lambda_handled last type of itself
	typedef typename
		reverse<typename result::slist_reversed_type>::type
	type;
	/// the amount of modified
	typedef typename itot<_scan::suffix::value+1> size;
};


/// at of tslist [0..n]
template <typename slist_tn>
struct size_of_range
{
	/// construct param struct for handle
	template <bool _is_continue_tc, size_t _count_tc>
	struct params
	{
		/// necessary member __is_continue
		static bool const __is_continue = _is_continue_tc;
		/// other's info
		static size_t const count = _count_tc;
	};
	/// construct handle struct
	template <typename slist_tn,typename suffix_tn,typename params_tn>
	struct handle
	{
		static bool const __is_continue = true;
		static size_t const count = params_tn::count+1;
		typedef params<__is_continue,count> type;
	};
	/// start the action of scan
	typedef typename
		scan
		<
			slist_tn,
			lambda<handle<_1,_2,_3>>,
			params<true,0>,
			itot<0>,
			lambda<next_of_slist<_1>>
		>
	_scan;
	/// return the result;
	static size_t const value = _scan::result::count;
};


/// at of tslist [0..n]
template
<
	typename slist_tn,
	size_t suffix_tc,
	typename presel_lmd = lambda<always_select<_1>>,
	typename nextslist_tn=next_of_slist<_1>
>
struct at_of_range
{
private:
	/// construct param struct for handle
	template <bool _is_continue_tc, size_t _suffix_tc,size_t _suffix_all>
	struct params
	{
		/// necessary member __is_continue
		static bool const __is_continue = _is_continue_tc;
		/// other's info
		static size_t const suffix = _suffix_tc;
		static size_t const suffix_all = _suffix_all;
	};
	/// construct handle struct
	template <typename slist_tn,typename suffix_tn,typename params_tn>
	struct handle
	{
		static bool const __is_selected = presel_lmd::call<slist_tn>::value;
		static bool const __is_continue = !(__is_selected && params_tn::suffix==0);

		static size_t const suffix = params_tn::suffix - (__is_selected?1:0);
		static size_t const suffix_all = params_tn::suffix_all+1;
		/// handle result
		typedef params<__is_continue,suffix,suffix_all> type;
	};
	/// start the action of scan
	typedef typename
		scan
		<
			slist_tn,
			handle<_1,_2,_3>,
			params<true,suffix_tc,0>,
			itot<0>,
			nextslist_tn
		>
	_scan;
	typedef typename _scan::type _range;

	/// return the result of current type;
	struct void_type
	{
		typedef void _t_type;
	};

public:
	typedef _range owner;
	typedef typename iff<is_void<owner>::value,void_type,owner>::type::_t_type type;
	//typedef typename range_type::_t_syms syms;
	//typedef typename range_type::_t_meta meta;
	typedef itot<suffix_tc-_scan::result::suffix> suffix;
	typedef itot<suffix_tc-_scan::result::suffix_all> suffix_all;
};

/// can accept both the tslist and range type
template
<
	typename slist_like_tn,
	size_t suffix_tc,
	typename presel_lmd = lambda<always_select<_1>>,
	typename nextslist_tn=next_of_slist<_1>
>
struct at
{
private:
	typedef typename
		met::iff
		<
			is_slist<slist_like_tn>::value,
			at_of_slist<slist_like_tn,suffix_tc>,
			at_of_range<slist_like_tn,suffix_tc,presel_lmd,nextslist_tn>
		>
		::type
	result;
public:
	typedef typename result::owner owner;
	typedef typename result::type type;
	typedef typename result::suffix suffix;
};


template <typename slist_tn,int begin_tc,int end_tc>
struct slice
{
private:
	template <typename _slist_tn,typename index_tn,typename prev_tested_tn>
	struct handle_remove_range
	{
		static bool const value
			= index_tn::value<begin_tc || index_tn::value>end_tc;
	};
public:
	typedef typename
		remove<slist_tn,handle_remove_range<_1,_2,_3>>::type
	type;
};

template <typename slist_tn,size_t size_tc>
struct left
{
	typedef typename
		slice <slist_tn,0,size_tc-1>::type
	type;
};


/// slist1_tn& = slist2_tn
template <typename slist1_tn,typename slist2_tn>
struct get_const_slist
{
	template <typename slist_tn>
	struct handle
	{
		typedef typename at_of_slist<slist_tn,0>::type type1;
		typedef typename at_of_slist<slist_tn,1>::type type2;
		static bool const __is_type1_const = is_con<type1>::value;
		static bool const __is_type2_const = is_con<type2>::value;
		static bool const __is_type1_null = is_null<type1>::value;
		static bool const __is_type2_null = is_null<type2>::value;
		static size_t const __const_bit1 = __is_type1_const?1:0;
		static size_t const __const_bit2 = __is_type2_const?1:0;
		static size_t const __null_bit1 = __is_type1_null?1:0;
		static size_t const __null_bit2 = __is_type2_null?1:0;
		typedef
			typename iff <(__null_bit1),int,
			typename iff <(__null_bit2),double,
			typename iff <(__const_bit1^__const_bit2)&&!__const_bit1,bool,
				void
			>::type>::type>::type
		type;
	};
	typedef typename merge_slist<slist1_tn,slist2_tn>::type merged_slist;
	typedef typename
		modify<merged_slist,lambda<handle<ox::_1>>>::type
	type;
};

/// just test elements of min size of both slists
template <typename slist1_tn,typename slist2_tn,char size_mode_tc>
struct is_slist_const_of
{
	typedef typename
		iff
		<
			is_con<slist1_tn>::value,
			modify<slist1_tn,lambda<add_con<ox::_1>>>,
			identity<slist1_tn>
		>
		::type::type
	slist1_type;
	typedef typename
		iff
		<
			is_con<slist2_tn>::value,
			modify<slist2_tn,lambda<add_con<ox::_1>>>,
			identity<slist2_tn>
		>
		::type::type
	slist2_type;
	typedef typename
		get_const_slist<slist1_type,slist2_type>::type
	const_slist_type;
	static int const __index1 = indexof<const_slist_type,bool>::value;
	static int const __index2 = indexof<const_slist_type,int>::value;
	static int const __index3 = indexof<const_slist_type,double>::value;

	typedef typename
		_switch
		<
			_case<size_mode_tc=='=',itot<__index1==-1 && __index2==-1 && __index3==-1> >,
			_case<size_mode_tc=='<',itot<__index1==-1                 && __index3==-1> >,
			_case<size_mode_tc=='>',itot<__index1==-1 && __index2==-1                > >,
			_case<true,itot<0> >
		>
		::type
	result_type;

	static bool const value = result_type::value!=0;
};


/// == => void
/// != => bool
/// 00 => double
/// == ==> int const <=> int const; int const <!=> int
/// = ==> int const <=> int; int & <!=> int
/// ~ ==> = and int& <=> int
/// < ==> base&=derived,<base,derived>,that is: first=cast<second>
/// > ==> derived=base&,<derived,base>,that is: second=cast<first>
template <typename slist1_tn,typename slist2_tn,char type_mode_tc>
struct get_slist_struct_equ
{
	template <typename type_tn>
	struct handle
	{
		typedef typename at_of_slist<type_tn,0>::type type1;
		typedef typename at_of_slist<type_tn,1>::type type2;
		typedef typename rmv_con<type1>::type noncon_type1_naked;
		typedef typename rmv_con<type2>::type noncon_type2_naked;
		typedef typename
			iff
			<
				type_mode_tc=='=',
				noncon_type1_naked,
				typename rmv_ref<noncon_type1_naked>::type
			>::type
		type1_naked;
		typedef typename
			iff
			<
				type_mode_tc=='=',
				noncon_type2_naked,
				typename rmv_ref<noncon_type2_naked>::type
			>::type
		type2_naked;
		static bool const __is_same = ___is_type_same(type1_naked,type2_naked);
		static bool const __is1_null = is_null<type1_naked>::value;
		static bool const __is2_null = is_null<type2_naked>::value;
		typedef
			typename iff<__is_same, void,
			typename iff<!__is1_null&&!__is2_null, bool,
			typename iff<__is1_null&&!__is2_null,int,
			typename iff<!__is1_null&&__is2_null,double,
				void
			>::type>::type>::type>::type
		type;
	};
	typedef typename merge_slist<slist1_tn,slist2_tn>::type merged_slist;
	typedef typename
		modify<merged_slist,lambda<handle<ox::_1>>>::type
	type;
};


/// length mode has four types
/// 1. '=' length slist1= length slist2
/// 2. '<' length slist1< length slist2
/// 3. '>' length slist1> length slist2
/// 4. '0' just match the first min count {slist1,slist2}

template <typename slist1_tn,typename slist2_tn,char type_mode_tc='=',char size_mode_tc='<'>
struct is_slist_struct_equ;

template <typename slist1_tn,typename slist2_tn,char type_mode_tc>
struct is_slist_struct_equ <slist1_tn,slist2_tn,type_mode_tc,'='>
{
	typedef typename
		get_slist_struct_equ<slist1_tn,slist2_tn,type_mode_tc>::type
	equ_slist_type;
	static int const __index0 = indexof<equ_slist_type,void>::value;
	static int const __index1 = indexof<equ_slist_type,bool>::value;
	static int const __index2 = indexof<equ_slist_type,int>::value;
	static int const __index3 = indexof<equ_slist_type,double>::value;
	static bool const value = __index1==-1 && __index2==-1 && __index3==-1;
};
template <typename slist1_tn,typename slist2_tn,char type_mode_tc>
struct is_slist_struct_equ <slist1_tn,slist2_tn,type_mode_tc,'<'>
{
	typedef typename
		get_slist_struct_equ<slist1_tn,slist2_tn,type_mode_tc>::type
	equ_slist_type;
	static int const __index0 = indexof<equ_slist_type,void>::value;
	static int const __index1 = indexof<equ_slist_type,bool>::value;
	static int const __index2 = indexof<equ_slist_type,int>::value;
	static int const __index3 = indexof<equ_slist_type,double>::value;
	static bool const value = __index1==-1 && __index3==-1;
};
template <typename slist1_tn,typename slist2_tn,char type_mode_tc>
struct is_slist_struct_equ <slist1_tn,slist2_tn,type_mode_tc,'>'>
{
	typedef typename
		get_slist_struct_equ<slist1_tn,slist2_tn,type_mode_tc>::type
	equ_slist_type;
	static int const __index0 = indexof<equ_slist_type,void>::value;
	static int const __index1 = indexof<equ_slist_type,bool>::value;
	static int const __index2 = indexof<equ_slist_type,int>::value;
	static int const __index3 = indexof<equ_slist_type,double>::value;
	static bool const value = __index1==-1 && __index2==-1;
};

/// just reserve the ref types
template <typename slist_tn>// = lambda<next_of_slist<void>>>
struct only_ref_of_slist
{
	template <typename _slist_tn,typename index_tn,typename prev_tested_tn>
	struct handle_remove_non_ref
	{
		typedef typename _slist_tn::_t_type _t_type;
		static bool const value = !is_ref<_t_type>::value;
	};
	typedef typename
		remove<slist_tn,handle_remove_non_ref<void,void,void>>::type
	type;
};


/// 32
template
<
	typename e1=ox::null_type,typename e2=ox::null_type,typename e3=ox::null_type,typename e4=ox::null_type,
	typename e5=ox::null_type,typename e6=ox::null_type,typename e7=ox::null_type,typename e8=ox::null_type,
	typename e9=ox::null_type,typename e10=ox::null_type,typename e11=ox::null_type,typename e12=ox::null_type,
	typename e13=ox::null_type,typename e14=ox::null_type,typename e15=ox::null_type,typename e16=ox::null_type,
	typename e17=ox::null_type,typename e18=ox::null_type,typename e19=ox::null_type,typename e20=ox::null_type,
	typename e21=ox::null_type,typename e22=ox::null_type,typename e23=ox::null_type,typename e24=ox::null_type,
	typename e25=ox::null_type,typename e26=ox::null_type,typename e27=ox::null_type,typename e28=ox::null_type,
	typename e29=ox::null_type,typename e30=ox::null_type,typename e31=ox::null_type,typename e32=ox::null_type
>
struct mkslist
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;
	typedef typename ts::tof<e5>::t t5; typedef typename ts::sof<e5>::t i5; typedef typename ts::mof<e5>::t m5;
	typedef typename ts::tof<e6>::t t6; typedef typename ts::sof<e6>::t i6; typedef typename ts::mof<e6>::t m6;
	typedef typename ts::tof<e7>::t t7; typedef typename ts::sof<e7>::t i7; typedef typename ts::mof<e7>::t m7;
	typedef typename ts::tof<e8>::t t8; typedef typename ts::sof<e8>::t i8; typedef typename ts::mof<e8>::t m8;
	typedef typename ts::tof<e9>::t t9; typedef typename ts::sof<e9>::t i9; typedef typename ts::mof<e9>::t m9;
	typedef typename ts::tof<e10>::t t10; typedef typename ts::sof<e10>::t i10; typedef typename ts::mof<e10>::t m10;
	typedef typename ts::tof<e11>::t t11; typedef typename ts::sof<e11>::t i11; typedef typename ts::mof<e11>::t m11;
	typedef typename ts::tof<e12>::t t12; typedef typename ts::sof<e12>::t i12; typedef typename ts::mof<e12>::t m12;
	typedef typename ts::tof<e13>::t t13; typedef typename ts::sof<e13>::t i13; typedef typename ts::mof<e13>::t m13;
	typedef typename ts::tof<e14>::t t14; typedef typename ts::sof<e14>::t i14; typedef typename ts::mof<e14>::t m14;
	typedef typename ts::tof<e15>::t t15; typedef typename ts::sof<e15>::t i15; typedef typename ts::mof<e15>::t m15;
	typedef typename ts::tof<e16>::t t16; typedef typename ts::sof<e16>::t i16; typedef typename ts::mof<e16>::t m16;
	typedef typename ts::tof<e17>::t t17; typedef typename ts::sof<e17>::t i17; typedef typename ts::mof<e17>::t m17;
	typedef typename ts::tof<e18>::t t18; typedef typename ts::sof<e18>::t i18; typedef typename ts::mof<e18>::t m18;
	typedef typename ts::tof<e19>::t t19; typedef typename ts::sof<e19>::t i19; typedef typename ts::mof<e19>::t m19;
	typedef typename ts::tof<e20>::t t20; typedef typename ts::sof<e20>::t i20; typedef typename ts::mof<e20>::t m20;
	typedef typename ts::tof<e21>::t t21; typedef typename ts::sof<e21>::t i21; typedef typename ts::mof<e21>::t m21;
	typedef typename ts::tof<e22>::t t22; typedef typename ts::sof<e22>::t i22; typedef typename ts::mof<e22>::t m22;
	typedef typename ts::tof<e23>::t t23; typedef typename ts::sof<e23>::t i23; typedef typename ts::mof<e23>::t m23;
	typedef typename ts::tof<e24>::t t24; typedef typename ts::sof<e24>::t i24; typedef typename ts::mof<e24>::t m24;
	typedef typename ts::tof<e25>::t t25; typedef typename ts::sof<e25>::t i25; typedef typename ts::mof<e25>::t m25;
	typedef typename ts::tof<e26>::t t26; typedef typename ts::sof<e26>::t i26; typedef typename ts::mof<e26>::t m26;
	typedef typename ts::tof<e27>::t t27; typedef typename ts::sof<e27>::t i27; typedef typename ts::mof<e27>::t m27;
	typedef typename ts::tof<e28>::t t28; typedef typename ts::sof<e28>::t i28; typedef typename ts::mof<e28>::t m28;
	typedef typename ts::tof<e29>::t t29; typedef typename ts::sof<e29>::t i29; typedef typename ts::mof<e29>::t m29;
	typedef typename ts::tof<e30>::t t30; typedef typename ts::sof<e30>::t i30; typedef typename ts::mof<e30>::t m30;
	typedef typename ts::tof<e31>::t t31; typedef typename ts::sof<e31>::t i31; typedef typename ts::mof<e31>::t m31;
	typedef typename ts::tof<e32>::t t32; typedef typename ts::sof<e32>::t i32; typedef typename ts::mof<e32>::t m32;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,tslist<t3,i3,m3,tslist<t4,i4,m4,tslist<t5,i5,m5,tslist<t6,i6,m6,tslist<t7,i7,m7,tslist<t8,i8,m8,
		tslist<t9,i9,m9,tslist<t10,i10,m10,tslist<t11,i11,m11,tslist<t12,i12,m12,tslist<t13,i13,m13,tslist<t14,i14,m14,tslist<t15,i15,m15,tslist<t16,i16,m16,
		tslist<t17,i17,m17,tslist<t18,i18,m18,tslist<t19,i19,m19,tslist<t20,i20,m20,tslist<t21,i21,m21,tslist<t22,i22,m22,tslist<t23,i23,m23,tslist<t24,i24,m24,
		tslist<t25,i25,m25,tslist<t26,i26,m26,tslist<t27,i27,m27,tslist<t28,i28,m28,tslist<t29,i29,m29,tslist<t30,i30,m30,tslist<t31,i31,m31,tslist<t32,i32,m32,
		void>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<_1,_2,_3>>::type type;
};

// 24, for compiling speed
template
<
	typename e1,typename e2,typename e3,typename e4,
	typename e5,typename e6,typename e7,typename e8,
	typename e9,typename e10,typename e11,typename e12,
	typename e13,typename e14,typename e15,typename e16,
	typename e17,typename e18,typename e19,typename e20,
	typename e21,typename e22,typename e23,typename e24
>
struct mkslist
<
	e1,e2,e3,e4,e5,e6,e7,e8,
	e9,e10,e11,e12,e13,e14,e15,e16,
	e17,e18,e19,e20,e21,e22,e23,e24,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;
	typedef typename ts::tof<e5>::t t5; typedef typename ts::sof<e5>::t i5; typedef typename ts::mof<e5>::t m5;
	typedef typename ts::tof<e6>::t t6; typedef typename ts::sof<e6>::t i6; typedef typename ts::mof<e6>::t m6;
	typedef typename ts::tof<e7>::t t7; typedef typename ts::sof<e7>::t i7; typedef typename ts::mof<e7>::t m7;
	typedef typename ts::tof<e8>::t t8; typedef typename ts::sof<e8>::t i8; typedef typename ts::mof<e8>::t m8;
	typedef typename ts::tof<e9>::t t9; typedef typename ts::sof<e9>::t i9; typedef typename ts::mof<e9>::t m9;
	typedef typename ts::tof<e10>::t t10; typedef typename ts::sof<e10>::t i10; typedef typename ts::mof<e10>::t m10;
	typedef typename ts::tof<e11>::t t11; typedef typename ts::sof<e11>::t i11; typedef typename ts::mof<e11>::t m11;
	typedef typename ts::tof<e12>::t t12; typedef typename ts::sof<e12>::t i12; typedef typename ts::mof<e12>::t m12;
	typedef typename ts::tof<e13>::t t13; typedef typename ts::sof<e13>::t i13; typedef typename ts::mof<e13>::t m13;
	typedef typename ts::tof<e14>::t t14; typedef typename ts::sof<e14>::t i14; typedef typename ts::mof<e14>::t m14;
	typedef typename ts::tof<e15>::t t15; typedef typename ts::sof<e15>::t i15; typedef typename ts::mof<e15>::t m15;
	typedef typename ts::tof<e16>::t t16; typedef typename ts::sof<e16>::t i16; typedef typename ts::mof<e16>::t m16;
	typedef typename ts::tof<e17>::t t17; typedef typename ts::sof<e17>::t i17; typedef typename ts::mof<e17>::t m17;
	typedef typename ts::tof<e18>::t t18; typedef typename ts::sof<e18>::t i18; typedef typename ts::mof<e18>::t m18;
	typedef typename ts::tof<e19>::t t19; typedef typename ts::sof<e19>::t i19; typedef typename ts::mof<e19>::t m19;
	typedef typename ts::tof<e20>::t t20; typedef typename ts::sof<e20>::t i20; typedef typename ts::mof<e20>::t m20;
	typedef typename ts::tof<e21>::t t21; typedef typename ts::sof<e21>::t i21; typedef typename ts::mof<e21>::t m21;
	typedef typename ts::tof<e22>::t t22; typedef typename ts::sof<e22>::t i22; typedef typename ts::mof<e22>::t m22;
	typedef typename ts::tof<e23>::t t23; typedef typename ts::sof<e23>::t i23; typedef typename ts::mof<e23>::t m23;
	typedef typename ts::tof<e24>::t t24; typedef typename ts::sof<e24>::t i24; typedef typename ts::mof<e24>::t m24;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,tslist<t3,i3,m3,tslist<t4,i4,m4,tslist<t5,i5,m5,tslist<t6,i6,m6,tslist<t7,i7,m7,tslist<t8,i8,m8,
		tslist<t9,i9,m9,tslist<t10,i10,m10,tslist<t11,i11,m11,tslist<t12,i12,m12,tslist<t13,i13,m13,tslist<t14,i14,m14,tslist<t15,i15,m15,tslist<t16,i16,m16,
		tslist<t17,i17,m17,tslist<t18,i18,m18,tslist<t19,i19,m19,tslist<t20,i20,m20,tslist<t21,i21,m21,tslist<t22,i22,m22,tslist<t23,i23,m23,tslist<t24,i24,m24,
		void>>>>>>>>>>>>>>>>>>>>>>>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<void,void,void>>::type type;
};

// 16, for compiling speed
template
<
	typename e1,typename e2,typename e3,typename e4,
	typename e5,typename e6,typename e7,typename e8,
	typename e9,typename e10,typename e11,typename e12,
	typename e13,typename e14,typename e15,typename e16
>
struct mkslist
<
	e1,e2,e3,e4,e5,e6,e7,e8,
	e9,e10,e11,e12,e13,e14,e15,e16,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;
	typedef typename ts::tof<e5>::t t5; typedef typename ts::sof<e5>::t i5; typedef typename ts::mof<e5>::t m5;
	typedef typename ts::tof<e6>::t t6; typedef typename ts::sof<e6>::t i6; typedef typename ts::mof<e6>::t m6;
	typedef typename ts::tof<e7>::t t7; typedef typename ts::sof<e7>::t i7; typedef typename ts::mof<e7>::t m7;
	typedef typename ts::tof<e8>::t t8; typedef typename ts::sof<e8>::t i8; typedef typename ts::mof<e8>::t m8;
	typedef typename ts::tof<e9>::t t9; typedef typename ts::sof<e9>::t i9; typedef typename ts::mof<e9>::t m9;
	typedef typename ts::tof<e10>::t t10; typedef typename ts::sof<e10>::t i10; typedef typename ts::mof<e10>::t m10;
	typedef typename ts::tof<e11>::t t11; typedef typename ts::sof<e11>::t i11; typedef typename ts::mof<e11>::t m11;
	typedef typename ts::tof<e12>::t t12; typedef typename ts::sof<e12>::t i12; typedef typename ts::mof<e12>::t m12;
	typedef typename ts::tof<e13>::t t13; typedef typename ts::sof<e13>::t i13; typedef typename ts::mof<e13>::t m13;
	typedef typename ts::tof<e14>::t t14; typedef typename ts::sof<e14>::t i14; typedef typename ts::mof<e14>::t m14;
	typedef typename ts::tof<e15>::t t15; typedef typename ts::sof<e15>::t i15; typedef typename ts::mof<e15>::t m15;
	typedef typename ts::tof<e16>::t t16; typedef typename ts::sof<e16>::t i16; typedef typename ts::mof<e16>::t m16;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,tslist<t3,i3,m3,tslist<t4,i4,m4,tslist<t5,i5,m5,tslist<t6,i6,m6,tslist<t7,i7,m7,tslist<t8,i8,m8,
		tslist<t9,i9,m9,tslist<t10,i10,m10,tslist<t11,i11,m11,tslist<t12,i12,m12,tslist<t13,i13,m13,tslist<t14,i14,m14,tslist<t15,i15,m15,tslist<t16,i16,m16,
		void>>>>>>>>>>>>>>>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<void,void,void>>::type type;
};

// 8, for compiling speed
template
<
	typename e1,typename e2,typename e3,typename e4,
	typename e5,typename e6,typename e7,typename e8
>
struct mkslist
<
	e1,e2,e3,e4,e5,e6,e7,e8,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;
	typedef typename ts::tof<e5>::t t5; typedef typename ts::sof<e5>::t i5; typedef typename ts::mof<e5>::t m5;
	typedef typename ts::tof<e6>::t t6; typedef typename ts::sof<e6>::t i6; typedef typename ts::mof<e6>::t m6;
	typedef typename ts::tof<e7>::t t7; typedef typename ts::sof<e7>::t i7; typedef typename ts::mof<e7>::t m7;
	typedef typename ts::tof<e8>::t t8; typedef typename ts::sof<e8>::t i8; typedef typename ts::mof<e8>::t m8;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,tslist<t3,i3,m3,tslist<t4,i4,m4,tslist<t5,i5,m5,tslist<t6,i6,m6,tslist<t7,i7,m7,tslist<t8,i8,m8,
		void>>>>>>>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<void,void,void>>::type type;
};

// 4, for compiling speed
template
<
	typename e1,typename e2,typename e3,typename e4
>
struct mkslist
<
	e1,e2,e3,e4,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,tslist<t3,i3,m3,tslist<t4,i4,m4,
		void>>>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<void,void,void>>::type type;
};
// 2, for compiling speed
template
<
	typename e1,typename e2
>
struct mkslist
<
	e1,e2,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;

	typedef
		tslist<t1,i1,m1,tslist<t2,i2,m2,void>>
	slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

public:
	typedef typename remove<slist_type,remove_null_type<void,void,void>>::type type;
};
// 1, for compiling speed
template
<
	typename e1
>
struct mkslist
<
	e1,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename iff<is_null<t1>::value,void,tslist<t1,i1,m1,void>>::type type;
};
// 0, for compiling speed
template
<>
struct mkslist
<
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
>
{
	typedef void type;
};

/// make slist - like type list
/// something wrong is here, let's check it later, or in the future
/// *****************************************

template
<
	typename slike_lambda_tn,
	typename e1=ox::null_type,typename e2=ox::null_type,typename e3=ox::null_type,typename e4=ox::null_type,
	typename e5=ox::null_type,typename e6=ox::null_type,typename e7=ox::null_type,typename e8=ox::null_type//,
	//typename e9=ox::null_type,typename e10=ox::null_type,typename e11=ox::null_type,typename e12=ox::null_type,
	//typename e13=ox::null_type,typename e14=ox::null_type,typename e15=ox::null_type,typename e16=ox::null_type,
	//typename e17=ox::null_type,typename e18=ox::null_type,typename e19=ox::null_type,typename e20=ox::null_type,
	//typename e21=ox::null_type,typename e22=ox::null_type,typename e23=ox::null_type,typename e24=ox::null_type,
	//typename e25=ox::null_type,typename e26=ox::null_type,typename e27=ox::null_type,typename e28=ox::null_type,
	//typename e29=ox::null_type,typename e30=ox::null_type,typename e31=ox::null_type,typename e32=ox::null_type
>
struct mkslike
{
	typedef typename lambdate<slike_lambda_tn>::type slike_lambda;
private:
	typedef typename ts::tof<e1>::t t1; typedef typename ts::sof<e1>::t i1; typedef typename ts::mof<e1>::t m1;
	typedef typename ts::tof<e2>::t t2; typedef typename ts::sof<e2>::t i2; typedef typename ts::mof<e2>::t m2;
	typedef typename ts::tof<e3>::t t3; typedef typename ts::sof<e3>::t i3; typedef typename ts::mof<e3>::t m3;
	typedef typename ts::tof<e4>::t t4; typedef typename ts::sof<e4>::t i4; typedef typename ts::mof<e4>::t m4;
	typedef typename ts::tof<e5>::t t5; typedef typename ts::sof<e5>::t i5; typedef typename ts::mof<e5>::t m5;
	typedef typename ts::tof<e6>::t t6; typedef typename ts::sof<e6>::t i6; typedef typename ts::mof<e6>::t m6;
	typedef typename ts::tof<e7>::t t7; typedef typename ts::sof<e7>::t i7; typedef typename ts::mof<e7>::t m7;
	typedef typename ts::tof<e8>::t t8; typedef typename ts::sof<e8>::t i8; typedef typename ts::mof<e8>::t m8;
	//typedef typename ts::tof<e9>::t t9; typedef typename ts::sof<e9>::t i9; typedef typename ts::mof<e9>::t m9;
	//typedef typename ts::tof<e10>::t t10; typedef typename ts::sof<e10>::t i10; typedef typename ts::mof<e10>::t m10;
	//typedef typename ts::tof<e11>::t t11; typedef typename ts::sof<e11>::t i11; typedef typename ts::mof<e11>::t m11;
	//typedef typename ts::tof<e12>::t t12; typedef typename ts::sof<e12>::t i12; typedef typename ts::mof<e12>::t m12;
	//typedef typename ts::tof<e13>::t t13; typedef typename ts::sof<e13>::t i13; typedef typename ts::mof<e13>::t m13;
	//typedef typename ts::tof<e14>::t t14; typedef typename ts::sof<e14>::t i14; typedef typename ts::mof<e14>::t m14;
	//typedef typename ts::tof<e15>::t t15; typedef typename ts::sof<e15>::t i15; typedef typename ts::mof<e15>::t m15;
	//typedef typename ts::tof<e16>::t t16; typedef typename ts::sof<e16>::t i16; typedef typename ts::mof<e16>::t m16;
	//typedef typename ts::tof<e17>::t t17; typedef typename ts::sof<e17>::t i17; typedef typename ts::mof<e17>::t m17;
	//typedef typename ts::tof<e18>::t t18; typedef typename ts::sof<e18>::t i18; typedef typename ts::mof<e18>::t m18;
	//typedef typename ts::tof<e19>::t t19; typedef typename ts::sof<e19>::t i19; typedef typename ts::mof<e19>::t m19;
	//typedef typename ts::tof<e20>::t t20; typedef typename ts::sof<e20>::t i20; typedef typename ts::mof<e20>::t m20;
	//typedef typename ts::tof<e21>::t t21; typedef typename ts::sof<e21>::t i21; typedef typename ts::mof<e21>::t m21;
	//typedef typename ts::tof<e22>::t t22; typedef typename ts::sof<e22>::t i22; typedef typename ts::mof<e22>::t m22;
	//typedef typename ts::tof<e23>::t t23; typedef typename ts::sof<e23>::t i23; typedef typename ts::mof<e23>::t m23;
	//typedef typename ts::tof<e24>::t t24; typedef typename ts::sof<e24>::t i24; typedef typename ts::mof<e24>::t m24;
	//typedef typename ts::tof<e25>::t t25; typedef typename ts::sof<e25>::t i25; typedef typename ts::mof<e25>::t m25;
	//typedef typename ts::tof<e26>::t t26; typedef typename ts::sof<e26>::t i26; typedef typename ts::mof<e26>::t m26;
	//typedef typename ts::tof<e27>::t t27; typedef typename ts::sof<e27>::t i27; typedef typename ts::mof<e27>::t m27;
	//typedef typename ts::tof<e28>::t t28; typedef typename ts::sof<e28>::t i28; typedef typename ts::mof<e28>::t m28;
	//typedef typename ts::tof<e29>::t t29; typedef typename ts::sof<e29>::t i29; typedef typename ts::mof<e29>::t m29;
	//typedef typename ts::tof<e30>::t t30; typedef typename ts::sof<e30>::t i30; typedef typename ts::mof<e30>::t m30;
	//typedef typename ts::tof<e31>::t t31; typedef typename ts::sof<e31>::t i31; typedef typename ts::mof<e31>::t m31;
	//typedef typename ts::tof<e32>::t t32; typedef typename ts::sof<e32>::t i32; typedef typename ts::mof<e32>::t m32;


	//typedef void s33;

	//typedef typename slike_lambda::apply<t32,i32,m32,s33>::type s32;
	//typedef typename slike_lambda::apply<t31,i31,m31,s32>::type s31;
	//typedef typename slike_lambda::apply<t30,i30,m30,s31>::type s30;

	//typedef typename slike_lambda::apply<t29,i29,m29,s30>::type s29;
	//typedef typename slike_lambda::apply<t28,i28,m28,s29>::type s28;
	//typedef typename slike_lambda::apply<t27,i27,m27,s28>::type s27;
	//typedef typename slike_lambda::apply<t26,i26,m26,s27>::type s26;
	//typedef typename slike_lambda::apply<t25,i25,m25,s26>::type s25;
	//typedef typename slike_lambda::apply<t24,i24,m24,s25>::type s24;
	//typedef typename slike_lambda::apply<t23,i23,m23,s24>::type s23;
	//typedef typename slike_lambda::apply<t22,i22,m22,s23>::type s22;
	//typedef typename slike_lambda::apply<t21,i21,m21,s22>::type s21;
	//typedef typename slike_lambda::apply<t20,i20,m20,s21>::type s20;

	//typedef typename slike_lambda::apply<t19,i19,m19,s20>::type s19;
	//typedef typename slike_lambda::apply<t18,i18,m18,s19>::type s18;
	//typedef typename slike_lambda::apply<t17,i17,m17,s18>::type s17;
	//typedef typename slike_lambda::apply<t16,i16,m16,s17>::type s16;
	//typedef typename slike_lambda::apply<t15,i15,m15,s16>::type s15;
	//typedef typename slike_lambda::apply<t14,i14,m14,s15>::type s14;
	//typedef typename slike_lambda::apply<t13,i13,m13,s14>::type s13;
	//typedef typename slike_lambda::apply<t12,i12,m12,s13>::type s12;
	//typedef typename slike_lambda::apply<t11,i11,m11,s12>::type s11;
	//typedef typename slike_lambda::apply<t10,i10,m10,s11>::type s10;

	//typedef typename slike_lambda::apply<t9,i9,m9,s10>::type s9;

	typedef void s9;
	typedef typename slike_lambda::apply<t8,i8,m8,s9>::type s8;
	typedef typename slike_lambda::apply<t7,i7,m7,s8>::type s7;
	typedef typename slike_lambda::apply<t6,i6,m6,s7>::type s6;
	typedef typename slike_lambda::apply<t5,i5,m5,s6>::type s5;
	typedef typename slike_lambda::apply<t4,i4,m4,s5>::type s4;
	typedef typename slike_lambda::apply<t3,i3,m3,s4>::type s3;
	typedef typename slike_lambda::apply<t2,i2,m2,s3>::type s2;
	typedef typename slike_lambda::apply<t1,i1,m1,s2>::type s1;

	typedef s1 slist_type;
	
	template <typename slist_tn,typename suffix_tn,typename prev_tested_tn>
	struct remove_null_type
	{
		static bool const value = is_null<typename slist_tn::_t_type>::value;
	};

	typedef typename relambda<slike_lambda_tn>::full slike_full_lambda;

public:
	typedef typename remove<slist_type,remove_null_type<_1,_2,_3>,slike_full_lambda>::type type;
};



/// mk slist batchly
template <typename type_tn, size_t count_tc>
struct rmkslist
{
	typedef typename
		rpush_front<void,type_tn,void,void,count_tc,tslist<void,void,void,void>>::type
	type;
};

/// make a tslist by type sequence given

template <typename func_tn>
struct mkslist_from_func
{
private:
	template <typename types_tn>
	struct reversed_slist_from_func
	{
		typedef met::infof_func<types_tn> inf;
		typedef typename inf::base base;
		typedef typename base::func_type base_func_type;
		/*
			????????????????????????????
			why donot we combine the up two lines into followed line?
			what's matter? sometimes can, sometimes not
			typedef typename inf::base::func_type base_func_type;
		*/
		typedef typename inf::end_type end_type;
		typedef typename reversed_slist_from_func<base_func_type>::type base_reversed_slist;
		typedef typename
			met::push_front<base_reversed_slist,end_type,void,void>::type
		type;
	};
	template <typename type_tn>
	struct reversed_slist_from_func <type_tn()>
	{
		typedef void type;
	};
	template <>
	struct reversed_slist_from_func <void>
	{
		typedef void type;
	};

public:
	typedef typename reversed_slist_from_func<func_tn>::type reversed_slist;
	typedef typename met::reverse<reversed_slist>::type type;
};



/*
	n : binary number
	s : suffix started
	e : suffix ended
	d : direction '+', means 1010 ==> <T,F,T,F>; '-' ==> <F,T,F,T>
	_0_t: when bit is 0, the type, orelse 1 type
 */
template <size_t n, size_t s, size_t e, typename _1_tn, typename _0_tn>
struct mkslist_from_2x
{
private:
	static size_t const __max_bits = sizeof(size_t)*8;
	static char const __d = (s==e)?'0':(s<e?'+':'-');
	static size_t const __s = __d=='+'?s:e;
	static size_t const __e = __d=='+'?e:s;
	static size_t const _ss = (__s+1)>=__max_bits?__max_bits-1:__s;
	static size_t const _ee = (__e+1)>=__max_bits?__max_bits-1:__e;

	static size_t const __c = (__s+1)>=__max_bits?0:_ee-_ss+1;
	static size_t const __n = n>>_ss;

	/// c : count , how many bits will be done with ?
	template <size_t _n, size_t _c, char _d, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside
	{
		typedef void type;
	};
	template <size_t _n, size_t _c, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,_c,'-',_1_t,_0_t>
	{
		static bool const __bool0 = !!(_n&(1<<(_c-1)));
		typedef typename met::iff<__bool0,_1_t,_0_t>::type bit0_type;
		typedef typename mkslist_from_2x_inside<_n,_c-1,'-',_1_t,_0_t>::type next_type;
		typedef tslist<bit0_type,void,void,next_type> type;
	};
	template <size_t _n, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,0,'-',_1_t,_0_t>
	{
		typedef void type;
	};
	/// c : count , how many bits will be done with ?
	template <size_t _n, size_t _c, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,_c,'+',_1_t,_0_t>
	{
		static bool const __bool0 = !!(_n&1);
		typedef typename met::iff<__bool0,_1_t,_0_t>::type bit0_type;
		typedef typename mkslist_from_2x_inside<(_n>>1),_c-1,'+',_1_t,_0_t>::type next_type;
		typedef tslist<bit0_type,void,void,next_type> type;
	};
	template <size_t _n, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,0,'+',_1_t,_0_t>
	{
		typedef void type;
	};
	/// c : ==1 , how many bits will be done with ?
	template <size_t _n, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,1,'0',_1_t,_0_t>
	{
		static bool const __bool0 = !!(_n&1);
		typedef typename met::iff<__bool0,_1_t,_0_t>::type bit0_type;
		typedef tslist<bit0_type,void,void,void> type;
	};
	/// if the Count is zero, we ignore the direction
	template <size_t _n, char _d, typename _1_t, typename _0_t>
	struct mkslist_from_2x_inside <_n,0,_d,_1_t,_0_t>
	{
		typedef void type;
	};


public:
	typedef typename mkslist_from_2x_inside<__n,__c,__d,_1_tn,_0_tn>::type type;
};

/// remove or others, okay, but at present, it is enough, add it, when necessary


/**
	生成n个重复类型的slist_type
*/
template <typename type_tn, size_t count_tn=1>
struct slist_repeat
{
	typedef typename slist_repeat<type_tn,count_tn-1>::type slist_next_type;
	typedef tslist<type_tn,void,void,slist_next_type> type;
};

template <typename type_tn>
struct slist_repeat <type_tn,0>
{
	typedef void type;
};

template <typename type_tn>
struct slist_repeat <type_tn,1>
{
	typedef tslist<type_tn,void,void,void> type;
};



___namespace2_end()
