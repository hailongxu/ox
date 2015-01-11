
/**
	作者：徐海龙
	2010-11-10
	创建一个更加简单的概念，创建一个线性异质容器
*/

#include "../ox/nsab.h"



#pragma once



#define ___def_has_type(label,t) \
template <typename _struct> struct has_type_##label { \
	typedef char yes [1]; \
	typedef char no [2]; \
	template <typename _struct> static yes& test_func(typename _struct::##t *); \
	template <typename _struct> static no& test_func(...); \
	static bool const value = sizeof(test_func<_struct>(0))==sizeof(yes); \
}


#define ___def_has_func(label,r,func_name,params) \
	template <typename _struct> struct has_func_##label { \
		typedef char yes [1]; \
		typedef char no [2]; \
		template <r(_struct::*)params > struct parameter ; \
		template <typename _struct2> static yes& test_func(parameter<&_struct2::##func_name >*);  \
		template <typename _struct2> static no& test_func(...); \
		static bool const value = sizeof(test_func<_struct>(0))==sizeof(yes); \
	}



#if 0

/// the usage of has-func

___def_has_func(f,void,f,(int));

struct test
{
	void f(int) {}
};

bool b = has_func_f<test>::value;

#endif