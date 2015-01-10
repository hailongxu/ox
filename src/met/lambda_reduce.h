
/**
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"
#include "./mod.h"
#include "./_if_switch.h"

#pragma once

___namespace2_begin(ox,met)

/*
	type lambda_reduce function, F(x) = f(g(x))
	f(x) := struct f <T>
	g(x) := struct g <T>
	F(x) := struct f <g<T>>

	preconditions:
	f<>:: has call<>
	g<>:: has call<>
	F<>:: has call<> too
 */

template
<
	typename lambda_tn,
	typename lambda_pram1_tn = void,
	typename lambda_pram2_tn = void,
	typename lambda_pram3_tn = void,
	typename lambda_pram4_tn = void,
	typename lambda_pram5_tn = void,
	typename lambda_pram6_tn = void
>
struct lambda_reduce;

/*
	f(x), g(x)
	F(x) = f(g(x))
 */
template
<
	typename lambda_tn,
	typename lambda_pram1_tn
>
struct lambda_reduce <lambda_tn, lambda_pram1_tn, void, void, void, void, void>
{
	/// stand func itself
	template <typename param1_tn>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type
		>
	{};
};

/*
	f(x,y), u(x), v(x)
	two params F(x,y) = f(u(x),v(y))
 */
template
<
	typename lambda_tn,
	typename lambda_pram1_tn,
	typename lambda_pram2_tn
>
struct lambda_reduce <lambda_tn, lambda_pram1_tn, lambda_pram2_tn, void, void, void, void>
{
	/// stand for func itself
	template <typename param1_tn, typename param2_tn>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type,
			typename lambda_pram2_tn::template call<param2_tn>::type
		>
	{};
};

/*
	f(x,y,z), u(x), v(x), w(x)
	two params F(x,y,z) = f(u(x),v(y),w(z))
 */
template
<
	typename lambda_tn,
	typename lambda_pram1_tn,
	typename lambda_pram2_tn,
	typename lambda_pram3_tn
>
struct lambda_reduce <lambda_tn, lambda_pram1_tn, lambda_pram2_tn, lambda_pram3_tn, void, void, void>
{
	template <typename param1_tn, typename param2_tn, typename param3_tn>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type,
			typename lambda_pram2_tn::template call<param2_tn>::type,
			typename lambda_pram3_tn::template call<param3_tn>::type
		>
	{};
};


/*
	f(x1,x2,x3,x4), g1(x), g2(x), g3(x), g4(x)
	F(x1,x2,x3,x4) = f(g1(x1),g2(x2),g3(x3),g4(x4))
 */

template
<
	typename lambda_tn,
	typename lambda_pram1_tn,
	typename lambda_pram2_tn,
	typename lambda_pram3_tn,
	typename lambda_pram4_tn
>
struct lambda_reduce <lambda_tn, lambda_pram1_tn, lambda_pram2_tn, lambda_pram3_tn, lambda_pram4_tn, void, void>
{
	template <typename param1_tn, typename param2_tn, typename param3_tn, typename param4_tn>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type,
			typename lambda_pram2_tn::template call<param2_tn>::type,
			typename lambda_pram3_tn::template call<param3_tn>::type,
			typename lambda_pram4_tn::template call<param4_tn>::type
		>
	{};
};

/*
	f(x1,x2,x3,x4,x5), g1(x), g2(x), g3(x), g4(x), g4(x)
	F(x1,x2,x3,x4,x5) = f(g1(x1),g2(x2),g3(x3),g4(x4),g5(x5))
 */

template
<
	typename lambda_tn,
	typename lambda_pram1_tn,
	typename lambda_pram2_tn,
	typename lambda_pram3_tn,
	typename lambda_pram4_tn,
	typename lambda_pram5_tn
>
struct lambda_reduce <lambda_tn, lambda_pram1_tn, lambda_pram2_tn, lambda_pram3_tn, lambda_pram4_tn, lambda_pram5_tn, void>
{
	template
	<
		typename param1_tn, typename param2_tn, typename param3_tn,
		typename param4_tn, typename param5_tn
	>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type,
			typename lambda_pram2_tn::template call<param2_tn>::type,
			typename lambda_pram3_tn::template call<param3_tn>::type,
			typename lambda_pram4_tn::template call<param4_tn>::type,
			typename lambda_pram5_tn::template call<param5_tn>::type
		>
	{};
};


/*
	f(x1,x2,x3,x4,x5,x6), g1(x), g2(x), g3(x), g4(x), g5(x), g6(x)
	F(x1,x2,x3,x4,x5,x6) = f(g1(x1),g2(x2),g3(x3),g4(x4),g5(x5),g6(x6))
 */

template
<
	typename lambda_tn,
	typename lambda_pram1_tn,
	typename lambda_pram2_tn,
	typename lambda_pram3_tn,
	typename lambda_pram4_tn,
	typename lambda_pram5_tn,
	typename lambda_pram6_tn
>
struct lambda_reduce //<lambda_tn, lambda_pram1_tn, lambda_pram2_tn, lambda_pram3_tn, lambda_pram4_tn, lambda_pram5_tn, lambda_pram6_tn>
{
	template
	<
		typename param1_tn, typename param2_tn, typename param3_tn,
		typename param4_tn, typename param5_tn, typename param6_tn
	>
	struct call
		:
		lambda_tn::template call
		<
			typename lambda_pram1_tn::template call<param1_tn>::type,
			typename lambda_pram2_tn::template call<param2_tn>::type,
			typename lambda_pram3_tn::template call<param3_tn>::type,
			typename lambda_pram4_tn::template call<param4_tn>::type,
			typename lambda_pram5_tn::template call<param5_tn>::type,
			typename lambda_pram6_tn::template call<param6_tn>::type
		>
	{};
};


___namespace2_end()