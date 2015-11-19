
#include <assert.h>
#include "../ox/nsab.h"
#include "../met/mod.h"
#include "../utl/data_t.h"
#include "allocator_default.h"
#include "vector_head.h"
#include "cpmv.h"
#include "vector.h"
#include "value_trait.h"


/*
vector 的设计分成两个部分,
第一部分:头设计,主要掌管,整体的变化,如整体的长度,整体的内存的删除和重建工作
第二部分:体设计,主要掌管,元素引起的变化,如一个元素的增加设删除所引起的数据的重建工作
*/

#pragma once

___namespace2_begin(ox,fit)





template<typename array,size_t added,typename vector_events_tn=void>
struct static_vector_rooter;

template<typename value_tn,size_t n,size_t added,typename vector_events_tn>
struct static_vector_rooter<value_tn[n],added,vector_events_tn>
{
	friend vector_tt<static_vector_rooter>;
	typedef typename value_trait<value_tn>::value_type value_type;
	typedef typename value_trait<value_tn>::value_construct value_construct;
	typedef value_tn array_type[n+added];

	//friend vector_on_size_changed;
	//friend string_on_size_changed;
protected:
	typedef static_vector_rooter self;
	typedef typename ox::fit::get_on_size_changed<vector_events_tn>::type on_size_changed;
	struct head_t
	{
		head_t(): _m_size(0) {}
		void set_size(size_t size) {_m_size=size;}
		size_t size() const {return _m_size;}
		size_t _m_size;
		array_type _m_data;
	};
	~static_vector_rooter()
	{
		_m_head._m_size = 0;
	}
	value_type* data_begin()
	{
		return _m_head._m_data;
	}
	head_t head() {return _m_head;}
	head_t _m_head;
	void add_size(int delta)
	{
		_m_head._m_size += delta;
	}
public:
	void swap(self& other)
	{
		assert (false && "not support");
	}
	size_t size() const
	{
		return _m_head._m_size;
	}
	size_t capacity() const
	{
		return n;
	}
	void reserve(size_t size,size_t size_copyed=-1)
	{
	}
	void resize(size_t size)
	{
		if (size<=_m_head._m_size)
		{
			vector_helper::destruct_objects(_m_head._m_data+size,_m_head._m_size-size);
			_m_head._m_size = size;
		}
		else if (size<capacity())
		{
			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		else
		{
			reserve(size-_m_head._m_size);
			vector_helper::construct_objects(_m_head._m_data+_m_head._m_size,size-_m_head._m_size);
			_m_head._m_size = size;
		}
		on_size_changed()(*this,0);
	}
};
//
//template<typename array,typename string_events_tn=string_event>
//struct static_string_rooter;
//template<typename value_tn,size_t n,typename string_events_tn>
//struct static_string_rooter<value_tn[n],string_events_tn>
//	: static_vector_rooter<value_tn[n],1,string_events_tn>
//{
//	friend vector_tt<static_string_rooter>;
//	typedef static_string_rooter self;
//	self()
//	{
//		*data_begin()=0;
//	}
//};
//
template <typename array_tn>
struct static_vector: vector_tt<static_vector_rooter<array_tn,0,void>>
{};
//
//template <typename array_tn>
//struct static_string: vector_tt<static_string_rooter<array_tn,string_event>>
//{};
//
//


/// vector=>{head,allcotor}
/// head=>{size,capacity,data_begin}
/// data_begin=>{value,value,,,,}


___namespace2_end()
