
/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 2013-2-27
 */

#include "../ox/nsab.h"
#include "../met/lambda.h"
#include "./mixture_unit_decl.h"



#pragma once




___namespace2_begin(ox,mix)



struct unitize
{
	//typedef instantiate_of_type<void,0,0,void> data_unit;

	typedef mixture_data<void,0,0,0,void> data_unit;
	typedef method_default<void,0,0,0,void> method_unit;
	typedef data_method_default<void,0,0,0,void> data_method_unit;
	typedef extern_method_default<void,0,0,0,void> extern_method_unit;
	/// property unit
	/// event unit
};



___namespace2_end()


