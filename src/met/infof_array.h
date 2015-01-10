
/**
	2008-12-7
	作者：徐海龙
	单独提出元树进行设计，并提供元方法，让元树的其它操作都建立在这个基础上
	2009-8-23
	作者：徐海龙
	提出了类型容器的概念，并且容器需要具备的特征
*/

#include "../ox/nsab.h"
#include "../ox/nothing.h"


#pragma once

___namespace2_begin(ox,met)



/// arr info
template <typename t>
struct inf_of_arr
{
	typedef ox::null_type type;
	static size_t const value = 0;
};
template <typename t, size_t n>
struct inf_of_arr <t[n]>
{
	typedef t type;
	static size_t const value = n;
};



___namespace2_end()