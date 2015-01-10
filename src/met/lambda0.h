
/**
	对类型函数包装成lambda
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"
#include "./lambda_decl.h"

/// for relambda
#include "./lambda_argument.h"


#pragma once


___namespace2_begin(ox,met)


/*
	lambda function declaration, 
	this will convert a common function into a lambda function
	f(x) <=> lambda(f)::call(x)
	f(x).return = f(x)::type = lambda(f)::call(x)::type
	lambda(f)::apply(x)::type <=> lambda(f)::call(x)
	lambda(f).call = lambda(f)::call(x)
	lambda(f).return = lambda(f)::call(x)::type
 */

template <typename type_tn>
struct is_lambda
{
	static bool const value = false;
};

template <typename func_tn>
struct is_lambda <lambda<func_tn>>
{
	static bool const value = true;
};

/*
	@bref if func_tn is lambda fun, return fun,
	or else trasfer func to lambda fun, and return
*/
template <typename func_tn>
struct lambdate
{
private:
	template <typename func, bool is>
	struct trans
	{
		typedef lambda<func> type;
	};
	template <typename func>
	struct trans <func,true>
	{
		typedef func type;
	};
public:
	typedef typename 
		trans<func_tn,is_lambda<func_tn>::value>::type
	type;
};


/**
	we should be add the when the type is lambda type, we should also do this
	condition correctly
	**************************
*/

template <typename func,typename default_param = void>
struct relambda
{};
/// 一个参数的lambda
template
<
	template<typename>class func,
	typename p1,
	typename default_param
>
struct relambda <func<p1>,default_param>
{
	typedef typename lambdate<func<_1>>::type full;
};
/// 二个参数的lambda
template
<
	template<typename,typename>class func,
	typename p1,
	typename p2,
	typename default_param
>
struct relambda <func<p1,p2>,default_param>
{
	typedef typename lambdate<func<_1,_2>>::type full;
};
/// 三个参数的lambda
template
<
	template <typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename default_param
>
struct relambda <func<p1,p2,p3>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3>>::type full;
};
/// 四个参数的lambda
template
<
	template<typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename default_param
>
struct relambda <func<p1,p2,p3,p4>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3,_4>>::type full;
};
/// 五个参数的lambda
template
<
	template<typename,typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename p5,
	typename default_param
>
struct relambda <func<p1,p2,p3,p4,p5>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3,_4,_5>>::type full;
};
/// 六个参数的lambda
template
<
	template<typename,typename,typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename p5,
	typename p6,
	typename default_param
>
struct relambda <func<p1,p2,p3,p4,p5,p6>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3,_4,_5,_6>>::type full;
};
/// 七个参数的lambda
template
<
	template<typename,typename,typename,typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename p5,
	typename p6,
	typename p7,
	typename default_param
>
struct relambda <func<p1,p2,p3,p4,p5,p6,p7>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3,_4,_5,_6,_7>>::type full;
};
/// 八个参数的lambda
template
<
	template<typename,typename,typename,typename,typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename p5,
	typename p6,
	typename p7,
	typename p8,
	typename default_param
>
struct relambda <func<p1,p2,p3,p4,p5,p6,p7,p8>,default_param>
{
	typedef typename lambdate<func<_1,_2,_3,_4,_5,_6,_7,_8>>::type full;
};

/*
	struct A<,>;
	struct B<,>;
	type = A<p1,p2>, mode = A<void,void>
	type1 = B<p1,p2>
	==> type ~ mode /// because equal between A and A
	==> type1 != mode /// beacuse unequal between A and B
	series: includes two conditions, any will return true
		1. if type is tmplate class, T<,,..>,
			if and only if the type T name must be same except the params
		2. if type is not, the type should be same
 */
template <typename type_tn, typename type_mode_tn>
struct is_same_series
{
private:
	typedef typename lambdate<type_tn>::type type1;
	typedef typename lambdate<type_mode_tn>::type type2;
	/// 这个当然也可以用apply的方式，需要用参数个数生成tslist_type，而且还要
	/// 修改call可以接收tslist<>的模式才可以，这个后面考虑再说
	typedef typename type1::defaulted<void>::reapply<>::type self1;
	typedef typename type2::defaulted<void>::reapply<>::type self2;
public:
	static bool const value = is_same<self1,self2>::value;
};



___namespace2_end()