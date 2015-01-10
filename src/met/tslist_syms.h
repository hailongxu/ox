



/**
	作者：徐海龙
	2010-11-10
	这个主要是为了对米一个类型除了可以用suffix来标识外，还可以用sym赖标识，会有更多的用途
*/

#include "../ox/tclasses.h"
#include "_if_switch.h"
#include "static_assert.h"
#include "mod.h"
#include "lambda.h"
#include "tslist_decl.h"
#include "tslist.h"



#pragma once

#include "../mix/mixture_feature_def.h"


___namespace2_begin(ox,met)



/*
	get the idtype from etype of tslist
 */

namespace __pri_byid
{
	template <typename slike_tn>
	struct always_select
	{
		static bool const value = true;
	};


	template <typename slist_tn, typename addr_tn>
	struct is_matched;

	template <typename slist_tn,typename type_tn,size_t off,size_t size>
	struct is_matched <slist_tn,taddr<type_tn,off,size>>
	{
		typedef typename slist_tn::_t_type _t_type;
		/// out
		static bool const value = anyxindexof<_t_type,type_tn>::value >= 0;
	};

	/// label
	template <typename slist_tn,typename type_tn,size_t off,size_t size>
	struct is_matched <slist_tn,saddr<type_tn,off,size>>
	{
		typedef typename slist_tn::_t_syms _t_syms;
		/// out
		static bool const value = anyxindexof<_t_syms,type_tn>::value >= 0;
	};

	template <typename slist_tn,size_t off,size_t size>
	struct is_matched <slist_tn,index<off,size>>
	{
		/// out
		static bool const value = true;
	};


	typedef lambda<is_matched<_1,_2>> is_matched_lambda;
	typedef lambda<always_select<_1>> always_selected_lambda;


	template <typename feat,typename sym>
	struct ishit_syms
	{
		static bool const value = is_same<typename feat::_t_syms,sym>::value;
	};


	/// for tulist
	template <typename slist_tn,typename addr_tn,typename preselfun=ox::met::lambda<always_select<ox::_1>>>
	struct is_matched_tulist;

	template <typename slist_tn,typename type_tn,size_t off,size_t size,typename preselfun>
	struct is_matched_tulist <slist_tn,taddr<type_tn,off,size>,preselfun>
	{
		typedef typename slist_tn::_t_type _t_type;
		/// out
		static bool const value = anyxindexof_tulist<_t_type,type_tn,preselfun>::value >= 0;
	};

	/// label, we compare <_t_syms::_t_syms> with <saddr>
	template <typename slist_tn,typename type_tn,size_t off,size_t size,typename preselfun>
	struct is_matched_tulist <slist_tn,saddr<type_tn,off,size>,preselfun>
	{
		typedef typename slist_tn::_t_syms _t_syms;
		/// out
		//static bool const value = anyxindexof_tulist<_t_syms,type_tn,preselfun>::value >= 0;
		static bool const value = indexof_feature_if<_t_syms,lambda<ishit_syms<ox::_1,type_tn>>,preselfun>::value >= 0;
		//___static_assert(!(is_same<typename slist_tn::_t_type,char>::value) || _t_syms::idiff::value==0);
	};

	template <typename slist_tn,size_t off,size_t size,typename preselfun>
	struct is_matched_tulist <slist_tn,index<off,size>,preselfun>
	{
		/// out
		static bool const value = true;
	};

	template <typename slitem>
	struct data_symbol_selected
	{
		static bool const value = slitem::idiff::value == 1;
	};

	typedef lambda<is_matched_tulist<_1,_2>> is_matched_tulist_lambda;
	typedef lambda<is_matched_tulist<_1,_2,ox::met::lambda<data_symbol_selected<ox::_1>>>> is_matched_data_tulist_lambda;



	template <typename addr_tn>
	struct is_label
	{
		static bool const value = false;
	};
	template <typename type_tn,size_t off,size_t to>
	struct is_label <saddr<type_tn,off,to>>
	{
		static bool const value = true;
	};

