
/**
	类型值类型，定义，是存储的某个实例的地址，总之，指针存的就是
	地址，只要存的是地址，我们就认为是指针类型
	从这个角度来说，函数，成员函数，成员数据指针都是指针
	数组是一个特殊的类型，我们称它为数组类型

	也就是说，除去指针和数组以外的我们都认为是值类型

	这个是值类型中的一种，就是class-like的


		date: 2007-5-30
	author: xuhailong
	histroy: typetrait
		2007-7-29: add return trait type, for return type with reference 
			the result we will store, its type we will use if different 
			from others.
			判断单个类型的归属问题，是何种类型

	C++ 类型分析【2008-12-9】
	C++ 类型主体（共3大类，可以细分为6种分类）
		【1】值类型
			单体类型：任意定义class，struct，union类型，基本类型，枚举类型
		【2】指针类型
			通用指针：在任意类型（包括值类型，指针类型，数组类型）后面加上就成为了指针类型，这时，这个类型就是通用指针类型，我们简称为指针类型，也是我们经常说的指针类型
			函数指针：可称为C函数指针，其典型形式：为R (*F) (...)，双括号
			类成员函数指针：专门为C++类设计的函数指针，其典型形式为：R (C::*F)(...)
			类成员数据指针：专门为C++类设计的数据指针，其典型形式为：C::*P
		【3】数组类型
			数组：一种特殊类型，其典型形式为：T [N]
		×注意×：引用和任意类型结合并不能形成类型主体
	C++ 参数传递方式
		by con : 常量传递
		by val : 值传递
		by ref : 引用传递
			对数组类型写法：T (&A) [N]
		by ptr : 指针传递
			对数组类型写法：T (*A) [N]
			数组传递时，是通过指针进行传递的，当然我们也可以通过引用进行传递
	C++ 类型修饰（共有3种分类）
		con 修饰：不影响类型主体
		ref 修饰：一类特殊类型主体变形
			引用修饰类型时，其位置非常特殊，只能放在被修饰类型的最后面，而且只能出现一次
		ptr 修饰：影响类型主体，并形成一新类型主题，
	C++ 类型混合修饰（共有12种分类）
		T
		T const
		T *
		T const *
		T * const
		T const * const
		T &
		T const &
		T * &
		T const * &
		T * const &
		T const * const &

	当然了，分析这些，不是为了分析而分析，是我在写程序是遇到的一些问题，经过很长时间的整理
	才得出了这些宝贵的经验，与大家分享，希望大家和我一起进步

	我们类库也将围绕这些特点实作出相应的代码，让你领会这类型的特性和奇异

	源于，我们在编写一个通用程序时，我们需要约束一些未知类型，或者探知一些类型
	以便于我们采取不同的策略和方法，已让我们的程序更健壮，在编译期就能发现错误，
	对不同的类型进行特化

	类库需要进行重写，规范化，以便以后通用
*/



/**
	类型值类型，定义，是存储的某个实例的地址，总之，指针存的就是
	地址，只要存的是地址，我们就认为是指针类型
	从这个角度来说，函数，成员函数，成员数据指针都是指针
	数组是一个特殊的类型，我们称它为数组类型

	也就是说，除去指针和数组以外的我们都认为是值类型

	这个是值类型中的一种，就是class-like的

*/

#include "../ox/nsab.h"

#include "./_if_switch.h"


#pragma once


___namespace2_begin(ox,met)


/**
	@brief
	C++的类型分为如下几类：class, class::*, *, class::void(*)(), &, num, (), []
	类 ： class: POD, !POD, union
	指针：*: class::*, ::*
	引用：&: &
	函数：(): ()
	数组：[]: []
	数字：num: !(class,*,&,(),[]) <=> buildin type(char,long,...), enum

	/// 这个类型一个很重要的作用是，可以进行数据直接搬移，不用调用构造
	值类型：value/binary: POD, num, *, union

	
	此外，引用类型，可以是对任一类型的映射，成为虚类型
*/


template <typename> struct is_arr;
template <typename> struct is_fun;
template <typename> struct is_class;
template <typename> struct is_con;
template <typename> struct is_vol;
template <typename> struct is_ref;
template <typename> struct is_sptr;
template <typename> struct is_memdat_ptr;
template <typename> struct is_memfun_ptr;
template <typename> struct of_func;

