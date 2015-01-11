


/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	mixture功能还要要进行外化，由外部函数进行统一操作，不要绑定到mixture里面去
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "../met/tslist.h"
#include "../met/mod.h"
#include "../met/_if_switch.h"
#include "../met/lambda.h"
#include "./mixture_unitize.h"
#include "./mixture_decl.h"
#include "./mixture_feature_def.h"


#pragma once


___namespace2_begin(ox,mix)


///// t1...t32, a type sequence, they can may be slist or type
//template
//<
//	typename t1=ox::null_type,typename t2=ox::null_type,typename t3=ox::null_type,typename t4=ox::null_type,
//	typename t5=ox::null_type,typename t6=ox::null_type,typename t7=ox::null_type,typename t8=ox::null_type,
//	typename t9=ox::null_type,typename t10=ox::null_type,typename t11=ox::null_type,typename t12=ox::null_type,
//	typename t13=ox::null_type,typename t14=ox::null_type,typename t15=ox::null_type,typename t16=ox::null_type,
//	typename t17=ox::null_type,typename t18=ox::null_type,typename t19=ox::null_type,typename t20=ox::null_type,
//	typename t21=ox::null_type,typename t22=ox::null_type,typename t23=ox::null_type,typename t24=ox::null_type,
//	typename t25=ox::null_type,typename t26=ox::null_type,typename t27=ox::null_type,typename t28=ox::null_type,
//	typename t29=ox::null_type,typename t30=ox::null_type,typename t31=ox::null_type,typename t32=ox::null_type
//>
//struct mktul_old
//{
//private:
//	typedef typename
//		met::mkslist
//		<
//			t1,t2,t3,t4,t5,t6,t7,t8,
//			t9,t10,t11,t12,t13,t14,t15,t16,
//			t17,t18,t19,t20,t21,t22,t23,t24,
//			t25,t26,t27,t28,t29,t30,t31,t32
//		>
//		::type
//	slist_of_slist;
//
//
//	template <typename _type_tn,typename _index_tn>
//	struct test_not_null
//	{
//		static bool const value = !___is_null(_type_tn);
//	};
//
//	typedef typename
//		met::linkall<slist_of_slist,test_not_null<ox::_1,ox::_2>>::type
//	slist_all;
//
//	typedef void metas_type;
//	typedef met::lambda<instantiate_of_type<ox::_1,ox::_2,ox::_3,ox::_4/*,ox::_5*/>> lambda_unitize;
//public:
//	typedef mixture<slist_all,metas_type> type;
//};
//
//
//// 1, for compiling speed
//template
//<
//	typename t, typename n
//>
//struct mktul_old
//<
//	tslist<t,n>,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
//	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
//	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,
//	ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type,ox::null_type
//>
//{
//private:
//	typedef met::lambda<instantiate_of_type<ox::_1,ox::_2,ox::_3,ox::_4/*,ox::_5*/>> unitize;
//public:
//	typedef mixture<tslist<t,n>,void> type;
//};


///// make mixture from type seqence
//template <typename type_sequence_tn,typename unit_tn=instantiate_of_type<ox::_1>>
//struct mktul
//{
//private:
//	typedef typename
//		met::iff
//		<
//			met::is_fun<type_sequence_tn>::value,
//			met::mkslist_from_func<type_sequence_tn>,
//			identity<type_sequence_tn>
//		>
//		::type::type
//	slist;
//public:
//	typedef mixture<slist,met::lambda<unit_tn>> type;
//};

///// make mixture from alias type
//template <typename mode_tn,typename sl,typename value_slist_tn,typename unit_tn>
//struct mktul <alias<mode_tn,sl,value_slist_tn>,unit_tn>
//{
//	typedef mixture<sl,met::lambda<unit_tn>> type;
//};