	template <typename saddr_tn>
	struct saddr_to_taddr
	{
		typedef taddr
			<
				typename saddr_tn::type,
				saddr_tn::offset,
				saddr_tn::size
			>
		type;
	};
}


/**
	sat = single-element at of slist
	slist_tn: type list
	sym_tn: the sym indentified a type
	suffix_tc: from where
	idof_tn: get idtype from etype of tslist
	-------
	return:
	owner: the current mixture of the hit type
	type: the current type of the hit element
	idoff: the sym offset number under the condition
	suffix: the suffix hit from zero of the whole slist
*/
namespace __private_sat
{
	template
	<
		typename slist_tn,
		typename someid_tn,
		size_t idoff=0,
		int suffix_tc=0
	>
	struct sat_of_slist
	{
		typedef void owner;
		typedef void type;
		typedef itot<-1> idoff;
		typedef itot<-1> suffix;
	};

	template 
	<
		typename type_tn,typename ids_tn,typename meta_tn,typename next_tn,
		typename someid_tn,size_t idoff,
		int suffix_tc
	>
	struct sat_of_slist<tslist<type_tn,ids_tn,meta_tn,next_tn>,someid_tn,idoff,suffix_tc>
	{
		template <typename o,int _idoff,int i>
		struct result_tt
		{
			typedef o owner;
			typedef typename owner::_t_type type;
			typedef itot<_idoff> idoff;
			typedef itot<i> suffix;
		};

		static bool const __is_hit = anyxindexof<ids_tn,someid_tn>::value >= 0;

		static size_t const __idoff_next = __is_hit?(idoff-1):idoff;
		static size_t const __suffix_next = suffix_tc+1;

		typedef typename
			iff
			<
				__is_hit && idoff==0,
				result_tt<tslist<type_tn,ids_tn,meta_tn,next_tn>,idoff,suffix_tc>,
				sat_of_slist<next_tn,someid_tn,__idoff_next,__suffix_next>
			>
			::type
		result_type;

	public:
		typedef typename result_type::owner owner;
		typedef typename result_type::type type;
		typedef typename result_type::idoff idoff;
		typedef typename result_type::suffix suffix;
	};
}

template
<
	typename slist_tn,typename someid_tn,size_t idoff=0
>
struct sat_of_slist
{
	typedef __private_sat::sat_of_slist<slist_tn,someid_tn,idoff,0> result_type;
	/// return
	typedef typename result_type::owner owner;
	typedef typename result_type::type type;
	typedef typename result_type::idoff idoff;
	typedef typename result_type::suffix suffix;
};


/// find the element which has not sym property
template
<
	typename slist_tn,size_t idoff=0
>
struct snat_of_slist
{
	typedef sat_of_slist<slist_tn,void,idoff> result;
	/// return
	typedef typename result::owner owner;
	typedef typename result::type type;
	typedef typename result::idoff idoff;
	typedef typename result::suffix suffix;
};


/**
	mat: multi-element at of slist
	return a set of element whose sym is equal
*/

namespace __private_rat
{
	template <typename o,size_t idoffset,size_t i>
	struct inf_ret_of_mat
	{
		typedef o owner;
		typedef typename o::_t_type type;
		typedef itot<idoffset> idoff;
		typedef itot<i> suffix;
	};

	template <typename type_tn,size_t idoff>
	struct reversed_tt
	{
		static size_t const offset_next = idoff;
		typedef type_tn reversed_type;
	};


	/**
		result is a list of a range of hit elements
		f: from the suffix based on 0 of the hit range list
		t: to the suffix based on 0 of the hit range list
	*/
	template
	<
		typename slist_tn,typename someid_tn,size_t f,size_t t=-1,
		size_t idoff=0,size_t suffix_tc=0,
		typename last_result=void
	>
	struct rat_of_slist
	{
		typedef last_result reversed_type;
		static size_t const offset_next = idoff;
	};