template <typename> struct is_num;
template <typename> struct is_value;
template <typename> struct is_funptr;
template <typename> struct is_ptr;


template <typename x, typename y> struct is_same;
template <typename f, typename t> struct is_convertible;
template <typename b, typename d> struct is_base_of;
template <typename> struct is_null;
template <typename> struct is_void;
template <typename> struct is_nothing;
template <typename> struct is_empty;


template <typename> struct add_con;
template <typename> struct add_ptr;
template <typename> struct add_ref;
template <typename> struct add_vcr;
template <typename> struct rmv_con;
template <typename> struct rmv_ptr;
template <typename> struct rmv_ref;
template <typename> struct rmv_vcr;
template <typename> struct add;
template <typename> struct rmv;


template <typename type_like> struct clone_ref;
template <typename type_like> struct clone_ptr;
template <typename type_like> struct clone_con;
template <typename type_like, typename mod_tn> struct clone_mod;

template <typename> struct cate;


/**
	@brief 判断类型是不是con类型
*/

template <typename t> struct is_con
{
	static bool const value = false;
};
template <typename t> struct is_con <t const>
{
	static bool const value = true;
};
template <typename t> struct is_con <t&>
{
	static bool const value = false;
};
template <typename t> struct is_con <t const&>
{
	static bool const value = true;
};

/**
	@brief 判断类型是不是 volatile 类型
*/

template <typename t> struct is_vol
{
	static bool const value = false;
};
template <typename t> struct is_vol <t volatile>
{
	static bool const value = true;
};
template <typename t> struct is_vol <t&>
{
	static bool const value = false;
};
template <typename t> struct is_vol <t volatile&>
{
	static bool const value = true;
};

/**
	@brief 判断类型是不是引用类型
*/

template <typename t> struct is_ref
{
	static bool const value = false;
};
template <typename t> struct is_ref <t&>
{
	static bool const value = true;
};


/**
	@brief 判断类型是不是指针类型
	this cptr contains T*, void(*)()
	sptr: simple pointer
*/

template <typename t> struct is_sptr
{
	static bool const value = false;
};
template <typename t> struct is_sptr<t*>
{
	static bool const value = true;
};
template <typename t> struct is_sptr<t*const>
{
	static bool const value = true;
};

template <typename t> struct is_memdat_ptr
{
	static bool const value = false;
};
template <typename t, typename C> struct is_memdat_ptr<t C::*>
{
	static bool const value = true;
};
template <typename t, typename C> struct is_memdat_ptr<t C::*const>
{
	static bool const value = true;
};


template <typename t> struct of_func
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<void> retype;
	typedef void type;
};
template <typename r>
struct of_func <r(*)()>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)();
};
template <typename r,typename p1>
struct of_func <r(*)(p1)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1);
};
template <typename r,typename p1,typename p2>
struct of_func <r(*)(p1,p2)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2);
};
template <typename r,typename p1,typename p2,typename p3>
struct of_func<r(p1,p2,p3)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3);
};
template <typename r,typename p1,typename p2,typename p3,typename p4>
struct of_func<r(p1,p2,p3,p4)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3,p4);
};
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5>
struct of_func<r(p1,p2,p3,p4,p5)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3,p4,p5);
};
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct of_func<r(p1,p2,p3,p4,p5,p6)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3,p4,p5,p6);
};
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct of_func<r(p1,p2,p3,p4,p5,p6,p7)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3,p4,p5,p6,p7);
};
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct of_func<r(p1,p2,p3,p4,p5,p6,p7,p8)>
{
	typedef ox::boolean<false> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<true> is_fun;
	typedef ox::identity<void> _class;
	typedef ox::identity<r> retype;
	typedef r(*type)(p1,p2,p3,p4,p5,p6,p7,p8);
};

