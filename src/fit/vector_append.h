


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
		size_t capacity = head().capacity();
		size_t used = head().size();
		void* next = (void*)head().next();
		size_t vsize = value_size()(value);
		value_pointer vp;
		switch (used+vsize>capacity?true:false)
		{
		case true:
			vp = on_fail(*this,value);
			break;
		case false:
			vp = value_construct()(next,value);
			head().add_size(vsize);
			break;
		}
		return vp;
	}
};

/// construct(ptr,value,off,size)
template <typename head_tn,typename space_trait=space_trait_tt<type_tn>>
struct loop_vector_append : head_tn
{
	typedef typename space_trait::value_pointer value_pointer;
	typedef typename space_trait::value_type value_type;
	typedef typename space_trait::value_size value_size;
	typedef typename space_trait::value_construct value_construct;

	head_tn& head() {return *this;}
	void clear()
	{
		head().clear();
	}
	/// the value supports partly construct
	value_pointer append(value_type const& value)
	{
		size_t capacity = head().capacity();
		size_t used = head().offset();
		void* next = head().next();
		size_t vsize = value_size()(value);
		value_pointer vp;
		if (used+vsize<=capacity)
		{
			vp = value_construct()(next,value,0,vsize);
			head().add_size(vsize);
		}
		else if (vsize<capacity)
		{
			size_t former_size = capacity-used;
			vp = value_construct()(next,value,0,former_size);
			value_construct()(head().data_begin(),value,former_size,vsize-former_size);
			head().add_size(vsize);
		}
		else /// vsize is longer than the capacity
		{
			vp = value_construct()(head().data_begin(),value,vsize-capacity,capacity);
			head().clear();
			head().add_size(capacity);
		}
		return vp;
	}
};

/// head
template <typename atom_tn,size_t tail_size_reserved>
struct space_append_head_mono_tt : protected vector_head_mono_tt<atom_tn>
{
	typedef atom_tn atom_type;
	typedef vector_head_mono_tt<atom_type> base;
	bool attach(ox::utl::data_t const& buff)
	{
		static size_t const __byte_size_meta = __head_size+tail_size_reserved*sizeof(atom_type);
		assert(buff.size>=__byte_size_meta);
		if (buff.size<__byte_size_meta) return false;
		set_data_begin((atom_type*)(buff.begin+__head_size));
		set_capacity((buff.size-__byte_size_meta)/sizeof(atom_type));
		return true;
	}
	void detach()
	{
		set_data_begin((char*)(0)+__head_size);
		set_capacity(0);
		set_size(0);
	}
	void add_size(size_t delta)
	{
		assert(delta+base::size()<=capacity());
		set_size(base::size()+delta);
	}
	void clear()
	{
		set_size(0);
	}
	atom_type const* next() const
	{
		return data_begin()+base::size();
	}
};

template <typename type_tn>
struct space_trait_tt
{
	typedef type_tn char_type;
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef value_type value_pointer;
	typedef ox::utl::cdata_tt<type_tn> value_const;
	struct value_size
	{
		size_t operator()(value_type const& value)
		{
			return value.size;
		}
	};
	struct value_construct
	{
		value_pointer operator()(void* buff,value_const const& value)
		{
			if (value.begin)
				memmove(buff,value.begin,value.size*sizeof(type_tn));
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


template <typename type_tn,size_t tail_size_reserved=0,typename space_trait=space_trait_tt<type_tn>>
struct buffer_append
	: vector_append<space_append_head_mono_tt<type_tn,tail_size_reserved>,space_trait>
{
	typedef vector_append<space_append_head_mono_tt<type_tn,tail_size_reserved>,space_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size));
	}
	type_tn* data() const
	{
		return (type_tn*)(data_begin());
	}
};

