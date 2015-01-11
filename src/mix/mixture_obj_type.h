

/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../met/_if_switch.h"
#include "../cxx/tags.h"



#pragma once



___namespace2_begin(ox,mix)



/// ox::owner --> this
/// void --> ox::empty_type /// when the data has void type

template <typename tulitem,typename ow>
struct type_for_instantiate
{
	typedef typename tulitem::_t_type _t_type;
	typedef ow owner;
	typedef typename
		met::iff
		<
			met::is_void<_t_type>::value,
			ox::identity<ox::empty_type>,
			met::iff
			<
				met::is_same<_t_type,ox::tags::owner>::value,
				owner*,
				_t_type
			>
		>
		::type::type
	type;
};




___namespace2_end()