/// member function
template <typename r,typename c>
struct of_func <r(c::*)()>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)();
};
template <typename r,typename c,typename p1>
struct of_func <r(c::*)(p1)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1);
};
template <typename r,typename c,typename p1,typename p2>
struct of_func <r(c::*)(p1,p2)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2);
};
template <typename r,typename c,typename p1,typename p2,typename p3>
struct of_func<r(c::*)(p1,p2,p3)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3);
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4>
struct of_func<r(c::*)(p1,p2,p3,p4)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4);
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5>
struct of_func<r(c::*)(p1,p2,p3,p4,p5)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5);
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6);
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6,p7)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6,p7);
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6,p7,p8)>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<false> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6,p7,p8);
};

/// member function const
template <typename r,typename c>
struct of_func <r(c::*)()const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)()const;
};
template <typename r,typename c,typename p1>
struct of_func <r(c::*)(p1)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1)const;
};
template <typename r,typename c,typename p1,typename p2>
struct of_func <r(c::*)(p1,p2)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3>
struct of_func<r(c::*)(p1,p2,p3)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4>
struct of_func<r(c::*)(p1,p2,p3,p4)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5>
struct of_func<r(c::*)(p1,p2,p3,p4,p5)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6,p7)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6,p7)const;
};
template <typename r,typename c,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct of_func<r(c::*)(p1,p2,p3,p4,p5,p6,p7,p8)const>
{
	typedef ox::boolean<true> is_memf;
	typedef ox::boolean<true> is_const;
	typedef ox::boolean<false> is_fun;
	typedef ox::identity<c> _class;
	typedef ox::identity<r> retype;
	typedef r(c::*type)(p1,p2,p3,p4,p5,p6,p7,p8)const;
};


template <typename t> struct is_memfun_ptr
{
	static bool const value = of_func<t>::is_memf::value;
};


/**
	@brief 判断类型是不是数组类型
*/

template <typename t>
struct is_arr
{
private:
	template <typename _t> struct _mass_is_arr;
	//typedef typename mass_rmv_con<type_tn>::type noncon_type;
	typedef t noncon_type;

public:
	static bool const value = _mass_is_arr<noncon_type>::value;

private:
	template <typename _t>
	struct _mass_is_arr
	{
		enum { value = false };
	};
	template <typename _t, size_t size_tc>
	struct _mass_is_arr <_t [size_tc]>
	{
		enum { value = true };
	};
}; // end of mass_is_arr



/**
	@brief 判断类型是不是函数类型
*/

template <typename type_tn>
struct is_fun
{
	/**
		void (*)() && void (* const)() 都是函数指针
	*/
private:
	/// 匹配数组
	template <typename _type_tn>
	static char classify_type(_type_tn[1]);
	/// 剩余匹配包括void，和函数类型
	template <typename _type_tn>
	static double classify_type(...);
	enum
	{
		__is_fun_or_void = sizeof(classify_type<type_tn>(0))==sizeof(double),
		__is_void = is_same<void,type_tn>::value
	};
public:
	enum { value = __is_fun_or_void && !__is_void };
};

/// how to do ?
//template <typename type_tn>
//struct is_memfun
//{};?
//template <typename type_tn>
//struct is_pod
//{};

/**
	@brief 判断类型是不是类类型
*/

template <typename type_tn>
struct is_class
{
private:
	typedef char class_type;
	typedef long nonclass_type;
	template <typename inner_type_tn> static class_type make_class_type(int inner_type_tn::*);
	template <typename inner_type_tn> static nonclass_type make_class_type(...);
public:
	static bool const value = sizeof(class_type)==sizeof(make_class_type<type_tn>(0));
};


/**
	@brief 判断类型是不是数类型
*/
template <typename type_tn>
struct is_num
{
	static bool const value
		=  ! is_class<type_tn>::value
		&& ! is_sptr<type_tn>::value
		&& ! is_memdat_ptr<type_tn>::value
		&& ! is_memfun_ptr<type_tn>::value
		&& ! is_ref<type_tn>::value
		&& ! is_fun<type_tn>::value
		&& ! is_arr<type_tn>::value;
};


///
/// 下面是常用的复合类型
///

/// the type is funptr
template <typename _type>
struct is_funptr
{
	static bool const value
		= is_sptr<_type>::value
		&& is_fun<typename rmv_ptr<_type>::type>::value;
};

template <typename type_tn>
struct is_ptr
{
	static bool const value
		= is_sptr<type_tn>::value
		|| is_memdat_ptr<type_tn>::value
		|| is_memfun_ptr<type_tn>::value;
};