	template
	<
		typename type_tn,typename ids_tn, typename meta_tn,typename next_tn,typename someid_tn,size_t f,size_t t,
		size_t idoff,size_t suffix_tc,typename last_result
	>
	struct rat_of_slist<tslist<type_tn,ids_tn,meta_tn,next_tn>,someid_tn,f,t,idoff,suffix_tc,last_result>
	{
		___static_assert(f<=t);

		static bool const __is_hit = anyxindexof<ids_tn,someid_tn>::value >= 0;

		typedef
			__private_rat::inf_ret_of_mat
			<
				tslist<type_tn,ids_tn,meta_tn,next_tn>, idoff, suffix_tc
			>
		inf;

		/// get the current result
		typedef typename
			iff
			<
				__is_hit && idoff>=f && idoff<=t,
				typename push_front<last_result,inf>::type,
				last_result
			>
			::type
		curr_result_reversed_type;

		static size_t const __idoff_next = __is_hit?(idoff+1):idoff;
		static size_t const __suffix_next = suffix_tc+1;
		typedef typename
			iff
			<
				__is_hit && idoff>=t,
				__private_rat::reversed_tt<curr_result_reversed_type,__idoff_next>,
				rat_of_slist<next_tn,someid_tn,f,t,__idoff_next,__suffix_next,curr_result_reversed_type>
			>
			::type
		next_result;

		static size_t const offset_next = next_result::offset_next;
		typedef typename next_result::reversed_type reversed_type;
		
	};
}


/**
	t = -1: to the end of the hit range
*/

template
<
	typename slist_tn,typename sym_tn,size_t f,size_t t=-1
>
struct rat_of_slist
{
	typedef typename
		__private_rat::rat_of_slist<slist_tn,sym_tn,f,t,0,0,void>
	execute;

	/// revert again
	static size_t const size = execute::offset_next-f;
	typedef typename reverse<typename execute::reversed_type>::type type;
};





//// list like

namespace syms_space
{
	/// construct param struct for handle
	template <bool _is_continue_tc, size_t _offset, size_t _suffix_tc, size_t _suffix_all_tc>
	struct params_of_handle
	{
		/// necessary member __is_continue
		static bool const __is_continue = _is_continue_tc;
		/// other's info
		static size_t const offset = _offset;
		static size_t const suffix = _suffix_tc; /// in selected
		static size_t const suffix_all = _suffix_all_tc; /// all the range
	};


	struct always_false
	{
		template <typename,typename>
		struct call
		{
			static bool const value = false;
		};
	};

	/// construct handle struct
	template <typename _slist_tn,typename suffix_tn,typename params_tn,typename addr_tn,typename presel_lmd,typename ishit_lmd>
	struct handle_of_sat
	{
		static bool const __is_selected = presel_lmd::call<_slist_tn>::value;
		static bool const __is_hit =
			iff <__is_selected,ishit_lmd,always_false>
				::type::call<_slist_tn,addr_tn>::value;
		/// when it is not selected, __is_hit is always false;
		/// __offset_next: down from high until 0

		static bool const __is_continue = !(__is_hit && params_tn::offset==0);
		static size_t const __offset_next = __is_hit?params_tn::offset-1:params_tn::offset;
		static size_t const __suffix_next = params_tn::suffix + (__is_selected?1:0);
		static size_t const __suffix_all_next = params_tn::suffix_all+1;
		/// handle result
		typedef syms_space::params_of_handle<__is_continue,__offset_next,__suffix_next,__suffix_all_next> type;
	};
}

/// sat of tslist [0..n]
template
<
	typename slist_tn,
	typename addr_tn,
	typename presel_lmd = __pri_byid::always_selected_lambda,
	typename ishit_lmd = __pri_byid::is_matched_lambda,
	typename nextslist_tn = next_of_slist<_1>
