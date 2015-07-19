


#include <assert.h>
#include "../ox/nsab.h"
#include "../utl/data_t.h"




___namespace2_begin(ox,fit)


template <typename head_tn,typename value_trait>
struct vector_append : head_tn
{
	typedef head_tn head_t;
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::value_size value_size;
	typedef typename value_trait::value_construct value_construct;
	typedef typename value_trait::pointer_is_null pointer_is_null;

	head_t& head() {return *this;}
	head_t const& head() const {return *this;}
	bool is_empty() const
	{
		return head().is_null()||head().used()==0;
	}
	struct on_event_fail
	{
		template<typename t1,typename t2>
		value_pointer operator()(t1 const& p1,t2 const& p2)
		{
			return value_pointer();
		}
	};
	value_pointer append(value_type const& value)
	{
		static on_event_fail on_fail;
		return append(value,on_fail);
	}
	template <typename on_fail_tn>
	value_pointer append(value_type const& value,on_fail_tn& on_fail)
	{
		size_t total = head().capacity();
		size_t used = head().size();
		char* next = head().next();
		size_t vsize = value_size()(value);
		value_pointer vp;
		switch (used+vsize>total?true:false)
		{
		case true:
			vp = on_fail(*this,value);
			break;
		case false: 
			vp = value_construct()(next,value);
			head().set_size(used+vsize);
			break;
		}
		return vp;
	}
};

struct buffer_append_head_mono_tt : protected vector_head_mono_tt<char>
{
	typedef vector_head_mono_tt<char> base;
	void attach(ox::utl::data_t const& buff)
	{
		set_data_begin(buff.begin+__head_size);
		set_capacity(buff.size-__head_size);
	}
	void detach()
	{
		set_data_begin((char*)(0)+__head_size);
		set_capacity(0);
		set_size(0);
	}
	char* next()
	{
		return data_begin()+base::size();
	}
};
template <typename char_tn>
struct string_append_head_mono_tt : protected vector_head_mono_tt<char>
{
	typedef vector_head_mono_tt<char> base;
	typedef char_tn char_type;
	bool attach(ox::utl::data_t const& buff)
	{
		assert(buff.size>=__head_size+1);
		if (buff.size<__head_size+1) return false;
		set_data_begin(buff.begin+__head_size);
		set_capacity(buff.size-__head_size-1);
		return true;
	}
	void detach()
	{
		set_data_begin((char*)(0)+__head_size);
		set_capacity(0);
		set_size(0);
	}
	char* next()
	{
		return data_begin()+base::size();
	}
	void clear()
	{
		base::set_size(0);
	}
};

template <typename type_tn>
struct space_trait_tt
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef value_type value_pointer;
	typedef ox::utl::cdata_tt<type_tn> value_const;
	struct value_size {size_t operator()(value_type const& value) {return value.size;}};
	struct value_construct
	{
		value_pointer operator()(char* buff,value_const const& value)
		{
			if (value.begin)
				memmove(buff,value.begin,value.size);
			return value_pointer((type_tn*)buff,value.size);
		}
	};
	struct pointer_is_null
	{
		bool operator()(value_pointer const& p)
		{
			return p.is_empty();
		}
	};
};


template <typename type_tn,typename space_trait=space_trait_tt<type_tn>>
struct buffer_append
	: vector_append<buffer_append_head_mono_tt,space_trait>
{
	typedef vector_append<buffer_append_head_mono_tt,space_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;
	void clear()
	{
		base::head().set_size(0);
	}
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size));
	}
};

template <typename type_tn>
struct string_space_trait_tt
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef ox::utl::data_tt<type_tn> value_pointer;
	struct value_size {size_t operator()(value_type const& value) {return value.size;}};
	struct value_construct
	{
		value_pointer operator()(char* buff,value_type const& value)
		{
			if (value.begin)
				memmove(buff,value.begin,value.size);
			*((type_tn*)buff+value.size) = 0;
			return value_pointer((type_tn*)buff,value.size);
		}
	};
	struct pointer_is_null
	{
		bool operator()(value_pointer const& p)
		{
			return p.is_empty();
		}
	};
};


template <typename type_tn,typename space_trait=string_space_trait_tt<type_tn>>
struct string_append
	: vector_append<string_append_head_mono_tt<type_tn>,space_trait>
{
	typedef vector_append<string_append_head_mono_tt<type_tn>,space_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size));
	}
};

template <typename char_tn>
struct loop_string_append_head_mono_tt : protected vector_head_mono_tt<char>
{
	typedef vector_head_mono_tt<char> base;
	typedef char_tn char_type;
	bool attach(ox::utl::data_t const& buff)
	{
		assert(buff.size>=__head_size+1);
		if (buff.size<__head_size+1) return false;
		set_data_begin(buff.begin+__head_size);
		set_capacity(buff.size-__head_size-1);
		return true;
	}
	void detach()
	{
		set_data_begin((char*)(0)+__head_size);
		set_capacity(0);
		set_size(0);
	}
	char* next()
	{
		return data_begin()+base::size();
	}
	void clear()
	{
		base::set_size(0);
	}
	size_t size() const
	{
		return base::size()%base::capacity();
	}
	void set_size(size_t size)
	{
		if (base::size()>base::capacity())
			size += base::capacity();
		if (size>base::capacity())
			size = size%base::capacity()+base::capacity();
		base::set_size(size);
	}
	char_type const* data() const
	{
		return base::data_begin();
	}
};
template <typename type_tn,typename space_trait=string_space_trait_tt<type_tn>>
struct loop_string_append
	: vector_append<loop_string_append_head_mono_tt<type_tn>,space_trait>
{
	typedef vector_append<loop_string_append_head_mono_tt<type_tn>,space_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;

	struct on_fail
	{
		template <typename value_tn>
		value_pointer operator()(base& vec,value_tn& space)
		{
			size_t capacity = vec.head().capacity();
			size_t size = vec.head().size();
			if (value_size()(space)>=capacity)
			{
				vec.head().clear();
				return vec.append(data_t(space.begin+space.size-capacity,capacity));
			}
			size_t first = capacity-size;
			vec.append(data_t(space.begin,first));
			return vec.append(data_t(space.begin+first,space.size-first));
		}
	};
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size),on_fail());
	}
};





___namespace2_end()