/**
	@brief 判断类型是不是值类型
*/
template <typename type_tn>
struct is_value
{
	static bool const value
		= ! is_class<type_tn>::value
		&&
		(
		   is_ptr<type_tn>::value
		|| is_num<type_tn>::value
		// is_pod<type_tn>::value
		//|| is_ref<type_tn>::value
		);
};


///
/// 下面是类型之间关系的判断
///

/// whether two type is same
template <typename type_x_tn, typename type_y_tn>
struct is_same
{
	enum { value = false };
};
template <typename type_tn>
struct is_same<type_tn,type_tn>
{
	enum { value = true };
};

/// whether the first type can be converted to the second type
template <typename from_tn, typename to_tn>
struct is_convertible
{
private:
	typedef char to_fake;
	typedef long from_fake;
	static from_fake test(...);
	static to_fake test(to_tn);
	static from_tn make_from();
public:
	static bool const value = sizeof(test(make_from()))==sizeof(to_fake);
};

/// is base of:
/// two example : struct A:B{}; A is base of A, A is base of B
//template <typename base_tn, typename derived_tn>
//struct is_base_of
//{
//	typedef typename add_con<base_tn>::type* base_const_ptr;
//	typedef typename add_con<derived_tn>::type* derived_const_ptr;
//	static bool const value
//		= is_convable<base_const_ptr, derived_const_ptr>::value
//		&& ! is_same<base_const_ptr, const void*>::value;
//};


template <typename b, typename d>
struct is_base_of
{
	typedef char& yes;
	typedef char(&no)[2];

	//template <typename t> static yes converable(d, t);
	//static no converable(b,int);

	//struct object
	//{
	//	operator b() const;
	//	operator d();
	//};

	static const bool __is_both_class = is_class<b>::value && is_class<d>::value;
	static const bool __is_not_same = !is_same<b,b>::value;
	static const bool __is_converable = is_convertible<b,d>::value;
	//static const bool __is_converable = sizeof(converable(object(),0)) == sizeof(yes);

	static const bool value = __is_both_class && __is_not_same && __is_converable;
};


/// 几个特殊判断
template <typename t> struct is_null { static bool const value = ox::met::is_same<t,ox::null_type>::value; };
template <typename t> struct is_void { static bool const value = ox::met::is_same<t,void>::value; };
template <typename t> struct is_empty { static bool const value = ox::met::is_same<t,ox::empty_type>::value; };
template <typename t> struct is_nothing { static bool const value = ox::met::is_null<t>::value||ox::met::is_void<t>::value; };




/**
	@brief T=> T*, T const, T&
*/

/**
	T=> T const
	[true]: T=> T const
	[false] : T&=> T&
*/

template <typename type_tn>
struct add_con
{
	typedef type_tn const type;

	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};
template <typename type_tn>
struct add_con <type_tn const>
{
	typedef type_tn const type;

	template <typename to_tn>
	struct to
	{
		typedef typename add_con<to_tn>::type type;
	};
};
/// other action will be taken when type is reference
template <typename type_tn>
struct add_con <type_tn&>
{
	typedef type_tn const& type; /// keep the original type
	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};

template <typename type_tn>
struct add_con <type_tn const&>
{
	typedef type_tn const& type;
	template <typename to_tn>
	struct to
	{
		typedef typename add_con<to_tn>::type type;
	};
};
/// for void, do nothing
template <>
struct add_con <void>
{
	typedef void type;
	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};

/// remove const
/// when type is mass, remove const
/// when type is jive, remove const due to the flag of ref_inc
template <typename type_tn> struct rmv_con
{
	typedef type_tn type;
	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};
template <typename type_tn> struct rmv_con <type_tn const>
{
	typedef type_tn type;
	template <typename to_tn>
	struct to
	{
		typedef typename rmv_con<to_tn>::type type;
	};
};
template <typename type_tn> struct rmv_con <type_tn const&>
{
	typedef type_tn& type;
	template <typename to_tn>
	struct to
	{
		typedef typename rmv_con<to_tn>::type type;
	};
};


/**
	T=> T*
*/

