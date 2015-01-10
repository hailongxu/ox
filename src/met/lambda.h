
/**
	对类型函数包装成lambda
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"
#include "./mod.h"
#include "./_if_switch.h"
#include "./tslist_decl.h"
#include "./lambda_decl.h"
#include "./lambda_argument.h"


#pragma once


___namespace2_begin(ox,met)




namespace __private_lambda
{
	/// call procedure

	namespace method0
	{
		template <typename func>
		struct do_call_defaulted
		{
			typedef func call;
			template <typename=void> struct apply { typedef func type; };

			typedef func recall;
			template <typename=void> struct reapply { typedef func type; };
		};

		template <typename func>
		struct do_call_defaulted_given
		{
			template <typename=void>
			struct defaulted : do_call_defaulted<func> {};
		};
	} /// end of default call

	namespace method1
	{
		template
		<
			template<typename>class func,
			typename default_param,
			typename p1
		>
		struct do_call_defaulted
		{
			template <typename _p1=default_param>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1>::type
				>
			{};
			template <typename _p1=default_param>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1>::type
					>
				type;
			};

			template <typename _p1=default_param>
			struct recall :	func <_p1>
			{};
			template <typename _p1=default_param>
			struct reapply
			{ typedef func <_p1> type; };
		};

		template
		<
			template<typename>class func,
			typename p1
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1>
			{};
		};
	} /// end of default call

	namespace method2
	{
		template
		<
			template<typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param
			>
			struct recall :	func <_p1,_p2>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2> type; };
		};

		template
		<
			template<typename,typename>class func,
			typename p1,
			typename p2
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2>
			{};
		};
	} /// end of default call
	namespace method3
	{
		template
		<
			template<typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param
			>
			struct recall :	func <_p1,_p2,_p3>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3> type; };
		};

		template
		<
			template<typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3>
			{};
		};
	} /// end of default call

	/// ********
	/// lambda's defalut param comes from the lambda<_1,_2,p1>, which p1 will passed into the 3th func param
	namespace method4
	{
		template
		<
			template<typename,typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3,
			typename p4
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4>::type,
					typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4>::type,
						typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param
			>
			struct recall :	func <_p1,_p2,_p3,_p4>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3,_p4> type; };
		};

		template
		<
			template<typename,typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3,
			typename p4
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3,p4>
			{};
		};
	} /// end of default call
	namespace method5
	{
		template
		<
			template<typename,typename,typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5>::type,
					typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5>::type,
					typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5>::type,
						typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5>::type,
						typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param
			>
			struct recall :	func <_p1,_p2,_p3,_p4,_p5>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3,_p4,_p5> type; };
		};

		template
		<
			template<typename,typename,typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3,p4,p5>
			{};
		};
	} /// end of default call
	namespace method6
	{
		template
		<
			template<typename,typename,typename,typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6>::type,
					typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6>::type,
					typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6>::type,
					typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6>::type,
						typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6>::type,
						typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6>::type,
						typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param
			>
			struct recall :	func <_p1,_p2,_p3,_p4,_p5,_p6>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3,_p4,_p5,_p6> type; };
		};

		template
		<
			template<typename,typename,typename,typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6>
			{};
		};
	} /// end of default call
	namespace method7
	{
		template
		<
			template<typename,typename,typename,typename,typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6,
			typename p7
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
					typename arg_func<default_param>::args<p7,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type,
						typename arg_func<default_param>::args<p7,_p1,_p2,_p3,_p4,_p5,_p6,_p7>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param
			>
			struct recall :	func <_p1,_p2,_p3,_p4,_p5,_p6,_p7>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3,_p4,_p5,_p6,_p7> type; };
		};

		template
		<
			template<typename,typename,typename,typename,typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6,
			typename p7
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6,p7>
			{};
		};
	} /// end of default call
	namespace method8
	{
		template
		<
			template<typename,typename,typename,typename,typename,typename,typename,typename>class func,
			typename default_param,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6,
			typename p7,
			typename p8
		>
		struct do_call_defaulted
		{
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param,
				typename _p8=default_param
			>
			struct call :
				func
				<
					typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p7,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
					typename arg_func<default_param>::args<p8,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type
				>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param,
				typename _p8=default_param
			>
			struct apply
			{
				typedef
					func
					<
						typename arg_func<default_param>::args<p1,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p2,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p3,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p4,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p5,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p6,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p7,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type,
						typename arg_func<default_param>::args<p8,_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>::type
					>
				type;
			};

			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param,
				typename _p8=default_param
			>
			struct recall :	func <_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8>
			{};
			template
			<
				typename _p1=default_param,
				typename _p2=default_param,
				typename _p3=default_param,
				typename _p4=default_param,
				typename _p5=default_param,
				typename _p6=default_param,
				typename _p7=default_param,
				typename _p8=default_param
			>
			struct reapply
			{ typedef func <_p1,_p2,_p3,_p4,_p5,_p6,_p7,_p8> type; };
		};

		template
		<
			template<typename,typename,typename,typename,typename,typename,typename,typename>class func,
			typename p1,
			typename p2,
			typename p3,
			typename p4,
			typename p5,
			typename p6,
			typename p7,
			typename p8
		>
		struct do_call_defaulted_given
		{
			template <typename default_param>
			struct defaulted
				: do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6,p7,p8>
			{};
		};
	} /// end of default call
}



/*
	lambda function declaration, 
	this will convert a common function into a lambda function
	f(x) <=> lambda(f)::call(x)
	f(x).return = f(x)::type
	lambda(f).call = lambda(f)::call(x)
	lambda(f).return = lambda(f)::call(x)::type
 */

