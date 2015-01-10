/**
	元选择语法
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"



#pragma once



___namespace2_begin(ox,met)

/**
@brief 编译期的类型选择定义
*/

//// if 控制语法，选出类型为type
template <bool is_true, typename true_tn,typename false_tn>
struct iff
{
	typedef true_tn type;
};

template <typename true_tn,typename false_tn>
struct iff <false,true_tn,false_tn>
{
	typedef false_tn type;
};


	//typedef
	//	_if
	//	<
	//	_elseif<false,int>,
	//	_elseif<true,char>
	//	>
	//	::type
	//type;
//typedef typename iff<a,b>::orif<c,d>::or<d>::type abc;
//typedef typename iff<a,b>::orif<c,d>::or<d>type abc;
//typedef typename iff<a,b>::or<c>::type abc;

//template <bool b,typename t>
//struct ifff
//{
//	template <bool _b,typename _t>
//	struct or
//	{
//		typedef typename ifff<_b,_t>::type iff;
//	};
//};
//iff<a,b>::or<c,d>::iff

/*
	<a,b>
	a ==> theother is b
	b ==> theother is a
 */

//template <typename type_tn, typename one_tn, typename other_tn>
//struct theother;
//
//template <typename one_tn, typename other_tn>
//struct theother <one_tn,one_tn,other_tn>
//{
//	typedef other_tn type;
//	typedef one_tn extype;
//	struct index { static size_t const value = 1; };
//	struct exindex { static size_t const value = 0; };
//};
//template <typename one_tn, typename other_tn>
//struct theother <other_tn,one_tn,other_tn>
//{
//	typedef one_tn type;
//	typedef other_tn extype;
//	struct index { static size_t const value = 0; };
//	struct exindex { static size_t const value = 1; };
//};



///
/// switch
///

/**
	@brief 编译期的类型选择定义
*/


template <bool b,typename t> struct _case
{
	static bool const value = b;
	typedef t type;
};
//template <size_t i,typename t> struct _case<ox::itot<i>,t>
//{
//	typedef ox::itot<i> cond;
//	typedef t type;
//};
//template <size_t i,size_t j,typename t> struct _case<ox::i2tot<i,j>,t>
//{
//	typedef ox::i2tot<i,j> cond;
//	typedef t type;
//};
//template <bool b,typename t> struct _case<ox::boolean<b>,t>
//{
//	typedef ox::boolean<b> cond;
//	typedef t type;
//};


template <typename c0=void,typename c1=void,typename c2=void,typename c3=void,typename c4=void,typename c5=void,typename c6=void,typename c7=void,typename c8=void>
struct _switch;


template <typename t0,typename c1,typename c2,typename c3,typename c4,typename c5,typename c6,typename c7,typename c8>
struct _switch <_case<false,t0>,c1,c2,c3,c4,c5,c6,c7,c8>
{
	typedef typename
		_switch <c1,c2,c3,c4,c5,c6,c7,c8> ::type
	type;
};
template <typename t0,typename c1,typename c2,typename c3,typename c4,typename c5,typename c6,typename c7,typename c8>
struct _switch <_case<true,t0>,c1,c2,c3,c4,c5,c6,c7,c8>
{
	typedef t0 type;
};

//template <bool i0,typename t0,typename c1,typename c2,typename c3,typename c4,typename c5,typename c6,typename c7,typename c8>
//struct _switch <_case<i0,t0>,c1,c2,c3,c4,c5,c6,c7,c8>
//{
//	typedef typename
//		iff
//		<
//			i>=f && i<=t,
//			ox::identity<t1>,
//			_switch<_case<t0,i>,c2,c3,c4,c5,c6,c7,c8>
//		>
//		::type::type
//	type;
//};
//template <size_t i,size_t j,typename t0,typename c2,typename c3,typename c4,typename c5,typename c6,typename c7,typename c8>
//struct _switch <_case<t0,i,j>,void,c2,c3,c4,c5,c6,c7,c8>
//{
//	typedef t0 type;
//};





___namespace2_end()