

/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "mixture_unitize.h"



#pragma once



___namespace2_begin(ox,mix)



/// tui: mixture tinf
template
<
	typename ssl,
	typename sl,
	typename uz = unitize,
	typename ms = void,
	typename edi = void
>
struct tinf;



___namespace2_end()