template <typename type_tn> struct add_ptr
{
	typedef type_tn* type;
};
template <typename type_tn> struct add_ptr <type_tn&>
{
	typedef type_tn*& type;
};


template <typename type_tn> struct rmv_ptr
{
	typedef type_tn type;
};
template <typename type_tn> struct rmv_ptr <type_tn*>
{
	typedef type_tn type;
};
template <typename type_tn> struct rmv_ptr <type_tn&>
{
	typedef type_tn& type;
};
template <typename type_tn> struct rmv_ptr <type_tn*&>
{
	typedef type_tn& type;
};


/*
	T=> T&
*/

template <typename type_tn>
struct add_ref
{
	typedef type_tn& type;

	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};

template <typename type_tn>
struct add_ref <type_tn&>
{
	typedef type_tn& type;

	template <typename to_tn>
	struct to
	{
		typedef typename add_ref<to_tn>::type type;
	};
};

template <>
struct add_ref <void>
{
	typedef void type;

	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};

template <typename type_tn>
struct rmv_ref
{
	typedef type_tn type;
	template <typename to_tn>
	struct to
	{
		typedef to_tn type;
	};
};

template <typename type_tn>
struct rmv_ref <type_tn&>
{
	typedef type_tn type;
	template <typename to_tn>
	struct to
	{
		typedef typename rmv_ref<to_tn>::type type;
	};
};



/**
@brief 把一个类型转成另一个类型的分类
*/


template <typename type_like> struct clone_ref
{
	template <typename type_tn> struct to
	{
		typedef type_tn type;
	};
	template <typename type_tn> struct to <type_tn &>
	{
		typedef type_tn type;
	};
};
template <typename type_like> struct clone_ref <type_like &>
{
	template <typename type_tn> struct to
	{
		typedef type_tn & type;
	};
	template <typename type_tn> struct to <type_tn &>
	{
		typedef type_tn & type;
	};
};

/// copy jive feature to the to<type>
/// @example
/*
 *  like-type: const int * &
 *  to-type: double
 *  result-type: double &

 *  like-type: int *
 *  to-type: double
 *  result-type: double /// if like-type has no "ref",
 *    do nothing to the to-type
 */
template <typename type_like>
struct clone_ptr
{
	typedef typename rmv_ref<type_like>::type noref;

	template <typename type_tn>
	struct to
	{
		typedef typename
			iff
			<
				is_ptr<noref>::value,
				type_tn*,
				type_tn
			>
			::type
		type;
	};
	template <typename type_tn>
	struct to <type_tn &>
	{
		typedef typename
			iff
			<
				is_ptr<noref>::value,
				type_tn*,
				type_tn
			>
			::type &
		type;
	};
};
template <typename type_like>
struct clone_ptr <type_like &>
{
	template <typename type_tn>
	struct to
	{
		typedef type_tn & type;
	};
	template <typename type_tn>
	struct to <type_tn &>
	{
		typedef type_tn & type;
	};
};

/// copy const feature of like-type to the type
/*
	like-type: const int * &
	to-type: double
	result-type: double const

	like-type: int * &
	to-type: double
	result-type: double /// if like-type has no "const",
	do nothing to the to-type
 */
template <typename type_like>
struct clone_con
{
	template <typename type_tn>
	struct to
	{
		typedef type_tn type;
	};
	template <typename type_tn>
	struct to <type_tn const>
	{
		typedef type_tn type;
	};
};
template <typename type_like>
struct clone_con <type_like const>
{
	template <typename type_tn>
	struct to
	{
		typedef type_tn const type;
	};
	template <typename type_tn>
	struct to <type_tn const>
	{
		typedef type_tn const type;
	};
};

/*
	clone the const, volatile, & to the type_tn
*/

template <typename type_like>
struct add_vcr
{
	template <typename t>
	struct to
	{
		typedef typename add_con<type_like>::to<t>::type ct;
		typedef typename add_ref<type_like>::to<ct>::type type;
	};
};

template <typename type_like>
struct rmv_vcr
{
	template <typename t>
	struct to
	{
		typedef typename rmv_ref<type_like>::to<t>::type next_type;
		typedef typename rmv_con<type_like>::to<next_type>::type type;
	};
};


