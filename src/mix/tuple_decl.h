
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"


#pragma once


___namespace2_begin(ox,mix)



/**
	tuple, a different type list
*/



template
<
	typename t1=void,
	typename t2=void,
	typename t3=void,
	typename t4=void,
	typename t5=void,
	typename t6=void,
	typename t7=void,
	typename t8=void
>
struct tuple;


template <size_t count>
struct tup;





___namespace2_end()