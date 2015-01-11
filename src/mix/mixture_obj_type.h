

/**
	2010-11-10
	���ߣ��캣��
	����һ�����Ӽ򵥵ĸ��������loki�µ�typelist
	����������ԭ����mixture������ԭ�����Ǹ�mixture�ǻ��������͵ģ��ܸ��ӣ��������
	����������д��һ������򵥵ĵ��������Ƚ�����
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