/// the first layout, t const*const&: const, *, &, 
template <typename mod>
struct add
{
	template <typename t>
	struct to
	{
		typedef t type;
	};
};
template <typename mod>
struct add <mod&>
{
	template <typename t>
	struct to
	{
		typedef typename add<mod>::to<t>::type prev_type;
		typedef typename add_ref<prev_type>::type type;
	};
};
template <typename mod>
struct add <mod*>
{
	template <typename t>
	struct to
	{
		typedef typename add<mod>::to<t>::type prev_type;
		typedef typename add_ptr<prev_type>::type type;
	};
};
template <typename mod>
struct add <mod const>
{
	template <typename t>
	struct to
	{
		typedef typename add<mod>::to<t>::type prev_type;
		typedef typename add_con<prev_type>::type type;
	};
};


/// the first layout, t const*const&: const, *, &, 
template <typename mod>
struct rmv
{
	template <typename t>
	struct from
	{
		typedef t type;
	};
};
template <typename mod>
struct rmv <mod&>
{
	template <typename t>
	struct from
	{
		typedef typename rmv_ref<t>::type nt;
		typedef typename rmv<mod>::from<nt>::type type;
	};
};
template <typename mod>
struct rmv <mod*>
{
	template <typename t>
	struct from
	{
		typedef typename rmv_ptr<t>::type  nt;
		typedef typename rmv<mod>::from<nt>::type type;
	};
};
template <typename mod>
struct rmv <mod const>
{
	template <typename t>
	struct from
	{
		typedef typename rmv_con<t>::type nt;
		typedef typename rmv<mod>::from<nt>::type type;
	};
};


template <typename type_like, typename mod>
struct clone_mod
{
	static bool const __is_like_ref = is_ref<type_like>::value; typedef typename rmv_ref<type_like>::type like1;
	static bool const __is_like_ptr = is_ptr<like1>::value; typedef typename rmv_con<like1>::type like2;
	                                                        typedef typename rmv_ptr<like2>::type like3;
	static bool const __is_like_con = is_con<like3>::value;


	static bool const __is_mod_ref = is_ref<mod_tn>::value; typedef typename rmv_ref<mod>::type mod1;
	static bool const __is_mod_ptr = is_ptr<mod1>::value; typedef typename rmv_con<mod1>::type mod2;
	                                                      typedef typename rmv_ptr<mod2>::type mod3;
	static bool const __is_mod_con = is_con<mod3>::value;

	static bool const __is_ref = __is_like_ref&&__is_mod_ref;
	static bool const __is_ptr = __is_like_ptr&&__is_mod_ptr;
	static bool const __is_con = __is_like_con&&__is_mod_con;

	template <typename type_tn>
	struct to
	{
		typedef typename iff <__is_con,typename add_con<type_tn>::type,type_tn>::type type1;
		typedef typename iff <__is_ptr,typename add_ptr<type1>::type,type1>::type type2;
		typedef typename iff <__is_ref,typename add_ref<type2>::type,type2>::type type3;

		typedef type3 type;
	};
};

/**
	@brief 把一个类型转成另一个等价的类型实体
*/
template <typename type_tn>
struct conv_to_mass
{
	typedef type_tn type;
};
template <typename type_tn>
struct conv_to_mass <type_tn &>
{
	typedef type_tn* type;
};


