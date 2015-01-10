
/**
	对类型函数包装成lambda
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com
 */

#include "../ox/nsab.h"
#include "../ox/tclasses.h"


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


///struct arg_null;

template <typename func_tn,typename default_param=ox::null_type>
struct lambda;


___namespace2_end()