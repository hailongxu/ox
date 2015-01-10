
/**
	对类型函数包装成lambda
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"


#pragma once




___namespace1_begin(ox)



template <size_t xth>
struct arg;

template <>
struct arg <1>
{
	//template <typename> apply;
	//template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	//struct apply < tslist<t1,tslist<t2,tslist<t3,tslist<t4,tslist<t5,tslist<t6,tslist<t7,tslist<t8,void>>>>>>>> >
	static size_t const value = 1;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t1 type;
	};
};
template <>
struct arg <2>
{
	static size_t const value = 2;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t2 type;
	};
};
template <>
struct arg <3>
{
	static size_t const value = 3;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t3 type;
	};
};
template <>
struct arg <4>
{
	static size_t const value = 4;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t4 type;
	};
};
template <>
struct arg <5>
{
	static size_t const value = 5;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t5 type;
	};
};
template <>
struct arg <6>
{
	static size_t const value = 6;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t6 type;
	};
};
template <>
struct arg <7>
{
	static size_t const value = 7;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t7 type;
	};
};
template <>
struct arg <8>
{
	static size_t const value = 8;
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct apply
	{
		typedef t8 type;
	};
};


typedef arg<1> _1;
typedef arg<2> _2;
typedef arg<3> _3;
typedef arg<4> _4;
typedef arg<5> _5;
typedef arg<6> _6;
typedef arg<7> _7;
typedef arg<8> _8;




___namespace1_end()






___namespace2_begin(ox,met)



template <typename default_param>
struct arg_func
{
	/// if the argfunc is argumen we apply, else use the input from user
	template
	<
		typename argfunc, 
		typename p1=default_param,
		typename p2=default_param,
		typename p3=default_param,
		typename p4=default_param,
		typename p5=default_param,
		typename p6=default_param,
		typename p7=default_param,
		typename p8=default_param
	>
	struct args
	{
		typedef argfunc type;
	};
	template <size_t nth, typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct args <ox::arg<nth>,t1,t2,t3,t4,t5,t6,t7,t8>
	{
		typedef typename ox::arg<nth>::apply<t1,t2,t3,t4,t5,t6,t7,t8>::type type;
	};
};



template <typename t>
struct is_arg_type
{
	static bool const value = false;
};
template <size_t xth>
struct is_arg_type <ox::arg<xth>>
{
	static bool const value = true;
};


___namespace2_end()