/////////////////////////////////////////////////
template <typename type_tn,size_t level_tc=-1,char direction_tc='-'>
struct cvatom
{
	typedef type_tn type;
	/// the atom type is const
	struct iscon { static bool const value = is_con<type>::value; };
	/// volatile of type
	struct isvol { static bool const value = false; };
	/// how many levels left
	struct rest { static size_t const value = level_tc; };
};
template <typename type_tn,size_t level_tc>
struct cvatom <type_tn&,level_tc,'-'>
{
	typedef cvatom<type_tn,level_tc> result;
	typedef typename result::type type;
	typedef typename result::iscon iscon;
	typedef typename result::isvol isvol;
	typedef typename result::rest rest;
};
template <typename type_tn>
struct cvatom <type_tn&,0,'-'>
{
	typedef type_tn& type;
	/// the atom type is const
	struct iscon { static bool const value = false; };
	struct isvol { static bool const value = false; };
	struct rest { static size_t const value = 0; };
};
template <typename type_tn,size_t level_tc>
struct cvatom <type_tn*,level_tc,'-'>
{
	typedef cvatom<type_tn,level_tc-1,'-'> result;
	typedef typename result::type type;
	typedef typename result::iscon iscon;
	typedef typename result::isvol isvol;
	typedef typename result::rest rest;
};
template <typename type_tn>
struct cvatom <type_tn*,0,'-'>
{
	typedef type_tn* type;
	/// the atom type is const
	struct iscon { static bool const value = false; };
	struct isvol { static bool const value = false; };
	struct rest { static size_t const value = 0; };
};
template <typename type_tn,size_t level_tc>
struct cvatom <type_tn*const,level_tc,'-'>
{
	typedef cvatom<type_tn,level_tc-1,'-'> result;
	typedef typename result::type type;
	typedef typename result::iscon iscon;
	typedef typename result::isvol isvol;
	typedef typename result::rest rest;
};
template <typename type_tn>
struct cvatom <type_tn*const,0,'-'>
{
	typedef type_tn*const type;
	/// the atom type is const
	struct iscon { static bool const value = true; };
	struct isvol { static bool const value = false; };
	struct rest { static size_t const value = 0; };
};

template <typename type_tn,size_t level_tc>
struct cvatom <type_tn,level_tc,'+'>
{
	typedef met::cvatom<type_tn,-1,'-'> rev_cvatom_result;
	static size_t const __level_size = -1 - rev_cvatom_result::rest::value;
	static size_t const __level_size_rev = __level_size-level_tc;
	typedef met::cvatom<type_tn,__level_size_rev,'-'> result;
	typedef typename result::type type;
	typedef typename result::iscon iscon;
	typedef typename result::isvol isvol;
	typedef typename result::rest rest;
};
template <typename type_tn>
struct cvatom <type_tn,0,'+'>
	: cvatom <type_tn,-1,'-'>
{
	//typedef met::cvatom<type_tn,-1,'-'> result;
	//typedef typename result::type type;
	//typedef typename result::iscon iscon;
	//typedef typename result::isvol isvol;
	//typedef typename result::rest rest;
};

template <typename type_tn,size_t level_tn=1,char direction_tc='-'>
struct atom
{
	typedef met::cvatom<type_tn,level_tn,direction_tc> result;
	typedef typename rmv_con<typename result::type>::type type;
	typedef typename result::iscon iscon;
	typedef typename result::isvol isvol;
	typedef typename result::rest rest;
};


/*

cate is devided into types as followed:

1. base value type:  standed for [int]             for example : int, enum, long, ...
2. referenece :      standed for [int&]            for example : t&
3. pointer :         standed for [int*]            for example : t*
4. class or struct : standed for [ox::empty_type]  for example, std::string ....

*/

/// must be value type
template <typename t> struct cate
{
	typedef typename
		iff <is_class<t>::value,ox::empty_type,int>::type
	type;
};

template <typename t> struct cate <t&> { typedef int& type; };
template <typename t> struct cate <t*> { typedef int* type; };
template <> struct cate <char> { typedef int type; };
template <> struct cate <wchar_t> { typedef int type; };
template <> struct cate <short> { typedef int type; };
template <> struct cate <int> { typedef int type; };
template <> struct cate <float> { typedef int type; };
template <> struct cate <double> { typedef int type; };
template <> struct cate <long> { typedef int type; };
template <> struct cate <long double> { typedef int type; };
template <> struct cate <long long> { typedef int type; };
template <> struct cate <unsigned char> { typedef int type; };
template <> struct cate <unsigned short> { typedef int type; };
template <> struct cate <unsigned int> { typedef int type; };
template <> struct cate <unsigned long> { typedef int type; };
template <> struct cate <unsigned long long> { typedef int type; };


/**
	@brief 获取对类型t的初始化参数
*/

template <typename t> struct param { typedef t const& type;};
template <typename t> struct param <t&> { typedef t& type;};
template <> struct param <void> { typedef void type;};


___namespace2_end()

/*
	history:
	2011-3-6: add clone_ref, modify the clone_ptr
 */

