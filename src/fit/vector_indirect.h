
#include <assert.h>
#include "../ox/nsab.h"
#include "allocator_default.h"
#include "../utl/data_t.h"
#include "vector_head.h"




___namespace2_begin(ox,fit)



template <typename value_tn>
struct indirect_vector_deleting_event
{
	/// the rooter is vector rooter
	/// value type of vector is node type of indirect vector
	template <typename node_tn>
	void operator()(size_t off,size_t size,node_tn* pnode)
	{
		(*pnode)->~value_tn();
	}
};
template <typename value_tn,typename value_allocator>
struct indirect_vector_inserted_event
{
	/// the rooter is vector rooter
	/// value type of vector is node type of indirect vector
	template <typename node_tn>
	void operator()(size_t off,node_tn* value)
	{
		typedef value_tn value_type;
		(*value) = value_allocator::allocate();
	}
};

template <typename type_tn>
struct value_trait
{
	typedef type_tn value_type;
	typedef value_type* value_pointer;
	typedef value_pointer node_type;
	struct value_size{size_t operator()(value_type const& value) {return sizeof(value);}};
};
template <typename type_tn>
struct space_value_trait
{
	typedef ox::utl::data_tt<type_tn> value_type;
	typedef value_type value_pointer;
	typedef value_pointer node_type;
	struct value_size{size_t operator()(value_type const& value) {return value.size;}};
};

template <typename value_tn>
struct space_indirect_allocator
{
	typedef ox::utl::data_tt<value_tn> value_type;
	typedef value_type value_pointer;
	static value_pointer allocate(value_type const& value)
	{
		value_type v = value;
		v.begin = new char[value.size];
		return v;
	}
	static void deallcoate(value_pointer const& vp)
	{
		delete vp.begin;
	}
};

template <typename value_tn>
struct allocator_trait
{
	typedef cppmalloc index_allocator;
	typedef object_allocator<value_tn> value_allocator;
};
template <typename value_tn>
struct space_allocator_trait
{
	typedef cppmalloc index_allocator;
	typedef space_indirect_allocator<value_tn> value_allocator;
};


template <typename value_trait,typename allocator_trait>
struct indirect_vector_rooter
{
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::node_type node_type;
	typedef typename allocator_trait::index_allocator index_allocator_type;
	typedef typename allocator_trait::value_allocator value_allocator_type;

	typedef cppmalloc_before_more<index_allocator_type,vector_head_mono_tt<node_type>::__head_size> mono_index_allocator_type;
	typedef vector_tt<vector_rooter<node_type,vector_head_mono_tt<node_type>,vector_event,mono_index_allocator_type>> vector_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
	value_pointer allocate_object(value_type const& value)
	{
		return value_allocator_type::allocate(value);
	}
	void resize(size_t size)
	{
		typedef indirect_vector_deleting_event<value_type> deleting_event;
		typedef indirect_vector_inserted_event<value_type,value_allocator_type> inserted_event;
		index().resize<deleting_event,inserted_event>(size);
	}
	void reserve(size_t size)
	{
		index().reserve(size);
	}
	size_t size() const
	{
		return index().size();
	}
	size_t capacity() const
	{
		return index().capacity();
	}
};
template <typename value_tn,typename node_array,typename allocator_tn>
struct static_indirect_vector_rooter
{
	typedef value_tn value_type;
	typedef value_type* value_pointer;
	typedef static_vector<node_array> vector_type;
	typedef typename vector_type::value_type node_type;
	typedef allocator_tn allocate_type;
	vector_type _m_index;
	vector_type& index() {return _m_index;}
	value_pointer allocate_object(value_type const& value)
	{
		return allocate_type().allocate(value);
	}
};

template <typename indirect_vector_rooter>
struct indirect_vector_tt: indirect_vector_rooter
{
	typedef indirect_vector_rooter rooter_type;
	typedef typename rooter_type::node_type node_type;
	typedef typename rooter_type::value_type value_type;
	typedef typename rooter_type::value_pointer value_pointer;
	rooter_type& rooter() {return *this;}
	rooter_type const& rooter() const {return *this;}
	node_type* insert(size_t off,value_type const& value)
	{
		value_pointer pv = rooter().allocate(value);
		return rooter().index().insert(off,pv);
	}
	node_type* push_back(value_type const& value)
	{
		value_pointer pv = rooter().allocate_object(value);
		return rooter().index().push_back(pv);
	}
	void erase(size_t off,size_t size=1)
	{
		rooter().index().erase(off,size,indirect_vector_deleting_event<value_type>());
	}
	template <typename act_tn>
	struct indirect_act
	{
		act_tn& _m_act;
		indirect_act(act_tn& act): _m_act(act) {}
		bool operator()(node_type& node)
		{
			return _m_act(*node);
		}
	};
	/// act(node)
	template <typename action>
	void foreach(action& act)
	{
		rooter().index().foreach(indirect_act<action>(act));
	}
};