template <typename func,typename default_param>
struct lambda
	: __private_lambda::method0::do_call_defaulted<func>
	, __private_lambda::method0::do_call_defaulted_given<func>
{
	struct is { static bool const value = false; };
	enum { params_count = 0 };
};

/// 一个参数的lambda
template
<
	template<typename>class func,
	typename p1,
	typename default_param
>
struct lambda <func<p1>,default_param>
	: __private_lambda::method1::do_call_defaulted<func,default_param,p1>
	, __private_lambda::method1::do_call_defaulted_given<func,p1>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };

	enum { params_count = 1 };

	typedef func<p1> self;
};

/// 为了简化调用形式
template
<
	template<typename> class func,
	typename p1,
	typename default_param
>
struct lambda <lambda <func<p1>>,default_param>
	: lambda<func<p1>,default_param>
{};


/// 二个参数的lambda
template
<
	template<typename,typename>class func,
	typename p1,
	typename p2,
	typename default_param
>
struct lambda <func<p1,p2>,default_param>
	: __private_lambda::method2::do_call_defaulted<func,default_param,p1,p2>
	, __private_lambda::method2::do_call_defaulted_given<func,p1,p2>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };

	enum { params_count = 2 };

	typedef func<p1,p2> self;
};

template
<
	template <typename,typename> class func,
	typename p1,
	typename p2,
	typename default_param
>
struct lambda <lambda<func<p1,p2>>,default_param>
	: lambda <func<p1,p2>,default_param>
{};


/// 三个参数的lambda
template
<
	template <typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename default_param
>
struct lambda <func<p1,p2,p3>,default_param>
	: __private_lambda::method3::do_call_defaulted<func,default_param,p1,p2,p3>
	, __private_lambda::method3::do_call_defaulted_given<func,p1,p2,p3>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };

	/// parameters count
	enum { params_count = 3 };

	typedef func<p1,p2,p3> self;
};

template
<
	template<typename,typename,typename>class func,
	typename p1,
	typename p2,
	typename p3,
	typename default_param
>
struct lambda <lambda<func<p1,p2,p3>>,default_param>
	: lambda<func<p1,p2,p3>,default_param>
{};


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
struct lambda <func<p1,p2,p3,p4>,default_param>
	: __private_lambda::method4::do_call_defaulted<func,default_param,p1,p2,p3,p4>
	, __private_lambda::method4::do_call_defaulted_given<func,p1,p2,p3,p4>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;
	typedef p4 p4_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };
	template <> struct params<4> { typedef p4_type type; };

	/// parameters count
	enum { params_count = 4 };

	typedef func<p1,p2,p3,p4> self;
};
template
<
	template<typename,typename,typename,typename> class func,
	typename p1,
	typename p2,
	typename p3,
	typename p4,
	typename default_param
