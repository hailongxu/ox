
/**
	2008-12-7
	作者：徐海龙
	单独提出元树进行设计，并提供元方法，让元树的其它操作都建立在这个基础上
	2009-8-23
	作者：徐海龙
	提出了类型容器的概念，并且容器需要具备的特征
*/

#include "../ox/nsab.h"
#include "../ox/def_macro.h"
#include "../ox/nothing.h"
#include "../ox/tclasses.h"



#pragma once


___namespace2_begin(ox,met)



/// these function just to overcome the bug of T(T1), 
/// when T1 is const int, the complie will convert T1 into int, others do not, xxxxx0000____000
template <typename type_tn>
struct keepit
	: identity<type_tn>
{};


template <typename type_tn> struct takeit
{
	typedef type_tn type;
};

template <typename type_tn> struct takeit <keepit<type_tn>>
{
	typedef type_tn type;
};



/// c function，or, class member function
/// param_amt: param_amount
template <typename cfunc>
struct infof_func
{
	/// sorry, we donot support
	typedef void base;
	typedef cfunc func_type;
	typedef void return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 0; };
	template <size_t> struct param;
	typedef void begin_type;
	typedef void end_type;
	typedef boolean<false> is;
};

/// R()
template <typename r>
struct infof_func <r()>
{
	typedef void base;
	typedef r func_type();
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 0; };
	template <size_t> struct param;
	template <> struct param<0> { typedef void type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<0>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(); };
};
/// R(*)()
template <typename r>
struct infof_func <r(*)()>
	: infof_func <r()>
{};
/// R(&)()
template <typename r>
struct infof_func <r(&)()>
	: infof_func <r()>
{};
/// R(C::*)()
template <typename c,typename r>
struct infof_func <r(c::*)()>
	: infof_func <r()>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)();
};


/// R(P)
template <typename r, typename p1>
struct infof_func <r(p1)>
	: infof_func <r()>
{
	typedef infof_func <r()> base;
	typedef r func_type(p1);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 1; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p1>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1); };
};
/// R(*)(P)
template <typename r, typename p1>
struct infof_func <r(*)(p1)>
	: infof_func <r(p1)>
{};
/// R(&)(P)
template <typename r, typename p1>
struct infof_func <r(&)(p1)>
	: infof_func <r(p1)>
{};
/// R(C::*)(P)
template <typename c,typename r, typename p1>
struct infof_func <r(c::*)(p1)>
	: infof_func <r(p1)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1);
};

/// R(P1,P2)
template <typename r, typename p1, typename p2>
struct infof_func <r(p1,p2)>
	: infof_func <r(p1)>
{
	typedef infof_func <r(p1)> base;
	typedef r func_type(p1,p2);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 2; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p2>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2); };
};
/// R(*)(P1,P2)
template <typename r, typename p1, typename p2>
struct infof_func <r(*)(p1,p2)>
	: infof_func <r(p1,p2)>
{};
/// R(&)(P1,P2)
template <typename r, typename p1, typename p2>
struct infof_func <r(&)(p1,p2)>
	: infof_func <r(p1,p2)>
{};
/// R(&)(P1,P2)
template <typename c, typename r, typename p1, typename p2>
struct infof_func <r(c::*)(p1,p2)>
	: infof_func <r(p1,p2)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2);
};

/// R(P1,P2,P3)
template <typename r, typename p1, typename p2, typename p3>
struct infof_func <r(p1,p2,p3)>
	: infof_func <r(p1,p2)>
{
	typedef infof_func <r(p1,p2)> base;
	typedef r func_type(p1,p2,p3);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 3; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p3>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3); };
};
/// R(*)(P1,P2,P3)
template <typename r, typename p1, typename p2, typename p3>
struct infof_func <r(*)(p1,p2,p3)>
	: infof_func <r(p1,p2,p3)>
{};
/// R(&)(P1,P2,P3)
template <typename r, typename p1, typename p2, typename p3>
struct infof_func <r(&)(p1,p2,p3)>
	: infof_func <r(p1,p2,p3)>
{};
/// R(C::*)(P1,P2,P3)
template <typename c, typename r, typename p1, typename p2, typename p3>
struct infof_func <r(c::*)(p1,p2,p3)>
	: infof_func <r(p1,p2,p3)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3);
};

/// R(P1,P2,P3,P4)
template <typename r, typename p1, typename p2, typename p3, typename p4>
struct infof_func <r(p1,p2,p3,p4)>
	: infof_func <r(p1,p2,p3)>
{
	typedef infof_func <r(p1,p2,p3)> base;
	typedef r func_type(p1,p2,p3,p4);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 4; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p4>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4); };
};
/// R(*)(P1,P2,P3,P4)
template <typename r,typename p1,typename p2,typename p3,typename p4>
struct infof_func <r(*)(p1,p2,p3,p3,p4)>
	: infof_func <r(p1,p2,p3,p3,p4)>
{};
/// R(&)(P1,P2,P3,P4)
template <typename r,typename p1,typename p2,typename p3,typename p4>
struct infof_func <r(&)(p1,p2,p3,p3,p4)>
	: infof_func <r(p1,p2,p3,p3,p4)>
{};
/// R(c::*)(P1,P2,P3,P4)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4)>
	: infof_func <r(p1,p2,p3,p3,p4)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4);
};