template <typename type_tn>
struct string_space_trait_tt
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef ox::utl::data_tt<type_tn> value_pointer;
	struct value_size
	{
		size_t operator()(value_type const& value)
		{
			return value.size;
		}
	};
	/// construct partly
	struct value_construct
	{
		value_pointer operator()(void* buff,value_type const& value)
		{
			if (value.begin)
				memmove(buff,value.begin,value.size*sizeof(type_tn));
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
	: vector_append<space_append_head_mono_tt<type_tn,1>,space_trait>
{
	typedef vector_append<space_append_head_mono_tt<type_tn,1>,space_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size));
	}
	void clear()
	{
		base::clear();
		append(0,0);
	}
	type_tn* data() const
	{
		return (type_tn*)(data_begin());
	}
};


/// loop structure
template <typename char_tn,size_t tail_size_reserved>
struct loop_space_append_head_mono_tt : protected space_append_head_mono_tt<char_tn,tail_size_reserved>
{
	typedef space_append_head_mono_tt<char_tn,tail_size_reserved> base;
	typedef char_tn char_type;
	using base::attach;
	using base::detach;
	using base::clear;
	char_type* next()
	{
		return data_begin()+offset();
	}
	size_t offset() const
	{
		return base::size()%base::capacity();
	}
	size_t size() const
	{
		return base::size()>base::capacity()?base::capacity():base::size();
	}
	void add_size(size_t delta)
	{
		size_t size = base::size()+delta;
		if (size>base::capacity())
			size = size%base::capacity()+base::capacity();
		base::set_size(size);
	}
	char_type const* data() const
	{
		return (char_type const*)base::data_begin();
	}
};
template <typename type_tn>
struct loop_buffer_space_trait_tt
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef ox::utl::data_tt<type_tn> value_pointer;
	struct value_size
	{
		size_t operator()(value_type const& value)
		{
			return value.size;
		}
	};
	/// construct partly
	struct value_construct
	{
		value_pointer operator()(void* buff,value_type const& value,size_t off,size_t size)
		{
			if (off>value.size) return value_pointer();
			if (off+size>value.size)
				size = value.size-off;
			if (value.begin)
				memmove(buff,value.begin+off,size*sizeof(type_tn));
			return value_pointer((type_tn*)buff,size);
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

template <typename type_tn,typename space_trait=loop_buffer_space_trait_tt<type_tn>>
struct loop_buffer_append
	: loop_vector_append<loop_space_append_head_mono_tt<type_tn,0>,space_trait>
{
	typedef loop_vector_append<loop_space_append_head_mono_tt<type_tn,0>,space_trait> base;
	void clear()
	{
		base::clear();
		append(0,0);
	}
	value_pointer append(type_tn const* begin,size_t size)
	{
		return base::append(value_type((type_tn*)begin,size));
	}
};


template <typename type_tn>
struct loop_string_space_trait_tt
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef ox::utl::data_tt<type_tn> value_pointer;
	struct value_size
	{
		size_t operator()(value_type const& value)
		{
			return value.size;
		}
	};
	/// construct partly
	struct value_construct
	{
		value_pointer operator()(void* buff,value_type const& value,size_t off,size_t size)
		{
			if (off>value.size) return value_pointer();
			if (off+size>value.size)
				size = value.size-off;
			if (value.begin)
				memmove(buff,value.begin+off,size*sizeof(type_tn));
			*((type_tn*)buff+size) = 0;
			return value_pointer((type_tn*)buff,size);
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

template <typename type_tn,typename space_trait=loop_string_space_trait_tt<type_tn>>
struct loop_string_append
	: loop_vector_append<loop_space_append_head_mono_tt<type_tn,1>,space_trait>
{
	typedef loop_vector_append<loop_space_append_head_mono_tt<type_tn,1>,space_trait> base;
	void clear()
	{
		base::clear();
		append(0,0);
	}
	value_pointer append(type_tn const* begin,size_t size)
	{
		return base::append(value_type((type_tn*)begin,size));
	}
};



___namespace2_end()