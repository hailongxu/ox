/**
	把类型转成最佳的参数传递的类型
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"

#include "./_if_switch.h"
#include "./mod.h"



#pragma once

___namespace2_begin(ox,met)


/**
	@brief	对参数具有只读性质
	如果是基本类型：要进行const & 修饰
	如果是类类型：要进行const & 修饰
	如果类型有指针：就不要再进行&修饰，直接加const
*/
template <typename type_tn>
struct param_readonly_type
{
private:
	typedef typename rmv_ref<type_tn>::type removed_ref_type;
public:
	typedef typename
		ox::met::iff
		<
			is_sptr<removed_ref_type>::value,
			add_con<removed_ref_type>,
			add_ref<typename add_con<removed_ref_type>::type>
		>
		::type
	outer_type;
	typedef typename outer_type::type type;
};

/**
	@brief	对参数具有可写性质
	如果是基本类型：要进行 & 修饰
	如果是类类型：要进行 & 修饰
*/
template <typename type_tn>
struct param_write_type
{
	typedef
		typename add_ref<
			typename rmv_con <
				typename rmv_ref<type_tn>
				::type
			>::type
		>::type
	type;
};

/**
	如果是&类型，继续保持原有类型
	如果有对象值传递，把变成对象引用
	如果是基本类型值传递，变成值传递
*/
template <typename type_tn>
struct param_auto_type
{
	typedef typename
		iff
		<
			is_con<type_tn>::value,
			param_readonly_type<type_tn>,
			param_write_type<type_tn>
		>
		::type
	outer_type;
	typedef typename outer_type::type type;
};

/**
	左值和右值保持类型一样，除非
	如果左值是对象值类型，把变成对象变成引用，其余修饰不变
	一般用在：用形式参数去初始化一个类成员内部变量，
	那个这个函数的形式参数应该是什么类型呢，这个就是用来解决这个问题的
	存储类型     函数初始化参数类型
	T*           T*const
	T&           T&
	class        class const&
	num          nun const
*/
template <typename type_tn>
struct param_type_for_init
{
	typedef typename add_vcr<int const&>::to<type_tn>::type type;
	//typedef typename add_vcr<int const&>::to<type_tn>::type type;
	//typedef
	//	typename iff
	//	<
	//		is_class<type_tn>::value,
	//		add_vcr<int const&>::to<type_tn>,
	//	typename iff
	//	<
	//		is_value<type_tn>::value && !is_sptr<type_tn>::value,
	//		add_con<type_tn>,
	//		identity<typename add_con<type_tn>::type>
	//	>
	//	::type>::type
	//	::type
	//type;
};
template <typename type_tn>
struct param_type_for_init <type_tn&>
{
	typedef type_tn& type;
};


/**
	目的：对一个对象的原样转发，最重要的是原来这个数据内容所代表的意义是不能变的
		把一个变量的引用先存储起来，然后，在以后的时候进行原样返回其这个对象
		一般用在：用形式参数去初始化一个类成员内部变量

	返回类型要和原类型在应用场合保持相同的特性，在其它应用场合保持等价性

	原类型     存储类型     取返回类型  初始化函数形式参数类型
	T&    =>   T&           T&          T&
	T*    =>   T*           T*          T*
	class =>   class&       class&      class&
	num   =>   num          num&        num const&

	num 为C++的基本类型，int,long,...，还包括enum
	这里没有考虑函数类型和数组类型
*/
template <typename type_tn>
struct store_type_for_forward
{
	typedef typename
		iff
		<
			is_class<type_tn>::value,
			add_ref<type_tn>,
			identity<type_tn>
		>
		::type::type
	type;
};

template <typename type_tn>
struct return_type_for_forward
{
	//typedef typename
	//	iff
	//	<
	//		is_class<type_tn>::value || is_num<type_tn>::value,
	//		add_ref<type_tn>,
	//		identity<type_tn>
	//	>
	//	::type
	//outer_type;
	//typedef typename outer_type::type type;
	typedef typename add_ref<type_tn>::type type;
};


___namespace2_end()