>
struct sat_of_range
{
private:
	/// start the action of scan
	typedef typename
		scan
		<
			slist_tn,
			syms_space::handle_of_sat<_1,_2,_3,addr_tn,presel_lmd,ishit_lmd>,
			syms_space::params_of_handle<true,addr_tn::offset,0,0>,
			itot<0>,
			nextslist_tn
		>
	_scan;
	typedef typename _scan::type _range;

	/// return the result of current type;
	/// the type will be used on finding nothing
	struct void_type { typedef void _t_type; };

public:
	typedef _range owner;
	typedef typename iff<is_void<owner>::value,void_type,owner>::type::_t_type type;
	typedef itot<_scan::result::offset> offset; /// from hight to low
	typedef itot<_scan::result::suffix> suffix; /// just in the set selected
	typedef typename _scan::suffix suffix_all; /// in all the range
};


/// can accept both the tslist and range type
template <typename slist_like_tn,typename addr_tn>
struct sat
{
private:
		//met::iff
		//<
			//is_slist<slist_like_tn>::value,
			//sat_of_slist<slist_like_tn,sym_tn,idoff,findid_tn>,
		//>
		//::type

	/// add the judgement of whether is slist is for the compling speed
	typedef
		sat_of_range
		<
			slist_like_tn,
			addr_tn,
			__pri_byid::always_selected_lambda,
			__pri_byid::is_matched_lambda,
			next_of_slist<_1>
		>
	result;
public:
	typedef typename result::owner owner;
	typedef typename result::type type;
	typedef typename result::offset offset;
	typedef typename result::suffix suffix;
	typedef typename result::suffix_all suffix_all;
};


/// can accept both the tslist and range type
/// the _t_syms will be considered as 2nd level
template
<
	typename slist_like_tn,
	typename addr_tn,
	typename presel_lmd = __pri_byid::always_selected_lambda
>
struct anysat
{
private:
		//met::iff
		//<
			//is_slist<slist_like_tn>::value,
			//sat_of_slist<slist_like_tn,sym_tn,idoff,findid_tn>,
		//>
		//::type

	/// add the judgement of whether is slist is for the compling speed
	typedef
		sat_of_range
		<
			slist_like_tn,
			addr_tn,
			presel_lmd,
			__pri_byid::is_matched_lambda,
			next_of_slist<_1>
		>
	result0;
	typedef typename result0::owner owner0;
	struct void_syms { typedef void _t_syms; };

private:
	/// keep finding if addr is a lebel
	/// and when the hited label is a list
	typedef typename __pri_byid::is_label<addr_tn> is_label;
	typedef typename iff<is_void<owner0>::value,void_syms,owner0>::type::_t_syms _t_syms; /// like box<i,n>
	//typedef typename __pri_byid::saddr_to_taddr<addr_tn>::type taddr2;

	typedef typename
		iff
		<
			/// when addr is a label address and the _t_syms is a list
			/// we contine to find the method result
			is_label::value && lambda<_t_syms>::is::value,
			sat_of_range
			<
				_t_syms,
				addr_tn/*taddr2*/,
				presel_lmd,
				__pri_byid::is_matched_lambda,
				next_of_slist<_1>
			>,
			result0
		>
		::type
	result;
	

public:
	typedef typename result::owner owner;
	typedef typename result::type type;
	typedef typename result::offset offset;
	typedef typename result::suffix suffix;
	typedef typename result::suffix_all suffix_all;
};




template
<
	typename slist_tn,
	typename addr_tn,
	typename presel_lmd = __pri_byid::always_selected_lambda,
	typename ishit_lambda = __pri_byid::is_matched_tulist_lambda,
	typename nextslist_tn = next_of_slist<_1>
>
struct sat_of_mixture_range
	: sat_of_range<slist_tn,addr_tn,presel_lmd,ishit_lambda,nextslist_tn>
{};


___namespace2_end()