/// R(P1,P2,P3,P4,P5)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5>
struct infof_func <r(p1,p2,p3,p3,p4,p5)>
	: infof_func <r(p1,p2,p3,p4)>
{
	typedef infof_func <r(p1,p2,p3,p4)> base;
	typedef r func_type(p1,p2,p3,p4,p5);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 5; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p5>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4,p5); };
};
/// R(*)(P1,P2,P3,P4,P5)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5>
struct infof_func <r(*)(p1,p2,p3,p3,p4,p5)>
	: infof_func <r(p1,p2,p3,p3,p4,p5)>
{};
/// R(&)(P1,P2,P3,P4,P5)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5>
struct infof_func <r(&)(p1,p2,p3,p3,p4,p5)>
	: infof_func <r(p1,p2,p3,p3,p4,p5)>
{};
/// R(C::*)(P1,P2,P3,P4,P5)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4,typename p5>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4,p5)>
	: infof_func <r(p1,p2,p3,p3,p4,p5)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4,p5);
};

/// R(P1,P2,P3,P4,P5,P6)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct infof_func <r(p1,p2,p3,p3,p4,p5,p6)>
	: infof_func <r(p1,p2,p3,p3,p4,p5)>
{
	typedef infof_func <r(p1,p2,p3,p3,p4,p5)> base;
	typedef r func_type(p1,p2,p3,p4,p5,p6);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 6; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p6>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4,p5,p6); };
};
/// R(*)(P1,P2,P3,P4,P5,P6)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct infof_func <r(*)(p1,p2,p3,p3,p4,p5,p6)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6)>
{};
/// R(&)(P1,P2,P3,P4,P5,P6)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct infof_func <r(&)(p1,p2,p3,p3,p4,p5,p6)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6)>
{};
/// R(*)(P1,P2,P3,P4,P5,P6)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4,p5,p6)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4,p5,p6);
};

/// R(P1,P2,P3,P4,P5,P6,P7)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6)>
{
	typedef infof_func <r(p1,p2,p3,p3,p4,p5,p6)> base;
	typedef r func_type(p1,p2,p3,p4,p5,p6,p7);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 7; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p7>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4,p5,p6,p7); };
};
/// R(*)(P1,P2,P3,P4,P5,P6,P7)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct infof_func <r(*)(p1,p2,p3,p3,p4,p5,p6,p7)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)>
{};
/// R(&)(P1,P2,P3,P4,P5,P6,P7)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct infof_func <r(&)(p1,p2,p3,p3,p4,p5,p6,p7)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)>
{};
/// R(C::*)(P1,P2,P3,P4,P5,P6,P7)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4,p5,p6,p7)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4,p5,p6,p7);
};

/// R(P1,P2,P3,P4,P5,P6,P7,P8)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)>
{
	typedef infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7)> base;
	typedef r func_type(p1,p2,p3,p4,p5,p6,p7,p8);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 8; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p8>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4,p5,p6,p7,p8); };
};
/// R(*)(P1,P2,P3,P4,P5,P6,P7,P8)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct infof_func <r(*)(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
{};
/// R(&)(P1,P2,P3,P4,P5,P6,P7,P8)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct infof_func <r(&)(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
{};
/// R(C::*)(P1,P2,P3,P4,P5,P6,P7,P8)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4,p5,p6,p7,p8);
};

/// R(P1,P2,P3,P4,P5,P6,P7,P8,P9)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8,typename p9>
struct infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)>
{
	typedef infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8)> base;
	typedef r func_type(p1,p2,p3,p4,p5,p6,p7,p8,p9);
	typedef r return_type;
	typedef void class_type;
	struct param_amt { static size_t const value = 9; };
	template <size_t suff> struct param { typedef typename base::param<suff>::type type; };
	template <> struct param<param_amt::value-1> { typedef typename takeit<p9>::type type; };
	typedef typename param<0>::type begin_type;
	typedef typename param<param_amt::value-1>::type end_type;
	typedef boolean<true> is;
	template <typename class_tn> struct mkmf { typedef r(class_tn::*type)(p1,p2,p3,p4,p5,p6,p7,p8,p9); };
};
/// R(*)(P1,P2,P3,P4,P5,P6,P7,P8,P9)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8,typename p9>
struct infof_func <r(*)(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
{};
/// R(&)(P1,P2,P3,P4,P5,P6,P7,P8,P9)
template <typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8,typename p9>
struct infof_func <r(&)(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
{};
/// R(C::*)(P1,P2,P3,P4,P5,P6,P7,P8,P9)
template <typename c,typename r,typename p1,typename p2,typename p3,typename p4,typename p5,typename p6,typename p7,typename p8,typename p9>
struct infof_func <r(c::*)(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
	: infof_func <r(p1,p2,p3,p3,p4,p5,p6,p7,p8,p9)>
{
	typedef c class_type;
	typedef r(c::*mfunc_type)(p1,p2,p3,p4,p5,p6,p7,p8,p9);
};

___namespace2_end()