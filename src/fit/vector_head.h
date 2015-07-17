#include <assert.h>
#include "../ox/nsab.h"
#include "../utl/data_t.h"




___namespace2_begin(ox,fit)


struct vector_sizes
{
	vector_sizes() : _m_size(0), _m_capacity(0) {}
	size_t _m_size;
	size_t _m_capacity;
};

template <typename value_tn>
struct vector_head_iso_tt : private vector_sizes
{
	vector_head_iso_tt(): _m_data_begin(0) {}
	void set_size(size_t size) {_m_size=size;}
	void set_capacity(size_t capacity) {_m_capacity=capacity;}
	void set_data_begin(value_tn* data_begin) {_m_data_begin=data_begin;}
	size_t size() const {return _m_size;}
	size_t capacity() const {return _m_capacity;}
	value_tn* data_begin() {return _m_data_begin;};
	size_t total_bytes() const {return _m_capacity*sizeof(value_tn);}
	value_tn* _m_data_begin;
};
template <typename value_tn>
struct vector_head_mono_tt
{
	vector_head_mono_tt() : _m_vector_begin(0) {}
	void set_size(size_t size)
	{
		assert(!is_null());
		sizes()->_m_size=size;
	}
	void set_capacity(size_t capacity)
	{
		assert(!is_null());
		sizes()->_m_capacity=capacity;
	}
	void set_data_begin(value_tn* data_begin)
	{
		_m_vector_begin=(char*)data_begin-__head_size;
	}
	size_t size() const
	{
		return is_null()?0:sizes()->_m_size;
	}
	size_t capacity() const
	{
		return is_null()?0:sizes()->_m_capacity;
	}
	value_tn* data_begin()
	{
		return (value_tn*)((char*)(_m_vector_begin)+sizeof(vector_sizes));
	};
	vector_sizes* sizes() const
	{
		return (vector_sizes*)_m_vector_begin;
	}
	size_t total_bytes() const
	{
		return is_null()?0:sizeof(vector_sizes)+capacity()*sizeof(value_tn);
	}
	bool is_null() const
	{
		return _m_vector_begin==0;
	}
	static size_t const __head_size = sizeof(vector_sizes);
	char* _m_vector_begin;
};


___namespace2_end()