/// make const mixture whose every element is readonly
template <typename type_sequence_tn,typename unit_tn=instantiate_of_type<ox::_1>>
struct make_mixture_const
{
private:
	typedef typename
		met::iff
		<
			met::is_fun<type_sequence_tn>::value,
			met::mkslist_from_func<type_sequence_tn>,
			identity<type_sequence_tn>
		>
		::type::type
	slist;

	typedef typename met::modify<slist,met::add_con<void>>::type slist_const;
public:
	typedef mixture<slist_const,met::lambda<unit_tn>> type;
};



template <typename t1,typename t2=ox::null_type,typename t3=ox::null_type>
struct make_mixture
{
	typedef typename ox::mix::tulist<t1,t2,t3>::type typelist;
	typedef mixture<typelist> type;
};

/// specialization for array
template <typename sl,size_t n>
struct make_mixture <sl[n],ox::null_type,ox::null_type>
{
	typedef mixture<sl[n],void,void,ox::tags::static_array> type;
};


___namespace2_end()


	//typedef typename insert_front_backward<void,t1>::type back_t1;
	//typedef typename insert_front_backward<back_t1,t2>::type back_t2_t1;
	//typedef typename insert_front_backward<back_t2_t1,t3>::type back_t3_t1;
	//typedef typename insert_front_backward<back_t3_t1,t4>::type back_t4_t1;
	//typedef typename insert_front_backward<back_t4_t1,t5>::type back_t5_t1;
	//typedef typename insert_front_backward<back_t5_t1,t6>::type back_t6_t1;
	//typedef typename insert_front_backward<back_t6_t1,t7>::type back_t7_t1;
	//typedef typename insert_front_backward<back_t7_t1,t8>::type back_t8_t1;
	//typedef typename insert_front_backward<back_t8_t1,t9>::type back_t9_t1;
	//typedef typename insert_front_backward<back_t9_t1,t10>::type back_t10_t1;
	//typedef typename insert_front_backward<back_t10_t1,t11>::type back_t11_t1;
	//typedef typename insert_front_backward<back_t11_t1,t12>::type back_t12_t1;
	//typedef typename insert_front_backward<back_t12_t1,t13>::type back_t13_t1;
	//typedef typename insert_front_backward<back_t13_t1,t14>::type back_t14_t1;
	//typedef typename insert_front_backward<back_t14_t1,t15>::type back_t15_t1;
	//typedef typename insert_front_backward<back_t15_t1,t16>::type back_t16_t1;
	//typedef typename insert_front_backward<back_t16_t1,t17>::type back_t17_t1;
	//typedef typename insert_front_backward<back_t17_t1,t18>::type back_t18_t1;
	//typedef typename insert_front_backward<back_t18_t1,t19>::type back_t19_t1;
	//typedef typename insert_front_backward<back_t19_t1,t20>::type back_t20_t1;
	//typedef typename insert_front_backward<back_t20_t1,t21>::type back_t21_t1;
	//typedef typename insert_front_backward<back_t21_t1,t22>::type back_t22_t1;
	//typedef typename insert_front_backward<back_t22_t1,t23>::type back_t23_t1;
	//typedef typename insert_front_backward<back_t23_t1,t24>::type back_t24_t1;
	//typedef typename insert_front_backward<back_t24_t1,t25>::type back_t25_t1;
	//typedef typename insert_front_backward<back_t25_t1,t26>::type back_t26_t1;
	//typedef typename insert_front_backward<back_t26_t1,t27>::type back_t27_t1;
	//typedef typename insert_front_backward<back_t27_t1,t28>::type back_t28_t1;
	//typedef typename insert_front_backward<back_t28_t1,t29>::type back_t29_t1;
	//typedef typename insert_front_backward<back_t29_t1,t30>::type back_t30_t1;
	//typedef typename insert_front_backward<back_t30_t1,t31>::type back_t31_t1;
	//typedef typename insert_front_backward<back_t31_t1,t32>::type back_t32_t1;