>
struct lambda <lambda<func<p1,p2,p3,p4>>,default_param>
	: lambda<func<p1,p2,p3,p4>,default_param>
{};

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
struct lambda <func<p1,p2,p3,p4,p5>,default_param>
	: __private_lambda::method5::do_call_defaulted<func,default_param,p1,p2,p3,p4,p5>
	, __private_lambda::method5::do_call_defaulted_given<func,p1,p2,p3,p4,p5>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;
	typedef p4 p4_type;
	typedef p5 p5_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };
	template <> struct params<4> { typedef p4_type type; };
	template <> struct params<5> { typedef p5_type type; };

	enum { params_count = 5 };

	typedef func<p1,p2,p3,p4,p5> self;
};

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
struct lambda <lambda<func<p1,p2,p3,p4,p5>>,default_param>
	: lambda <func<p1,p2,p3,p4,p5>,default_param>
{};

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
struct lambda <func<p1,p2,p3,p4,p5,p6>,default_param>
	: __private_lambda::method6::do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6>
	, __private_lambda::method6::do_call_defaulted_given<func,p1,p2,p3,p4,p5,p6>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;
	typedef p4 p4_type;
	typedef p5 p5_type;
	typedef p6 p6_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };
	template <> struct params<4> { typedef p4_type type; };
	template <> struct params<5> { typedef p5_type type; };
	template <> struct params<6> { typedef p6_type type; };

	enum { params_count = 6 };

	typedef func<p1,p2,p3,p4,p5,p6> self;
};

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
struct lambda <lambda<func<p1,p2,p3,p4,p5,p6>>,default_param>
	: lambda<func<p1,p2,p3,p4,p5,p6>,default_param>
{};

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
struct lambda <func<p1,p2,p3,p4,p5,p6,p7>,default_param>
	: __private_lambda::method7::do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6,p7>
	, __private_lambda::method7::do_call_defaulted_given<func,p1,p2,p3,p4,p5,p6,p7>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;
	typedef p4 p4_type;
	typedef p5 p5_type;
	typedef p6 p6_type;
	typedef p7 p7_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };
	template <> struct params<4> { typedef p4_type type; };
	template <> struct params<5> { typedef p5_type type; };
	template <> struct params<6> { typedef p6_type type; };
	template <> struct params<7> { typedef p7_type type; };

	enum { params_count = 7 };

	typedef func<p1,p2,p3,p4,p5,p6,p7> self;
};

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
struct lambda <lambda<func<p1,p2,p3,p4,p5,p6,p7>>,default_param>
	: lambda<func<p1,p2,p3,p4,p5,p6,p7>,default_param>
{};


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
struct lambda <func<p1,p2,p3,p4,p5,p6,p7,p8>,default_param>
	: __private_lambda::method8::do_call_defaulted<func,default_param,p1,p2,p3,p4,p5,p6,p7,p8>
	, __private_lambda::method8::do_call_defaulted_given<func,p1,p2,p3,p4,p5,p6,p7,p8>
{
	struct is { static bool const value = true; };

	typedef p1 p1_type;
	typedef p2 p2_type;
	typedef p3 p3_type;
	typedef p4 p4_type;
	typedef p5 p5_type;
	typedef p6 p6_type;
	typedef p7 p7_type;
	typedef p8 p8_type;

	template <size_t suffix_tn> struct params;
	template <> struct params<1> { typedef p1_type type; };
	template <> struct params<2> { typedef p2_type type; };
	template <> struct params<3> { typedef p3_type type; };
	template <> struct params<4> { typedef p4_type type; };
	template <> struct params<5> { typedef p5_type type; };
	template <> struct params<6> { typedef p6_type type; };
	template <> struct params<7> { typedef p7_type type; };
	template <> struct params<8> { typedef p8_type type; };

	enum { params_count = 8 };

	typedef func<p1,p2,p3,p4,p5,p6,p7,p8> self;
};

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
struct lambda <lambda<func<p1,p2,p3,p4,p5,p6,p7,p8>>,default_param>
	: lambda<func<p1,p2,p3,p4,p5,p6,p7,p8>,default_param>
{};


//// simplier form of using

/// one param
template
<
	template<typename>class func
>
struct lambda1
	: lambda<func<ox::_1>>
{
	typedef func<ox::_1> func_type;
	typedef lambda<func_type> type;
};

/// two param
template
<
	template<typename,typename>class func
>
struct lambda2
	: lambda<func<ox::_1,ox::_2>>
{
	typedef func<ox::_1,ox::_2> func_type;
	typedef lambda<func_type> type;
};

/// three param
template
<
	template<typename,typename,typename>class func
>
struct lambda3
	: lambda<func<ox::_1,ox::_2,ox::_3>>
{
	typedef func<ox::_1,ox::_2,ox::_3> func_type;
	typedef lambda<func_type> type;
};

/// four param
template
<
	template<typename,typename,typename,typename>class func
>
struct lambda4
	: lambda<func<ox::_1,ox::_2,ox::_3,ox::_4>>
{
	typedef func<ox::_1,ox::_2,ox::_3,ox::_4> func_type;
	typedef lambda<func_type> type;
};

/// five param
template
<
	template<typename,typename,typename,typename,typename>class func
>
struct lambda5
	: lambda<func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5>>
{
	typedef func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5> func_type;
	typedef lambda<func_type> type;
};

/// six param
template
<
	template<typename,typename,typename,typename,typename,typename>class func
>
struct lambda6
	: lambda<func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6>>
{
	typedef func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6> func_type;
	typedef lambda<func_type> type;
};

/// seven param
template
<
	template<typename,typename,typename,typename,typename,typename,typename>class func
>
struct lambda7
	: lambda<func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6,ox::_7>>
{
	typedef func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6,ox::_7> func_type;
	typedef lambda<func_type> type;
};

/// eight param
template
<
	template<typename,typename,typename,typename,typename,typename,typename,typename>class func
>
struct lambda8
	: lambda<func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6,ox::_7,ox::_8>>
{
	typedef func<ox::_1,ox::_2,ox::_3,ox::_4,ox::_5,ox::_6,ox::_7,ox::_8> func_type;
	typedef lambda<func_type> type;
};



___namespace2_end()