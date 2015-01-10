/**
	静态断言
*/

#include "../ox/nsab.h"
#include "./mod.h"

#pragma once


___namespace2_begin(ox,met)

/**
	编译期断言
 */


/**
	编译器在展开模板时，检查类型是否存在的条件，原则是非必要尽量不检查，
	比如只是定义，但是这个类型定义，也没有怎么具体使用，编译器是不会报错的，
	但是如果对类型求大小，这时必须，就知道类型是否存在，否则，怎么求大小，
	所以，编译期断言就是借助于这个特性进行的，boost和loki都是这样用的
 */
namespace inside
{
	template <int> struct compile_error;
	template <> struct compile_error<true> {};
	template <int> struct static_check_tt {};
	template <bool> struct type_size_ok_tt;
	template <> struct type_size_ok_tt<true>{};
}

#define ___static_assert_message(expr, msg) \
	{ ox::met::inside::compile_error<((expr) != 0)> error_##msg; (void)error_##msg;} 
/// 静态断言宏定义
#define ___static_assert(b) \
	typedef ox::met::inside::static_check_tt< \
		sizeof(ox::met::inside::type_size_ok_tt<b>) \
	> \
	___macro_internal_ox_type_typedef_compile_static_assert_global_macro__2010_11_06

/**
	宏定义易出现的错误：
	#define A(x) x;
	#define B(x,y) x+y
	A(B(1,2)) ; /// error, why? 因为A把B(1看成了第一个参数
*/
#define ___static_assert_same_type(type_from_tn, type_to_tn) \
	___static_assert((ox::met::is_same<type_from_tn,type_to_tn>::value))

#define ___static_desert_same_type(type_from_tn, type_to_tn) \
	___static_assert(!(ox::met::is_same<type_from_tn,type_to_tn>::value))


___namespace2_end()