template <typename value_tn>
struct indirect_vector: indirect_vector_tt<indirect_vector_rooter<value_trait<value_tn>,allocator_trait<value_tn>>>
{};
template <typename value_tn>
struct variable_indirect_vector: indirect_vector_tt<indirect_vector_rooter<space_value_trait<value_tn>,space_allocator_trait<value_tn>>>
{};
template <typename value_tn,typename node_array,typename allocator_tn=object_allocator<value_tn>>
struct static_indirect_vector: indirect_vector_tt<static_indirect_vector_rooter<value_tn,node_array,allocator_tn>>
{};

template <typename value_tn>
struct mono_value_trait : value_trait<value_tn>
{
	static value_type* construct(char* buff,value_type const& value)
	{
		return (new (buff) value_type(value));
	}
};
template <typename value_tn>
struct space_mono_value_trait : value_trait<value_tn>
{
	static value_pointer construct(char* buff,value_type const& value)
	{
		return value_pointer(buff,value.size);
	}
};
template <typename value_trait,typename allocator_tn=cppmalloc>
struct mono_indirect_vector_rooter
{
	typedef typename value_trait::value_type value_type;
	typedef typename value_trait::value_pointer value_pointer;
	typedef typename value_trait::node_type node_type;
	typedef typename value_trait::value_size value_size;
	typedef allocator_tn allcator_type;
	struct index_whole_size_getter
	{
		size_t _m_bytes_more;
		size_t operator()(size_t size)
		{
			return size+_m_bytes_more;
		}
	};
	struct data_head : vector_head_mono_tt<char>
	{
		void attach(char* vector_begin)
		{
			_m_vector_begin = vector_begin;
		}
	};
	typedef cppmalloc_before_more<allcator_type,vector_head_mono_tt<node_type>::__head_size> mono_index_allocator_type;
	typedef vector_tt<vector_rooter<node_type,vector_head_mono_tt<node_type>,vector_event,mono_index_allocator_type>> index_vector_type;
	typedef vector_rooter<char,data_head,vector_event,nullmalloc> data_vector_type;

	struct data_vector_wrapper : data_vector_type
	{
	public:
		using data_vector_type::data_begin;
		using data_vector_type::~data_vector_type;
		using data_vector_type::allocator;
	};
	struct index_vector_public : index_vector_type
	{
	public:
		using index_vector_type::data_begin;
		using index_vector_type::head;
	};


	index_vector_type _m_index;
	index_vector_type& index() {return _m_index;}

	node_type* vector_data_begin(index_vector_type& index_ref)
	{
		return ((index_vector_public*)(&index_ref))->head().data_begin()+index_ref.capacity();
	}
	value_pointer allocate_object(value_type const& value)
	{
		size_t byte_data_size = value_size()(value);
		reserve(index().size()+1,byte_data_size);
		char* data_begin = (char*)(vector_data_begin(index()));
		data_vector_wrapper data_vector;
		data_vector.attach(data_begin);
		assert(data_vector.capacity()>=data_vector.size()+byte_data_size);

		size_t old_size = data_vector.size();
		data_vector.resize(old_size+byte_data_size);
		return value_trait::construct(data_begin+old_size,value);
	}
	void resize(size_t size)
	{
		typedef indirect_vector_deleting_event<value_type> deleting_event;
		typedef indirect_vector_inserted_event<value_type> inserted_event;
		index().resize<deleting_event,inserted_event>(size);
	}
	void reserve(size_t size,size_t byte_data_size)
	{
		char* data_begin = (char*)(vector_data_begin(index()));
		data_vector_wrapper data_vector;
		data_vector.attach(data_begin);
		size_t usable = data_vector.capacity()-data_vector.size();
		size_t index_usable = index().capacity()-index().size();
		/// whether we need to reallocate the memory
		if (index_usable>=size && usable>=byte_data_size) return;
		size_t index_capacity = index_usable<size?index().size()+size:index().capacity();
		size_t total_data_size = data_vector.total_bytes()+byte_data_size-usable;
		index_whole_size_getter size_getter = {total_data_size};
		index_vector_type index_new;
		index_new.reserve(index_capacity,-1,size_getter);
		/// get the new data vector, copy the old data to the new data
		data_vector_wrapper data_vector_new;
		data_begin = (char*)(vector_data_begin(index_new));
		data_vector_new.attach(data_begin);
		char* value_begin = data_vector_new.data_begin();
		for (size_t i=0;i<index_new.size();++i)
		{
			node_type* node = index_new.push_back(node_type());
			*node = (value_type*)(data_vector_new.allocator().allocate(byte_data_size));
			new (*node) value_type (*(index().at(i))); /// copy construct the value
		}
		index().swap(index_new);
	}
	size_t size() const
	{
		return index().size();
	}
	size_t capacity() const
	{
		return index().capacity();
	}
};
template <typename value_tn,typename allocator_tn=cppmalloc>
struct mono_indirect_vector:
	indirect_vector_tt<
		mono_indirect_vector_rooter<
			mono_value_trait<value_tn>,
			allocator_tn>>
{};
template <typename value_tn,typename allocator_tn=cppmalloc>
struct mono_space_indirect_vector:
	indirect_vector_tt<
		mono_indirect_vector_rooter<
			space_value_trait<value_tn>,
			allocator_tn>>
{};


___namespace2_end()