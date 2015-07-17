


#include <assert.h>
#include "../ox/nsab.h"
#include "../utl/data_t.h"




___namespace2_begin(ox,fit)


template <typename head_tn,typename value_trait,typename vector_trait>
struct vector_append : head_tn
{
	typedef head_tn head_t;
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::value_size value_size;
	typedef typename value_trait::value_construct value_construct;
	typedef typename value_trait::pointer_is_null pointer_is_null;
	typedef typename vector_trait::on_size_changed on_size_changed;

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
		char* begin = head().data_begin();
		char* next = begin+used;
		size_t vsize = value_size()(value);
		value_pointer vp;
		switch (used+vsize>total?true:false)
		{
		case true:
			vp = on_fail(this,value);
			break;
		case false: 
			vp = value_construct()(next,value);
			head().set_size(used+vsize);
			break;
		}
		if (!pointer_is_null()(vp)) on_size_changed()(*this,vsize);
		return vp;
	}
};

struct buffer_append_head_mono_tt : protected vector_head_mono_tt<char>
{
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
			//if (value.is_empty())
				return value_pointer((type_tn*)buff,value.size);
			//memmove()
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

struct vector_append_trait
{
	struct on_size_changed
	{
		template <typename vector_tn>
		void operator()(vector_tn& vec,size_t size_appended)
		{
		}
	};
};
template <typename type_tn,typename space_trait=space_trait_tt<type_tn>>
struct buffer_append
	: vector_append<buffer_append_head_mono_tt,space_trait,vector_append_trait>
{
	typedef vector_append<buffer_append_head_mono_tt,space_trait,vector_append_trait> base;
	typedef ox::utl::data_tt<type_tn> data_t;
	void attach(ox::utl::data_t const& buff)
	{
		base::head().set_data_begin(buff.begin+head_t::__head_size);
		base::head().set_capacity(buff.size-head_t::__head_size);
	}
	void detach()
	{
		base::head().set_data_begin(0);
		base::head().set_capacity(0);
		base::head().set_size(0);
	}
	void clear()
	{
		base::head().set_size(0);
	}
	value_pointer append(type_tn const* data,size_t size)
	{
		return base::append(data_t((type_tn*)data,size));
	}
};

//struct string_append_trait
//{
//	struct on_size_changed
//	{
//		template <typename vector_tn>
//		void operator()(vector_tn& vec,size_t size_appended)
//		{
//			vec.head().data_begin()+
//		}
//	};
//};

template <typename type_tn>
struct string_space_trait_tt
{
	typedef size_t value_type;
	typedef ox::utl::data_tt<type_tn> value_pointer;
	struct value_size {size_t operator()(value_type const& value) {return value;}};
	struct value_construct
	{
		value_pointer operator()(char* buff,value_type const& value)
		{
			*((type_tn*)buff+value) = 0;
			return value_pointer((type_tn*)buff,value);
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
	: buffer_append<type_tn,space_trait>
{
};




//template <typename type_tn>
//struct loop_buffer_append
//	: vector_append<buffer_append_head_mono_tt,space_trait<type_tn>>
//{
//	typedef vector_append<buffer_append_head_mono_tt,space_trait<type_tn>> base;
//	typedef ox::utl::cdata_tt<type_tn> cdata_t;
//	void attach(ox::utl::data_t const& buff)
//	{
//		base::head().set_data_begin(buff.begin+head_t::__head_size);
//		base::head().set_capacity(buff.size-head_t::__head_size);
//	}
//	void clear()
//	{
//		base::head().set_size(0);
//	}
//	value_pointer append(type_tn const* data,size_t size)
//	{
//		return append(cdata_t(data,size));
//	}
//	value_pointer append(cdata_t const& data)
//	{
//		value_pointer vp = base::append(data.size);
//		memmove(vp.begin,data.begin,data.size);
//		return vp;
//	}
//};




___namespace2_end()